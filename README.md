# RMUA2026 模拟器逆向资料

本仓库是 RoboMaster 2026 自主无人机（RMUA）官方模拟器 **12.0.0.5** 的静态逆向结果、数据导出和复现脚本。

目标不是替代官方规则，而是让参赛队能够：

- 理解 seed、地图、航段、门/工厂/端口、碰撞/超时/风等仿真机制；
- 批量生成不同 seed 的训练场景；
- 从静态数据中重建赛道，设计 RL 的观测、动作和 reward；
- 在换设备后继续提问和复现分析。

## 先看什么

| 文档 | 内容 |
|---|---|
| [docs/01_target_and_toolchain.md](docs/01_target_and_toolchain.md) | 目标文件、哈希、引擎/pak、REA/Ghidra/UAssetAPI 工具链 |
| [docs/02_maps_and_routes.md](docs/02_maps_and_routes.md) | 12 张图、12 段路线、门/工厂/端口坐标与样条点 |
| [docs/03_seed_config_game_logic.md](docs/03_seed_config_game_logic.md) | seed、GameConfig 7 个隐藏字段、计时、状态机、Score |
| [docs/04_ros_interface.md](docs/04_ros_interface.md) | ROS 话题、服务、控制模式、传感器 |
| [docs/05_airsim_dynamics.md](docs/05_airsim_dynamics.md) | AirSim SimpleFlight/FastPhysics、车辆参数、噪声 |
| [docs/06_blueprints_and_ui.md](docs/06_blueprints_and_ui.md) | BP_GameConfig、BP_Gate、GUI、Lobby 的解析结果 |
| [docs/07_reproduction.md](docs/07_reproduction.md) | 从零下载、解包、解析、反编译和重生成数据 |
| [docs/08_open_questions.md](docs/08_open_questions.md) | 静态分析无法确定的点（尤其官方裁判端计分） |
| [docs/09_evidence_index.md](docs/09_evidence_index.md) | 每个结论对应的文件/函数地址/证据 |
| [docs/10_drl_training_notes.md](docs/10_drl_training_notes.md) | 面向 DRL 的训练信号、reward、并行建议 |
| [docs/11_artifact_manifest.md](docs/11_artifact_manifest.md) | 已入库/未入库资料清单与大文件重生成方式 |
| [docs/12_local_seed_and_training_assessment.md](docs/12_local_seed_and_training_assessment.md) | 本机 seed/NullRHI/并发实测与边界 |
| [docs/13_seed_randomtree_trace.md](docs/13_seed_randomtree_trace.md) | RandomTree 随机位置路径的静态追踪 |
| [docs/14_obstacle_asset_and_vehicle_trace.md](docs/14_obstacle_asset_and_vehicle_trace.md) | 车辆/线障资源、碰撞网格、生成入口 |
| [docs/15_seed_dependency_matrix.md](docs/15_seed_dependency_matrix.md) | 门/树/车辆/线障/风是否受 seed 影响的总表（2026-11-04） |
| [docs/16_headless_requirements.md](docs/16_headless_requirements.md) | 官方 NullRHI 实测边界与高并发训练环境要求 |
| [docs/17_runtime_validation_20261104.md](docs/17_runtime_validation_20261104.md) | 动态实验、开发注入和证据边界 |
| [docs/18_acceptance_ledger.md](docs/18_acceptance_ledger.md) | 动态验收进度、失败样本与未完成项 |
| [docs/20_final_acceptance.md](docs/20_final_acceptance.md) | 最终验收：Done/Partial/Blocked、可训练交付与已知限制 |

## 核心结论（TL;DR）

- 模拟器是 **UE4.27 + AirSim/ROS**；Linux Shipping 主程序 116,748,832 字节，pak 1,927,609,305 字节，pak 使用 Oodle Kraken/Zlib。
- 官方包内含 **12 张地图** `RMUA2024OL1..12`；每张图有 **12 条 RouteConfig、12 个 Gate、12 个 Factory、12 个 CheckPort、24 条 SideTrack/Arrow**。OL1 的路线顺序与 OL2–OL12 不同。
- 2026-11-06 静态复核：12 张图的门相对布局是同一组几何的**刚性变换**（仅 yaw+平移，门间距差 ≤0.012 cm，拟合残差 ≤0.009 cm，缩放完全一致）。不要把 OL1–OL12 当成 12 套不同门布局；环境资产/主题是否不同仍需单独验证，门洞物理尺寸未测。
- 命令行 `seed` 由 `"seed"` 字段解析，默认 **1**；seed 初始化 `FRandomStream`，并影响**随机树、车辆、线障和室外风**。门是地图静态数据，不由 seed 改位置；**seed 不负责选择地图**。逐项证据和边界见 `docs/15_seed_dependency_matrix.md`。
- Lobby 默认打开 `/Game/Maps/RMUA2024OL/RMUA2024OL1`；要跑 OL2–OL12 需要改地图 URL/启动参数，而不是只换 seed。
- `Configs/GameConfig.json` 实际读取 **7 个布尔字段**：
  `ChargeGlobally`、`IgnoreHoverCollision`、`AllowAllReports`、`IgnoreAllHitCollision`、`IgnoreOverTime`、`IgnoreWind`、`AllowChangeState`。
  README 只写了其中 2 个。
- 计时：每个状态有默认上限（180/180/180/200/220/200 秒），另有 30 秒、2 秒（OutTime）相关计时；`IgnoreOverTime` 会跳过超时结算。
- 仿真内部 `Score` 是 `GameConfig + 0x2fc` 的计数器，在航段状态切换（case 2）时自增；GUI 的 `Score` 绑定它。**官方裁判端分数不在这份客户端里**。
- ROS 桥在 `libairsimros.so`；控制入口为 `/airsim_node/drone_1/rotor_pwm_cmd`（4 路 PWM）或速度控制话题；`/airsim_node/reset` 重置。

## 数据文件

- `artifacts/maps/maps_summary_all.json`：12 张图的完整结构化摘要（路线、门、工厂、端口、检查端口、样条点）。
- `artifacts/tables/`：可直接用于训练的 CSV
  - `gates.csv`：地图/门名/世界坐标/yaw/scale
  - `factories.csv`：工厂世界坐标
  - `ports.csv`：端口世界坐标
  - `check_ports.csv`：检查端口
  - `routes.csv`：路线样条点（local + 按组件变换计算出的 world）
- `artifacts/pseudocode/anchors_pseudo3.c`：Ghidra 定向反编译结果（seed、GameConfig、状态机、计时、AirSim PWM）。
- `artifacts/blueprints/`：BP_GameConfig、BP_Gate、GUI、Lobby 的 UAssetAPI JSON 与 Kismet token。
- `artifacts/configs/`：GameConfig、ROSConfig、DefaultEngine/DefaultGame、AirSim 2024 settings。
- `artifacts/disasm/`、`artifacts/symbols/`、`artifacts/strings/`、`artifacts/vtables/`：静态证据。
- `scripts/`：解析/反编译/导出脚本（路径按本机 `D:\RMUA2026-re` 写，换机需改）。
- `repro/`：下载与复现脚本。

## 重要限制

- 官方评测服务器/裁判端不在模拟器客户端中；本仓库只能证明**仿真器内部**的状态、计时和 Score 计数器，不能证明官方最终计分公式。
- Blueprint 事件图已解析为 Kismet token，但没有全部翻译成高级语言；数据级字段已提取。
- 路线样条的 world 坐标由“组件位置/旋转 + 局部点”计算；CSV 中 scale 未参与（该组件 scale 多为默认 1）。在引擎里跑一次 `/airsim_node/initial_pose` / `pose_gt` 可复核。
- 原始 12 张图的 UAssetAPI JSON 共约 2.4 GB、解包资源约 4.3 GB、模拟器 zip 约 2.0 GB，未进入 Git；用 `docs/07_reproduction.md` 重新生成。
- 不要用 `GameConfig` 的调试字段伪造正式成绩；这些字段只建议用于数据采集/训练环境。

## 官方来源

- 模拟器仓库：<https://github.com/RoboMaster/IntelligentUAVChampionshipSimulator>（分支 `RMUA2026-01`）
- 基础示例：<https://github.com/RoboMaster/IntelligentUAVChampionshipBase>（分支 `RMUA2026`）
- 规则中心：<https://bbs.robomaster.com/wiki/20204847/809873>
- 模拟器包：`https://sz-rm-rmua-dispatch-prod.oss-cn-shenzhen.aliyuncs.com/b39a0194e982f0d987153c6016feb325/simulator_12.0.0.5.zip`

## 复现

```powershell
# 1) 下载并校验模拟器
powershell -ExecutionPolicy Bypass -File repro\download_simulator.ps1

# 2) 解包
tar -xf D:\RMUA2026-re\simulator_12.0.0.5.zip -C D:\RMUA2026-re\sim

# 3) 按 docs/07_reproduction.md 使用 repak / UAssetAPI / Ghidra
```

```bash
# 重新生成所有地图摘要和 CSV（Linux/WSL）
python3 scripts/extract_all_maps.sh   # 内部调用 extract_map_summary2.py
python3 scripts/combine_maps.py
python3 scripts/export_map_tables.py
```

## 使用建议（面向 DRL）

- **域随机化**：同一张图用不同 `seed` 启动，获得不同航段随机量；用 `IgnoreOverTime`/`IgnoreAllHitCollision` 只在采集/调试时关闭终止条件。
- **多地图训练**：OL2–OL12 是不同布局，需要改 Lobby 的 Level URL 或启动参数；不要假设 seed 会切换地图。
- **并行仿真**：模拟器 ROS 节点/话题名固定（`/airsim_node/drone_1`），同一 ROS master 下多开会导致话题冲突；建议每个实例独立容器/ROS master，或改 ROS 命名空间（需要改代码/桥接层）。本机固定 PWM + IMU/LiDAR 负载下 4 实例两轮 300 秒完成，6/8 实例同一负载各出现一次 SIGSEGV；训练规格保守按 4 实例并保留失败日志，见 `docs/16`、`docs/19`。
- **性能**：用 `run_simulator_offscreen.sh`/`-RenderOffscreen`；在 `settings.json` 删除不用的相机关闭渲染负载；Lidar 32 线/200k 点/秒是主要负载之一。
- **重置**：优先用 `/airsim_node/reset` 服务做 episode 重置；`takeoff/land` 服务控制起飞降落。
