# 动态机制验收清单

状态截至 2026-11-06；本表是逐项证据与未完成项，不是最终验收通过声明。目标版本为模拟器 12.0.0.5，主程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`。官方发布文件保持只读；标注为注入的场次在开发容器中临时更改寄存器或内存，调试真值只用于事后评估。运行器的 `observed` 只代表其所检查条件，不自动证明每一个采样流成功。

重要方法限制：仓库中 此前检查到的 32 个旧版本运行器通过调试风话题首条消息判断就绪，虽然固定 PWM 策略不读取风值或位姿，但探针和动作启动时序依赖该调试流。按“调试真值只用于事后评估”的严格解释，这些场次不能直接作为符合该约束的最终控制验收，需以非调试话题就绪门槛重跑或逐场证明无该依赖。新车辆对照改用普通 IMU 消息就绪。

另一个独立限制：stage 6 seed 123 的 6.5 秒 PWM 补样延迟是根据前场事后调试风时序选择的；即使命令器在线不订阅真值，输入设计仍使用了调试真值。按用户严格约束，该补样仅保留为诊断失败记录，不能用于最终受限验收。

| 要求 | 当前证据 | 状态与剩余工作 |
|---|---|---|
| 从既有基线继续；不重做门/PWM/四实例基线 | `docs/17_runtime_validation_20261104.md`、Git 历史 | 复用原有基线；新增的是室外注入、风下 PWM、车辆、碰撞和不同并发范围。 |
| 强制进入 stage 4/5/6 或直接进入指定入口 | `experiments/stage_inject_{4,5,6}_20261104/`、`scripts/stage_inject.gdb` | 三 stage 的入口参数已注入，每 stage 三 seed 各两场均命中 `0x2fbac30`、`0x2fc77b0`、`0x2fb23b0`；车辆入口另通过调用目标重定向命中并返回。注入场不是合法赛程。 |
| 风：seed 123/456/2026 各重复，算法/参数/分布、动态原始数据 | `experiments/wind_offline_model_20261105/`、`experiments/wind_persistent_{4,5,6}_20261105/`、docs/15、17 | fresh 生成的全部 18 场关键帧由离线 float32 模型逐字节重建；持续注入的 18 场记录随时间变化的 ROS 矢量。正常赛程前随机流消耗、自然风区、时钟标定和留出 seed 尚未证明。单次状态写入场的风话题冻结 25 秒，失败样本见 `experiments/wind_transition_{4,5,6}_20261105/`。 |
| 粗/细线障：seed、算法、数量/位置/运动/碰撞 | `experiments/line_offsets_{4,5,6}_20261105/`、`experiments/obstacle_inventory_{4,5,6}_20261105/`、docs/14、15 | 内层候选的 LCG 和距离公式、三 seed 两次候选点/序列一致性已核验；候选不等于实际 spawn 或障碍间距，阶段 4 有大量 SpawnActor 碰撞失败。已识别粗/细 HISM 并记录全部实例提交变换与返回索引：stage 4 为 272/101、stage 5 为 594/524、stage 6 为 0/1434；同 seed 重复的变换哈希一致。固定夹具中已匹配粗细线碰撞组件和候选实例索引；自然场景接触、运动及字段独立类型尚未验收。 |
| 车辆：seed、分支、路径、速度和动态轨迹 | `experiments/vehicle_redirect_4_20261105/`、`experiments/vehicle_forcegate_4_20261105/`、`experiments/vehicle_world_4_20261105/` | 入口/返回三 seed 两次；额外强制状态门槛后记录车辆五点路径、进度和组件世界坐标。seed 456 两次路径不同；停车夹具三 seed 两次记录了车辆 actor 和推定子组件来源的命中。自然时序、分支概率、路径重复性原因和自然赛程接触仍未确定。 |
| 碰撞：回调与动态物理 | `experiments/collision_inject_ignore{0,1}_20261105/`、`experiments/fall_collision_pilot_20261105/` | 合成回调在忽略标志 0/1 下三 seed 两次分别返回 state 12/7；另有开环 PWM 运行时回调。粗/细线夹具场三 seed 两次组件与候选实例索引匹配；车辆停车夹具三 seed 两次来源匹配，额外先导按事件指针对应游戏碰撞回调。另在停止状态强制后，粗、细线及车辆停车夹具各三 seed 两次，来源事件经指针和同线程顺序关联到 7→12 写入，随后离散采样仍为 12；见各释放矩阵。实例字段独立类型、力、完整结算和自然终止仍未确认。 |
| PWM：动作/输出/动力学，seed 重复 | `experiments/wind_pwm_step_{4,5,6}_20261105/`、`experiments/pwm_20261104/` | 三 stage、三 seed 各两次固定开环命令、PWM 输出、IMU/GPS 和事后位姿已保存；另有三 stage、三 seed 各两次的动态风与非零 PWM 近似重叠样本；补充同主机三 seed 两次 ROS 发布至接收与命令回调至调试 PWM 输出定时；未加载探针对照三场也匹配后续阶跃。物理电机响应、扰动因果及可辨识动力学模型尚缺，header 首次匹配差不是端到端延迟。 |
| 传感器：IMU、GPS、LiDAR、相机、时间/噪声 | `experiments/wind_persistent_{4,5,6}_20261105/`、`experiments/sensor_clock_rendered_20261104/` | 三 stage/seed/重复有 IMU、GPS、LiDAR 消息时钟；IMU 分量统计与原始流已保存，变化不能单独解释为噪声。NullRHI 相机为空，有渲染单场相机有数据；同主机 IMU/LiDAR 发布至订阅回调已按唯一时间戳配对，但采样至发布尚未标定。动态障碍点云、多 seed 图片与准确端到端延迟未验收。 |
| reset / 随机流重放 | `experiments/reset_instances_20261105/`、`experiments/reset_wind_interpose_{20261105,stage5_20261105,stage6_20261105}/` | 无高度干预的六场响应均为 false；根高度固定与显式风重播种下，stage 4/5/6 各三 seed 两次均 true，线障提交及风数组前后重现。原生成功和完整物理／车辆／自然赛程重放仍未验证。 |
| 8 实例崩溃根因或稳定并发上限 | `experiments/concurrency_gdb_20261104/`、`experiments/concurrency_frequency_overlap/`、`experiments/concurrency_followup_20261104/` | 八实例曾崩溃，单次栈指向频率统计 deque，但根因未证；七路两次各 300 秒存活后，追加七路在全部就绪约 9 秒后有一实例退出 139；不能声称七路稳定。四路两次在固定 PWM 发布及 IMU/LiDAR 订阅负载下各完成约 300 秒观察；八路加锁与未加锁空载各一次完成 180 秒。它们是本机特定负载的有限观察，长期稳定及八路根因仍未知。 |
| 训练规格和逐类可复现脚本 | `docs/15_seed_dependency_matrix.md`、`docs/16_headless_requirements.md`、`docs/19_training_environment_spec.md`、`experiments/training_host_20261105/`、`scripts/` | 硬件、镜像 ID、二进制和 settings 哈希、实际 settings 副本及已完成机制的脚本已记录；自定义训练环境完整等价和未完成类的验证尚缺。 |
| OL1 12 门和额外地图 | `experiments/runtime_trace_20261104_v2/`、`experiments/ol2_direct_gates_20261105/` | OL1 与开发补丁直接加载的 OL2 均覆盖三 seed 两次、12 门 BeginPlay；OL2 直接加载缺少可用 IMU；重定向后虽有 ROS 流但来源世界和正常状态未确认。门洞物理尺寸与可飞行跨图初始化未完成。 |
| 失败样本、限制、原始数据与提交推送 | docs/09、15、16、17、Git 历史；各 `experiments/` | 各试验失败/原始流保留；逐阶段提交并推送。仍需在最终交付时重核远端 HEAD、当前文件和所有未完成项。 |

重要失败样本包括：`experiments/vehicle_call_pilot_20261104/`（GDB call 未完成）、`experiments/vehicle_trampoline_v2_pilot_20261105/`（错误返回标记后 SIGSEGV）、`experiments/stage_reset_inject_pilot_20261104/`（缺 ROS 服务类型，未发出调用）、`experiments/force_wind_tick_4_20261105/`（PWM 消息类型缺失，其他流保留）、`experiments/pwm_20261104/seed456_motor_pulses_failed/`（位置限制）及上述并发和地图失败。不要把失败场次的 `observed` 状态解释为全部机制通过。

剩余工作按证据缺口推进：实际粗/细线和车辆实例/碰撞对象及动态数据；自然或等价开发注入下的动态风与 PWM/传感器耦合和时钟标定；reset 成功及随机流重放；8 路根因或经明确持续时间、重复次数和负载限制界定的稳定上限；额外地图与最终逐项复核。若不能证明某项，保留为未完成，不用已有局部样本替代。

线障首场已区分粗/细组件并采集提交变换和非负返回索引；初始位置 LiDAR 原始帧已保存，动态风与固定 PWM 采样有时间重叠。三 stage/seed 重复正在运行，真实几何接触仍未证明。

stage 4 三 seed 各两次粗/细 HISM 组件提交变换及原始 LiDAR 帧已采样；不同帧的 LiDAR 字节差不能单独解释为 seed 差异。物理碰撞对象仍未证明。

三个 stage 的粗/细 HISM 组件提交变换与原始初始位置 LiDAR 各三 seed 两次完成；真实障碍接触、动态可见性及自然赛程仍未证明。

固定世界坐标粗/细线夹具与无夹具的单 seed 先导场呈不同事后轨迹并记录回调；碰撞对方与因果尚不明，多 seed 重复进行中。夹具位置为预设输入，不使用调试真值控制。

stage 4 三 seed 两次已在持续风过渡期间记录开环 PWM、传感器和事后真值，时间重叠不证明扰动因果或动力学辨识；stage 5/6 对照仍在采集。

stage 6 seed 123 采用事后选定的固定延迟补样后，两场均有动态风与非零 PWM 近似重叠；不证明因果或已标定时延。细线固定夹具组六场与无夹具的事后 z 范围不同，碰撞组件归因仍未知；粗线重复尚在进行。

粗、细及无夹具的固定 PWM 对照均已完成三 seed 两次；运动和回调差异可复查，但不能识别每个事件的对方组件或推广自然碰撞。粗线单场已有 `DroneHitEvent` vtable 证据。

车辆随机桶 2/7/0 的三 seed 两次已从运行时状态重建；seed 456 后续路径重复性差的完整原因仍未明。

完整 reset 和车辆空载对照见 `experiments/reset_instances_20261105/reset_analysis.json`、`experiments/vehicle_fixture_idle_{0,1}_20261105/vehicle_fixture_analysis.json`。车辆固定路径组六次均写入一次，记录的最近世界原点距离在约 29–344 cm；每场有两个 `DroneHitEvent`，最近原点样本前后一秒内均无事件。车辆采样位置不是无人机碰撞体位置，事件不含已识别的对方，不能将该负样本解释为碰撞通道是否工作。seed 456 两场固定路径仅有 12 个世界样本，覆盖范围不同。

车辆桶受控对照已完成：`experiments/vehicle_forced_bucket_4_20261105/analysis.json` 中，三 seed 各两次将原桶 2/7/0 强制为 0，六次五点路径字节一致并有车辆运动样本；仅证明该开发注入路径可重复。原 seed 456 间接分支路径不重复仍保留，原因未证。

reset 后风数组已有连续随机流的可复现解释：一次播种、两次连续生成（各 54 次 `rand()`）可逐字节复现六场前后共 12 组数组，见 `experiments/reset_instances_20261105/wind_stream_analysis.json`。这不证明成功 reset 或排除不可见的等效状态操作。

粗线碰撞组件已取得更强证据：`experiments/line_event_component_thick_20261105/event_component_analysis.json` 的三 seed 各两次事件源 RCX 与粗线 HISM 指针同场匹配 39–43 次；该组件共享多个实例，具体实例、碰撞力及自然赛程仍未确认。事件对象的构造分配仅 16 字节，不能从以往额外读取的字节推断对方。先导 PWM 类型缺失失败保存在 `experiments/line_event_source_pilot_20261105/`。

细线组件也已覆盖三 seed 各两次：`experiments/line_event_component_thin_20261105/event_component_analysis.json` 中，每场 16–24 次事件源参数匹配细线 HISM，另有 4 次指向其他组件；同样不识别具体实例。

粗线夹具实例级受控证据：`experiments/line_hit_instance_thick_20261105/hit_instance_analysis.json` 中三 seed 各两次，预选实例返回索引 7；同组件 39–46 次命中结果块 `+0x58` 均为 7，其他组件均为 -1。候选偏移尚未由独立类型元数据确认，限制为该版本和注入场；先导场返回断点持续输出的缺陷保留在 `experiments/line_hit_result_thick_pilot_20261105/`。

粗细线的第 8 个夹具提交实例均有三 seed 各两次的索引对照，细线每场 16–29 次命中候选值与返回索引 7 相同，见 `experiments/line_hit_instance_thin_20261105/hit_instance_analysis.json`。此为本二进制受控实验的实例归因；独立类型元数据、自然赛程和碰撞力仍待验证。

车辆正向接触仍未取得：普通 IMU 就绪的 `experiments/vehicle_contact_imu_gate_20261105/contact_analysis.json` 中，三 seed 各两次共六场的事件来源均不匹配本场车辆 actor 或所采样组件，样本数分别 18、20、22、21、21、21。仅表示该固定路径及动作窗口内的比较结果，不能排除其他子组件或未观察到的碰撞。

粗线夹具实例对照已用普通 IMU 就绪重跑三 seed 各两次：`experiments/line_hit_imu_thick_20261105/hit_instance_analysis.json` 的 41–43 个同组件命中/场均与索引 7 一致；字段 ABI 和自然场景仍待验证。

细线也已完成普通 IMU 就绪重跑：`experiments/line_hit_imu_thin_20261105/hit_instance_analysis.json` 的三个 seed 各两次均返回索引 7，每场 16–20 个同组件命中的候选值均为 7。仍需独立类型和自然场景验证。

普通 IMU 就绪的 stage 4 长序列复验见 `experiments/wind_pwm_imu_long_4_20261105/`（三 seed 各两次）及先导场。六场的 0.2→0.3→0.4 上升阶段有匹配输出，但后续 0.2 和 0 阶段均无匹配，输出维持 0.4；因果未定，不能将后半段当成有效阶跃或已标定时延。风变化与前述非零命令阶段每场有 41 个近似重叠样本，但这不证明因果。输入时间表在既有风实验之后制定，其设计未证明独立于此前事后真值，不计入严格受限控制验收。此结果不改变 reset、车辆自然碰撞、额外地图、并发上限和最终训练规格的未完成状态。

PWM 分发链对照补充：seed 123 的两场无新增分发断点对照均复现下降阶段无匹配；两场在 `0x26ea0f0`、`0x26ea000` 增加 change-only 断点的成功场则在两个入口及 PWM 输出均观察到下降值。断点改变调度，不能确认停留根因或标定延迟；一次带断点启动在就绪前 SIGSEGV，未产生动作数据。见 `experiments/pwm_chain_{control,pilot,trace}_20261105/`。

额外地图门核验取得直接运行证据：独立补丁配置直接加载 OL2，seed 123/456/2026 各两次均捕获并与 OL2 静态表配对 12 门，60 个跨运行比较位置、四元数和缩放差为零。只覆盖 BeginPlay；长等待试场无 IMU 话题并报告世界更新异常，不能视为可飞行或证明门洞尺寸。Lobby URL 单独替换仍加载 OL1 的失败场也保留。见 `experiments/ol2_direct_gates_20261105/gate_comparison.json`。

OL2 直接启动缺失接口的初始化诊断：同配置直接启动 OL1 也在 30 秒内无 IMU；OL2 只读断点在 LiDAR 频率记录路径捕获 `ros::TimeNotInitializedException`。开发 preload 提前初始化时间后三次在频率表访问处 SIGSEGV，一次确认 `node_instance` 为 NULL；并未恢复接口，不能当成修复或八实例根因。见 `experiments/ol1_direct_control_pilot_20261105/`、`experiments/ol2_direct_throw_pilot_20261105/`、`experiments/ol2_ros_time_init_{crash_trace,node_trace}_20261105/`。

OL2 初始化进一步诊断：节点为空时跳过首次频率统计后，先导场记录节点创建与 IMU，随后自动切至 OL1。改写 Browse 的 OL1 目标为 OL2 后，三 seed 各两次出现两次 OL2 LoadMap 入口、第二组门事件和 IMU/LiDAR 流，但没有第二次完成日志；追加的单场未命中第二组门之后的 `0x2fb8f40` 更新断点，不能确认场景或传感器来自完成切图后的世界。取消切图的先导场反复触发 264 次取消且未命中更新断点，属于失败样本。训练可用性仍未验收。见 `experiments/ol2_redirect_20261105/post_load_analysis.json`、`experiments/ol2_world_tick_pilot_20261105/`、`experiments/ol2_cancel_travel_pilot_20261105/`。

经过未修改 Lobby 的单次 OL2 切图对照：晚期和 Browse 入口早期重定向各有 seed 123 先导场，均捕获 12 门和 ROS 消息，但无游戏状态更新/风播种断点；长场 GPS 全零、事后姿态四元数模 0.5。相同静止探针的 OL1 对照捕获 45 次状态更新、风播种及单位姿态四元数。差异原因未定，OL2 可用性仍未通过。见 `experiments/lobby_ol2_{long,early}_pilot_20261105/`、`experiments/lobby_ol1_control_pilot_20261105/`。

受控成功响应：stage 4 三 seed 各两次，在 reset 回调期间每次将根组件世界 z 写为 0（21–23 次），官方服务均返回 `success: True`，检查块七个 float 均为 0。六场前后各 373 个线障提交变换集合相同、风数组不同且与 libc 连续随机流模型逐字节一致；干预结束后事后姿态 z 约 0.3085–0.3088 m。该成功响应依赖高度干预，不证明无干预物理 reset、车辆重放或风重放。见 `experiments/reset_root_hold_20261105/`。

受控补全风随机状态：stage 4 三 seed 各两次，在固定根组件高度并于第二次风生成首个指定调用点显式 `srand(seed)` 的场中，六场官方 reset 返回 true；前后各 373 个线障提交变换多重集合相同，前后 35 项风数组逐字节相同且均与独立模型吻合。该场依赖两个运行时干预及 stage 注入；未证明原生 reset 自动重播种，也未证明车辆、物理接触和完整场景重放。见 `experiments/reset_wind_interpose_20261105/`。

stage 5 单 seed 先导在相同高度固定和显式重播种下返回 true，前后各 1118 次线障提交集合相同、风数组各 37 项与独立模型逐字节一致；事后位置采样前后相距约 0.00070 m，仅为静止、注入场的单次结果，见 `experiments/reset_wind_interpose_stage5_pilot_20261105/`。

stage 6 seed 123 的单次高度固定与重播种先导返回 true，前后各 1434 次线障提交集合相同，35 项风数组及独立模型逐字节一致；事后静止位置采样前后相距约 0.000026 m。仍需重复、多 seed 和完整物理场景证据。

stage 5 完整矩阵已扩展三 seed 各两次：六场在高度固定及第二次风生成前重播种下服务均 true；前后各 1118 个线障提交集合及 37 项风数组一致，风均与模型逐字节吻合。前段末与后段首的事后位置差约 0.00018–0.00089 m，不验证速度、接触、车辆和完整物理重放。见 `experiments/reset_wind_interpose_stage5_20261105/`。

stage 6 受控重放矩阵已扩展三个 seed 各两次：六场响应 true、每场前后各 1434 个线障提交集合和 35 项风数组一致，全部风数组与独立模型逐字节一致；前段末与后段首的事后静止位置差约 0.00016–0.00052 m。仍不证明完整物理、车辆或自然赛程重放，见 `experiments/reset_wind_interpose_stage6_20261105/`。

车辆接近与移动参数先导：固定 z=-150 cm 路径、seed 123 静止场最近的根组件原点距离约 116.13 cm，两个事件来源均未匹配车辆。另以 z=0 cm 路径推定 `actor+0x240` 的子组件（Outer 和相对变换与 HoverTruck 资源相符），事后以其采样变换把无人机根原点投到静态凸包：103 对同步样本中 5 点在凸包内，但两个命中来源均不匹配车辆或该子组件。该子组件类型和运行时碰撞配置未独立确认，静态点包含不证明物理形状接触。见 `experiments/vehicle_proximity_zminus150_pilot_20261105/`、`experiments/vehicle_child_z0_pilot_20261105/`。

在同一路径及种子下，新增断点的移动调用参数对照分别令调用前 `rsi` 低字节为原值 0 和强制 1：各一场的有效配对中分别有 2/86、5/100 个根原点落入推定凸包，各有两个非车辆来源命中，未观察到匹配车辆或推定子组件的命中。汇编只显示此参数被按字节转发，不能证明其是扫掠开关或确定碰撞缺失的原因；结果是受调试影响的单 seed 先导，不是自然碰撞验收。路径和实验假设参考了先前事后真值，运行期间控制不读取真值，不能计作设计独立的严格验收。见 `experiments/vehicle_move_arg{0,1}_pilot_20261105/`、`artifacts/rea/vehicle_move_{call,callee}_12_0_0_5.txt`。

同 z=0 路径附加固定开环 PWM 单 seed 先导：命令进程退出码为 0，事后采样显示无人机根原点改变，106 对 0.03 秒内配对中 4 点落入推定车辆子组件的静态凸包；20 个命中来源均不匹配车辆 actor 或推定子组件。不能仅从命令成功推出电机阶跃完整执行，不能从点包含推出实际物理接触，也不能排除其他子组件或调试时序影响；该路径设计参考过往事后真值。原始命令、PWM 输出、传感器和 GDB 日志见 `experiments/vehicle_child_pwm_pilot_20261105/`。

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

离线随机数 ctypes 后端现拒绝非 glibc 主机，以免把其他 libc 序列误认作目标序列；包含平台守卫的 39 项测试通过，日志见 `experiments/offline_model_host_validation_20261106/post_fix_platform_guard.log`。

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
