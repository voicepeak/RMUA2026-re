# Seed 影响矩阵：门、树、车辆、线障与风

日期：2026-11-04。目标：以模拟器 12.0.0.5（主程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`）为固定版本，区分“地图静态作者数据”和“命令行 seed 驱动的运行时生成”。本页只记录已定位的静态路径、参数和证据边界；没有把静态推断冒充成完整运行时认证。

## 结论表

| 对象 | 是否受命令行 seed 影响 | 证据 | 边界 |
|---|---|---|---|
| 门 / `ARouteGate2024_1` | **否（地图固定）** | 门 actor 的变换来自地图序列化；`ARouteGate2024_1` 的自定义 vtable 入口只有 `0x2fc1360`，只读取组件/ID 并设置状态，不写 Transform，也没有 `FRandomStream`/`rand` 调用。路线切换通过 `0x2fc13d0` 只切换可见性/碰撞；`0x2fc6cc0` 重置样条时没有写门变换。 | 已核对类入口和路线激活路径；未对 12 张图的每一个门做运行时逐门比对。历史 9 对同视角图像相近，但不是全赛道证明。 |
| 随机树 / `ARandomTree` | **是** | `0x2fa3dd0` 使用 `FRandomStream` 风格 LCG，调用 `0x2fa65b0` 做拒绝采样生成位置，再走 `0x2faafa0` 创建 `RandomTree`；该调用在初始化路径中出现 3 次（`0x2fa1292`、`0x2fa204e`、`0x2fa231f`）。 | 无法只算 seed 的前几个 LCG 值预测整张图；拒绝采样和调用次数不固定。 |
| 车辆 / `AHoverVehicle2024_1` | **是** | `0x2fc8900` 从全局单例 `0x2fcc220` 的 `+0x1a0` 取 seed，更新 LCG；生成的 `[0,1)` 随机数乘 11 取整，`<6` 时进入 `0x2fc93c0` 车辆路线初始化。seed 由 `0x2fcc5a0` 写入 `+0x19c/+0x1a0`。 | `6/11` 是理想均匀分支概率，不是实测比例；不保证每 seed 必然出现不同车辆结果。 |
| 线障 / `BP_ThickLine`、`BP_ThinLine` | **是** | 室外段初始化 `0x2fbac30` 在 stage 4/5/6 调用 `0x2fc77b0`；`0x2fc77b0` 使用单例 `+0x1a0` 的 LCG 生成沿样条间距，并调用 `0x2fc3fc0`/`0x2fc4980` 放置线障。两类线障的凸碰撞网格已提取（`artifacts/maps/obstacle_collision_meshes.json`）。 | 放置数量、精确间距分布、运行时运动和碰撞通道仍需实测；静态只证明 seed 进入该生成路径。 |
| 风 / `GameState2024_1` | **是** | `GameState2024_1` 的 `0x2fb8f40` 在初始化时调用 `0x2fb2040(param_3+0x200, seed)`；`0x2fb2040` 就是 `srand(seed)`。风序列生成器 `0x2fb23b0` 随后使用 libc `rand()`，stage 4/5/6 的路线初始化 `0x2fbac30` 会调用它。此前“风没有证据随 seed 变化”的表述已撤回。 | 静态路径已确认；尚未在实际风区逐 seed 采样。起点/大厅风为零不能代表室外段。 |
| 地图选择 | **否** | seed 不参与地图 URL；地图由启动参数/Lobby 默认值决定。 | 换地图仍需显式切图。 |

## 室外段和风的参数

`0x2fb9960` 是 0..12 的状态切换。状态 2/4/6/8/10 分别调用 `0x2fbac30`，最后参数为 1..6；其中 4/5/6 段会进入风和线障生成。`0x2fbac30` 末尾调用：

```text
0x2fb23b0( GameConfig+0x2bc, 350.0, 2.0, 1.5, state+0x200 )  // stage 4
0x2fb23b0( GameConfig+0x2c0, 660.0, 4.0, 2.0, state+0x200 )  // stage 5
0x2fb23b0( GameConfig+0x2c4, 1800.0, 15.0, 4.0, state+0x200 ) // stage 6
```

`GameConfig+0x2bc/+0x2c0/+0x2c4` 在默认配置中对应 200/220/200 秒（与 `docs/03` 的时段常量表一致）。`0x2fb23b0` 生成约 8–10 秒间隔的分段风时间序列，`0x2fb2050` 取标量、`0x2fb2130` 插值成三维向量；`0x2fb2040` 明确调用 `srand`。如果默认坐标单位是 UE 厘米，350/660/1800 分别是 3.5/6.6/18.0 m/s 量级的基准风速；这只是从常量和接口推断，仍应在 `/airsim_node/drone_1/debug/wind` 上按段实测。

## 门为什么仍判为固定

1. 门类的 vtable 为 `0xcd6820`，`1616` 字节，只有 `0x2fc1360` 一个自定义入口；`0x2fc1360` 读取组件/索引并设置 `+0x220/+0x224`，没有 Transform 写入或随机调用。
2. 路线初始化 `0x2fbac30` 对门/组件调用 `0x2fc13d0`，后者只切换可见性和碰撞；`0x2fc6cc0` 重置样条时写的是 spline/进度/箭头数据，没有门位置。
3. 每张图有 12 个 `RouteGate` 静态变换；这些是地图作者数据。换 seed 只改变树、车辆、线障和风，不改变这些 actor 的序列化变换。
4. 历史同视角图像对（seed 123/456）在 x≈12、52、150 m 等样本相近；这不能替代全门核验，但和“门没有随机写路径”一致。

## 对训练的直接影响

- 不能只用“换 seed”来随机化门；门布局必须通过换图（OL1..OL12）或显式改地图资源来变化。
- 车辆、线障、风三者都受 seed 影响，且分别使用不同阶段的生成入口；训练环境应按 stage 4/5/6 分别建模，而不是把 seed 当成一个全局难度参数。
- 树的 seed 依赖会影响视觉/LiDAR 背景，但对比赛门路线的影响需要单独评估；不要把树的位置当成门位置。
- 任何高并发近似都要在留出 seed 上分别比较“门固定、车辆/线障/风随机”的分布，不能只比较起点图像。

## 证据文件

- `artifacts/rea/followup_native.c`：门 BeginPlay、RandomTree BeginPlay、风区类入口。
- `artifacts/rea/followup_callers.c`：`0x2fb8f40`（GameState 风初始化/更新）、`0x2fbac30`（室外段初始化）、`0x2fc77b0`（线障生成）。
- `artifacts/rea/followup_state.c`：`0x2fb9960` 状态切换、`0x2fba970` 重置、门激活 `0x2fc13d0`。
- `artifacts/rea/wind_generation_trace.c`：`0x2fb2040` 的 `srand`、`0x2fb23b0` 的 `rand()` 风序列。
- `artifacts/rea/hover_route.c`、`hover_callers.c`：车辆 seed 分支和路线推进。
- `artifacts/rea/line_callers.c`、`line_native.c`：线障生成/碰撞相关入口。
- `artifacts/maps/obstacle_collision_meshes.json`：粗线/细线/车辆凸碰撞网格。

## 2026-11-04 运行时补充

OL1 对 seed 123/456/2026 各重复两次，12 门 BeginPlay 的位置、四元数姿态、组件缩放逐门一致；只覆盖该图和三个 seed，缩放不等于门洞真实尺寸。六次都观测到 `0x2fb2040` 的 seed 参数与输入一致。所有运行停留起点，风话题为零，尚未命中 `0x2fb23b0` / `0x2fc77b0` / `0x2fc8900`，因此风、车辆和线障仍需要实际 stage 4/5/6 核验。服务 reset 返回 `success: False`，不能推断随机数重放。逐门数据、方法和限制见 `docs/17_runtime_validation_20261104.md` 及 `experiments/runtime_trace_20261104_v2/`。

## 2026-11-04 reset 探针补充

seed 123 的官方 ROS reset 调用期间，只读断点看到随机流当前值从已消耗的 `214098071` 经 `0x2fced20(-1)` 恢复为 `123`，随后 `0x2fba970` 入口读取到当前值 `123`。这为本次 FRandomStream 状态重置提供运行时证据，但不证明所有 seed、全部生成路径或场景事件重放。`0x2fb2040` 在本次 reset 期间未再次命中，尚不能排除其他路径调用 libc `srand`。原始事件、时间戳、汇编和响应见 `experiments/reset_semantics_20261104/`。

seed 456 的另一次 reset 探针从回调入口到采样结束未命中已校验的 glibc `srand`/`srandom` 公共入口，同时观测随机流恢复为 456；这仍不排除其他随机状态写入或证明场景重放。seed 123 静态样条传感器航迹只到 stage 1，室外三类随机路径的运行时结论仍未获得，详见 docs/17。

OL2 请求的四种启动选项在实际运行均回到 OL1；原请求参数不是加载结果。跨地图的 seed/门关系仍待实际切图后验证，日志和核对索引见 `experiments/gates_ol2_20261104/actual_maps_summary.json`。

## 注入式室外生成探针（进行中）

`experiments/stage_inject_pilot_20261104/` 在 OL1 初始路线设置的游戏线程断点上将 `0x2fbac30` 参数从 `(1,3,1)` 改为官方 stage 4 所用的 `(8,12,4)`。seed 123 观察到线障生成入口、风生成入口及 35 条 16 字节风关键帧。风入口的浮点参数实测为 `200,350,2,1.5`；关键帧原始字节和采样见 GDB 日志。这是修改运行时寄存器的开发实验，不是合法赛程或完整场景等价证明。ROS 无人机仍在起点，风话题为零，不能用该话题否定生成的风序列。

注入 stage 4 对三个 seed 各重复两次：每 seed 风数组字节完全一致、不同 seed 数组不同；数据及注入限制见 `experiments/stage_inject_4_20261104/analysis.json` 和 docs/17。不能把这项确定性推广到 reset 或完整赛程。

stage 5 三 seed 各两场的注入风关键帧也逐字重复；生成入口计数、原始样本和采样退出码见 `experiments/stage_inject_5_20261104/`。车辆直接函数调用初探失败，尚不构成车辆动态证据。

**更正**：stage 6 浮点入参的顺序经运行时寄存器核验为 `200, 1800, 15, 4`，本页先前将最后两个数写反。三个 seed 在 ROS reset 返回 false 的注入场里，reset 前后的 stage 4 风数组均不同；这只证明这些具体场次不重放风数组，不证明 reset 成功或所有随机状态机制。

车辆在开发容器内通过临时改写调用目标命中 `0x2fc8900` 并返回，见 `experiments/vehicle_redirect_pilot_20261105/`；尚未据此认定车辆实际运动。线障对象候选点数组首场见 `experiments/obstacle_inventory_pilot_20261105/`。

stage 4 三 seed 各两次的内部线障间距点集合在同 seed 两场一致、跨 seed 不同；点数分别 371/373/373。它们并非已确认 spawn 实例，详见 `experiments/obstacle_inventory_4_20261105/`。

开发注入额外强制通过状态字符串分支后，seed 123 的车辆初始化产生五点路径，观测到组件世界坐标随时间变化；见 `experiments/vehicle_world_pilot_20261105/`。这不代表自然赛程必然生成该车。

stage 6 的内部候选点在每个 seed 两场逐字集合一致，跨 seed 不同；车辆强制状态门槛的六场中 seed 456 路径重复性未成立，原因未确定。详见 docs/17。

风更新强制注入的起点场已采到非零风向量；车辆组件世界轨迹已采三个 seed 各两次，但 seed 456 路径不重现。限制及原始文件见 docs/17。

stage 4 强制风状态的三个 seed 各两场均观测到非零 ROS 风，见 `experiments/force_wind_tick_4_20261105/analysis.json`；PWM 采样在这些场次失败，独立修复试场见 docs/17。

stage 4 的三 seed 各两次强制风场均采到风与 PWM、IMU/GPS/LiDAR 时钟；开放环 PWM 步进首场也已记录。均是起点开发注入，见 docs/17。

stage 5 强制风更新六场均采到非零风及 PWM、传感器流；样本并非真实室外遍历，见 `experiments/force_wind_tick_5_20261105/`。

stage 6 强制风更新三 seed 两次均获得非零 ROS 风；完整时序和对应 PWM、传感器流见 `experiments/force_wind_tick_6_20261105/`。

单次强制风状态后延长至 25 秒，stage 4/5 风话题向量保持不变；动态风过渡尚未证明，原因正在追踪。风关键帧本身来自生成器内存。

持续逐 tick 写入室外状态的试场已观测 stage 4 风关键帧索引推进及 ROS 风向量变化；单次状态注入仍会出现 25 秒冻结，原因未完全确定。

stage 6 强制风场中的固定 PWM 步进已覆盖三 seed 各两次；短场风保持初始向量，不能视作风过渡与动力学耦合证明。

持续注入单场中，关键帧速度和角度的线性过渡及三角函数可近似重建 ROS 风矢量；1230 条样本误差 RMSE 约 0.0383（存在未校准时钟偏差）。证据和适用边界见 docs/17。

### 风关键帧离线模型（已对照）

`scripts/wind_offline_model.py` 用 float32 和本机 libc `srand/rand` 重建风生成：每次 `u = (rand() & 0xffffff) / 16777215.0`，保持长度为 `8+2u` 秒、保持区间之间过渡 2 秒；每轮在剩余预算至少 11 秒时生成保持段。选一个保持段速度为上限，其余速度围绕由目标积分推导的基准加 `2.4u-1.2`，再按下限及 `上限-0.01` 截断；每段方向为 `360u` 度。输入依次为 `(时长,目标积分,上限,下限)`：stage 4 `(200,350,2,1.5)`、5 `(220,660,4,2)`、6 `(200,1800,15,4)`。角度、速度在过渡段线性插值，详见 docs/17。代码中的单精度运算顺序是字节级复现的一部分。

对于本机 fresh 初始化、直接以输入 seed 初始化 libc 且生成前没有其他 `rand()` 消费的 18 个采样，模型与所有运行时关键帧逐字节相等，见 `experiments/wind_offline_model_20261105/model_results.json`。此模型调用宿主 libc，其他 libc 实现可能不同；正常赛程前的全局 `rand` 消耗和 reset 后随机状态不同，不能仅凭 seed 保证复现。`disassembly_excerpt.txt` 显示低 24 位掩码，禁止替换成通用 `rand()/RAND_MAX`。

### 线障候选距离的分布和边界

`0x2fc77b0` 的一次内层候选距离并非简单“相邻障碍随机间距”：汇编在随机流 LCG 更新后，把状态高 23 位装入 `[1,2)` 的 float 尾数，再减 1，得到 `u∈[0,1)`；候选沿样条偏移的绝对值为 `1500+7000u` cm，即 `[15,85)` m。反向样条改变符号；外层基准按 ±10000 cm（100 m）步进，候选还会裁剪到样条端点，失败时可重复尝试。因此这不是最终生成的线障间距分布。LCG 为 `state=(state*0x0bb38435+0x3619636b) mod 2^32`，且其他放置和采样会消费同一流。

stage 4 seed 123 的开发注入试场捕获 198 个内层候选，绝对偏移约 15.058–84.895 m；根据捕获的更新后状态与 float32 操作重建，最大差约 `4.9e-6` cm（文本输出取整误差）。基准值每次相隔 10000 cm。见 `experiments/line_offsets_pilot_20261105/offset_analysis.json`、`scripts/stage_line_offsets.gdb` 和 `scripts/analyze_line_offsets.py`；位置和 spawn 成功率另行核验。

持续逐 tick 状态注入的 stage 4/5 已覆盖三 seed 各两次，并观测 ROS 矢量随关键帧推进；近似插值误差和时钟限制见 docs/17。

stage 4 的内层候选距离公式已在三个 seed 各两次共 1188 个调用上核对；每 seed 的状态/基准/偏移序列重复一致。见 `experiments/line_offsets_4_20261105/`。

stage 6 持续状态注入动态风已覆盖三 seed 各两次；stage 5 线障候选偏移公式和重复序列也已核对。详细误差及开发注入限制见 docs/17。

持续风和开环 PWM 注入场额外记录到非直接调用的碰撞回调，但其对方对象尚未识别；状态被逐 tick 强制，不能将回调次数外推到比赛。

stage 6 的内层候选距离三 seed 各两次也完成核对；stage 4/5/6 三组同 seed 序列一致。详见 `experiments/line_offsets_6_20261105/` 和 docs/17。

动态风注入的 IMU 统计和 LiDAR 消息时序覆盖三 stage、三 seed 和重复，具体值及不能单独作为噪声模型的限制见 docs/17。
