# 车辆与线障：资源和原生代码追踪

基于模拟器 12.0.0.5 的 pak（SHA-256 `2407662b224ed28c64412402af044da85aced07d78ce6fc0948e6be11ea2a0c2`）及主程序（SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`），均为静态分析，未证明运行时所有实例和轨迹。

## 预制体

`BP_GameConfig2024_1` 的默认 `VehiclePrefabs` 引用 `BP_Hover_1_C`，`LinePrefabs` 引用 `BP_ThickLine_C` 和 `BP_ThinLine_C`。这证明默认资源引用关系，不证明每张地图没有覆盖，也不能仅靠字段名认定生成数量或实测行为。

从 pak 提取并用 UAssetAPI 1.1.0 解析后：

- `BP_Hover_1_C` 继承原生 `HoverVehicle2024_1`，含 `RouteSpline`、`VehicleRoot` 和 `Hover` 静态网格组件；后者使用 `HoverTruck` 网格，相对位置约 `(75.4155, -0.0003, -80.9424)`、yaw 约 `-90.0002°`，相对 scale 为 `(0.5, 0.5, 0.5)`。这些是组件相对变换，不能直接作为世界位置或碰撞尺寸。
- 粗线、细线 Blueprint 各引用各自静态网格；未在这两个 Blueprint 中解析到事件图。运动逻辑或碰撞几何可能来自其他 native 类、运行时实例、网格或构造代码。
- 粗线网格的 `BodyInstance` 设置了 `Custom` 碰撞配置；序列化的响应数组对 `WorldStatic`、`WorldDynamic`、`PhysicsBody`、`Vehicle`、`Destructible` 为 `ECR_Ignore`。未列出项和实例覆盖仍需结合 UE 默认配置和运行时确认；不能据此断言无人机一定能穿过粗线。

提取脚本 `scripts/extract_obstacle_assets.py` 使用 `repak` Python 0.2.3.post1；提取的 `.uasset` 检查 UE 资源魔数，全部源对象 SHA-256 在 `artifacts/rea/obstacle_assets_manifest.json`。解析产物为 `artifacts/blueprints/BP_Hover_1.json`、`BP_ThickLine.json`、`BP_ThinLine.json`，需保留 UAssetAPI 版本、外部引用和未解析字段等限制。

随后解析了三个网格的 `BodySetup_0.AggGeom.ConvexElems`，通过 `scripts/export_obstacle_collision.py` 导出 `artifacts/maps/obstacle_collision_meshes.json`：

| 网格 | 凸元素 | 顶点 / 三角面 | 顶点局部范围 |
|---|---:|---:|---|
| `thickline` | 1 | 16 / 28 | x≈±5.1、y≈±5.1、z≈±350.12 |
| `thinline` | 1 | 16 / 28 | x≈±2.6、y≈±2.6、z≈±350.12 |
| `HoverTruck` | 1 | 32 / 60 | x≈-175.49..175.95、y≈-449.07..436.35、z≈0.03..295.12 |

若资源坐标为 UE 厘米，粗/细线网格局部包围尺度约为 `0.102 × 0.102 × 7.002 m` / `0.052 × 0.052 × 7.002 m`。这是从凸顶点推算的局部轴对齐范围，不是运行时世界包围盒；实际碰撞形状还需应用凸元素、组件和 actor 变换、碰撞通道及实例覆盖。车辆网格还需叠加前述 Blueprint 相对旋转、缩放和位置。解析后的原始网格 JSON、顶点/索引及未解释的元素变换均保留，提取清单为 `artifacts/rea/obstacle_meshes_manifest.json`。

导出器检查了索引范围、三角面内重复索引及边的关联次数：三份网格的每条索引边均恰被两个三角面引用。这是有限的拓扑一致性检查，不证明变换链、碰撞通道或 PhysX 运行时结果正确。

## 车辆原生类

静态类注册函数 `0x2fd71c0` 使用 `/Script/RMUA` 和 `HoverVehicle2024_1`；该类 vtable 为 `0xcd5b80`，slot 143 指向 `0x2fc0180`。定点反编译见 `artifacts/rea/hover_native.c`，原始地址和无全程序分析的限制与 `docs/13_seed_randomtree_trace.md` 相同。

`0x2fbff60` 将对象偏移 `+0x230` 置 1，将 `+0x234` 清零，并把 float 输入保存到 `+0x238`；它还向内部组件处理输入点数组。`0x2fc0180` 在 `+0x230` 非零时，比较 `+0x234` 与从组件获得的标量；满足条件时，以该值查询组件并设置另一组件变换，之后执行 `+0x234 += 帧输入 × +0x238`。结合组件名 `RouteSpline` 和 `VehicleRoot`，**推断**这是沿样条推进的车辆运动，`+0x234` 为进度、`+0x238` 为推进速率；尚需确认具体 UE 函数签名、坐标空间、速度单位和实际运行值。

在 `0x2f90000..0x2fe0000` 范围中，`0x2fc95f5` 调用 `0x2fbff60`（见 `artifacts/rea/hover_start_refs.txt`），且之前通过 `0x2fc98c0` 构造传入点数组。这是下一步追踪车辆路线及速度来源的入口；搜索范围之外或间接调用不在此结论覆盖内。

进一步依据 ELF `.eh_frame` 找到调用所在函数边界为 `0x2fc93c0..0x2fc96a9`。`artifacts/rea/hover_route.c` 显示该函数在速度参数大于零且状态字符串检查满足条件时，从候选对象数组中按与输入位置的距离选择对象，调用 `0x2fc98c0` 形成点数组，再将该数组和速度参数传给 `0x2fbff60`。`0x2fc98c0` 使用两个常量 `+2100` 和 `-2100`，按输入起终值的顺序选择方向，并逐步查找相邻候选；其点数组与车辆组件样条的关系有较强静态证据，但仍需确认调用参数的来源、完整坐标变换和运行时值。若坐标单位为厘米，步长为 21 米；不能仅据此将它等同于车辆的移动速度。

## 线障生成路径（2026-11-04）

`0x2fbac30` 是室外段初始化函数，stage 4/5/6 分别调用 `0x2fc77b0` 生成线障：

```text
stage 4: 0x2fc77b0(120.0, 120.0, spline, 20, 5,  20, 10, 1)
stage 5: 0x2fc77b0(100.0, 120.0, spline, 20, 15, 24, 24, 2)
stage 6: 0x2fc77b0( 80.0, 120.0, spline,  0, 45,  0, 60, 3)
```

`0x2fc77b0` 沿 `RouteSpline` 从起点走到终点，在每一轮用单例 `0x2fcc220` 的 `+0x1a0` 更新 LCG，生成下一段的随机间距/位置，再调用 `0x2fc3fc0` 或 `0x2fc4980` 创建粗线/细线。因此线障不是地图静态摆放，而是**由命令行 seed 参与的运行时生成**。反编译见 `artifacts/rea/followup_callers.c` 的 `0x2fc77b0`，以及已有的 `line_callers.c`、`line_native.c`。

线障的凸碰撞网格已提取；粗/细线局部轴向范围约 0.102/0.052 m 见方、高约 7.0 m。生成参数中 120/100/80、20/24/0、10/24/60 等字段的精确语义（间距、数量、侧向范围或车道）尚未逐一标定，不能仅凭这些数字断言运行时数量。

## seed 对车辆分支的静态证据

1. `0x2fcc220` 返回一个全局单例，首次访问会调用构造函数 `0x2fcc5a0`。该构造函数默认 seed 为 1，解析命令行的 UTF-16 `seed` 字段，并将 seed 同时写入对象 `+0x19c` 和 `+0x1a0`；见 `artifacts/pseudocode/anchors_pseudo3.c` 的 `REA_2fcc5a0`。因此此前文档把这两个位置笼统描述为 `GameState` 需要保留类身份未完全确认的限制。
2. `ARouteSpline2024_1` 的 vtable slot 143 指向 `0x2fc8090`。在该函数的部分路线进度阈值跨越分支中，调用 `0x2fc8900`；见 `artifacts/rea/route_gate_native.c`。这不是说起点必定立刻生成车辆。
3. `0x2fc8900` 从上述单例的 `+0x1a0` 取值并使用同一 LCG 更新；按浮点构造生成 `[0,1)` 随机数、乘以 11、截断为整数，结果小于 6 时走向调用 `0x2fc93c0` 的分支。代码和精确分支见 `artifacts/rea/hover_callers.c`、`artifacts/rea/hover_path_callers.txt`。若输入近似均匀，分支概率约为 6/11；这是由静态表达式推断的理想比例，非运行实测。
4. 这条路径为“命令行 seed 能影响车辆初始化的分支选择”提供了直接静态证据，但并不保证任意两个 seed 的结果不同，也不证明所有车辆或所有运动轨迹均由这一个随机数决定。前面的随机调用顺序、阶段和运行状态会影响实际结果。

运行时尝试：seed 123、`-nullrhi`，使用历史开发侦察的 VEL + `pose_gt` 路线（非比赛输入），GDB 在 `0x2fc89a7` 和 `0x2fbff60` 放只读日志断点。约 150 秒人工设限后主动中断，达到 12 个参考点、最后约 NED `(248.2, -21.25, -19.32)` m；两个断点均未命中，覆盖不完整。`KeyboardInterrupt` 是设定时间结束，不能将 `fault: null` 当作全程成功，也不能据此否定后续阶段车辆逻辑。日志及轨迹在 `experiments/seed_audit_20261103/hover_scout_seed123/`；容器已清理。

## 下一步

追踪 `0x2fc93c0` 的调用方及参数，确认点数组与速度是否使用 seed、地图样条或状态；再对 `0x2fbff60` 和 `0x2fc0180` 设置有界运行时断点，记录创建实例、速度、输入点及时间，并用多次同 seed 与跨 seed 对比。线障生成入口已定位到 `0x2fc77b0`，下一步是标定其参数、运行时数量/位置和碰撞事件。
