# seed 随机位置路径：RandomTree 静态追踪

目标：本机 `RMUA-Linux-Shipping`，SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`。此页是静态证据，不声称已观察到对象在运行时的位置或完整初始化顺序。

## 已观察到的调用和类型

1. 已有 `artifacts/disasm/seed_refs.disasm` 中 `0x2fa1292` 调用 `0x2fa3dd0`；该函数会更新同一状态中的 `FRandomStream` 风格 LCG，常量为 `0x0bb38435`、`0x3619636b`。
2. `0x2fa3dd0` 的一条分支先构造随机位置，再两次调用 `0x2fa65b0`（`0x2fa40a2`、`0x2fa40bf`）；函数的调用链和数据依赖见 `artifacts/pseudocode/anchors_pseudo3.c`、`artifacts/disasm/route_init.disasm`、`artifacts/rea/seed_callees.c`。
3. 该分支在创建候选对象后调用 `0x2faafa0`。该函数经 `0x2fd9270` 取得类型并验证对象继承链；`0x2fd9270` 的注册参数字符串为 `/Script/RMUA` 和 `RandomTree`。由此静态追踪可将此路径关联到 **RMUA `RandomTree` 类**。尚未证实每个生成对象都成功创建，也不能仅凭类名判定其视觉外观或碰撞行为。
4. 本次使用 Ghidra 12.1.2 对主程序仅作 `-noanalysis` 定点反编译，函数名由脚本临时命名，不是原始源码符号。结果见 `artifacts/rea/seed_callees.c`；这是 Ghidra 输出，不是 REA MCP Evidence。类名通过 ELF PT_LOAD 虚拟地址映射至原文件，按 UTF-16LE 读取：`0x91a3a8` 为 `/Script/RMUA`、`0x775d0e` 为 `RandomTree`；该只读段虚拟地址比文件偏移大 `0x200000`。

## 位置采样可见规律

在 `0x2fa65b0` 伪代码中，每次调用从随机流生成候选向量、对 XY 分量归一化，并从传入参考位置产生 XY 方向上的候选点；候选点与已接受点及另一个参考点的平面距离被检查。函数在接受 3 个候选之后返回。常量经 ELF 地址映射读取为：随机径向系数 7.0 + 5.0×随机数、乘以 100；间距比较常量 500。若这一坐标空间使用 UE 通常的厘米单位，这相当于候选半径约 7–12 m、最小平面间距约 5 m，单位和边界条件仍需运行时校验。

拒绝采样会消耗不固定数量的随机数，因此不能只计算 seed 的前几个 LCG 值来预测整个场景；还需要完整的初始状态、对象输入、调用次数和真实浮点行为。这里的“两次生成各 3 个候选”仅是该分支中的局部静态行为，不是对整张图树木数量的断言。

`scripts/inspect_randomtree_constants.py` 可按 ELF program header 重新读取这些字符串和常数，输出示例为 `artifacts/rea/randomtree_constants.json`。

## 有界运行时探针（未命中）

以开发副本、seed 123、OL1、`-nullrhi` 启动 GDB，只读断点放在 `0x2fa3dd0` 入口与 `0x2fa41dd` 创建调用前，启动后观察约 30 秒，未命中；此前仅含后者的约 40 秒场次也未命中。日志为 `artifacts/rea/randomtree_runtime_seed123_retry.log` 和 `randomtree_runtime_seed123.log`。这两场均未控制无人机进入后续阶段，未命中只说明所覆盖的时段和分支没有记录到断点，不证明函数不会执行或该类不存在。模拟器和诊断容器已清理。

## 风的证据边界

这条 `RandomTree` 路径本身**不能证明**风随 seed 变化，亦不能证明门、车辆和细条的行为。此前 README 等将此函数的随机量描述为“风等”，证据不足，已更正。

2026-11-04 补充：风另有独立路径，现已定位。`GameState2024_1` 的 `0x2fb8f40` 在初始化时读取单例 `0x2fcc220` 的 `+0x1a0`（命令行 seed），调用 `0x2fb2040`；后者就是 `srand(seed)`。风序列生成器 `0x2fb23b0` 使用 `rand()`，室外段 stage 4/5/6 的 `0x2fbac30` 会以 GameConfig 的时段和 350/660/1800 等常量调用它。因此 **seed 确实影响室外风**；但这仍不能证明其他阶段/位置的风或本 `RandomTree` 路径与风有直接关系。详见 `docs/15_seed_dependency_matrix.md` 与 `artifacts/rea/wind_generation_trace.c`。

REA 6.8.0 + Ghidra 12.1.2 对 ROS 桥函数 `AirSimROSTransformWind` 的独立反编译保存在 `artifacts/rea/wind_transform.json`，Evidence ID `ev_a20b44a5cd63a1581f5920ac9ac5672891fcf8a1343d9807033bff957f211bc3`，目标库 SHA-256 为 `69d5ea3302bdd5f20b842ec3d3e84bafcb50ef3649ada42c4d346c9bcd389885`。它显示该桥对输入风向量做 tf2 变换并写回；具体坐标系与风源并未由这一函数单独证明。

REA 对主程序以 900 秒 Ghidra 全量自动分析尝试 `0x2fd9270` 超时（`provider_timeout`），无可用反编译 Evidence；记录见 `artifacts/rea/randomtree_class_rea.json`。本页主程序结论来自独立 Ghidra `-noanalysis` 定点结果及原始文件，不是该次 REA 全量结果。不要把该超时当作目标不存在或分析完整的证据。

## 后续

追踪 `RandomTree` 的生成类配置、创建对象的运行时变换及地面投影；再分别定位 `VehiclePrefabs`、`LinePrefabs`、门和风区的初始化入口。需要多 seed 同一地图的运行时对象清单交叉验证，不能将这条局部调用链推广到完整赛道。
