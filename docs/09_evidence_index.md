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
| 车辆函数重定向入口、分支和返回 | `scripts/stage_vehicle_redirect.gdb`、`experiments/vehicle_redirect_pilot_20261105/`；替代原调用并更改随机流 |
| 车辆返回探针失败 | `experiments/vehicle_trampoline_v2_pilot_20261105/`；标记先于目标入口且 SIGSEGV |
| 线障候选点内部数组 | `scripts/stage_obstacle_inventory.gdb`、`experiments/obstacle_inventory_pilot_20261105/`；不是已确认 spawn 实例 |
| stage 4 候选点三 seed 重复 | `experiments/obstacle_inventory_4_20261105/analysis.json`、`scripts/analyze_obstacle_inventory.py` |
| 车辆注入六场入口及返回顺序 | `experiments/vehicle_redirect_4_20261105/analysis.json`、`scripts/analyze_vehicle_redirect.py` |
| 车辆初始化/tick 尚未命中的单场 | `experiments/vehicle_dynamics_pilot_20261105/`、`scripts/stage_vehicle_dynamics.gdb` |
| stage 5 线障候选点三 seed 重复 | `experiments/obstacle_inventory_5_20261105/analysis.json` 和原始日志 |
| 强制通过状态检查后的车辆路径、进度和组件世界坐标 | `experiments/vehicle_forcegate_pilot_20261105/`、`experiments/vehicle_world_pilot_20261105/`、`scripts/stage_vehicle_world.gdb`；开发注入 |
| stage 6 候选点三 seed 重复 | `experiments/obstacle_inventory_6_20261105/analysis.json` |
| 六场强制状态门槛的车辆初始化和进度 | `experiments/vehicle_forcegate_4_20261105/analysis.json`；seed 456 两次路径不同 |
| 三 seed 两次车辆组件世界坐标 | `experiments/vehicle_world_4_20261105/analysis.json`、各 `gdb.log`；状态门槛被强制 |
| 注入风状态的非零 ROS 风 | `experiments/force_wind_tick_pilot_20261105/`、`scripts/stage_force_wind_tick.gdb`；起点注入，非自然赛程 |
| stage 4 强制风、传感器及 PWM 订阅失败 | `experiments/force_wind_tick_4_20261105/analysis.json`、各流原始文件 |
| PWM 原始消息订阅修复试场 | `experiments/force_wind_wire_pilot_20261105/`、`scripts/pwm_any_probe.py`、`scripts/analyze_forced_wind.py` |
| 修复 PWM 后 stage 4 风/传感器三 seed 两次 | `experiments/force_wind_tick_4_wire_20261105/analysis.json` 和原始流 |
| 风注入下固定开环 PWM 步进单场 | `experiments/wind_pwm_step_pilot_20261105/`、`scripts/pwm_wind_step_probe.py`；不读取真值控制 |
| stage 5 强制风/传感器三 seed 两次 | `experiments/force_wind_tick_5_20261105/analysis.json` 和原始流 |
| 合成碰撞回调首场 state 7→12 | `experiments/collision_inject_pilot_20261105/`、`scripts/stage_collision_inject.gdb`；不是物理接触 |
| stage 6 强制风、PWM、传感器三 seed 两次 | `experiments/force_wind_tick_6_20261105/analysis.json` 和原始流 |
| stage 4 固定 PWM 开环三 seed 两次 | `experiments/wind_pwm_step_4_20261105/pwm_analysis.json`、`analysis.json`；header 匹配不代表端到端延迟 |
| stage 5 固定 PWM 开环三 seed 两次 | `experiments/wind_pwm_step_5_20261105/pwm_analysis.json` 和原始流 |
| 单次注入后 stage 4/5 风话题 25 秒保持不变 | `experiments/wind_transition_4_20261105/`、`experiments/wind_transition_5_20261105/`；更新原因待查 |
| 合成碰撞回调及忽略标志三 seed 对照 | `experiments/collision_inject_ignore0_20261105/analysis.json`、`experiments/collision_inject_ignore1_20261105/analysis.json`；非几何接触 |
| 逐 tick 强制状态后风索引与话题变化 | `experiments/wind_persistent_pilot_20261105/`、`scripts/stage_force_wind_persistent.gdb` |
| stage 6 单次注入后风话题 25 秒保持不变 | `experiments/wind_transition_6_20261105/` |
| stage 6 固定 PWM 开环三 seed 两次 | `experiments/wind_pwm_step_6_20261105/pwm_analysis.json` 与原始流；短场风向量保持不变 |
| 风关键帧至 ROS 矢量的近似重建 | `experiments/wind_persistent_pilot_20261105/wind_interpolation_analysis.json`、`scripts/analyze_persistent_wind.py`；时钟未标定 |
| 风生成 float32 离线模型与 18 场逐字节比较 | `scripts/wind_offline_model.py`、`experiments/wind_offline_model_20261105/model_results.json`、`disassembly_excerpt.txt` |
| 线障内层偏移 LCG/float32 与 198 次运行时值 | `scripts/stage_line_offsets.gdb`、`scripts/analyze_line_offsets.py`、`experiments/line_offsets_pilot_20261105/offset_analysis.json`；非最终间距 |
| stage 4/5 持续注入动态风与关键帧重建，三 seed 两次 | `experiments/wind_persistent_4_20261105/wind_interpolation_analysis.json`、`experiments/wind_persistent_5_20261105/wind_interpolation_analysis.json` |
| 训练诊断主机、镜像和实用 settings 副本 | `experiments/training_host_20261105/host.json`、`settings.json`、`scripts/capture_training_host.py`、docs/16 |
| stage 4 线障偏移三 seed 两次 | `experiments/line_offsets_4_20261105/offset_analysis.json` 及原始日志 |
| stage 6 持续注入动态风三 seed 两次 | `experiments/wind_persistent_6_20261105/wind_interpolation_analysis.json` |
| stage 5 线障候选偏移三 seed 两次 | `experiments/line_offsets_5_20261105/offset_analysis.json` |
| 持续状态和开环 PWM 场的运行时碰撞回调 | `experiments/fall_collision_pilot_20261105/`、`scripts/stage_fall_collision.gdb`；碰撞对方未知且状态被反复覆盖 |
| stage 6 线障候选偏移三 seed 两次 | `experiments/line_offsets_6_20261105/offset_analysis.json` |
| 动态风 18 场传感器时钟和 IMU 分布 | `experiments/wind_persistent_{4,5,6}_20261105/` 各 `sensor_clock_summary.json`、`imu_analysis.json`，`scripts/analyze_imu_samples.py` |
| 三 stage/三 seed 的固定 PWM 事后位置范围 | `experiments/wind_pwm_step_{4,5,6}_20261105/motion_analysis.json`、`scripts/analyze_pwm_motion.py`；真值未用于控制 |
| 七实例第一轮 300 秒空载存活 | `experiments/concurrency_7_long_a_20261105/results.json`、`batch00_attempt00_liveness.jsonl`；有限窗口 |
| 七实例第二轮 300 秒存活及 60 秒订阅负载 | `experiments/concurrency_7_long_b_20261105/results.json`、`measurement.json` 和逐秒 liveness；一个 IMU header 重复 |
| 粗/细线组件映射与实例首场 | `experiments/line_instances_pilot_20261105/component_constructor.disasm`、`component_selector.disasm`、`instances_analysis.json`、原始变换日志 |
| 原始 LiDAR 帧及布局首场 | `experiments/lidar_snapshot_pilot_20261105/`、`scripts/lidar_snapshot_probe.py` |
| 动态风与固定 PWM 的时间重叠首场 | `experiments/wind_pwm_transition_pilot_20261105/overlap_analysis.json`、`scripts/analyze_wind_pwm_overlap.py`；非因果/延迟证明 |
| stage 4 粗/细组件变换三 seed 两次 | `experiments/line_instances_4_20261105/instances_analysis.json`、各 GDB 原始变换 |
| stage 4 LiDAR 原始帧三 seed 两次 | `experiments/lidar_snapshot_4_20261105/lidar_analysis.json`、每场 payload 与 metadata |
| 非直接调用碰撞回调的分派栈 | `experiments/collision_stack_pilot_20261105/`、`scripts/stage_collision_stack.gdb`；接触对象未知 |
| stage 5/6 粗/细组件变换三 seed 两次 | `experiments/line_instances_5_20261105/instances_analysis.json`、`experiments/line_instances_6_20261105/instances_analysis.json` |
| stage 5/6 初始位置原始 LiDAR 三 seed 两次 | `experiments/lidar_snapshot_5_20261105/lidar_analysis.json`、`experiments/lidar_snapshot_6_20261105/lidar_analysis.json` |
| 原点粗/细线夹具及无夹具的固定 PWM 先导对照 | `experiments/line_fixture_thick_pilot_20261105/`、`experiments/line_fixture_0_pilot_20261105/`、`experiments/line_fixture_-1_pilot_20261105/`、`scripts/stage_line_fixture.gdb`；未确认碰撞对象 |
| stage 4 持续风过渡与 PWM 三 seed 两次 | `experiments/wind_pwm_transition_4_20261105/overlap_analysis.json`、`pwm_analysis.json`、原始消息；近似时间对齐 |
| 碰撞分派事件前 96 字节及栈 | `experiments/line_fixture_event_pilot_20261105/`、`scripts/stage_line_fixture_event.gdb`；对象长度及接触对方未知 |
| stage 5 动态风过渡与 PWM 三 seed 两次 | `experiments/wind_pwm_transition_5_20261105/overlap_analysis.json` 和原始流 |
| 固定线障夹具实验无夹具对照三 seed 两次 | `experiments/line_fixture_control_20261105/`、`scripts/run_line_fixture.py` |
| stage 6 动态风与固定 PWM 三 seed 两次 | `experiments/wind_pwm_transition_6_20261105/overlap_analysis.json`；seed 123 非零命令未与变化重叠 |
| 碰撞事件 vptr 对应 DroneHitEvent | `experiments/line_fixture_event_pilot_20261105/event_analysis.json`、`event_vtable_symbols.txt`；不识别接触对象 |
| stage 6 seed 123 迟发固定 PWM 补样 | `experiments/wind_pwm_transition_6_seed123_late_20261105/overlap_analysis.json`、`scripts/pwm_transition_late_probe.py` |
| 细线固定夹具与无夹具三 seed 两次 | `experiments/line_fixture_thin_20261105/`、`experiments/line_fixture_control_20261105/`；未确认碰撞对方 |
| 粗/细线固定夹具三 seed 两次与对照 | `experiments/line_fixture_comparison_20261105.json`、`experiments/line_fixture_{control,thin,thick}_20261105/`、`scripts/analyze_fixture_comparison.py`；事件对方未识别 |
| 车辆随机桶三 seed 两次运行时重建 | `experiments/vehicle_branch_4_20261105/branch_analysis.json`、`scripts/analyze_vehicle_branch.py`、`scripts/stage_vehicle_branch.gdb` |
| false reset 前后线障提交与风数组对照 | `experiments/reset_instances_pilot_20261105/reset_analysis.json`、`scripts/stage_reset_instances.gdb`；单 seed 首场 |
| 固定车辆路径、世界轨迹及回调首场 | `experiments/vehicle_fixture_pilot_20261105/`、`scripts/stage_vehicle_fixture.gdb`；未证明车辆接触 |
| false reset 与车辆空载对照三 seed 两次 | `experiments/reset_instances_20261105/reset_analysis.json`、`experiments/vehicle_fixture_idle_{0,1}_20261105/vehicle_fixture_analysis.json`；reset 返回 false、车辆事件对方未知 |
| 车辆桶强制 0 的三 seed 两次对照 | `experiments/vehicle_forced_bucket_4_20261105/analysis.json`、`scripts/stage_vehicle_forced_bucket.gdb`、`scripts/run_stage_vehicle_forced_bucket.py`；仅开发注入 |
| false reset 前后风随机流连续性复算 | `experiments/reset_instances_20261105/wind_stream_analysis.json`、`scripts/analyze_reset_wind_stream.py`；三 seed 各两次逐字节吻合连续流模型 |
| 命中事件构造点及粗线 HISM 组件归因 | `experiments/line_event_source_static_20261105/`、`experiments/line_event_component_thick_20261105/event_component_analysis.json`、`scripts/analyze_line_event_component.py`；组件共享实例，不能区分具体线 |
| 细线 HISM 组件命中三 seed 两次 | `experiments/line_event_component_thin_20261105/event_component_analysis.json`；共享组件不识别具体实例 |
| 粗线命中结果候选实例索引及失败探针 | `experiments/line_hit_instance_thick_20261105/hit_instance_analysis.json`、`experiments/line_hit_result_thick_pilot_20261105/`、`scripts/analyze_line_hit_instance.py` |
| 粗细线候选命中实例索引的完整重复矩阵 | `experiments/line_hit_instance_{thick,thin}_20261105/hit_instance_analysis.json`、`scripts/run_line_hit_result.py`；候选偏移待独立类型验证 |
| 车辆命中来源先导及中止样本 | `experiments/vehicle_contact_pilot_20261105/`、`experiments/vehicle_contact_world_pilot_20261105/`、`experiments/vehicle_contact_z400_pilot_20261105/INVALID.md`；前两场调试流就绪，后一场注入前中止 |
| 普通 IMU 就绪的车辆来源对照 | `experiments/vehicle_contact_imu_gate_20261105/contact_analysis.json`、`scripts/run_vehicle_contact.py`、`scripts/analyze_vehicle_contact.py`；六场未命中已知车辆 actor/采样组件 |
| 受约束不合格的 stage 6 迟发补样 | `experiments/wind_pwm_transition_6_seed123_late_20261105/`；延迟根据调试风事后时序选择，仅保留诊断 |
| 粗线实例碰撞普通 IMU 就绪复验 | `experiments/line_hit_imu_thick_20261105/hit_instance_analysis.json`；三 seed 两次，固定开环 PWM |
| 细线实例碰撞普通 IMU 就绪复验 | `experiments/line_hit_imu_thin_20261105/hit_instance_analysis.json`；三 seed 两次，固定开环 PWM |

| 普通 IMU 就绪的 stage 4 长 PWM 对照及未跟随样本 | `experiments/wind_pwm_imu_long_4_20261105/phase_coverage.json`、`overlap_analysis.json`、原始流；`experiments/wind_pwm_imu_long_pilot_20261105/`；`scripts/analyze_pwm_phase_coverage.py`、`scripts/run_wind_pwm_fixed_long.py`；输入设计独立性及下降沿因果未证 |

| PWM 分发链与无新增断点对照 | `experiments/pwm_chain_control_20261105/phase_coverage.json`、`experiments/pwm_chain_pilot_20261105/chain_analysis.json`、`experiments/pwm_chain_trace_20261105/chain_analysis.json`；`scripts/stage_pwm_chain_probe.gdb`、`scripts/analyze_pwm_chain.py`；trace 第二场就绪前 SIGSEGV，保留原日志 |

| OL2 直接加载的 12 门、三 seed 各两次 | `experiments/ol2_direct_gates_20261105/gate_comparison.json`、原始 GDB 日志；`experiments/ol2_config_overlay_pilot_20261105/`（IMU 就绪失败）和 `experiments/ol2_overlay_retry_pilot_20261105/`（只改 Lobby URL 仍加载 OL1）；`scripts/build_lobby_map_overlay.py`、`scripts/run_ol2_direct_gate_trace.py` |

| OL1 直接启动对照和 OL2 ROS 初始化异常 | `experiments/ol1_direct_control_pilot_20261105/`、`experiments/ol2_direct_throw_pilot_20261105/`、`experiments/ol2_ros_time_init_pilot_20261105/`、`experiments/ol2_ros_time_init_crash_trace_20261105/`、`experiments/ol2_ros_time_init_node_trace_20261105/`；`scripts/ros_time_preinit.cpp`、`scripts/runtime_ol2_throw_trace.gdb`、`scripts/runtime_ol2_time_preinit.gdb`；均未恢复 IMU |

| 直接 OL2 频率门控、切图重定向及未完成场景证据 | `experiments/ol2_frequency_gate_pilot_20261105/`、`experiments/ol2_url_fields_pilot_20261105/`、`experiments/ol2_redirect_20261105/post_load_analysis.json`、`experiments/ol2_interrupt_stack_pilot_20261105/`、`experiments/ol2_world_tick_pilot_20261105/`、`experiments/ol2_cancel_travel_pilot_20261105/`；`scripts/runtime_ol2_{frequency_gate,redirect,cancel_travel}.gdb`、`scripts/analyze_ol2_redirect.py`；第二次加载完成与游戏推进未证 |

| 经 Lobby 的 OL2 早／晚切图与 OL1 静止对照 | `experiments/lobby_ol2_long_pilot_20261105/seed123_repeat1/sample_analysis.json`、`experiments/lobby_ol2_early_pilot_20261105/`、`experiments/lobby_ol1_control_pilot_20261105/`、`experiments/lobby_ol2_init_return_pilot_20261105/`、`scripts/analyze_lobby_ol2_samples.py`；场景更新及 OL2 可训练性未证 |

| reset 检查数据来源与根组件高度干预下成功响应 | `experiments/reset_cache_static_20261105/`、`experiments/reset_cache_{trace,source}_pilot_20261105/`、`experiments/reset_root_hold_20261105/{root_hold_analysis,reset_analysis,wind_stream_analysis}.json`；`scripts/stage_reset_root_hold.gdb`、`scripts/analyze_reset_root_hold.py`；成功依赖高度干预，风未重放 |

| stage 4 高度固定与风重播种下的受控 reset 生成重放 | `experiments/reset_wind_interpose_20261105/{reset_analysis,reseeded_wind_analysis,root_hold_analysis}.json`、`experiments/reset_wind_interpose_pilot_20261105/interposer_manifest.json`、`scripts/wind_reseed_interposer.c`、`scripts/analyze_reset_wind_reseeded.py`；`experiments/reset_rng_trace_pilot_20261105/failed_reseed_analysis.json` 是失败 GDB 跳转，不证明完整物理重放 |

| stage 5 干预式 reset 单 seed 先导及事后位置 | `experiments/reset_wind_interpose_stage5_pilot_20261105/`、`scripts/analyze_reset_pose_prepost.py`；仅单场、不证明完整物理重放 |

| stage 6 干预式 reset 单 seed 先导 | `experiments/reset_wind_interpose_stage6_pilot_20261105/`；前后提交与风匹配模型，完整物理重放未证 |

| stage 5 干预式 reset 三 seed 两次 | `experiments/reset_wind_interpose_stage5_20261105/{reset_analysis,reseeded_wind_analysis,pose_prepost_analysis}.json`；实际 spawn、车辆及物理状态未完整验证 |

| stage 6 干预式 reset 三 seed 两次 | `experiments/reset_wind_interpose_stage6_20261105/{reset_analysis,reseeded_wind_analysis,pose_prepost_analysis}.json`；只验证提交与关键帧，完整物理和自然赛程重放未证 |

- 车辆接近和子组件几何先导：`experiments/vehicle_proximity_zminus150_pilot_20261105/`、`experiments/vehicle_child_z0_pilot_20261105/`；移动调用参数成对先导：`experiments/vehicle_move_arg0_pilot_20261105/`、`experiments/vehicle_move_arg1_pilot_20261105/`；调用汇编 `artifacts/rea/vehicle_move_call_12_0_0_5.txt` 与 `artifacts/rea/vehicle_move_callee_12_0_0_5.txt`。分析条件及未验收项见 `docs/18_acceptance_ledger.md`。

- 同车辆路径的固定开环 PWM 先导：`experiments/vehicle_child_pwm_pilot_20261105/`；运行脚本 `scripts/run_vehicle_child_pwm.py`，结果边界见验收台账。

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

- 训练环境规格草案及未完成门槛：`docs/19_training_environment_spec.md`。

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

## 2026-11-06 收尾证据

| 结论或未完成项 | 证据 |
|---|---|
| 12 图门布局是同一几何的刚性变换 | `scripts/verify_gate_rigid_layout.py`、`experiments/ol_gate_rigid_20261106/analysis.json`；门间距差 ≤0.012 cm、yaw+平移残差 ≤0.009 cm、缩放一致；OL2 BeginPlay 6/6 场 12/12 门匹配来自 `experiments/ol2_direct_gates_20261105/gate_comparison.json` |
| 自然生成粗线实例的受控组件扫掠与 7→12 | `experiments/natural_line_component_20261106/{manifest,analysis}.json`、各 seed `gdb.log`；`scripts/stage_natural_line_sweep2.gdb`；三 seed 目标组件+索引 0 命中 30/34/30，target 后 10 ms 内 state 12 请求 21/22/21；非自然飞行 |
| 原生 reset 检查组件不随固定 PWM 物理运动 | `experiments/reset_native_component_20261106/analysis.json`、`fixed_pwm_move_reset/`、`fixed_lift_reset/`；`scripts/fixed_pwm_lift_reset_probe.py`、`scripts/fixed_lift_reset_probe.py`、`scripts/stage_reset_native_probe.gdb`；服务 false，成功仍依赖诊断干预 |
| 固定 PWM + 订阅负载下 6/8 实例失败与 4 实例上限 | `experiments/concurrency_pwm_failure_summary_20261106/summary.json`、`experiments/concurrency_6_pwm_300s_20261106/`、`experiments/concurrency_8_pwm_180s_20261106/`、`experiments/concurrency_8_pwm_gdb_180s_20261106/`；6 实例 seed 123 容器寿命约 32.2 s、8 实例 seed 2026 约 11.1 s，均 139 非 OOM，栈在 `0x2fbda42` 空读取；GDB 8 实例场另捕获 seed 789 `0x26b6223` r12=-1 读取地址 7、seed 1002 `rip=0` 空函数指针调用 |
| 最终训练环境约束更新 | `docs/19_training_environment_spec.md`、`docs/15_seed_dependency_matrix.md`、`docs/16_headless_requirements.md`、`docs/17_runtime_validation_20261104.md`、本页与 `docs/18_acceptance_ledger.md` |
