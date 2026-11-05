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
