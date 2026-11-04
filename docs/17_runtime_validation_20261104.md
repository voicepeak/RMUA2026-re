# 运行时核验与并发实验（2026-11-04）

目标文件 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`，OL1、独立进程、未改官方二进制。调试器是只读断点，但暂停可能影响时序；所有真值仅用于事后评估。本节是开发诊断，不是比赛等价性或成绩证明。

## 已观测的 seed、门与 reset

`experiments/runtime_trace_20261104_v2/` 保存 seed 123/456/2026 每个两次的 GDB 原始输出、风 CSV、reset 响应、manifest。每次都观测到 singleton 初始化的 configured/initial/current 等于输入 seed，`0x2fb2040` 命中且 seed 参数一致。每次还看到两次 `0x2fba970` 入口，其入口读取的 initial/current 此时均为输入 seed；没有观测到随机状态先被消耗再重置的全过程，不能据此认定 FRandomStream 或 libc rand 被重置。

每次 OL1 均捕获 12 个门的 BeginPlay 组件变换。用初始位置与静态门表最近邻配对，六次的 60 个跨运行对比中，位置、四元数姿态角及缩放的最大差均为 0，记录中的 FName 索引/序号也相同；逐门值和配对误差见 `gate_comparison.json`。这支持所测版本/地图/三个种子的门变换固定；不是所有 seed 或其他地图的穷举证明。组件缩放不是已测量的门洞物理尺寸；其他地图和碰撞边界仍需核验。

每次正式 ROS reset 服务响应均为 `success: False`。`rosservice` 退出码 0 只表示服务通信成功，不等于 reset 成功；原始 v2 manifest 中的 `status: observed` 是旧脚本只检查退出码的产物，不能解释为 reset 成功。新版脚本额外检查响应。REA 反编译 `artifacts/rea/reset_bridge_rea.json` 显示回调根据 pose 检查设置响应，但不能单凭此推断每次失败原因。两次 reset 入口命中也不能证明风和障碍重放。

风话题在起点有采样，所见为零；stage 4/5/6 未到达，`0x2fb23b0`、`0x2fc77b0`、`0x2fc8900` 没有运行时命中，因此风序列、线障、车辆及种子重放仍是静态路径证据，不能把零风外推到室外。此前控制器未完成全赛道；为遵守不改规则、不绕过碰撞的约束，没有强行切换 stage。现场记录第一版工具失败和修复后的第二版，不删除失败日志。

## PWM 与传感器

`experiments/pwm_20261104/seed123_closed_loop` 是有渲染的官方模拟器、seed 123、PWM 闭环位置参考阶跃，14 s，独立真值事后评估；不是单电机开环系统辨识。记录了 PWM 发布、IMU/GPS、姿态位置真值、执行器和风原始流，以及配置和源码快照。969 条 PWM 命令均匹配到观测，发布间隔 p50/p95/p99 为 12.62/21.16/23.65 ms；稳定段真值相对参考的位置 RMSE 为 [0.322, 0.050, 0.028] m，姿态估计与真值夹角 RMSE 0.0115 rad。它反映本次闭环和参考，不能当作通用 PWM→力矩传递函数或网络延迟。

NullRHI 四实例 15 s 的订阅观测：每个 IMU 约 100 Hz、LiDAR 约 10.08 Hz，IMU 到达间隔 p99 10.17–10.23 ms，观察到的消息序号缺口为零。样本记录单调时钟、墙钟、header stamp 和 sequence；不同时间域未做同步校准，不能以时间戳差冒充端到端延迟，也不能把观察端无缺口推广为无丢帧。相机、外部采样至 ROS 的延迟与真实硬件延迟尚未标定。

seed 456 电机开环脉冲尝试发出 141 条指令后触发 `POSITION_LIMIT`，退出码 2，`evaluation_valid: false`；原始数据保留在 `experiments/pwm_20261104/seed456_motor_pulses_failed/`，不能用于拟合有效传递函数。

## 四/八实例及复现

4 实例进程 CPU 约 109.45–110.24%（单核=100%），峰值 RSS 763.5–764.5 MiB；IMU 首次就绪的观测上界约 8.36–11.54 s，受检查顺序/调度影响。`pool4/measurement.json` 有原始时序。8 实例一次启动后在测量中仅 5 实例存活，日志包含 SIGSEGV；另一次在就绪前失败并清理。原因未定位，不应声称可稳定 8 路或将其归因于 OOM。原始数据和失败日志均在 `experiments/benchmark_20261104/`，详见 docs/16。

使用与本次相同的主机 ROS、Docker 镜像和模拟器目录，示例：

```bash
python3 scripts/run_readonly_trace.py --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --messages /home/tianbot/RUMA-by-helinjun/beta/rmua_ws --output /tmp/rmua-trace --seeds 123,456,2026 --repeats 2 --port 11520 --sample-seconds 3 --display :1 --reset
python3 scripts/analyze_gate_trace.py /tmp/rmua-trace --output /tmp/rmua-trace/gate_comparison.json
python3 scripts/headless_pool.py start --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --seeds 123,456,2026,789 --port 11421 --output /tmp/rmua-pool4 --display :1
python3 scripts/measure_headless_pool.py /tmp/rmua-pool4/pool.json --seconds 15 --output /tmp/rmua-pool4/measurement.json
python3 scripts/headless_pool.py stop /tmp/rmua-pool4/pool.json
```

PWM 实验还依赖原飞控工程的容器及其运行脚本。运行示例为：

```bash
cd /home/tianbot/RUMA-by-helinjun/main
RMUA_CONTAINER=rmua_calib_20261104 tools/start_gpu_env.sh
RMUA_CONTAINER=rmua_calib_20261104 DISPLAY=:1 OPENBLAS_NUM_THREADS=1 python3 -m evaluation.seed_runner.pwm_identification --seed 123 --hover-seconds 14 --step --evaluate-truth --stationary-bootstrap --record-pwm-commands
docker rm -f rmua_calib_20261104
```

调试镜像 `rmua/re-probe:20261104` 由本机 `rmua/noetic-xal:driver-recovery-20261012` 增加 GDB 构建，运行需要本地已安装这些镜像及匹配的 ROS 消息；地址、路径、显示号和端口需按环境调整。并发脚本不改官方程序，但使用本地 Docker/ROS 开发环境。

未完成：可达 stage 4/5/6 的合规航迹、风/动态障碍实际序列及同 seed 重放；门真实尺寸与多地图；PWM 参数辨识与相机时间/延迟；reset 两类随机数验证；同轨迹 NullRHI/有渲染模式的风、障碍、碰撞对照。现有结果不足以交付等价或高并发训练模拟器。

## 后续：reset 与并发问题

seed 123 官方 reset 的只读回调探针观测到活动分支执行，随后检查 `GETPOSE_UE` 结果的 7 个原始 float 为 `[0, 0, 0, 0, 0, 0, -37.9084167]`；最后一个字段绝对值超过回调的 0.2 阈值，所以该次响应 `success: False`。这里只确认原始偏移与判断，不能擅自认定字段含义或推广至全部 false 响应。

同一次过程中，随机流 current 在 `0x2fced20(-1)` 入口为 `214098071`，稍后 `0x2fba970` 入口为 `123`；反汇编显示该 helper 以配置 seed 重写 initial/current。这表明本次已消费的 FRandomStream 状态被恢复。该次 reset 期间 `0x2fb2040` 没有再次命中，无法排除其他路径直接调用 libc `srand`，也不能据此认定风、障碍或整场景可重复。官方 `Reset.srv` 是空请求，没有 seed 参数；可指定 seed 的已验证方式是新进程启动参数，但事件级重放仍未验证。参见 `experiments/reset_semantics_20261104/`。

八路失败顶帧 `0x26b6223` 是普通 32 位整数内存读取，尚无出错指针和对象生命周期证据；另一次失败没有栈。相同 seed 42 的空载单进程对照运行至少 83 秒后仍存活，高于此前八路失败的约 60 秒。此对照不足以判定端口、ROS、驱动、资源或模拟器内部哪一项是根因，也不足以给出长期稳定并发上限。短测保守使用 4 路或更小批次，失败留档并从新进程重新运行；批处理工具及对照原始证据见 `experiments/concurrency_followup_20261104/`。

后续 seed 456 独立运行中，在 reset 回调入口对已校验 SHA-256 的 glibc `srand`/`srandom` 公共入口设只读断点：从安装到 reset 后采样结束未命中；FRandomStream 已消费值 `1410508404` 恢复为 `456`，`GETPOSE_UE` 的 `+0x18` 为 `-37.9077873`，响应仍为 false。该窗口内的入口未命中不排除其他方式修改 libc 随机状态，更不证明事件序列可重放。原始输出及时间域说明见 `experiments/reset_semantics_20261104/run_seed456_libc/`。

## 后续：合法传感器赛段探测

首次历史视觉航点尝试在发现其世界坐标可能依赖历史 `pose_gt` 后主动中止；该次数据完整保留，但不算满足真值仅事后评估的导航。第二次 seed 123 使用 cooked 地图静态 spline、GPS、IMU 和 LiDAR，不使用在线或预录的 debug 真值控制。它捕获 `0x2fb2040` 一次、stage 1 初始化一次，state 0→1→12；未命中 `0x2fb23b0`、`0x2fc77b0`、`0x2fc8900`。11,210 条 wind 记录全为零，仅说明本次范围。state 1 到 12 的 debugger 单调钟间隔为 180.053 秒，与静态代码所述默认超时相符，但没有直接原因码，超时原因仍是推断；切换时最近 GPS 位置约 `[174.71, 4.84, -9.93]` m，不是中止时的最终位置。

该静态 spline 采样长度约 683 m，若在 180 秒内完整行进其几何均速须约 3.79 m/s；目前控制器该段限速约 1 m/s，hub 连接、工厂触发和更高速的安全包络均未验证。因此尚无合规到达 stage 4/5/6 的证据，更没有 seed 123/456/2026 的室外重复采样。`experiments/stage_probe_20261104/` 保留轨迹、双时钟风采样、断点、静态路线推导、配置、主动中止记录和复现命令。`summary.json` 的 ROS header 是消息源时间，墙钟与单调钟另列；GameState timer 字段的仿真时间映射仍未校准。首趟原始 manifest 的 LiDAR 标记已由补充文件更正，SIGINT 下 `fault: null` 也不代表成功。

8 路带 GDB 空载复现中，8 路均完成 IMU 就绪，约 89 秒监测内 7 路持续存活；seed 2024 在启动约 78.16 墙钟秒后发生 SIGSEGV。崩溃指令 `mov (%r12),%r12` 的 `r12=0x1e3100200`，调用栈包含 `AirSimROSFrame → record_frequency_data → deque::pop_front → operator delete`；具体内存破坏来源未知。主机可用内存的最低观测值约 28.2 GiB、容器 `OOMKilled: false`，不同 master 端口均曾监听。它们缩小了本次观察范围，不能排除资源、其他端口或驱动因素，也不能把这条不同的栈与先前 `0x26b6223` 断言同因。反编译证据显示 `record_frequency_data` 函数本体未见显式锁，但调用者可能持锁，现场也未证明并发访问同一队列。GDB 改变时序；7 路 89 秒不是稳定并发上限。完整证据、采样和复现见 `experiments/concurrency_gdb_20261104/`。

## 后续：渲染传感器和频率统计重入

`experiments/sensor_clock_rendered_20261104/` 是 seed 123、RenderOffscreen 官方程序 20 秒只读采样：IMU 2005 帧、约 100.00 Hz；LiDAR 197 帧、约 9.82 Hz；左右相机各 365 帧、约 18.23 Hz，图像为 960×720 bgr8，记录非空负载长度。每条记录包含宿主 Unix 墙钟、单调钟、ROS 回调时钟和消息 header；本次序号未见缺口或回退，但不等于证明源端无丢帧。IMU 到达间隔 p99 为 11.39 ms，相机左/右分别 84.49/85.23 ms。`arrival_ros - header_stamp` 数值原样保留，来源采样语义及 ROS 与仿真时钟映射尚未标定，不能将其宣称为真实端到端延迟。脚本与复现命令见本目录的 manifest 和 `scripts/run_rendered_sensor_probe.py`。

`experiments/concurrency_frequency_overlap/runtime_seed123/` 使用 GDB all-stop 的 entry/finish 只读探针，在单实例静止场景记录到 343 次不同线程中 `record_frequency_data` 的调用生命周期重叠，覆盖 frame、imu、lidar、rotor_pwm；本次 0 次同 topic 重叠。探针本身会暂停和重排线程；生命周期重叠不证明同时执行或共享 deque 的数据竞争，更不能确认是 SIGSEGV 根因。原日志、探针自测和静态调用表见 `experiments/concurrency_frequency_overlap/`。

## 后续：OL2 启动方式尝试

四组地图选择实验（URL、ExecCmds、map flag、GameDefaultMap ini）请求 OL2，但引擎日志均显示最终 `LoadMap` 为 OL1；之前原始 manifest 的 `map` 是请求参数，不是实际加载证明。实际加载路径清单见 `experiments/gates_ol2_20261104/actual_maps_summary.json`；不把这些样本当作跨地图门验证。`scripts/run_map_gate_trace.py` 现在根据实际 LoadMap 输出标记 `requested_map_not_observed`。OL2 逐门运行时核验和实际门洞物理尺寸仍未完成。

有渲染窗口中追加了一次官方交互控制台尝试：在 OL1 加载后向 RMUA 窗口发送打开控制台和 `open ...OL2` 的键盘事件；未观测到 OL2 的 `LoadMap`。此次没有独立截图证明控制台打开或接收命令，不能推断命令是否被禁用。原日志和时钟见 `experiments/map_console_20261104/`；仍未完成跨图运行时验证。

另用 OL2 包路径替代可执行程序的首个 `RMUA` 参数，seed 123/456 均实际加载 Lobby 然后 OL1，见 `experiments/gates_ol2_firstarg_20261104/`；这不排除其他官方切图方式。地图探针现在以最终 `LoadMap` 路径核验目标，并保留其他运行失败状态。

室外路线导航准备：新增 GPS 位置与 IMU 姿态的诊断桥，只发布独立诊断话题，不订阅真值或发控制指令。seed 123、NullRHI、地面静止 12 秒的 121 个 GPS 回调均通过样本有限性、姿态归一、IMU 新鲜度和 header 时间差检查；最大源时间差约 12 ms，最大 IMU 回调年龄约 10.1 ms。原始数据与实际运行脚本哈希见 `experiments/gps_imu_bridge_20261104/`。这是给后续传感器导航做的接口探针，尚未验证坐标系、动态姿态误差或连接 beta 控制器的安全性，也没有推进赛段。

对已有 seed 123 PWM 运动场次做独立事后时间匹配：GPS 170/170 样本匹配到 20 ms 内真值，位置误差范数 RMSE 0.0728 m；IMU 1698/1698 匹配，姿态四元数夹角 RMSE 0.0000876 rad。数据来自单次小范围闭环（真值三轴范围约 0.95/0.21/1.02 m），不能证明高速场景、整个赛道的坐标系一致或在线延迟。源文件哈希、逐样本结果、匹配方式见 `experiments/gps_imu_bridge_20261104/offline_motion_alignment/`；真值只用于事后评估。

后续已在独立 VEL 诊断场将 beta 的调试位姿订阅重映射到 GPS/IMU 桥，用静态地图 spline 1 与高度指南驱动，开启 LiDAR 后端；第二、三场 ROS 图证实 route_follower 订阅诊断位姿且 debug/pose_gt 无订阅者。首场错误地对 NED 指南重复叠加 z_offset，起始参考跳变 4.31 m、随后进入 state 12，直接原因未证实；失败保留。修正 offset 后，2 m/s 60 秒 GPS 最后 x≈106.7 m；启用发布前 LiDAR 新鲜度检查的 4 m/s 60 秒场最后 x≈209.7 m。两次仅观测 stage 1、按墙钟超时停止；不是比赛完成或高速全程安全证明。见 `experiments/beta_sensor_scout_20261104/`、`experiments/beta_sensor_scout_corrected_20261104/`、`experiments/beta_sensor_scout_4ms_20261104/`。

seed 123 的后续 6 m/s 场使用相同无真值控制输入及 LiDAR 新鲜度保护，沿静态 spline 1 到达其局部候选路径终点，GPS 最后约 `[597.17,-99.15,-133.59]` m；控制器 `GOAL_REACHED` 仅表示局部路线终点，不是官方终点。其状态仅观测 0→1 / stage 1，9,154 条风均为零，未进入室外。该点与 spline 3 之间有约 147.5 m 未验证连接；详见 `experiments/beta_sensor_scout_6ms_20261104/`。

在此基础上，seed 123 将静态 spline 1 与反向 spline 3 用约 147.53 m 未验证的直线候选连接，并保持 LiDAR 后端、发布前新鲜度检查及 6 m/s 上限。180 秒有界场次中到约 `[615.04,-80.69,-133.36]` m、路线站距约 689 m 后控制器多次报告 `COMMAND_BRAKING`，未完成连接；不能在缺少进一步证据时断定制动原因。状态仍只有 0→1、stage 1，9155 条风均为零；未命中室外风、线障、车辆入口。完整轨迹、ROS 图、状态/风日志和候选路线见 `experiments/beta_sensor_scout_1to3_20261104/`。这是失败/受限的独立 VEL 开发诊断，不是赛程完成。

连接处的重复场新增只读 LiDAR 快照与控制器决策采样：station 685–695 m 的 546 条控制器遥测中，406 条为 `COMMAND_BRAKING`，121 条为 `COMMAND_BLOCKED`。末次点云以最近 GPS/IMU 近似转换后，在朝向 spline 3 的前方近场区域检测到密集回波，最近前向投影约 1.33 m；传感器时间差、点云和每条决策均保留在 `experiments/hub_lidar_probe_20261104/`。这些观测与直线候选被障碍限制相符，但不能仅凭此识别障碍物或证明碰撞边界。无回波也不代表空间可通行。

随后以同一传感器控制链测试将候选连接最多抬高 8 m、前后各 80 m 渐变的路线。seed 123 场仅到 stage 1，GPS 最后约 `[571.08,-110.93,-134.36]` m 时观测 state 1→12，运行器按预设停止控制；没有原因码，不能判断是否碰撞。记录了 6,113 条零风和两帧枢纽近侧 LiDAR，未到室外段。抬高是基于有限回波提出的假设，不能把无回波当作净空证明；失败样本、遥测和复现见 `experiments/beta_sensor_hub_lift_20261104/`。

抬高路线原因探针复现：该次 state 1 期间没有命中指定的碰撞回调入口 `0x2fb7110`；首次 state 12 请求的调用栈返回地址为 `0x2fb9517`，紧接在 `0x2fb9512` 对状态函数的调用之后。结合该地址机器码，可将本次请求定位到 `GameState+0x180==0` 时累积 `+0x1ac` 并超过 `GameConfig+0x2cc`（默认 OutTime 2 秒）的分支，而非普通 180 秒段计时分支；入口记录 `+0x1ac=2.000006`。尚未确定为何 `+0x180` 为零，也不能排除其他碰撞路径或把本次原因自动套用到先前场次。只读断点、栈、对应汇编和完整数据见 `experiments/hub_lift_cause_20261104/`。

对抬高场的 `0x2fb9830` 返回值和相关对象字段进行只读 transition 探针：该场 predicate 在 GPS 约 x=556–571 m 间多次在 0/1 切换。连续几次 0 区间结束时 `+0x1ac` 分别约 0.4、1.3、1.9 秒，间歇回到 1 未将已累计值清零；再次回 0 后约达 2 秒进入 state 12。此场支持 OutTime 在这些间断区间累计，而非每次回到 1 后重置。0 次快照中 `+0x30/+0x40/+0x60/+0x70` 数组无通过项；部分 1 次快照中数组 `+0x30` 的 index 0 满足 `+0x288 > +0x2b4`。弱引用分支、对象完整类型与空间语义仍未完全确认，不能单凭该快照认定具体场景边界或推广到所有 seed。见 `experiments/hub_predicate_trace_20261104/`；GDB 暂停可能影响时序，GPS 是按到达单调钟近邻配对。

后续对抬高场的 OutTime 判定对象做只读根位置清单：`array30` index 0 与静态 `1_1` 样条根位置精确匹配；该场判定间歇回到 1 时，这个对象的有符号字段满足 `+0x288=2 > +0x2b4=1`。`array30` 12 个中 8 个按根位置唯一匹配样条、4 个因位置重合有歧义；`array40` 和 `array60` 各 12 个分别与端口和工厂根位置唯一匹配。位置匹配不是 FName 解码或完整类证明，弱引用分支和字段的重叠语义仍待验证；这仅支持该场离开相关样条区域与 OutTime 累计有关的假设。原始清单与全部歧义见 `experiments/zone_inventory_20261104/`。

非抬高 1→3 候选路线的 seed 123 场，对 `0x2fc8d80` 记录到 99 个分段事件：进入型、离开型及另一类事件各 33 次，进入序号为 32→0。相邻同对象事件中，33/33 进入型后的 `+0x288 - +0x2b4` 增 1，32/32 有后续记录的离开型减 1，33/33 另一类型无变化；与静态代码对集合插入/移除的解释一致，但不能仅凭此断言精确碰撞形状或评分语义。最后一个分段退出后，总 predicate 在本观察窗口没有记录到 1→0，可能由其他分支满足，原因未确定。试验仍在 stage 1，GDB 会影响调度。逐事件及时间域见 `experiments/segment_overlap_trace_20261104/`。

### 真值隔离更正

此前部分 `run_beta_sensor_scout.py` 的场次虽然控制器没有订阅 `/debug/pose_gt`，运行器却轮询 GDB 的 `requested_state=12` 并据此发送 SIGINT 停止控制。那些 manifest 中的 `debug_truth_control: false` 只能描述导航话题，不代表整个实验生命周期完全未使用调试内部状态；凡 `status: race_end_stop` 均受此限制，不应计作满足严格“调试真值只用于事后评估”的验收。现在的运行器不再基于 GDB 状态停止控制，只使用固定时限和控制器自身传感器保护；GDB 日志用于事后分析。旧 manifest 和实验日志保持原样。

## 室外段开发注入试运行

通过 `scripts/stage_inject.gdb` 将初始 `0x2fbac30` 的寄存器实参 `(1,3,1)` 改为官方切换代码中的 stage 4 实参 `(8,12,4)`；这是内存/寄存器注入，而非合法飞行。seed 123 试运行命中 `0x2fc77b0` 和 `0x2fb23b0`，后者入口的四个浮点参数为 200、350、2、1.5，并在返回后读到 35 个 16 字节条目。日志逐条保存字节和四个浮点视图；每个最后的第四字段实际上以原始字节保存，不能直接当作浮点物理量。ROS 采样仍在起点、风为零；此法尚未验证风区实际作用、碰撞或 reset 重放。原始日志和 manifest 在 `experiments/stage_inject_pilot_20261104/`；更多 seed、重复与 stage 正在采集。

stage 4 的三个 seed 各两场已完成：每个 seed 的两次风关键帧原始字节逐字一致，三个 seed 的风数组彼此不同。各场 35 个条目中，可配对的保持区间约 8.015–9.985 秒，过渡区间为 2 秒，关键帧标量 1.5–2.0。末项没有后继，故不计入区间统计。每场 `0x2fc77b0` 入口 1 次、`0x2fc4980` 入口 7 次、`0x2fc3fc0` 入口 198 次，均为调用尝试而非成功实例数量；对应日志中分别有 231–283 条 SpawnActor 碰撞失败警告，不能将该场的放置分布当作正常赛程的完成场景。探针中的 `thin_place` / `thick_place` 是函数追踪标签，尚未验证每个调用对应的资产类型。`experiments/stage_inject_4_20261104/analysis.json` 含逐关键帧和比较结果，脚本为 `scripts/analyze_stage_injection.py`。

stage 5 六场注入也完成：每个 seed 两次风关键帧字节一致，三个 seed 不同，每场 37 条；可配对保持约 8.015–9.985 秒、过渡 2 秒，样本关键帧标量约 2.402–4.0。每场线障生成入口 2 次、`0x2fc4980` 14 次、`0x2fc3fc0` 628 次，仅代表入口尝试。seed 123 第一次风话题采样命令退出码 137，其余为超时退出码 124；关键帧来自 GDB 内存，不依赖该话题。原始文件及比较见 `experiments/stage_inject_5_20261104/`。

车辆直接调用的首个探索失败：风数组生成返回断点中以 GDB `call` 请求 `0x2fc8900`，调试器报告无法取得寄存器、线程仍在运行，容器未达到就绪。该场不能认定函数完成或得到车辆行为，保留 `experiments/vehicle_call_pilot_20261104/`，后续应改变调用方法。reset 注入首次试验所用 ROS 工作区缺少 `airsim_ros/Reset` 类型，服务调用未发出；保留 `experiments/stage_reset_inject_pilot_20261104/`，并改用包含该类型的工作区重试。

stage 6 三 seed 各两次的注入风数组同 seed 逐字一致、不同 seed 不同；每场 35 条，保持约 8.015–9.985 秒，过渡 2 秒，样本关键帧标量约 9.537–15.0。入口实测 `xmm0..3 = 200,1800,15,4`，更正此前 docs/15 的末两参数顺序。每场两次线障生成入口、14 次 `0x2fc4980` 和 804 次 `0x2fc3fc0` 入口均为尝试，不是成功生成数。见 `experiments/stage_inject_6_20261104/`。

换用含 Reset 服务类型的工作区后，对三个 seed 各做一次 stage 4 注入、ROS reset、再注入：服务都返回 `success: False`，每场两个风数组字节均不同。此结果不能证明成功 reset 或单独归因于 libc 状态；先前只读探针曾观察 FRandomStream 恢复 seed。GDB 因临时断点回调继续执行而重复输出第二个数组一次，第三份 dump 并非第三次生成。`scripts/analyze_injected_reset.py` 和 `experiments/stage_reset_inject_retry_20261104/` 保存完整数据及哈希。

替代 GDB `call` 的车辆探索用游戏线程上修改返回地址和寄存器的方法，在 seed 123 命中 `0x2fc8900` 以及 `0x2fc93c0` 分支；尚未观察到调用返回，不能据此断言车辆生成或运动。运行器的 `observed` 只验证了风和线障入口、话题就绪，不验证车辆返回；详见 `experiments/vehicle_trampoline_pilot_20261104/`。

车辆调用后续：`experiments/vehicle_trampoline_v2_pilot_20261105/` 的返回标记在目标入口之前出现，随后 SIGSEGV；该标记无效，不能称目标函数返回。另一种试运行 `experiments/vehicle_redirect_pilot_20261105/` 在开发容器中临时将游戏线程 `0x2fbb5a9` 的现有 call 目标改为 `0x2fc8900`，提供对应路线对象和 1000 cm 浮点参数；观察到目标入口、`0x2fc93c0` 分支以及原调用位置的返回，并恢复被改写的指令字节。它替代了原函数调用、改变了随机流和场景，不能称为自然赛程；车辆对象、运动和碰撞仍待采集。官方磁盘文件没有修改。

`experiments/obstacle_inventory_pilot_20261105/` 在 stage 4 注入场读出 31 个曾进入线障放置入口的对象的 `+0x250` 数组，共 371 个三浮点点位，附原始字节。静态代码使用此数组进行候选点间距检查；这些是内部记录的点，不是已证实成功 spawn 的实例或碰撞形状。正在对三个 seed 做重复采样。

stage 4 线障候选数组对 seed 123/456/2026 各两次：每场均读到 31 个对象数组，点数分别 371/373/373；按原始点字节作为多重集合比较，同 seed 两场一致，不同 seed 不同。对象地址被排除，所有声明点数均成功读取；这仍是内部间距点，而非已证明 spawn 的线障数量或运动。复现和分析见 `experiments/obstacle_inventory_4_20261105/`、`scripts/analyze_obstacle_inventory.py`。

同样六场车辆调用重定向均观察目标入口并返回。seed 123 和 2026 的两场中，`0x2fc93c0` 在重定向调用返回前命中；seed 456 的该入口出现在返回之后，不能归因于重定向调用。`experiments/vehicle_redirect_4_20261105/analysis.json` 按事件顺序区分。seed 123 单场增加 `0x2fbff60` 和车辆 tick 探针但未观察到车辆初始化或活动 tick；入口命中不等于生成/运动，见 `experiments/vehicle_dynamics_pilot_20261105/`。较长的六场旧跳转试验没有观测到返回，原始失败数据在 `experiments/vehicle_trampoline_long_20261105/`。

stage 5 的六场候选点数组也已完成，同 seed 两次点字节多重集合一致、三个 seed 不同；具体数组数和原始点见 `experiments/obstacle_inventory_5_20261105/analysis.json`。它们仍不是成功生成数。

车辆机制进一步验证：`experiments/vehicle_forcegate_pilot_20261105/` 在重定向调用外，把 `0x2fc93c0` 的状态字符串查找结果从 -1 临时改为 0；seed 123 于是进入 `0x2fbff60`，输入 5 个三维路径点、速度字段 667，随后活动 tick 的进度从 0 增至约 6019。独立重复试场 `experiments/vehicle_world_pilot_20261105/` 在设置车辆组件位置后读取其世界坐标，约 9 秒观测中由 `(-3317.54,4146.44,1775.10)` cm 移至 `(1263.86,7826.84,252.87)` cm。原始点、世界坐标、单调时钟及 tick 保留在日志；以 UE 厘米解释时速度字段为 6.67 m/s，但 GDB 干扰时间且这不是自然路径、车辆碰撞或真实评测证明。此状态分支强制和调用重定向都只改变开发进程内存。

stage 6 线障候选数组六场均读取 64 个对象：seed 123/456/2026 分别为 1430/1420/1422 个点；每 seed 两次点字节多重集合一致，跨 seed 不同。所有数组声明点数与读取数相符。见 `experiments/obstacle_inventory_6_20261105/analysis.json`；仍不能等同于成功 spawn。

额外强制状态检查的车辆六场均观察一次初始化及 5 个路径点、活动进度 tick。seed 123 和 2026 的两次路径字节各自一致，seed 456 两次路径字节不同；该注入场里 seed 456 的车辆相关调用发生在重定向函数返回后，触发时序与其他 seed 不同，具体差异原因未证实。不能声称所有 seed 的车辆路径可重复。见 `experiments/vehicle_forcegate_4_20261105/analysis.json` 和原始日志。

车辆组件世界坐标现已覆盖三个 seed 各两次：每场均记录 5 个路径点及至少 11 个世界坐标样本；路径字节在 seed 123、2026 各自两次一致，seed 456 两次不同。该实验使用重定向和状态检查强制，且采样时长不同步，不能把末位置差异直接解释为模型误差。见 `experiments/vehicle_world_4_20261105/analysis.json`。

风更新试运行在 stage 4 生成后将状态写为 7、将段起始时钟设为当前单调钟并在内存中禁用超时；无人机仍在初始位置，采到 485 条非零 ROS 风数据，首个向量约 `(0.173,-1.490,0)`。场景由开发注入改变，不能当作自然比赛或风区空间分布证明；见 `experiments/force_wind_tick_pilot_20261105/`。扩展采样时发现 beta ROS 工作区没有生成 `RotorPWM` 消息类型，`rostopic echo` 的 PWM 输出为空；将保留该失败并改用核验 ROS 类型和 MD5 的原始消息订阅。
