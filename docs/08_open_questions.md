# 08 未确定项与边界

以下问题是静态分析到这里仍不能 100% 证明的；后续应优先用运行时抓包/日志/受控实验确认。

## 1. 官方裁判端计分

- 模拟器客户端有 `Score` 计数器（`GameConfig+0x2fc`）和 `meter_report` 服务，但**没有完整的官方积分公式**。
- 官方评测服务器可能按 gate、factory、meter、time、collision 等上报另行计分。
- 需要看规则手册或抓取评测端 `/airsim_node/custom_msgs` / 日志才能确定。
- 建议 RL reward 先用：门通过、段推进、工厂/端口事件、飞行时间、碰撞、风扰动，而不是只拟合 `Score`。

## 2. 状态枚举与时间上限的语义映射

- 已确认 `GameState+0xd0` 是 0..12 的状态枚举，tick 按状态选择 `+0x2b0..0x2c4` 的时间上限。
- 已确认 `+0x2c8=30s`、`+0x2cc=2s` 的检查；`IgnoreOverTime` 会跳过。
- 尚未逐状态确认 `0..12` 对应的中文/英文状态名、段编号和是否和 12 个 RouteConfig 一一对应。
- `REA_2fb9960` 里写 `GameConfig+0x308` 的状态字符串可运行时读出；建议跑一局打印 `/airsim_node/custom_msgs` 或 GUI 的 `State:`。

## 3. 门触发逻辑

- 已提取门的 actor 位置、yaw、scale 和 `RouteGateIndex`。
- 尚未反编译 `ARouteGate2024_1` 的触发/通过判定函数；BP_Gate 没有事件图。
- 需要知道触发体形状/半径、是否要求穿过方向、是否允许擦边、重复通过是否计数。
- 建议用 `pose_gt` 录制一次过门，结合 `Score`/状态字符串反推。

## 4. 工厂巡检与 meter_report

- 已提取 12 个 Factory、12 个 CheckPort 的位置和 `meter_report(index,value)` 服务。
- 尚未确认：进入工厂的判定、仪表值范围、`index` 与工厂的精确对应、重复上报是否计分、是否影响状态。
- 需要看规则手册和运行时 `OnMeterReport` 日志（`0x2fbf258` 附近）。

## 5. 路线样条 world 坐标

- `routes.csv` 的 world 坐标由 `component.relative_location + R_yaw(local)` 计算，scale 默认按 1。
- 在 UE 中 spline 可能还有 actor/attach 链；虽然本次解析到 `1_1` 的 root 就是 Spline 组件，但其他图/其他组件建议用 `GetLocationAtSplinePoint` 校验。
- 最好在模拟器里让无人机沿样条飞一次，和 `pose_gt` 对齐。

## 6. seed→随机量的精确分布

- 已确认 `seed` 从命令行解析、存入 GameState、调用 `FRandomStream`。
- 尚未逐行还原 `REA_2fa3dd0` 的随机值到风/航段的映射、取值范围、是否影响门位置。
- 建议固定地图、扫 seed 1..N，记录 `/airsim_node/drone_1/debug/wind` 和 `pose_gt`，做相关性分析。

## 7. 地图切换

- 已确认本地默认 OL1，Lobby 只硬编码 OL1。
- 尚未实测 `-map=`/`open`/`-ExecCmds` 是否能在本 build 直接切到 OL2–OL12。
- 官方评测端如何选图（每轮换图/每队随机图）需要规则或评测日志确认。

## 8. 多实例并行

- ROS 节点/话题名固定 `/airsim_node/drone_1`，同 master 多实例冲突。
- 尚未验证 `libairsimros.so` 是否支持通过环境变量改 namespace；大概率需要改桥接层或每个实例独立 master/容器。
- 训练并行方案建议：一容器一 ROS master + 一 GPU 进程；或改 AirSimROS node 初始化参数（需要重新编译）。

## 9. 控制量与 PWM 归一化

- `RotorPWM.msg` 的 PWM 范围/单位需要以实际 msg 定义为准（0..1 还是 0..1000）。
- `moveByMotorPWMs` 只做命令排队；SimpleFlight 内部如何把 PWM 映射到 RPM/推力，需要看 AirSim 源码或运行时标定。
- 速度控制的单位/急停语义在 README 有描述，但 `vel_body_cmd` 与 `vel_cmd_body_frame` 的名称差异需运行时确认。

## 10. reset 与随机流

- `/airsim_node/reset` 会重置 Score/计时/状态，但**是否重置 seed/FRandomStream** 未验证。
- 若每个 episode 需要独立随机 seed，最稳妥是重启进程/容器；若 reset 保留随机流，则同一 seed 的多次 reset 可能产生相关性。
