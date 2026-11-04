# 运行时核验与并发实验（2026-11-04）

目标文件 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`，OL1、独立进程，官方发布文件只读。最初的历史场次使用只读断点；后文明确标示的开发场次使用进程内状态、寄存器或调用目标注入。调试器可能影响时序，真值仅用于事后评估；开发场次不是比赛等价性或成绩证明。

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

强制风状态 stage 4 的六场中均采到非零风、IMU、GPS、LiDAR 消息时钟及事后真值；风样本每场 383–385 条，seed 123/456 的此次采样速度模约 1.5，seed 2026 约 1.907。每场 PWM 的 `rostopic echo` 均因工作区缺少消息类退出 1，不能把运行器的 `observed` 当作 PWM 采集成功；原样保留 `experiments/force_wind_tick_4_20261105/`。随后采用 `rospy.AnyMsg` 解码并核验类型/MD5 的独立试场采到 500 条 PWM，见 `experiments/force_wind_wire_pilot_20261105/`。采样期间未由该探针发布 PWM 指令，不能用静止初态数据声称 PWM 阶跃响应。`scripts/analyze_forced_wind.py` 分开统计各流和退出码。

修复后的 stage 4 风状态六场均录到非零风及 800–801 条 PWM 原始消息，类型和 MD5 校验均通过；IMU 约 800 条、LiDAR 80–81 帧，完整计数和退出码见 `experiments/force_wind_tick_4_wire_20261105/analysis.json`。这些场次没有发送 PWM 指令。独立的 seed 123 场使用预定的四电机相同 PWM 序列 0→0.2→0.3→0.4→0.2→0，发送 248 条命令，接收 801 条实际 PWM，且风样本非零；命令器不订阅任何传感器或真值。该单场仅是开环响应原始采样，未标定延迟或证明动力学等价，见 `experiments/wind_pwm_step_pilot_20261105/`、`scripts/pwm_wind_step_probe.py`。

stage 5 强制风状态下三个 seed 各两次均有非零风、PWM、IMU/GPS 和 LiDAR 时钟。各 seed 本次约 8 秒采样的风速模分别约 3.990、3.792、3.125；每场 PWM 800–801 条、解码错误 0。具体时序和样本见 `experiments/force_wind_tick_5_20261105/`；该探针不发布 PWM 命令。

碰撞裁决单场直接注入试验：在开发容器内将一个现有调用临时重定向到 `0x2fb7110`，先将 state 置 7、`IgnoreAllHitCollision` 置 0；观察该回调请求 state 12，返回时状态为 12。之后原初始化又请求 state 0，日志中另有后续碰撞回调，因此本项只证明直接注入时的回调分支，不证明实际碰撞形状、接触力或真实事件。见 `experiments/collision_inject_pilot_20261105/`。

stage 6 强制风六场均获得非零风、PWM、IMU/GPS 和 LiDAR 时钟；三个 seed 样本风速模分别约 9.572、9.541、10.396，每场 800–801 个 PWM，解码错误 0。stage 4 的固定开环 PWM 已覆盖三 seed 各两次：各场约 248 条命令、800–801 条实际 PWM；五个非初始零的命令阶段均在该阶段内观察到对应输出。以消息 header 首次匹配所算时间差约 1.9–40.1 ms，但源头与时钟语义未标定，不能称为端到端延迟；初始 0 的输出未匹配，保留 null。原始数据和分析见 `experiments/force_wind_tick_6_20261105/`、`experiments/wind_pwm_step_4_20261105/`。注入状态、初始地面位置、固定开环输入和仅事后真值均须保留为结论边界。

stage 5 固定开环 PWM 六场均发送 248 条命令、采到 800–801 条输出；非初始零阶段的同阶段 header 首次匹配差约 0.1–37.7 ms。不能将 header 差当作已标定的端到端延迟，初始零没有匹配。见 `experiments/wind_pwm_step_5_20261105/`。

对单 seed 延长到 25 秒后，单次状态写入的 stage 4 和 5 风话题分别在 1230/1231 条样本内保持同一向量，未覆盖生成数组预示的过渡。单次状态注入可能被后续状态变化覆盖，也可能涉及其他更新条件，原因尚未确认；前述 8 秒非零风样本因此只能证明一次风状态输出，不证明持续演化。保留 `experiments/wind_transition_4_20261105/` 和 `experiments/wind_transition_5_20261105/`，正在测试持续注入与时钟追踪。

碰撞回调开发注入对照已覆盖三个 seed 各两次：将 state 设为 7 后直接调用 `0x2fb7110`，`IgnoreAllHitCollision=0` 的六场返回时均为 state 12；标志为 1 的六场返回时均为 state 7。两组均在调用后恢复原指令字节，见 `experiments/collision_inject_ignore0_20261105/`、`experiments/collision_inject_ignore1_20261105/` 和 `scripts/analyze_collision_inject.py`。这验证被注入回调的分支，不是线障/车辆物理接触或碰撞几何验收。

stage 6 单次状态注入的 25 秒风也保持同一向量（1234 条），见 `experiments/wind_transition_6_20261105/`。另一个 stage 4 单 seed 试场每个游戏 tick 将 state 重新写为 7，并只在首次设置段起始单调时钟；其关键帧索引从 0 推进到 1、2 等，25 秒 ROS 风矢量发生变化，速度模约 1.50–1.84。该试场保留 GDB 时钟、索引和传感器原始数据于 `experiments/wind_persistent_pilot_20261105/`；持续注入改变每个 tick 且断点影响调度，不能视为自然赛程或确定单次注入冻结的根因。

stage 6 固定开环 PWM 六场均发送 248 条命令并记录 800–802 条实际 PWM；非初始零阶段同阶段首次 header 匹配差约 0.1–38.9 ms，不能解释为经过标定的端到端延迟。各流和失败/缺失的初始零匹配见 `experiments/wind_pwm_step_6_20261105/`。三个 stage 的固定序列均有三 seed 各两次，但这些短场的单次状态注入风向量保持不变；动态风与 PWM 的耦合尚未完成验证。

对持续注入的 stage 4 seed 123 风试场，用生成器关键帧和 GDB 的墙钟/单调钟对估算相对时间：保持区间直接取关键帧速度和角度；标记为过渡的区间对速度和角度分别线性插值，再按 `(speed*cos(angle), speed*sin(angle), 0)` 重建。1230 条 ROS 向量与该模型的二维误差 RMSE 约 0.0383、最大约 0.207；包括调试暂停和未经标定的 ROS header/墙钟偏差。该试场的角度从约 276.6° 到 9.6°，数据与直接线性插值角度相符，不能据此推广到所有角度边界。逐条原始流、关键帧和脚本见 `experiments/wind_persistent_pilot_20261105/wind_interpolation_analysis.json`、`scripts/analyze_persistent_wind.py`。

离线风模型已与 stage 4/5/6 × seed 123/456/2026 × 两次 fresh 运行的全部关键帧逐字节吻合，见 `experiments/wind_offline_model_20261105/`、`scripts/wind_offline_model.py`。反汇编确认 `rand()` 只取低 24 位再除以 16777215，保持时间分布为 `8+2u` 秒；公式、上限/下限和依赖条件见 docs/15。此证据不覆盖 reset 后或任何其他消费者提前推进 libc 随机流的序列。

线障阶段距离采样首场记录 198 个内层候选及每次更新后的随机流状态；`1500+7000u` cm（反向时取负）的 float32 重建最大差约 `4.9e-6` cm，基准距离相邻相差 10000 cm。它不是最终障碍间距，且候选失败/裁剪可能改变结果。见 `experiments/line_offsets_pilot_20261105/` 和 docs/15。

持续逐 tick 强制状态的 stage 4、5 现各覆盖三个 seed 两次、每场约 25 秒。每场约 1230 条风向量、四舍五入至 `1e-6` 后有 83 个不同二维向量；按上述近似钟域模型重建，stage 4 的二维 RMSE 约 0.0074–0.0400，stage 5 约 0.0206–0.1493。所有样本有调试器暂停和未经校准的 header/墙钟偏差；这不是自然赛程、稳态精度或时延认证。原始风、PWM、IMU/GPS、LiDAR 时钟、逐 tick 状态和比较结果见 `experiments/wind_persistent_4_20261105/`、`experiments/wind_persistent_5_20261105/`。

stage 4 的线障内层候选偏移已覆盖三 seed 各两次，每场 198 次；三组的同 seed 状态/基准/偏移序列一致。按捕获状态重建偏移的最大误差不超过 `5e-6` cm（日志小数输出精度），采样偏移绝对值总体约 15.032–84.895 m。详见 `experiments/line_offsets_4_20261105/offset_analysis.json`；它们不是 spawn 成功位置。

stage 6 持续状态注入的三个 seed 各两次完成：每场约 1233–1235 条风矢量，近似墙钟对齐下与关键帧模型的二维 RMSE 约 0.0312–0.3733，最大单样本误差约 0.204–1.750；仍包含调试暂停与未标定钟域误差。见 `experiments/wind_persistent_6_20261105/`。

stage 5 线障候选偏移三 seed 各两次均为 628 个调用，同 seed 的状态/基准/偏移序列一致；样本绝对偏移约 15.051–84.992 m，基于捕获状态的 float32 重建最大文本误差约 `5e-6` cm。见 `experiments/line_offsets_5_20261105/offset_analysis.json`，不能当作实际障碍间距。

在持续强制 stage 4 状态、动态风和固定开环 PWM 的独立 seed 123 场，未直接调用碰撞回调，而是对 `0x2fb7110` 与状态入口设断点：记录 20 次回调和 20 次 state 12 请求；事后位置真值的 z 范围约 -5.096 至 0.836 m（NED）。该场每 tick 强制 state 7，能在回调后覆盖 state 12，因此回调次数与正常赛程终止行为不同；尚未解码碰撞对方或校准时间，不能将所有回调认定为无人机落地、线障或车辆接触。原始日志、PWM、IMU/GPS/风和事后真值见 `experiments/fall_collision_pilot_20261105/`。这补充了真实运行时回调样本，但物理接触对象和碰撞响应仍未验收。

stage 6 线障内层偏移三 seed 各两次每场 804 次，同 seed 状态/基准/偏移序列一致；样本绝对偏移约 15.003–84.996 m，float32 按捕获状态重建的最大文本误差约 `5e-6` cm。三个室外 stage 的候选距离均已覆盖三 seed 和重复；仍不能把这些值直接当作成功实例或碰撞位置。见 `experiments/line_offsets_6_20261105/offset_analysis.json`。

持续状态注入的 18 个动态风场中，独立订阅的 IMU 到达频率约 100 Hz、LiDAR 约 10.04–10.10 Hz，IMU 到达间隔 p99 约 10.21–10.33 ms、LiDAR 约 99.31–108.60 ms；观察端序号缺口合计 0。NullRHI 相机消息约 20 Hz，但每帧 0×0、0 字节，不能作为图像训练样本。18 场 IMU z 轴加速度均值约 -9.8081 至 -9.8059、总体标准差约 0.0420–0.0440（消息单位按 ROS 定义）；这些样本混合状态、运动、风与传感器效应，不能直接当作噪声参数。每场 `sensor_clock_summary.json`、`imu_analysis.json`、原始数据和 `scripts/summarize_sensor_clock.py`、`scripts/analyze_imu_samples.py` 可复算；消息到达时钟不证明采样端或端到端延迟。

对三 stage、三 seed、各两次固定 PWM 场的 `pose_gt` 做独立事后统计：所有 18 场的 NED z 全局观测范围约 -5.10 至 1.04 m，末次 x/y 与风状态、采样时长均相关；逐场初末值和范围见 `experiments/wind_pwm_step_{4,5,6}_20261105/motion_analysis.json`。固定命令器没有读取真值；这些数不等于净高度、碰撞点或已辨识的动力学模型，原始姿态/位置、IMU 和命令时钟保留。

七实例空载长时验证第一轮：三个目标 seed 加 789/1001/1002/1003，独立 ROS master，全部 IMU 就绪后 300 秒存活，清理成功。`experiments/concurrency_7_long_a_20261105/` 含逐次容器状态、启动和清理日志；这是一次有限空载窗口，非训练负载的稳定上限或八实例崩溃根因。独立第二轮正在进行。

七实例长时第二轮再次全数就绪、300 秒存活并清理成功。运行期间 60 秒并发订阅记录 IMU 约 100 Hz、LiDAR 约 9.01–10.09 Hz；每进程 CPU 约 109.8–110.1% 单核、RSS 峰值约 694–763 MiB。一个 IMU 的两帧 header 相同（不是已证明回退），各实例源时间/墙钟比约 0.897–0.999；采样线程增加了订阅负载，源时间语义仍未标定。完整观测见 `experiments/concurrency_7_long_b_20261105/measurement.json`。这是两次 300 秒有限实验，不能推广为永久稳定或室外训练负载上限。

线障组件实例首场：在 stage 4 seed 123 注入场，`0x2fc3db0` 共向两个组件提交 373 个 48 字节变换，全部调用返回非负索引；选择位 1 有 272 次、0 有 101 次。构造函数使用 `HISM_ThickLine` 名称创建 `GameConfig+0x240`、`HISM_ThinLine` 创建 `+0x248`，`0x2fc3db0` 选择位 1/0 分别选择这两个组件；相关构造与选择反汇编和字符串在 `experiments/line_instances_pilot_20261105/`。因此可分别记录粗线/细线的组件提交变换与返回索引，但这不证明可见性、精确物理几何或接触。完整 48 字节和浮点视图保留在 GDB 日志；正在做三 stage/seed 重复。

`experiments/lidar_snapshot_pilot_20261105/` 在初始无人机位置保存一帧 PointCloud2 原始 payload 和布局，15882 点全部 XYZ 有限，内容哈希匹配。它是传感器局部 frame 的原始样本，不能证明室外线障可见或点云精度。`experiments/wind_pwm_transition_pilot_20261105/` 的固定延迟开环 PWM 与持续强制状态风在近似 ROS header 对齐下有 8 个风矢量变化样本落在最近命令 0.2 的阶段；只证明采样时间重叠，不证明因果关系或精确控制延迟，命令器不读取真值。

stage 4 三 seed 各两次的线障组件提交均为粗线 272、细线 101，373 个返回索引均非负；同 seed 两场按选择位和原始变换字节组成的多重集合一致，跨 seed 不同。见 `experiments/line_instances_4_20261105/instances_analysis.json` 和每场变换日志；这是提交及返回观测，不证明碰撞可用。stage 4 同样完成六帧初始位置 LiDAR 原始 PointCloud2：每帧 15251–16333 个点、XYZ 均有限，payload 哈希匹配，采样时间及数据布局不同导致点数不同，不把逐帧字节差当作 seed 效应；见 `experiments/lidar_snapshot_4_20261105/`。

后续单场对前五个非直接调用的碰撞回调采集栈，均经 `0x2fce60b ← 0x2fcd85a ← 0x2fcd479` 等地址分派后进入 `0x2fb7110`，但缺少事件参数/对方对象的可验证解码，不能将该栈命名为某个具体接触。`experiments/collision_stack_pilot_20261105/` 保留回调栈、固定命令、风和事后真值；状态仍被逐 tick 强制，暂停改变时序。

stage 5/6 的粗细线组件提交变换也已各覆盖三 seed 两次：stage 5 每场粗线 594、细线 524，stage 6 每场粗线 0、细线 1434；这些调用均返回非负索引，同 seed 的变换字节多重集合一致，跨 seed 不同。计数仅适用于所测注入/版本/地图及调用窗口，未证明实际可碰撞或全赛程场景；见 `experiments/line_instances_{5,6}_20261105/`。stage 5/6 各六帧初始位置 LiDAR 原始 payload 共 12 帧，点数 14773–17097，XYZ 均有限且哈希匹配；采样帧不同，不能把点数差直接归因于 seed 或室外障碍。见 `experiments/lidar_snapshot_{5,6}_20261105/`。

粗/细线接触夹具先导试验在 stage 4、seed 123 用相同固定开环 PWM，并将第一条对应 HISM 提交变换临时替换为预先指定的世界原点夹具（中心 `(0,0,350)` cm、单位旋转与缩放）；无夹具对照不替换变换。坐标是预设输入，不由调试真值决定，真值只做事后统计。三场均发送 248 条命令：无夹具、粗线、细线的事后 NED z 范围分别约 `[-5.097,0.864]`、`[0.005,0.424]`、`[-0.015,0.461]` m；回调次数分别 21、47、25。夹具场的运动与对照不同，但尚未解码回调对方/组件，也未建立夹具世界变换与无人机接触的独立对应，不能将差异作为碰撞因果或通道结论。持续强制 state 7 会覆盖 state 12，GDB 暂停也影响调度。原始命令、输出、位姿、回调栈、LiDAR 帧和夹具前后变换分别见 `experiments/line_fixture_-1_pilot_20261105/`、`experiments/line_fixture_thick_pilot_20261105/`、`experiments/line_fixture_0_pilot_20261105/`；正在做多 seed 重复。

stage 4 的持续状态风与固定延迟开环 PWM 现已完成三 seed 各两次。每场风矢量出现 41–82 个相邻变化样本；按 ROS header 最近前一条 PWM 命令近似对齐，各场在非零命令阶段分别有 9–33 个变化样本。命令器仅按固定时间表发布，不订阅真值；原始风、命令、实际 PWM、IMU/GPS、事后位姿及重建误差见 `experiments/wind_pwm_transition_4_20261105/`。这是同一采样窗口的关联，不证明扰动导致某个运动变化，也不构成已校准的端到端延迟或动力学辨识。

另一个粗线夹具场对前五次碰撞回调读取分派事件指针 `r12` 的 96 字节；五次开头均为 vtable 值 `0xbacf60` 和类型值 29。所读范围不证明对象长度，后续字节可能是相邻内存，不能视作已解码的碰撞对象；尚不能把回调关联到夹具。原始事件和栈见 `experiments/line_fixture_event_pilot_20261105/`。

stage 5 持续风过渡与固定延迟开环 PWM 已覆盖三 seed 各两次。每场 41–77 个风矢量变化样本，其中近似 header 对齐到非零命令阶段的数量为 1–37；不同场启动/调度时间会改变重叠数量。完整消息、姿态事后统计与 `overlap_analysis.json` 在 `experiments/wind_pwm_transition_5_20261105/`；不能将重叠解释为因果或已标定延迟。

固定夹具实验的无夹具对照已完成三个 seed 各两次；六场事后 NED z 最小值约 -5.14 至 -4.80 m，均保留固定 PWM、LiDAR 原始帧及碰撞回调日志，见 `experiments/line_fixture_control_20261105/`。粗/细夹具组仍在采集；对照数据不能单独证明接触或归因。

stage 6 持续风过渡与固定延迟开环 PWM 的三 seed 各两次已完成；seed 456 的两场、2026 的两场有风变化与非零 PWM 命令近似重叠，而 seed 123 两场在首次风变化前均已回到零命令，不能声称其非零阶段覆盖动态风。保留 `experiments/wind_pwm_transition_6_20261105/`，另以预先固定的更晚命令时间做 seed 123 重试；该选择来自事后分析，而非飞行控制读取调试真值。

对上述五个事件补做符号核对：二进制导出的 `vtable for DroneHitEvent` 地址为 `0xbacf50`，五个运行时事件的 vptr 均为其地址点 `0xbacf60`（+16 字节），类型值均为 29。因此可以将这些分派事件识别为 `DroneHitEvent`，而非仅凭回调名称推测；仍不能识别碰撞对方，也不能确定事件后 96 字节读出是否属于同一对象。证据见 `experiments/line_fixture_event_pilot_20261105/event_analysis.json` 和 `event_vtable_symbols.txt`。

stage 6 seed 123 的补样改用预定的 6.5 秒延迟（依据前次事后时序选择），两场均有 41 个风向量变化样本，其中 35/37 个按近似 header 对齐落在非零 PWM 命令阶段；命令器仍不订阅真值。原来的未重叠场次不删除，补样原始数据见 `experiments/wind_pwm_transition_6_seed123_late_20261105/`。三个 stage、三个 seed、各两次均已获得动态风变化与非零开环命令近似重叠的样本，仍不能证明因果或标定时延。

细线固定世界原点夹具组六场完成：三 seed 各两次都按预设替换一次细线变换，248 条命令且观测最大 PWM 为 0.4；事后 NED z 最小值约 -0.079 至 -0.016 m，无夹具六场为 -5.142 至 -4.802 m。细线组碰撞回调 20–37 次、对照 19–22 次，范围有重叠。差异与夹具阻碍运动相符，但仍未解码回调对方或排除状态/调度影响；不能把该对照单独作为碰撞因果和通道证明。数据见 `experiments/line_fixture_thin_20261105/` 与 `experiments/line_fixture_control_20261105/`；夹具坐标为预设输入，调试真值只在事后统计。

原点夹具对照三组现均覆盖三个 seed 各两次。各场相同固定命令序列、248 条命令、最大观测 PWM 约 0.4；事后 z 最小值无夹具为 -5.142 至 -4.802 m、细线夹具为 -0.079 至 -0.016 m、粗线夹具为 -0.016 至 0.012 m。回调数分别为 19–22、20–37、43–49；粗线单独事件样本的 vptr 对应 `DroneHitEvent`。每个夹具场恰有一次预设变换写入，对照没有；这些差异与夹具阻碍运动并触发命中相符，但事件未携带已解码的对方组件，持续强制 state 及 GDB 暂停改变行为，不能把全部回调精确归因于夹具或宣布自然赛程碰撞通道验证。`experiments/line_fixture_comparison_20261105.json`、`scripts/analyze_fixture_comparison.py` 和三组原始流可复算；位置真值仅事后使用。

车辆随机桶在 stage 4 重定向六场中逐场采集：seed 123/456/2026 分别为 2/7/0，重复一致；从更新后随机流状态重建的桶和 `<6` 分支均匹配。样本不证明理想 `6/11` 概率或自然赛程中的相同随机流位置；见 `experiments/vehicle_branch_4_20261105/` 和 `scripts/analyze_vehicle_branch.py`。

reset 与不同随机流的直接对照首场：seed 123 的 ROS reset 返回 false，reset helper 入口的 FRandomStream current 从 2071961785，至随后游戏 reset 入口为 123；前后各 373 个粗/细组件提交变换的多重集合字节一致，而风关键帧数组不同。该现象与两类随机流重置方式不同相符，但不能证明 libc 状态变化的具体原因，或已成功重置物理场景。第三次风 dump 是断点遗留导致的第二数组重复输出。见 `experiments/reset_instances_pilot_20261105/reset_analysis.json`、`scripts/analyze_reset_instances.py`；三 seed 两次正在采集。
