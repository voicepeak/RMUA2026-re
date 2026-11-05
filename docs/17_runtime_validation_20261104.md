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

车辆固定路径夹具首场将强制初始化的五点路线改为预设世界坐标 x `[-5000,-2500,0,2500,5000]` cm、y=z=0，速度字段 667；输入不依赖调试真值。seed 123 记录到车辆组件世界 x 从约 -4999.94 到 3955.20 cm，期间有无人机碰撞回调，同时固定 PWM 飞行的事后 z 范围约 -5.03 至 1.03 m。回调对方未识别，且无人机和车辆未做同步碰撞体配准，不能将这些回调归因于车辆接触；见 `experiments/vehicle_fixture_pilot_20261105/`。正在追加无人机空载与车辆原路线对照。

reset 对照已扩展为三个 seed 各两次：六场服务响应均为 false；各场游戏 reset 入口 FRandomStream current 回到对应 seed，前后各 373 个线障组件提交变换多重集合哈希一致，而风数组哈希均不同。同 seed 的这两种结果重复一致；第二组风 dump 的重复是遗留断点输出，不能当成第三次生成。未证明物理场景成功重置或风随机状态改变的原因。完整数据和复算见 `experiments/reset_instances_20261105/`。

车辆空载对照在原路线及预设固定路线下各做三个 seed 两次，不发送 PWM。固定路径每场均写入一次，世界位置最近原点距离约 29–344 cm，最近样本前后一秒均无 DroneHitEvent；每场仍有两个其他时间的事件。seed 456 固定路线两次仅采到 12 个世界样本，其他 seed 为 26 个；原路线 seed 456 的轨迹也不同于其他 seed。世界原点不是已配准的无人机碰撞体位置，采样有限且事件对方未知，因此该负样本不证明车辆碰撞或其缺失。完整日志与分析见 `experiments/vehicle_fixture_idle_{0,1}_20261105/`。

车辆直接分支受控对照：在已记录的 stage 4 入口重定向和状态门强制基础上，首次运行至 `0x2fc89de` 时记录原 EAX 桶并临时置零，三 seed 各两次的原桶为 2、7、0，六次均初始化五点路径，路径字节 SHA-256 均为 `d055667cc723c7a55d2bdcbb5d4fa9e84098d43dadf36818229a68e2f8cd2cc0`，速度字段 667，并各有 12–13 个世界位置样本。覆盖桶、状态和代码调用为开发容器内注入，输入不使用调试真值；原二进制挂载只读。该对照说明 seed 456 在强制直接分支下路径可重复，不能确定自然间接分支路径不重复的原因。完整日志、脚本与复算见 `experiments/vehicle_forced_bucket_4_20261105/`、`scripts/run_stage_vehicle_forced_bucket.py` 和 `scripts/analyze_vehicle_dynamics.py`。运行器的 `observed` 基于原采集版本的风/线条件，车辆桶及路径应以 analysis 和 GDB 日志另行核查；脚本已加入后续运行的显式检查。

复现车辆桶固定对照（`SIM_DIR` 指向只读官方模拟器目录，`ROS_WS` 指向消息工作区；输出目录须不存在）：

```bash
python3 scripts/run_stage_vehicle_forced_bucket.py --stage 4 --simulator "$SIM_DIR" --settings experiments/training_host_20261105/settings.json --messages "$ROS_WS" --output /tmp/rmua-vehicle-bucket --seeds 123,456,2026 --repeats 2 --sample-seconds 5 --port 12770
python3 scripts/analyze_vehicle_dynamics.py /tmp/rmua-vehicle-bucket
```

对完整 false reset 矩阵追加独立离线连续流检验：每场对 glibc 2.35 仅执行一次 `srand(seed)`，调用已有 float32 模型两次，中间不播种、不额外消费。六场的前后 35 项风数组共 12 组均与观测字节完全相等，各次生成消耗 54 个 `rand()`。这为 wind reset 后变化提供可复现的连续流解释，且比仅比较哈希更强；但输出一致无法排除产生相同状态的隐藏操作，实验服务均返回 false，不能推广为成功 reset 的行为。证据与复算：`python3 scripts/analyze_reset_wind_stream.py experiments/reset_instances_20261105`，输出 `wind_stream_analysis.json`。模型允许 seed 为 None 以继续当前 libc 状态；重新核验原 stage 4/5/6 三 seed 各两次 fresh 数据仍全部逐字节吻合。

碰撞组件归因补充：反汇编 `0x2630780` 在调用事件分派前分配 16 字节、写入 `DroneHitEvent` 的 vptr 和类型 29；因此旧日志对事件指针读取的 96 字节中，16 字节之后不能作为事件字段。该入口断点采集到栈顶返回地址 `0x50a2f1e`，其前一条指令 `0x50a2f18` 通过虚表间接调用，按该点之前的实参装载可核对事件来源 RCX 的组件身份。证据在 `experiments/line_event_source_static_20261105/` 和 `experiments/line_event_source_retry_20261105/`。

新探针在粗线实例提交处记录 HISM 组件指针，并在事件源入口记录参数；stage 4 三 seed 各两次、固定开环 PWM 下，每场分别有 39–43 个事件源 RCX 与粗线组件相同，另有 4 个指向不同组件。指针只在同一进程内比较，粗线 HISM 在场景中承载多个实例，因而不能仅据此断言命中了固定夹具那一根，或把全部事件归因于夹具；状态注入和 GDB 也影响行为。完整日志与统计见 `experiments/line_event_component_thick_20261105/`、`scripts/analyze_line_event_component.py`。首场 `experiments/line_event_source_pilot_20261105/` 的固定 PWM 发布因 RotorPWM 类型缺失失败，保留为失败样本；发布器增加按已核对 MD5 动态生成消息的回退后，重试及六个粗线样本发布成功。

复现粗线组件对照（`SIM_DIR`、`ROS_WS` 为已有目录，输出目录须不存在；细线使用 `--selector 0`）：

```bash
python3 scripts/run_line_event_source.py --selector 1 --stage 4 --simulator "$SIM_DIR" --settings experiments/training_host_20261105/settings.json --messages "$ROS_WS" --output /tmp/rmua-line-event --seeds 123,456,2026 --repeats 2 --sample-seconds 6 --port 12840
python3 scripts/analyze_line_event_component.py /tmp/rmua-line-event
```

细线组件重复对照已完成：stage 4 三 seed 各两次，固定开环 PWM 全部成功，每场有 16–24 次事件源 RCX 与该场提交细线实例的 HISM 组件匹配，另有 4 次指向不同组件。完整数据和可复算统计在 `experiments/line_event_component_thin_20261105/`。和粗线一样，HISM 指针只确认组件身份，不能单独区分该组件的实例或证明自然赛程碰撞力。

进一步跟踪具体粗线实例：预先选择第 8 个粗线 HISM 提交（ordinal 7），其返回索引在三 seed 各两次均为 7；在事件源入口读取的独立 136 字节命中结果块中，同组件事件的 `+0x58` 有符号 32 位值均为 7，每场 39–46 个；每场其他组件的 4 个事件该值为 -1。另一个第 1 个提交的先导场，其首个返回索引为 0，同组件命中值也均为 0。依据相同进程中的组件指针、索引和候选字段匹配，受控场的样本支持命中该夹具实例；该偏移仍需独立类型元数据或更多索引检验，不能推广到其他版本或自然赛程。第一版探针的临时返回断点意外对后续 373 次提交持续输出，保留 `experiments/line_hit_result_thick_pilot_20261105/`，修正后每场仅记录一次返回索引。全部原始字节与统计见 `experiments/line_hit_instance_thick_20261105/`、`scripts/analyze_line_hit_instance.py`；索引是预设输入，调试真值未进入控制。

细线第 8 个提交的实例对照已覆盖三 seed 各两次：每场返回索引均为 7，同组件命中结果 `+0x58` 均为 7（每场 16–29 次），其他组件每场 4 次均为 -1。与粗线结果共同支持这些注入场中的夹具实例命中，字段语义尚未由独立类型元数据核验。先前仅凭组件指针无法区分实例的限制仍成立；此处增加的是返回索引和原始命中块对照。详见 `experiments/line_hit_instance_thin_20261105/`。

复现指定实例对照：

```bash
python3 scripts/run_line_hit_result.py --selector 1 --stage 4 --fixture-ordinal 7 --simulator "$SIM_DIR" --settings experiments/training_host_20261105/settings.json --messages "$ROS_WS" --output /tmp/rmua-line-instance --seeds 123,456,2026 --repeats 2 --sample-seconds 6 --port 12880
python3 scripts/analyze_line_hit_instance.py /tmp/rmua-line-instance
```

细线将 `--selector` 改为 0，并使用独立输出目录和端口。

车辆命中来源比较先导：沿用固定五点路径和开环 PWM，事件源记录无人机碰撞组件、源 actor/组件，并与同一进程记录的车辆 actor 及采样组件指针比较。先导两场的命中来源均未匹配车辆 actor 或采样组件；有限采样、未识别子组件和强制状态意味着不能证明车辆碰撞不存在。两场使用 debug/wind 消息到达判断就绪，仅作诊断，严格约束的验收对照另用普通 IMU 就绪。

纠正记录：试图以先导场读取的调试位姿选择 z=400 cm 路径，违反调试真值仅用于事后评估的约束；试运行在车辆路径、stage、风和线注入前中止，没有有效实验样本，详见 `experiments/vehicle_contact_z400_pilot_20261105/INVALID.md`。另一个旧运行器批次因发现调试风就绪依赖被中止，记录在 `experiments/vehicle_contact_fixed_20261105/INVALID.md`。此后保持既有固定路径，启动就绪改为普通 IMU 消息，不以调试位置或风值调节动作。

严格就绪对照：沿用既有固定车辆世界路径和开环 PWM，改以普通 IMU 消息可用作为就绪条件，三个 seed 各两次均记录一次夹具写入、一个初始化车辆 actor，命中来源分别为 18、20、22、21、21、21 个，均不匹配本场的车辆 actor 或车辆采样组件；六次 PWM 发布退出码均为 0。事件来源、world 和姿态仅用于事后分析。窗口有限，actor 对应关系及未采样子组件仍有边界，不能证明车辆绝无接触或碰撞通道失效。完整数据见 `experiments/vehicle_contact_imu_gate_20261105/`。

复现命令（`SIM_DIR`、`ROS_WS` 为已有目录，输出目录须不存在）：

```bash
python3 scripts/run_vehicle_contact.py --fixture 1 --stage 4 --simulator "$SIM_DIR" --settings experiments/training_host_20261105/settings.json --messages "$ROS_WS" --output /tmp/rmua-vehicle-contact --seeds 123,456,2026 --repeats 2 --sample-seconds 12 --port 12950
python3 scripts/analyze_vehicle_contact.py /tmp/rmua-vehicle-contact
```

限制更正：此前 stage 6 seed 123 的 6.5 秒补样延迟根据调试风时序的事后分析确定，虽然命令器本身不订阅真值，但实验输入设计已使用该真值；按用户“调试真值只用于事后评估”的严格约束，该补样只能留作诊断失败记录，不能计入最终受限验收。

修复调试风就绪依赖后，粗线第 8 个提交的固定夹具和开环 PWM 对照以普通 IMU 消息判断就绪重跑：三个 seed 各两次，返回索引均为 7，各场 41–43 次同组件命中块 `+0x58` 均为 7，固定 PWM 发布均成功。调试风和位姿只采集供事后分析，命令时间表和夹具输入未读取其值。`experiments/line_hit_imu_thick_20261105/` 保存完整数据及 `hit_instance_analysis.json`；该偏移仍是当前版本受控推断，非已确认的跨版本 ABI。

细线第 8 个提交的普通 IMU 就绪复验同样完成三个 seed 各两次：每场返回索引为 7，16–20 次同组件命中块 `+0x58` 均为 7，固定 PWM 均成功。完整原始数据及分析在 `experiments/line_hit_imu_thin_20261105/`，复现命令同粗线，将 `--selector` 设置为 0，且使用独立输出目录与端口。

## 普通 IMU 就绪的长 PWM 复验与失败边界

`experiments/wind_pwm_imu_long_4_20261105/` 在 stage 4 持续状态注入下，对 seed 123/456/2026 各两次执行固定 20 秒对称 PWM：0（1 s）、0.2、0.3、0.4、0.2（各 4 s）、0（3 s）。命令器无订阅；运行器以普通 IMU 首条消息就绪，再并行采集风、PWM、IMU/GPS、LiDAR 和事后位姿。六场上升的三个非零阶段分别有 397–402 个同阶段匹配输出；后续 0.2 和 0 阶段均为零匹配，所有对应输出均为 0.4（各约 398–401、297–301 条）；初始零也没有匹配。调试日志中存在 old=12 被逐 tick 写回 7 的记录，尚不能由此确定输出不跟随的原因、接收端处理或物理电机状态。该实验不能支持下降沿时延、动力学或成功结束的声明。

每场风变化有 41 个样本按近似 header 对齐到前述 0.3/0.4 命令阶段；风与 PWM 的关联不是因果证明。六场 IMU 分别约 2300 条、LiDAR 各 232 条，单调钟到达间隔 p99 分别约 10.22–10.35 ms、99.39–104.52 ms；这些是订阅端间隔而非端到端时延。NullRHI 相机没有有效图像。输入时间表是在先前风采样之后制定，虽不在线读取真值，其设计来源无法证明独立于事后真值，故不能计入严格受限控制验收。先导场及失败输出在 `experiments/wind_pwm_imu_long_pilot_20261105/`；`phase_coverage.json`、`pwm_analysis.json`、`overlap_analysis.json`、各 `sensor_clock_summary.json` 和原始流可复算。`scripts/analyze_wind_pwm_overlap.py` 已限制到最后一条已记录命令，避免把命令结束后的风变化无限归属于最后阶段；旧分析文件仍保留其当时结果。

## PWM 分发链断点对照

复用 stage 4 长命令、普通 IMU 就绪和持续风状态注入，对 seed 123 做两场无新增断点对照：下降的 0.2 和 0 阶段均无同阶段匹配输出，分别采到 400–401、297–298 条输出。另在原 GDB 脚本上增加 `0x26ea0f0` 分发入口和 `0x26ea000` 异步执行入口 change-only 断点：先导及另一成功场在下降阶段两入口均记录到与输入一致的四个 float，PWM 输出分别有 399/400、387/400 个 0.2 匹配及 298/299、293/298 个零匹配。原始单调时钟、各阶段计数及分析见 `experiments/pwm_chain_pilot_20261105/`、`experiments/pwm_chain_trace_20261105/`、`experiments/pwm_chain_control_20261105/` 和 `scripts/analyze_pwm_chain.py`。分发入口反编译见 `artifacts/rea/physics_pwm_native.c`，其静态代码中模式为 2 时向异步函数提交四个 float。

这些断点使用 all-stop 调试并改变调度，成功组与对照组也不保证其他隐变量相同，因此结果支持实验条件敏感，不能证明停留由调度、碰撞或特定代码分支造成。两个入口也记录到其他未经独立类型/生命周期确认的内存字节；不能将其直接定性为损坏命令。断点计数仅记录值变化，并非完整调用计数。不同时间戳的首次差值不构成端到端延迟。带断点的另一场在加载 OL1 后、IMU 就绪前于 `0x331cb09` SIGSEGV，未采集命令；保留其 manifest 和 GDB 日志，不能推断八实例根因。固定时间表在既有风实验之后设计，其输入独立性未证，不计入严格受限控制验收。

## OL2 直接加载与门核验

构造独立开发补丁 pak，保留原包不变：第一种只将 Lobby 序列化 URL 中唯一的 OL1 等长替换为 OL2；补丁已挂载但仍加载 OL1。首个单文件嵌套挂载启动退出 125，stderr 未由旧 runner 保留，原因未确定。第二种在相同 Lobby 补丁中另将 `RMUA/Config/DefaultEngine.ini` 的 `GameDefaultMap` 设为 OL2，配合同一请求参数，日志显示直接加载 `/Game/Maps/RMUA2024OL/RMUA2024OL2?Name=Player`。不能单独由两种组合证明配置读取的全部细节。原包、补丁、内容和二进制哈希、patch 文件与生成脚本分别保存在 `experiments/ol2_overlay_pilot_20261105/`、`experiments/ol2_overlay_retry_pilot_20261105/`、`experiments/ol2_config_overlay_pilot_20261105/` 和 `scripts/build_lobby_map_overlay.py`。

`experiments/ol2_direct_gates_20261105/` 的 seed 123/456/2026 各两次均有实际 OL2 LoadMap、对应 seed 初始化及 12 个门 BeginPlay。按位置与 `artifacts/tables/gates.csv` 最近邻一对一关联，每场 12/12 与 OL2 静态表匹配，最大位置差小于 0.000001 cm；首场最小第二候选距离约 2809.96 cm。以 seed 123 首场为基准的 60 个逐门比较中，位置、四元数角和缩放差均为零；FName 数值也相同。关联不独立解码 actor 名称，采样只覆盖 BeginPlay，缩放不等于门洞碰撞尺寸。原始 GDB 日志、manifest 和 `gate_comparison.json` 可复算。

独立的 seed 123 长等待试场直到 90 秒就绪超时仍只有 `/rosout`、`/rosout_agg`，并在加载后报告 `Exception occurred while updating world`；无 IMU 不能宣称无人机可飞行，错误原因未定位。六个门试场按实际 LoadMap 和 12 条事件停止，不检查传感器；其 `observed` 不代表世界更新或训练接口正常。

## 直接启动的 ROS 初始化诊断

为比较是否 OL2 特有，使用相同 overlay 机制将默认地图改为 OL1，seed 123 直接加载 OL1，观测 12 门，但普通 IMU 就绪等待 30 秒失败、引擎输出世界更新异常；其 ROS 图快照只有 `/rosout` 和 `/rosout_agg`。正常经过 Lobby 的 OL1 才已观测普通 IMU。该单场对照支持直接启动路径或初始化时序是共同因素，不能单独证明 Lobby 是唯一原因。原始记录及补丁哈希见 `experiments/ol1_direct_control_pilot_20261105/`。

OL2 直接加载后用只读 GDB 断点捕获 12 门之后首次 `__cxa_throw`：RTTI 为 `ros::TimeNotInitializedException`，栈依次包括未符号化的 `librostime.so` 帧、`airsim_ros::record_frequency_data`、`airsim_ros::AirSimROSLidar`、主程序 `0x26ed2d6`。见 `experiments/ol2_direct_throw_pilot_20261105/`。此异常与随后世界更新异常日志时间相近，但未独立证明日志由该次异常产生；all-stop 调试改变线程时序。

进一步在开发容器中 preload 一个仅调用 `ros::Time::init()` 的构造器：该试验不修改官方文件。三场均在 IMU 就绪前 SIGSEGV；后两场取得完整栈，为 LiDAR → `record_frequency_data` → map `count/find/_M_begin`；最后一场在 `record_frequency_data` 返回地址 `0x184be6` 对应库内的静态 `node_instance` 地址读出 NULL。已保存 GDB 栈、原始日志、preload 源码、编译命令和哈希：`experiments/ol2_ros_time_init_pilot_20261105/`、`experiments/ol2_ros_time_init_crash_trace_20261105/`、`experiments/ol2_ros_time_init_node_trace_20261105/`。`artifacts/rea/frequency_rea.json` 和本机库 `0x184bc6` 反汇编显示该频率表在 `node_instance+0xb0`，与该次空指针读取相符；不证明多实例此前崩溃为同一原因。此 preload 不构成修复或时延验收。

## 频率门控与地图切换诊断

在开发 GDB 场中，以已核对版本的 `record_frequency_data` 入口（库偏移 `0x184b87`）读取 `node_instance`（库偏移 `0x26ad60`）；仅指针为空时将函数提前返回，节点非空时撤除断点。seed 123 先导场实际跳过一次，随后命中 `InitAirSimROS` 并观察到非空节点与普通 IMU；约五秒后实际 LoadMap 为 OL1。该实验支持早期频率统计阻断初始化的受控解释，但调试器修改了函数行为和线程调度，不是官方启动修复。见 `experiments/ol2_frequency_gate_pilot_20261105/`。

只读 Browse 探针及 URL 字段检查显示，实际切图调用的候选结构 `+0x28` 为 `/Game/Maps/RMUA2024OL/RMUA2024OL1`；日志和转储见 `experiments/ol2_browse_trace_pilot_20261105/`、`experiments/ol2_url_fields_pilot_20261105/`。随后在同一开发场把该精确 UTF-16 字符串等长替换为 OL2，并保留前述频率门控。三个 seed 各两次均见初始 OL2 加载完成、重定向记录、第二次 OL2 LoadMap 入口、24 个门事件；采样时各有 500–501 条 IMU 和 50–51 条非空 LiDAR，NullRHI 相机为空。每场只出现一次 OL2 加载完成日志，不能确认第二次加载已完成或将这些 ROS 消息归属于第二个世界；旧 runner 的 `observed` 仅检查入口、注入和采样条件，不代表后者，已在后续 runner 增加完成日志检查。`experiments/ol2_redirect_20261105/post_load_analysis.json`、原始日志和 `scripts/analyze_ol2_redirect.py` 保存这一负向核验；先导结果在 `experiments/ol2_redirect_pilot_20261105/`。

单 seed 调试中断快照显示采样结束时主线程位于 `usleep` 调用，而非当时停在 ROS 初始化栈；单次快照不能证明整体无死锁或切图完成。另一场在捕获第二组 12 门后未见 `0x2fb8f40` 游戏状态更新断点；日志输出可能缓冲、探针也改变调度，不能单凭该负结果认定游戏状态永不更新。见 `experiments/ol2_interrupt_stack_pilot_20261105/`、`experiments/ol2_world_tick_pilot_20261105/`。

另一次在调用点取消 OL1 切图并人为返回 0，以保留初始 OL2：在采样窗口内重复拦截 264 次切图请求，未命中预期的游戏状态更新断点；尽管 IMU/LiDAR 消息仍存在，不能认定世界正常推进。保留 `experiments/ol2_cancel_travel_pilot_20261105/` 为失败样本，禁止将旧 runner 的 `observed` 解读为训练通过。更改后的取消运行器会在反复请求或无更新证据时标记未验证；模拟器官方文件未修改。

## 经 Lobby 的 OL2 启动对照

复用官方原包、默认 Lobby 与独立 ROS master，在 Browse 内将精确 OL1 URL 等长写为 OL2；不再使用频率门控或修改包。seed 123 短场和长场均捕获 Lobby→OL2 的 LoadMap 入口及 12 门，ROS 接收 IMU/LiDAR，但在采样期间均未命中 `0x2fb8f40` 状态更新或 `0x2fb2040` 风播种，未看到 OL2 加载完成日志。长场 25 秒调试风后，分别采样约三秒的普通 IMU（270 条）、GPS（28 条）和仅供事后评估的调试位姿（270 条）：GPS 三轴位置均为 0，调试位姿位置均为 0、四元数模恒为 0.5；零位置单独不证明错误，非单位四元数和无状态更新不能用于验收。数据、`sample_analysis.json`、时间序列及 manifest 在 `experiments/lobby_ol2_long_pilot_20261105/`；该真值不用于动作或就绪判断。

同 seed、相同探针和静止采样的无重定向 Lobby→OL1 对照观察到 45 个按约一秒节流的状态更新记录、一次风播种和一次游戏 reset；GPS 三轴均有非零或变化，事后姿态四元数模为 1。控制场自身也没有目标地图 `LoadMap Took` 行，故此前仅凭该日志缺失判断地图未完成并不充分；但 OL2 未命中更新和非单位调试姿态仍使其训练就绪未证。对照见 `experiments/lobby_ol1_control_pilot_20261105/`；仅单 seed 单场，不能推断统计稳定性。

将改写点从 Browse 日志辅助函数提前到 Browse 入口 `0x593c900`，根据反汇编在入口读取 `RDX` 所指的候选 URL（`+0x28`）；仍然只修改精确匹配的 OL1 等长字符串。单场观察到 OL2 入口和 12 门，但没有状态更新/风播种，27 条 GPS 位置均为零，272 条事后姿态四元数模均为 0.5。该结果反驳“仅因改写发生太晚即可恢复”的简单假设，不排除其他 URL 内部状态、地图配置或初始化路径。证据见 `experiments/lobby_ol2_early_pilot_20261105/` 和 `browse_entry.disasm`。

`experiments/lobby_ol2_init_return_pilot_20261105/` 的 GDB entry/finish 探针观察到一次 `InitAirSimROS` 入口及返回，故本场不能解释为该函数没有返回；这不证明后续世界初始化成功。插件函数的静态 REA Evidence `ev_1036822c48affb93d5b0ad58a01ba217a650fe0f13e4e4ea3a44db3408ec7832` 导出在 `experiments/lobby_ol2_long_pilot_20261105/rea_initialization_evidence.json`，反编译显示先 `ros::init`，再构造并设置 `node_instance`；静态结果本身不证明运行时调用顺序。GDB 修改 URL 和 all-stop 调试均限制因果解释。

## reset 检查来源与受控成功响应

`getpose_ue` 的原生分发将响应前 16 字节置零，再将全局 `0x715c3d0/3d4/3d8` 写至 `+0x10/+0x14/+0x18`；函数 `0x26f2160` 从事件对象 `+0x10/+0x14/+0x18` 写入这三个全局值。事件产生路径读取 actor 根组件 `+0x1d0/+0x1d4/+0x1d8`，动态记录中的事件 vptr `0xbacf38` 与导出 `SyncDroneLocationEvent` 虚表地址点对应；受控 reset 场记录该写入者的最后分量从接近 +3 降到约 −37.9，服务检查时仍为约 −37.9。静态摘录、命令字符串和版本哈希见 `experiments/reset_cache_static_20261105/`；运行日志见 `experiments/reset_cache_{trace,source}_pilot_20261105/`。静态 vptr/字段解释限该版本，不单独证明真实世界动力学。

在 `experiments/reset_root_hold_20261105/` 的 stage 4 注入场，普通 IMU 就绪后调用官方 reset：仅在 ROS reset 回调运行时，在事件生产入口把根组件世界 z 写为 0，并在回调读取完成时停止写入；没有改写服务响应或其检查结果。seed 123/456/2026 各两次均记录 21–23 次高度写入、检查块七个 float 为 0、服务响应 `success: True`。该干预改变 actor 根组件，不证明无干预的物理 reset 成功；已知 reset 检查对 `+0x0c` 使用 `abs(value)-1 < 0.2`，在本实现写零时仍通过，不能由此推断有效姿态。先导场见 `experiments/reset_root_hold_pilot_20261105/`。

六个成功响应场在 stage 4 前后各提交 373 个线障变换，每场前后多重集合哈希相同；风数组前后不同。离线 float32 模型按每 seed 仅播种一次连续生成两次，六场前后 12 组风数组逐字节匹配，每次消耗 54 个 `rand()`。匹配不排除产生等效状态的隐藏操作，也不能推广正常赛程。释放固定后另采普通 IMU 与仅事后评估的姿态：姿态 z 在各自采样窗口恒定，六场约 0.3085–0.3088 m；该观测不证明物理引擎内部状态或其他机制已重放。原始流、`root_hold_analysis.json`、`reset_analysis.json` 和 `wind_stream_analysis.json` 可复算；第二组风 dump 的额外副本由遗留返回断点产生，不计作第三次生成。

## 受控恢复风随机状态与失败跳转

对 stage 4 成功响应场追加开发干预：预加载 `scripts/wind_reseed_interposer.c` 构建的共享对象，在识别到首次风生成已到达选择调用点后，于下一次风生成的首个 duration 随机调用点、调用真实 `rand()` 之前，恰好一次调用真实 `srand(seed)`。用户输入的 seed 来自启动参数环境；策略不读取调试真值。此 interposer 将进程内 `rand()` 调用经互斥锁串行化并修改全局 libc 状态，依赖本版本非 PIE 调用地址，可能改变调度，不能视为官方行为或直接用于其他场景。构建命令、源码/共享对象/镜像哈希及触发约束见 `experiments/reset_wind_interpose_pilot_20261105/interposer_manifest.json`。

`experiments/reset_wind_interpose_20261105/` 对 seed 123/456/2026 各两次均观察到一次目标 seed 重播种、根高度固定 22 次及 reset `success: True`。每场前后各 373 个线障提交变换多重集合哈希一致，风数组各 35 项逐字节相同；12 组数组分别与独立 float32 模型按对应 seed fresh 生成的结果逐字节相同。每场干预释放后的事后位姿 z 在约 0.30858–0.30870 m；这只覆盖短时采样，不证明内部物理状态、实际 spawn、车辆、碰撞或完整场景重放。原始日志、`reset_analysis.json`、`reseeded_wind_analysis.json`、`root_hold_analysis.json` 可复算。

失败样本：最初尝试在风生成入口通过 GDB 改 RIP 构造对 `srand` 的调用，出现 begin/rejoin 标记但风不相同；追加对可执行文件 PLT 的只读断点，在两标记之间未捕获 `srand`，前 12 次 `rand` 返回与 seed 123 连续流的第 55–66 次逐个相等。该窗口只覆盖该 PLT，不能排除其他入口的隐藏调用，但足以否定将标记当作已验证重播种；调试跳转还多执行了生成器并可能污染栈，故该场不能用于重放。证据和失败说明见 `experiments/reset_wind_reseed_pilot_20261105/`、`experiments/reset_rng_trace_pilot_20261105/failed_reseed_analysis.json`。

stage 5 seed 123 单次先导使用同一 interposer 和根高度固定，并在 reset 前后分别采集仅供事后分析的姿态：服务响应 true，前后各 1118 个线障提交变换集合哈希一致，风数组各 37 项前后逐字节一致并均匹配 stage 5 独立模型。两段各约两秒采样中，前段末位置与后段首位置相距约 0.000698 m；没有验证速度、接触或同一真实物理状态，不能凭静止位置判断完整重放。结果及原始流在 `experiments/reset_wind_interpose_stage5_pilot_20261105/`。

stage 6 seed 123 单次先导使用相同固定高度和 interposer：ROS reset 返回 true，前后各 1434 个线障提交变换集合哈希一致，35 项风关键帧前后逐字节相同且均与 stage 6 离线模型吻合。事后位置前段末与后段首相距约 0.0000256 m；采样为静止场且间隔非零，不证明速度、碰撞、车辆或完整场景重放。原始数据、各分析结果见 `experiments/reset_wind_interpose_stage6_pilot_20261105/`。

stage 5 在同一根高度固定和 interposer 条件下扩展到 seed 123/456/2026 各两次：六场服务均 true，根高度固定 13–14 次；每场前后各 1118 个线障提交变换集合一致，37 项风数组前后逐字节相同，并与对应 seed 的独立 stage 5 模型逐字节一致。事后姿态前段末与后段首位置差约 0.000179–0.000884 m，采样是静止且分时进行，不能推断速度、碰撞或完整物理场景重放。见 `experiments/reset_wind_interpose_stage5_20261105/` 的 manifest、原始流、`reset_analysis.json`、`reseeded_wind_analysis.json` 和 `pose_prepost_analysis.json`。

stage 6 在相同根高度固定及 interposer 条件下扩展到 seed 123/456/2026 各两次：六场 reset 响应均为 true、每场根高度写入 9–11 次；前后各 1434 个线障提交变换集合一致，35 项风数组前后逐字节相同，并均与对应 seed 的 stage 6 离线模型逐字节一致。前段末与后段首的事后静止位置差约 0.000162–0.000520 m；分时采样不能证明速度、接触或整个场景重放。原始流与分析见 `experiments/reset_wind_interpose_stage6_20261105/`。

两个 interposer 运行器现对模拟器主程序 SHA-256 做前置检查，非目标文件在启动容器前被拒绝；已有 stage 4/5/6 数据目录保存执行时原版运行器，其哈希与 manifest 对应。验证只保障调用地址所依据的主程序身份，不证明镜像、共享库或所有配置与其他环境等价。

2026-11-05 车辆接近先导：z=0 路径的推定车辆子组件静态凸包包含无人机根原点样本，但未观测到车辆来源命中；移动调用整数参数 0/1 的单 seed 成对测试同样未得到车辆命中。采样、子组件类型、物理碰撞配置及调试干预限制详见 `docs/18_acceptance_ledger.md`。

七实例追加场在全部 IMU 就绪约 9 秒内失败：seed 789 退出码 139，Docker 非 OOM；其他六个由批处理清理。订阅观测器接近崩溃才启动，未保存任何消息或建立连接的证据，不能认定订阅导致崩溃；无调用栈，无法与先前故障归因。此前两次 300 秒存活只代表其具体空载窗口，不能称七实例稳定上限。`experiments/concurrency_7_subscribed_300s_20261105/failure_analysis.json` 保留时间、Docker 状态和观测器被手动中断的缺口。

四实例持续订阅复测：seed 123/456/2026/789 在全部 IMU 就绪后完成 345 秒存活和清理，其中每路 IMU/LiDAR 流式观测约 300.04–300.05 秒；IMU 约 100 Hz、LiDAR 约 10.08–10.09 Hz，模拟器进程约 109.6–110.1% 单核 CPU、RSS 峰值约 761.8–764.6 MiB。seed 2026 的一条 IMU header stamp 为 0，相邻序号为 5569、5570、5571，不能将该零值解释成有效时钟的回退；四路相邻非零 stamp 均未回退，各有 3–8 对相邻 stamp 相等。只验证本机静止、无 PWM、无相机负载的一次有限窗口，不证明长期或训练稳定，也不能证明零时间戳来源。`experiments/concurrency_4_stream_300s_20261105/stream/independent_analysis.json` 和逐条原始流可复核。

ROS 传输路径取得首场函数级定时：容器内编译的 LD_PRELOAD 钩住 IMU/LiDAR/PWM 发布函数与 PWM 命令回调，按非零且双方唯一的 header stamp 把同一宿主单调钟入口与接收回调配对。seed 123、NullRHI、12 秒先导中，IMU 1201 对中位/p99 为 0.235/0.385 ms，LiDAR 121 对为 0.455/0.564 ms，PWM 输出 1202 对为 0.156/0.328 ms；命令发送返回后的采样时间至模拟器命令回调 237 对为 0.270/0.427 ms，后者不是完整的发布到接收耗时。钩子和观测器改变时序，未计物理采样、动力学和执行器延迟；具体条件、非配对消息和钟域检查见 `experiments/transport_probe_container_pilot_20261105/transport_analysis.json`。首个使用宿主编译库的试场因容器缺少 GLIBC_2.34 启动失败，证据保留在 `experiments/transport_probe_pilot_20261105/`。

同主机 NullRHI ROS 传输探针现覆盖 seed 123/456/2026 各两次：每场唯一配对 IMU 1201–1203、LiDAR 121–122、PWM 输出 1202–1203、命令回调 233–237；各场 p99 范围依次为 0.324–0.385、0.561–0.655、0.241–0.335、0.338–0.431 ms。最后一项从 rospy 发布返回后记时，不代表完整发送延迟。六场固定 PWM 序列的五个非初始转换共 30 段，从首个匹配命令回调到首个匹配调试 PWM 发布为 15.63–62.20 ms，前一已配对输出均不匹配该阶段；初始零命令阶段均无匹配输出，且未匹配或排除的记录保留。阶段时间是相关观察，不证明物理电机/机体响应或真实传感器采样延迟，钩子与观测器会改变时序；输入策略沿用既有诊断序列，不能据此宣称设计独立。详见 `experiments/transport_matrix_20261105/summary.json` 和各场的 `pwm_phase_transport_analysis.json`。

无 LD_PRELOAD 对照：同固定动作和普通 ROS 观测器下，seed 123/456/2026 各一场的初始零阶段无匹配调试 PWM，后续五个阶段均有匹配；从发送返回到首条匹配输出分别约 29.22–61.34 ms。六场探针组与三个对照组均出现匹配，但样本量和时间顺序不足以确定探针是否改变延迟或解释先前强制风场中的输出停留。反编译 `PublishRotorPWM` 显示数值来自 GET_ROTOR_PWM，而 header 时间来自单独的 GET_IMU_DATA；不能把该 header 当作 PWM 采样时间。`PublishIMU` 的 header 来自 GET_IMU_DATA 返回时间戳，但物理采样时刻仍未确认。证据见 `experiments/transport_control_comparison_20261105/summary.json`、`artifacts/rea/ros_transport_functions_rea.json`；Ghidra 分析地址相对 ELF 增加 0x100000 基址。

车辆来源正向事件先导：在 seed 123、注入 stage 4 的调试场中，预设车辆路径终点靠近初始无人机位置并在 IMU 就绪后延迟 10 秒发送固定开环 PWM。66 个 `HIT_SOURCE` 中 13 个同时匹配本场车辆 actor 和 `actor+0x240` 推定子组件，首次匹配发生在 0.2 命令后，最近车辆 tick 的进度为 5027.88 cm；采样车辆位置已停止变化。196 对紧邻采样有 149 点落入推定静态凸包，几何计算是辅助证据。夹具路径与动作时间参考过往事后观察，运行时动作不读取真值；GDB 注入和逐 tick 状态覆盖使其不能代表自然赛程终止、碰撞力或完整接触流形。见 `experiments/vehicle_parked_pwm_pilot_20261105/parked_contact_analysis.json`、`mesh_proximity_analysis.json` 和原始事件。

停车夹具 seed 123 的无 PWM 对照记录 2 个非车辆命中，车辆 actor 与推定子组件同时匹配 0 次；197 对近时采样中 141 点位于推定凸包。与先导 PWM 场的 13 次车辆来源事件形成受控差异，但只有一对运行，且状态、路径和调试调度均被干预，不能单凭它判定接触力或推广到正常赛程。见 `experiments/vehicle_parked_idle_seed123_repeat1_20261105/parked_contact_analysis.json`。

停车夹具 PWM 场完成 seed 123/456/2026 各两次：六场车辆 actor 和推定子组件同时匹配的来源事件数依次为 13、12、12、12、12、12；每场首个匹配事件在首个非零命令发送记录后约 0.034–0.075 秒。静止对照仍只有 seed 123 一场，故不构成统计因果估计。GDB 强制 stage、路径与状态，动作设计参考过往事后真值，且游戏状态可被后续强制写入覆盖；不能将结果作为正常赛程终止或物理冲量的证明。逐事件及几何配对见 `experiments/vehicle_parked_matrix_20261105/summary.json` 和各运行的 `parked_contact_analysis.json`。

车辆来源到游戏回调的指针链先导：在相同停车路径及 PWM 条件下，于 `0x2630797` 记录 `DroneHitEvent` 分配指针，并在 `0x2fb7110` 记录回调事件参数；按时间和指针对应，66 个分配均与回调匹配，其中 12 个对应本场车辆 actor 和推定子组件。相同 12 次入口捕获 `r8=1`、`r9` 所指 0x88 字节和 XMM 原始值；结合调用处装载，候选位置约 x=-2.15..7.11、y=-4.22..8.68、z=-81.22..-80.61 cm，候选法向约 (0,0.0471,-0.9989)，候选冲量向量均为零。向量标签仍是调用约定推断；调用方在该路径直接清零相关寄存器，传参为零不证明实际物理接触力为零。GDB 可改变调度且状态强制可覆盖终止；不将事件延迟当作正常时延。见 `experiments/vehicle_contact_detail_pilot_20261105/contact_detail_analysis.json`、原始 gdb.log 和 `experiments/line_event_source_static_20261105/caller_disassembly.txt`。

运动后 reset 先导：seed 123 stage 4、固定开环 PWM 后调用官方服务；在高度固定与显式风重播种干预下返回 true，前后 373 个线障提交集合和 35 项风数组一致。只读 API 运动状态在动作窗口出现最大线速度模约 7.39、位置 z 最低约 -4.53；reset 入口前 23 ms 的位置约 (1.116,0.170,0.298)，线速度为零。检查标记后约 1.06 秒的样本位置约 (0,0,0.308618)，距动作前静止样本约 0.000160，线速度与动作前均为零。位置/姿态/线速度偏移由导出访问器汇编支持，坐标与单位仍须独立核验；此场 reset 前速度已为零，不证明运动中速度重置，也不证明完整物理、碰撞或车辆重放。见 `experiments/reset_motion_kinematics_pilot_20261105/kinematics_analysis.json`、`reset_analysis.json`、`reseeded_wind_analysis.json` 与 `artifacts/rea/kinematics_*.txt`。

运动中即时 reset 先导：预设 PWM 脉冲与 reset 调用在同一 ROS 节点内连续执行，不读取调试真值。seed 123 的 API 运动状态在 reset 入口前约 98 ms 记录线速度模约 0.931、位置约 (0.060,0.002,-4.437)；服务在高度固定及显式风重播种干预下返回 true，前后 373 个线障提交和 35 项风数组一致。检查标记后约 1.03 秒的样本仍在运动，位置距动作前约 1.834、线速度模约 4.933；约 2.09 秒后位置距动作前约 0.00010、线速度为零。节点在服务返回后发送零 PWM，根组件固定和调试暂停亦影响动力学，因此这些是分时状态观察，不能单独归因于 reset 或证明完整物理重放。见 `experiments/reset_moving_kinematics_pilot_20261105/kinematics_analysis.json` 和原始命令、事件与 GDB 数据。

对即时 reset 先导补充标记附近采样：桥接检查块记录为七个零 float，其前约 9.99 ms 的 API 运动状态位置 z 约 -1.988、线速度 z 约 -4.531。两个输出来自不同接口和坐标/采样时刻，不能直接做坐标差；它们说明受干预的检查响应不等同于同时验证 API 运动状态已静止。

运动中即时 reset 扩展至 stage 4 三 seed 各两次：六场在根高度固定和显式风重播种下均返回 true，前后各 373 个线障提交集合一致、35 项风数组与对应模型逐字节一致。服务入口前最近采样相隔约 24–108 ms，API 线速度模约 0.859–0.975；检查标记后约 1 秒，位置与动作前的差约 1.423–1.834、速度差约 4.933–5.367；约 2.01–2.09 秒后六场速度差均为零，位置差约 0.000088–0.009425。单位和坐标需独立核验；最后样本是有限分时观察，命令节点在服务返回后发送零 PWM，GDB 和重播种均影响行为，不能证明原生 reset、完整物理、碰撞或车辆重放。逐场及复算见 `experiments/reset_moving_matrix_20261105/summary.json`。

运动轨迹重放先导：seed 123 的固定开环 PWM 序列在受控 reset 前后各执行一次；服务返回 true 后固定发送零 PWM 三秒，再重放同一序列。按各阶段单调钟时间每 0.2 秒配对、每侧最近采样误差不超过 75 ms，18 对 API 运动状态中最大相对位置差约 0.174、最大线速度差约 0.607；前后发布命令数分别 174 和 173。该场未同步记录实际 PWM 输出，不能证明连续执行输入相同；三秒等待基于先前事后观测选定，未用于在线读取真值。GDB 干预、不同仿真步进和异步采样均限制比较；该单场不足以证明完整轨迹、碰撞或车辆重放。见 `experiments/reset_trajectory_replay_pilot_20261105/trajectory_replay_analysis.json`。

同步 PWM 观测的轨迹重放先导：seed 123 两个非零阶段分别收到 249 和 250 条 PWM 调试输出，其中 244 和 245 条四路值匹配命令；首个匹配输出距首个非零命令记录约 56 ms。18 对近时 API 状态中最大相对位置差约 0.413、最大线速度差约 6.768；两项均发生在阶段约 2.4 秒，其两侧采样时间误差分别约 46 和 25 ms。匹配部分输出不能证明连续输入一致；异步采样、GDB 干预及 reset 后零 PWM 影响结果，此场不能认作成功的完整轨迹重放。原始数据和配对见 `experiments/reset_trajectory_pwm_pilot_20261105/trajectory_replay_analysis.json`。

同步 PWM 的运动轨迹重放扩展至 stage 4 三 seed 各两次：六场均 true，前后各 373 个线障提交集合与风数组哈希一致。每场有 18 对 API 运动状态，每侧距目标时刻不超过 75 ms，两侧实际阶段时间最多相差约 98.4 ms；各场最大相对位置差为 0.262–0.499，最大线速度差为 0.346–6.768。每段非零动作窗口约 249–251 个 PWM 调试采样中有 239–250 个匹配目标值，不能据此证明连续输入完全相同。大差异可能受快速接触及异步采样影响，原因尚未确认；不能称完整轨迹重放通过。调试器、stage/根高度干预、风重播种及依据先前事后观察选择的三秒等待均限制外推。数据与配对见 `experiments/reset_trajectory_matrix_20261105/summary.json`。

受控 reset 碰撞重放先导：seed 123 的固定动作 reset 前后各触发 17 个命中，均指向同一进程中的同一 actor/component 指针；按事件顺序比较阶段相对时间，最大绝对差约 2.731 ms。服务在高度固定和风重播种下返回 true，前后各 373 个线障提交与风数组哈希一致；18 对 API 状态的最大相对位置差约 0.266、最大速度差约 1.186。事件按顺序配对并不证明物理接触完全一致，指针也未独立标识对象类型；GDB 暂停、固定三秒等待和注入均限制外推。原始和分析见 `experiments/reset_collision_replay_pilot_20261105/`。

本机离线模型验证：使用 `RMUA_ELF` 指向已校验 SHA-256 的目标二进制后 38 项测试通过，包括已有 18 场风关键帧回归。新增检查发现并修复有符号边界以上 seed 的 glibc TYPE_3 初始化，以及默认 ctypes 多实例共享进程全局随机状态的问题；默认流现在相互独立，显式 ctypes 后端仍共享 libc 状态。默认路径在本机首次运行失败和配置后的日志均保存在 `experiments/offline_model_host_validation_20261106/`；模型的引擎边界仍不等同真实碰撞或完整环境。

受控 reset 碰撞重放现覆盖 stage 4、三个 seed 各两次：六场服务均 true，前后各 373 个线障提交及风数组哈希一致；每场动作前后各有 16–18 个 `HIT_SOURCE`，同场前后均存在一个共同 actor/component 来源，但部分场次事件数不等。按事件顺序的描述性配对最大阶段时间差约 2.73–39.58 ms，不能证明对应物理事件相同。每场 18 对运动状态的两侧阶段时间最多相差约 104 ms；各场最大相对位置差约 0.092–0.499、最大速度差约 0.464–6.635。来源对象类型未独立确认；调试器、阶段注入、根高度固定、风重播种及事后选定的固定等待限制外推，不构成完整物理重放验收。见 `experiments/reset_collision_matrix_20261106/summary.json` 及各场原始记录。

车辆状态释放先导：在 stage 4 停车夹具场中，风状态强制写入开始约十秒后停止，释放时记录状态值 7；之后首次车辆 actor/推定子组件来源命中的事件对象，经分配指针匹配进入 `0x2fb7110` 回调，同线程随后请求 state 12。该场总计 12 个车辆来源事件对象与回调配对；此版只记录状态请求，尚未在赋值后直接采样状态。车辆路径、初始阶段和动作时序仍受开发干预，时序依据过往事后观察选择；不能当作普通赛程或不受调试影响的碰撞延迟。见 `experiments/vehicle_release_state_pilot_20261106/contact_detail_analysis.json` 与原始 GDB 日志。

车辆状态赋值先导：固定时间停止状态强制写入时记录 state=7；随后首个车辆 actor/推定子组件来源事件按指针进入碰撞回调，同线程在下一回调前执行 state 12 的赋值，赋值后读取 current=12、previous=7。此后 63 个被动状态采样均为 12，首末间隔约 14.26 秒；离散采样不能排除间隙变化，也不证明评分或整个 episode 终止。阶段、车辆路径和前十秒状态仍由 GDB 干预，释放及动作时序参考过往事后数据。见 `experiments/vehicle_release_state_v2_pilot_20261106/release_state_analysis.json`。

车辆状态释放矩阵覆盖三个 seed 各两次：六场释放时状态均为 7，各有 12 个释放后车辆 actor/推定子组件来源事件与回调按指针匹配；每场一个车辆回调之后、同线程下一回调之前观测到状态赋值 previous=7、current=12。之后 63–64 个被动采样均为 12，首末间隔约 14.20–14.49 秒。离散采样不能排除间隔内变化，也不验证评分或整个 episode 终止；初始 stage、车辆路径及前十秒状态由 GDB 干预，计时参考过往事后观察。详见 `experiments/vehicle_release_state_matrix_20261106/summary.json`。

并发根因诊断准备：针对目标 `libairsimros.so` 的三个频率统计导出函数构建开发用 LD_PRELOAD 互斥探针，独立夹具验证转发、float 返回、异常时解锁和多线程串行化。单模拟器 12 秒先导正常采到 IMU、LiDAR、PWM，探针前 5000 次入口累计至少一次 try-lock 竞争。该结果说明加锁运行中存在调用竞争，不证明原始未加锁场的实际数据竞争或八实例崩溃根因；直接内存访问和未覆盖调用仍可能存在，插桩会改变调度。构建与范围见 `experiments/frequency_guard_development_20261106/`，原始先导见 `experiments/frequency_guard_single_pilot_20261106/`。

探针中的 `contended` 字段准确表示 `std::mutex::try_lock()` 未立即获得锁的次数；由于 C++ 允许 try_lock 偶发失败，计数本身不是未插桩版本真实竞争的证明。

频率统计加锁八实例先导：固定八 seed、NullRHI、独立 ROS master，全部 IMU 就绪后观测 180 秒，八实例均存活并按 manifest 清理；逐次 liveness 记录覆盖约 179.5 秒。每实例周期性计数至少约 69000 次入口，try_lock 未立即成功累计快照为 1–24 次。仅为空载单次有限观察；探针改变调度、仅覆盖三个导出入口，try_lock 也可能偶发失败，不能因此认定数据竞争根因或修复八实例崩溃。见 `experiments/concurrency_8_frequency_guard_180s_20261106/guard_analysis.json`；同条件未加锁对照另行记录。

频率统计八实例对照：加锁及未加锁各一场，八 seed、主程序/settings 哈希和镜像 ID 相同，各在 IMU 就绪后完成 180 秒空载存活，liveness 采样覆盖约 179.5 秒；两场端口和运行时间不同。未加锁场也未复现故障，因此该对照不能判断互斥是否改变间歇崩溃概率，也不能确定既有崩溃根因。两场未持续订阅传感器或执行训练动作；见 `experiments/frequency_guard_control_comparison_20261106/comparison.json`、两组原始日志及 `artifacts/rea/frequency_getters_69d5ea.txt`。

四实例固定 PWM 并发短场：四路发布器均连接，在各自约 15 秒内分别记录 743 条预定动作；IMU、LiDAR 每路均与发布窗口重叠约 15 秒，IMU 约 100 Hz、LiDAR 约 9.1–9.9 Hz，所有进程正常清理。这是无在线真值的合成动作压力先导，命令被发布不证明每条被执行，也不代表完整训练或长期稳定。见 `experiments/concurrency_4_pwm_pilot_20261106/workload_analysis.json`；缺少开始/结束标记时重叠不被推定，分析单元测试已通过。

车辆离线最近组件距离数值修正：目标两个选择路径使用 float32 SIMD 乘加及平方根；此前模型在乘加阶段使用 Python 双精度。构造坐标 (29.60000038,34.5,23.10000038) 的独立 C float 夹具结果为 50.9903946，旧模型为 50.9903908；现已逐次 float32 舍入，40 项离线模型测试通过。该边界样例不证明组件身份或整条车辆路径等价，见 `experiments/offline_distance_20261106/` 和 `artifacts/rea/vehicle_*nearest_distance_sse.txt`。

四实例固定 PWM 负载首轮：seed 123/456/2026/789，四路发布器各记录 14854–14861 条命令，每路 IMU 和 LiDAR 接收与发布窗口重叠约 300.00–300.02 秒；四个模拟器及观测器完成并清理。IMU 接收约 100 Hz、LiDAR 约 9.61–9.93 Hz，单模拟器进程约 209.9–210.5% 单核 CPU、RSS 峰值约 763.9–770.3 MiB；seed 2026 有一条 IMU 零时间戳，相邻非零时间戳无回退。动作是无在线真值的固定序列，仅证明发布与订阅的有限压力窗口，不证明逐条执行、完整训练或长期稳定。见 `experiments/concurrency_4_pwm_300s_20261106/workload_analysis.json` 及 `stream/independent_analysis.json`。

四实例固定 PWM 负载第二轮亦完成：两轮每路发布 14854–14861 条命令，IMU/LiDAR 与各自发布窗口重叠约 300.00–300.02 秒，八个实例观测均完成并按 manifest 清理。单模拟器进程约 209.7–210.6% 单核 CPU、RSS 峰值约 763.4–770.3 MiB；IMU 接收约 100 Hz、LiDAR 约 9.50–9.93 Hz，IMU 消息源时间/墙钟比约 0.944–0.986。两轮共有一条 IMU 零时间戳，相邻非零时间戳未见回退。固定开环命令发布和传感器订阅是合成负载，不证明每条命令已执行、完整训练或长期稳定；只能作为本机、该配置和该负载下两次有限四实例观察。见 `experiments/concurrency_4_pwm_repeats_20261106/summary.json`。

粗线状态释放先导：seed 123、stage 4 固定粗线夹具的 HISM 返回实例索引 7；预设十秒后停止状态强制，释放时 state=7。随后有 44 个来源组件及候选 `FHitResult+0x58` 值都匹配该夹具的事件按分配指针进入碰撞回调，其中一个回调后、同线程下一回调前观察到 previous=7、current=12；后续 61 个离散采样均为 12，覆盖约 13.60 秒。索引字段仍为运行时归因推断，不能证明独立 ABI、物理冲量或完整评分终止；stage、夹具和释放/动作时间均为开发场干预，时间选择参考过往事后观察。见 `experiments/line_release_thick_pilot_20261106/line_release_analysis.json`。

细线状态释放先导：seed 123 的固定细线夹具返回索引 7，释放 state 强制时值为 7；释放后 16 个事件的来源组件及候选命中索引均匹配该夹具，一个同线程回调后、下一回调前观察到 state 从 7 写为 12；随后 61 个离散采样均为 12，覆盖约 13.80 秒。候选偏移尚未独立类型确认，路径、夹具、阶段和计时均为开发干预；不证明冲量、评分或完整自然赛程终止。见 `experiments/line_release_thin_pilot_20261106/line_release_analysis.json`。

2026-11-06 线障状态释放矩阵：粗线、细线各对 seed 123/456/2026 重复两次；停止 state 7 强制时十二场均为 7。释放后按夹具组件和候选索引匹配并以分配指针对接回调，粗线每场 39–44 对、细线 16–19 对，未匹配回调均为 0；每场恰有一次同线程、下一配对回调之前的状态写入关联，记录 previous 7/current 12。首次采到 12 后各场 61–62 次离散样本均为 12，跨度 13.60–13.85 秒。这不是连续状态或完整结算的证明：stage、夹具及初始状态经调试器注入，动作时序使用过往事后真值选择，偏移 0x58 的候选实例索引未独立定型；GDB 会改变执行时序。逐场来源、配对、流和限制见 `experiments/line_release_matrix_20261106/summary.json` 及其引用目录。

2026-11-06 状态 12 完成分支追加受控先导：seed 123 的粗、细线夹具各一场，释放状态强制后，以组件、候选索引、事件指针和同线程顺序关联的回调进入状态函数，第三参数均为 0；赋值后直接断点读到配置文本指针指向 UTF-16 `Finished`，函数正常返回，内部计数进入和返回均为 0，状态 previous/current 为 7/12。进入前文本已是 `Finished`；释放前也观察到状态 12 写入，但未独立确认先前文本来源，因此这里只证明本次受控路径的写入而非文本首次变化。另外保留未设置文本写入断点的粗线先导；注入、事后真值选时、字段推断和 GDB 扰动的限制仍在，对外评分、自然赛程和完整 episode 结算未获验证。证据及逐场日志见 `experiments/line_settlement_pilots_20261106/summary.json`。

2026-11-06 车辆状态 12 分支先导：seed 123 的停车夹具释放状态强制后，车辆 actor 和推定子组件来源事件按对象指针对接回调，随后同线程进入状态函数，第三参数为 0，写入 previous/current 7/12；在文本指针写入后读到 UTF-16 `Finished`，函数正常返回，内部计数调用前后均为 0。进入前文本已为 `Finished`，故无法证明首次变化；单场开发夹具涉及 stage、路径及初始状态注入，动作时序依据既有事后真值选择，候选子组件和 GDB 时间扰动限制仍在，未证明自然碰撞冲量、对外计分或完整结算。详见 `experiments/vehicle_settlement_pilot_20261106/settlement_analysis.json` 与原始日志。

2026-11-06 单实例频率函数非串行探针：同 key 的读写或写写调用按 64 位哈希和跨线程函数入口/返回区间检查，不为目标函数加互斥锁；合成夹具验证同键重叠、不同键对照和浮点返回。seed 123、NullRHI、固定开环 PWM 与传感器采样的 60 秒先导完成并清理；周期快照至少覆盖 23000 次调用，记录同键跨线程读写重叠 0、未跟踪计数快照 0。采样计数是下界，探针可漏检短暂重叠，hash 可能碰撞且会改变调度；区间重叠不等于 deque 内存并发，未检测到也不能排除竞争或判定八实例崩溃根因。见 `experiments/frequency_overlap_interposer_pilot_20261106/overlap_analysis.json` 和开发说明。

2026-11-06 八实例频率函数非串行探针第一场：八 seed、NullRHI、独立 ROS master 空载，全部就绪后 liveness 采样覆盖约 89.42 秒，八实例存活并清理；各进程快照至少 34000–35000 次调用，未跟踪计数快照均为 0。四进程记录 6 次跨线程同哈希调用区间重叠：5 次写写事件的哈希和长度与 `frame` 一致，1 次读写事件与 `imu` 一致；键名为哈希推断，日志未记录事件时间，无法判断它们与就绪时刻的先后。探针不串行化函数，但改变调度、可能漏检且有哈希碰撞风险；区间重叠不能证明 deque 的内存访问同时发生，也不能归因既有崩溃。运行器参数名为 `--frequency-guard`，实际装载的是非加锁探针。本场使用 `rmua/noetic-xal:driver-recovery-20261012` 镜像，不同于先前加锁及未加锁对照的 `rmua/re-probe:20261104`，故不能作为同镜像对照；完整记录见 `experiments/concurrency_8_frequency_overlap_90s_20261106/overlap_analysis.json`。

2026-11-06 八实例频率函数非串行探针同镜像复测：使用先前加锁/未加锁对照同一 `rmua/re-probe:20261104` 镜像、相同二进制/settings 和八 seed，空载 liveness 采样覆盖约 89.60 秒，八实例存活并清理。每进程调用快照至少 34000–35000 次，未跟踪计数快照为 0；四个进程共记录 25 个同哈希跨线程函数区间重叠：2 个写写事件的哈希和长度与 `frame` 一致，23 个读写事件分别对应候选 `imu` 15、`gps` 5、`rotor_pwm` 3。topic 名称仅为哈希推断；日志没有事件时间，不能判断与就绪时刻的先后。非加锁探针仍改变调度且可能漏检，函数区间重叠不能证明 deque 内存访问同时发生，也不能证明先前 SIGSEGV 的成因。这是 90 秒空载有限观察，非训练稳定测试；见 `experiments/concurrency_8_frequency_overlap_matched_90s_20261106/overlap_analysis.json`。

2026-11-06 频率 deque 对象探针单实例先导：非加锁 preload 仅在三个频率函数内跟踪动态绑定的 `push_back`、`pop_front`、`size`，以进程内 deque 指针识别同一对象并排除同线程嵌套。合成夹具验证同对象读写重叠、不同对象对照和返回值转发；seed 123、NullRHI、固定开环 PWM 的 15 秒先导观测到三类方法，操作周期快照至少 11000 次，同对象跨线程方法区间重叠及未跟踪计数均为 0。探针改变调度并延长方法区间，且遗漏内联、迭代器与其他路径；零结果不能排除竞争，正结果也不证明内存指令并发或先前崩溃成因。原始记录和范围见 `experiments/frequency_deque_pilot_20261106/deque_overlap_analysis.json`、`experiments/frequency_deque_probe_20261106/README.md`。

2026-11-06 八实例 deque 对象探针：在 `rmua/re-probe:20261104`、相同二进制/settings、八 seed 的空载场，八进程在约 89.24 秒 liveness 采样期间均存活并清理；各进程日志快照至少 78000–80000 次目标方法调用，未跟踪计数快照均为 0。seed 2026 的同一进程内 deque 地址 `0x7838a1394060` 记录两次跨线程方法区间重叠：线程 149 的 `push_back` 进入时已有其他线程写方法活跃；线程 148 的 `size` 进入时也有其他线程写方法活跃。其他线程具体身份和 topic 未记录，不能推断两个事件指向同一对线程；事件出现在首次 liveness 样本之前约 1.27 秒。合成夹具另验证同对象读写、写写及不同对象对照。探针不加目标锁，但延长测量区间、改变调度并遗漏内联/迭代器等路径；因此该观察不证明具体内存指令同时执行，亦不证明早先八实例 SIGSEGV 根因。完整来源、方法、对象地址和时间戳见 `experiments/concurrency_8_deque_overlap_90s_20261106/deque_overlap_analysis.json`。

## 2026-11-06 自然线障实例、原生 reset 组件与 6/8 实例失败

自然线障实例因果先导：stage 4 注入后捕获第一个粗线 HISM 提交的自然变换；GDB 将实际无人机碰撞组件沿 x 轴扫过该实例，并把状态每 tick 写为 7、清零 `+0x180/+0x1a8/+0x1ac`。seed 123/456/2026 各一场分别记录 30/34/30 次命中来源组件等于目标粗线组件、候选索引 `+0x58=0` 的事件；目标 hit 后 10 ms 内出现 state 12 请求 21/22/21 次，12 ms 内出现 `7→12` 写入 21/22/21 次。seed 123 首个目标 hit 后约 2.7 ms 请求、3.5 ms 写入 12，随后重复。这是受控组件扫掠，不是自然飞行；扫掠中另有其他来源组件命中，stage 注入、每 tick 状态强制和 GDB 调度均限制外推。见 `experiments/natural_line_component_20261106/` 与 `scripts/stage_natural_line_sweep2.gdb`。

原生 reset 组件先导：seed 123 的 stage 4 注入场用固定 rotor PWM 0.3 持续 2.5 s，事后 API/`pose_gt` z 约从 0.309 m 变到 -4.82 m，但 reset 回调读取的七个 float 最后仍为 `0,0,0,0,0,0,-37.9076614`，服务返回 false；固定速度抬升场同样未改变该组件且返回 false。该结果说明当前复位检查所读组件不随这次物理运动变化；此前 `success: True` 仍依赖 root-height 写入和风随机状态干预。没有找到不干预的原生成功，也没有证明完整物理/车辆重放。见 `experiments/reset_native_component_20261106/`。

6/8 实例固定 PWM 负载失败：同一 `rmua/re-probe:20261104`、相同二进制/settings，6 实例 seed 123 在容器启动约 32.2 s 退出 139，8 实例 seed 2026 在约 11.1 s 退出 139，均非 OOM；引擎日志 SIGSEGV 在 `0x2fbda42` 读取地址 0，栈含 `0x2e31691`、`0x2fb979e`、`0x5e54336`、`0x2fbd8e1`。4 实例同一负载两轮各约 300 s 完成。该差异支持当前训练规格保守取 4 实例；6/8 各只有一次失败，不能给出统计概率或单一根因，且该栈与先前频率 deque 栈不同。见 `experiments/concurrency_pwm_failure_summary_20261106/summary.json`。

额外地图门静态复核：12 张图的门间距两两差 ≤0.012 cm；每图仅用 yaw+平移拟合到 OL1 的残差 ≤0.009 cm，z 平移每图内为常数、缩放完全一致；OL2 BeginPlay 运行时 6/6 场、12/12 门匹配。门赛道几何是刚性副本，未验证门洞物理尺寸、环境资产或飞行可用性。见 `experiments/ol_gate_rigid_20261106/analysis.json` 与 `scripts/verify_gate_rigid_layout.py`。

GDB 附着的 8 实例固定 PWM 追加场：相同 8 seed、镜像、二进制/settings，固定 PWM 与 IMU/LiDAR 订阅负载下，容器寿命约 13.9–15.3 秒内有两路崩溃。seed 789 在 `0x26b6223` 执行 `cmpl $0x0,0x8(%r12,%r13,1)`，`r12=-1,r13=0`，即读取地址 7；seed 1002 在 `rip=0` 处发生空函数指针调用，`r12=0,r13=0`。前者与先前频率/记录路径现场一致，后者是新的独立现场。GDB 改变调度，两个现场各只有一次捕获，不能给出概率或共同上游原因，但支持“至少两个故障现场”的结论。见 `experiments/concurrency_8_pwm_gdb_180s_20261106/`、`experiments/concurrency_pwm_failure_summary_20261106/summary.json` 和更新后的 `scripts/crash_capture.gdb`。

自然车辆路径碰撞先导：stage 4 注入后把现有调用重定向到 `0x2fc8900`，用自然随机状态产生车辆；seed 123/2026 的自然五点路径原始字节一致，seed 456 在该设置下没有 `VEHICLE_INIT`。seed 123/2026 各 27 次命中来源为本场车辆 actor 与 `actor+0x240` 子组件；各 23 次在 55 ms 内伴随 state 12 请求和 `7→12` 写入，最近请求中位约 18.1/20.9 ms。该场把无人机碰撞组件传送到车辆子组件上方 100 cm 并每 tick 强制 state 7，不是自然飞行；事件级因果仍由既有释放矩阵的指针对接和同线程顺序支持。见 `experiments/natural_vehicle_component_20261106/analysis.json`、`scripts/stage_natural_vehicle_collision.gdb`。

ROS 传输时延收尾汇总：从既有 `transport_matrix_20261105` 六场（seed 123/456/2026 各两次）提取 p99 范围，IMU 发布至订阅 0.324–0.385 ms、LiDAR 0.561–0.655 ms、PWM 输出 0.241–0.335 ms、命令发送返回到模拟器回调 0.338–0.431 ms；另有五个非初始转换的首个匹配回调至调试 PWM 输出 15.63–62.20 ms，三场无 preload 对照为 29.22–61.34 ms。它们仍不含物理采样、动力学或执行器延迟，钩子和观测器改变时序。见 `experiments/transport_latency_summary_20261106/summary.json`。
