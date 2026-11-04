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
0x2fb23b0( GameConfig+0x2c4, 1800.0, 4.0, 15.0, state+0x200 ) // stage 6
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
