# 09 证据索引

| 结论 | 证据 |
|---|---|
| 模拟器版本/大小/哈希 | `README.md`、`docs/01_target_and_toolchain.md`、`repro/download_simulator.ps1` |
| 引擎 UE4.27 / pak V11 / Oodle | `artifacts/configs/DefaultGame.ini`、`repak info`、`docs/01` |
| 12 张地图/地图默认 OL1 | `artifacts/configs/DefaultEngine.ini`、`artifacts/blueprints/RMUA2024Lobby.json`、`artifacts/maps/maps_summary_all.json` |
| 每图 12 段/12 门/12 工厂/12 端口 | `artifacts/maps/maps_summary_all.json`、`artifacts/tables/*.csv` |
| 门/工厂/端口坐标 | `artifacts/tables/gates.csv`、`factories.csv`、`ports.csv`、`check_ports.csv` |
| 路线样条点 | `artifacts/tables/routes.csv`、`artifacts/maps/maps_summary_all.json` |
| 路线顺序 OL1 特殊 | `artifacts/maps/maps_summary_all.json` 的 `RouteConfigs[].RouteSpline.name` |
| seed 从命令行解析、默认 1 | `artifacts/pseudocode/anchors_pseudo3.c` 的 `REA_2fa0e80`；`artifacts/strings/seed_context.txt` |
| seed 使用 FRandomStream | `artifacts/pseudocode/anchors_pseudo3.c` 的 `REA_2fa3dd0`；`artifacts/disasm/route_init.disasm` |
| GameConfig 7 字段 | `artifacts/pseudocode/anchors_pseudo3.c` 的 `REA_2fb3a50`；`artifacts/disasm/config_func.disasm` |
| 字段偏移 +0x2d0..+0x2d6 | 同上；`artifacts/disasm/flag_uses.disasm` |
| `IgnoreAllHitCollision` 跳过碰撞 | `0x2fb712e`（`flag_uses.disasm`） |
| `IgnoreOverTime` 跳过超时 | `0x2fb92d3`、`0x2fb9410`、`0x2fb94ff` |
| `IgnoreWind` 置零风 | `0x2fb948d` |
| `AllowAllReports` / `AllowChangeState` | `0x2fb7fe9`、`0x2fb8090`、`0x2fb812e`、`0x2fb816e`、`0x2fb86dc`、`0x2fb88fb` |
| 时间上限 180/180/180/200/220/200 | `REA_2fb3a50` 初始化 + `REA_2fb8f40` tick |
| FlightTime = 六段累计 | `REA_2fba830`（`anchors_pseudo3.c`） |
| Score = GameConfig+0x2fc，case 2 自增 | `REA_2fb9960`；GUI `Get Score` 绑定 `ResolvedOwner=-7` |
| GUI 绑定 GameConfig 属性 | `artifacts/blueprints/GUI2024.json`、`GUI2024.kismet3.txt` |
| Lobby 默认 OL1 | `artifacts/blueprints/RMUA2024Lobby.json` 的 `LevelExport.URL.Map` |
| BP_Gate 无事件图 | `artifacts/blueprints/BP_Gate.json` |
| ROS 话题/服务 | 官方 README、`Base/basic_dev/src/airsim_ros/msg|srv`、`docs/04_ros_interface.md` |
| 车辆参数 0.9kg/0.18m/惯量/升力/反扭/最大转速 | 官方 README、`docs/05_airsim_dynamics.md` |
| AirSim 模块/控制入口 | `artifacts/symbols/main.symbols.demangled`、`moveByMotorPWMs` @ `0x612efc0` |
| 传感器/相机/噪声配置 | `artifacts/configs/settings_Game2024_1.json` |
| ROS 帧容差/录制配置 | `artifacts/configs/ROSConfig.json` |
| 原始 pak 文件清单 | `artifacts/strings/pak_files.txt` |
| 主程序段/动态符号 | `artifacts/symbols/main.sections.txt`、`main.dynamic.txt`、`main.symbols.demangled` |
| 反编译函数列表/地址 | `scripts/DumpAnchors.java`、`artifacts/pseudocode/anchors_pseudo3.c` |
| 地图解析脚本/重生成 | `scripts/extract_map_summary2.py`、`export_map_tables.py`、`combine_maps.py`、`docs/07_reproduction.md` |

## 2026-11-04 追加证据

| 结论 | 证据 |
|---|---|
| 门是地图静态、无 seed 变换路径 | `ARouteGate2024_1` vtable `0xcd6820`，入口 `0x2fc1360`；激活 `0x2fc13d0`；`artifacts/rea/followup_native.c` / `followup_state.c` |
| 风由 seed 初始化 | `0x2fb8f40` → `0x2fb2040`（`srand(seed)`）→ `0x2fb23b0`（`rand()`）；`artifacts/rea/followup_callers.c`、`wind_generation_trace.c` |
| 线障由 seed 生成 | `0x2fbac30` 调 `0x2fc77b0`；后者使用单例 `+0x1a0` LCG；`artifacts/rea/followup_callers.c`、`line_callers.c` |
| 车辆由 seed 分支 | `0x2fc8900` 使用 `+0x1a0` LCG，`<6/11` 进入 `0x2fc93c0`；`artifacts/rea/hover_callers.c` |
| 随机树由 seed 生成 | `0x2fa3dd0` / `0x2fa65b0` / `0x2faafa0`；`docs/13` |
| 总表 | `docs/15_seed_dependency_matrix.md` |

## 2026-11-04 运行时补充

| 结论或未完成项 | 证据 |
|---|---|
| OL1 逐门变换、三 seed 两次采样 | `experiments/runtime_trace_20261104_v2/gate_comparison.json`、各运行 `gdb.log`；`scripts/analyze_gate_trace.py` |
| seed 初始化 / reset 返回 false / 起点零风 | 同目录各 `gdb.log`、`reset.log`、`*_wind.csv`；`scripts/runtime_trace.gdb`、`scripts/run_readonly_trace.py` |
| reset 响应判断静态代码 | `artifacts/rea/reset_bridge_rea.json`，REA/Ghidra 反编译 |
| 四/八实例 CPU、RSS、消息时序及失败 | `experiments/benchmark_20261104/`，`scripts/headless_pool.py`、`scripts/measure_headless_pool.py`、`scripts/pool_probe_node.py` |
| PWM 闭环和事后真值 | `experiments/pwm_20261104/seed123_closed_loop/`、`seed456_motor_pulses_failed/`（POSITION_LIMIT） |
| 结论范围及仍缺证据 | `docs/17_runtime_validation_20261104.md` |

## 2026-11-04 后续卡点追踪

| 范围 | 证据 |
|---|---|
| 8 路 SIGSEGV、单 seed 单实例对照及降级边界 | `experiments/concurrency_followup_20261104/`；`scripts/monitor_pool_liveness.py`、`scripts/run_pool_batches.py` |
| reset 返回 false 的活动分支、7 个原始浮点和随机流复位 | `experiments/reset_semantics_20261104/`；`scripts/reset_probe.gdb`、`scripts/reset_probe.py`、`scripts/run_reset_diagnostic.py` |
| 室外赛段试飞、断点及其时钟范围 | `experiments/stage_probe_20261104/`、`scripts/stage_trace.gdb`、`scripts/stage_wind_recorder.py`；未到 stage 4/5/6 |
| 8 路崩溃现场及频率记录函数 | `experiments/concurrency_gdb_20261104/`、`artifacts/rea/frequency_rea.json`、`scripts/run_gdb_pool_repro.py`；不同故障栈不可直接等同 |

| 后续补充 | 证据 |
|---|---|
| RenderOffscreen IMU、LiDAR、双相机原始时间和速率 | `experiments/sensor_clock_rendered_20261104/`、`scripts/sensor_clock_probe.py`、`scripts/run_rendered_sensor_probe.py`；ROS 时间差不等于已标定端到端延迟 |
| 频率统计多线程重叠 | `experiments/concurrency_frequency_overlap/`、`scripts/frequency_overlap_probe.py`；GDB 改变调度，不能证明根因 |
| OL2 请求实际加载 OL1 | `experiments/gates_ol2_20261104/actual_maps_summary.json` 和各次 `gdb.log`；不构成跨图验证 |
| 有渲染窗口 OL2 控制台尝试 | `experiments/map_console_20261104/`、`scripts/run_map_console_probe.py`；未确认控制台接受输入，未观察到 OL2 |

验收逐项状态与未完成项见 `docs/18_acceptance_ledger.md`。
| OL2 首参数启动尝试 | `experiments/gates_ol2_firstarg_20261104/`、`scripts/run_map_firstarg_probe.py`；实际仍加载 OL1 |
| 无真值 GPS/IMU 诊断位姿接口静止采样 | `experiments/gps_imu_bridge_20261104/`、`scripts/gps_imu_pose_bridge.py`；未用于控制，未验证飞行坐标/动态性能 |
| GPS/IMU 对既有 PWM 运动的事后真值匹配 | `experiments/gps_imu_bridge_20261104/offline_motion_alignment/`、`scripts/evaluate_sensor_pose_alignment.py`；单 seed 小范围，真值未进入控制 |
| GPS/IMU 桥接 beta VEL + 静态 spline 实飞尝试 | `experiments/beta_sensor_scout_20261104/`、`experiments/beta_sensor_scout_corrected_20261104/`、`experiments/beta_sensor_scout_4ms_20261104/`；含失败、ROS 图、状态与风日志，仅 stage 1 |
| 6 m/s 无真值局部静态路线尝试 | `experiments/beta_sensor_scout_6ms_20261104/`；到局部 spline 终点，不是赛程完成 |
| 1→3 静态路线候选连接试验 | `experiments/beta_sensor_scout_1to3_20261104/`；连接处制动、仍在 stage 1 |
| 枢纽 LiDAR 点云及控制器制动决策 | `experiments/hub_lidar_probe_20261104/`、`scripts/hub_lidar_probe.py`、`scripts/analyze_hub_lidar.py`；近似传感器姿态转换，非碰撞网格真值 |
| 抬高 1→3 连接候选失败 | `experiments/beta_sensor_hub_lift_20261104/`；state 1→12，原因未判定，未到 stage 4 |
| 抬高路线 state 12 的 2 秒阈值分支 | `experiments/hub_lift_cause_20261104/`、`scripts/stage_trace_cause.gdb`；未知 `+0x180` 为什么为零 |
| 抬高场 predicate 与累计 OutTime | `experiments/hub_predicate_trace_20261104/`、`scripts/charge_state_probe.py`、`scripts/stage_trace_predicate.gdb`；原始对象字段尚未完整类型化 |
| OutTime 判定对象与地图根位置匹配 | `experiments/zone_inventory_20261104/`、`scripts/zone_inventory_probe.py`、`scripts/analyze_zone_inventory.py`；FName 未解码、部分样条位置有歧义 |
| 路线分段事件与活动计数 | `experiments/segment_overlap_trace_20261104/`、`scripts/stage_trace_segment.gdb`、`scripts/analyze_segment_trace.py`；GDB 对时序有影响 |

更正：部分旧 VEL 诊断运行器曾根据 GDB state 12 停止控制；精确范围与严格真值隔离限制见 docs/17 的“真值隔离更正”。

| 室外段寄存器注入试运行 | `scripts/stage_inject.gdb`、`scripts/run_stage_injection.py`、`experiments/stage_inject_pilot_20261104/`；stage 4 命中线障与风生成，注入场不代表比赛赛程 |
| stage 4 三 seed 两次注入的风关键帧与入口计数 | `experiments/stage_inject_4_20261104/analysis.json`、各 `gdb.log`、`scripts/analyze_stage_injection.py`；调用数不等于成功生成数 |
| stage 5 三 seed 重复注入 | `experiments/stage_inject_5_20261104/analysis.json`、原始 GDB 和 ROS 日志 |
| 车辆 GDB call 与 reset 类型加载失败 | `experiments/vehicle_call_pilot_20261104/`、`experiments/stage_reset_inject_pilot_20261104/`；均未证明目标动作成功 |
| stage 6 三 seed 重复注入及入参顺序更正 | `experiments/stage_inject_6_20261104/analysis.json` 与各 GDB 日志 |
| ROS reset false 后风数组变化 | `experiments/stage_reset_inject_retry_20261104/analysis.json`、`scripts/analyze_injected_reset.py`；重复 dump 不代表第三次生成 |
| 车辆寄存器跳转命中入口和一个分支 | `experiments/vehicle_trampoline_pilot_20261104/`、`scripts/stage_vehicle_trampoline.gdb`；未确认返回或车辆运动 |
