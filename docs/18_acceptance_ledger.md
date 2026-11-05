# 动态机制验收清单

状态截至 2026-11-05；本表是逐项证据与未完成项，不是最终验收通过声明。目标版本为模拟器 12.0.0.5，主程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`。官方发布文件保持只读；标注为注入的场次在开发容器中临时更改寄存器或内存，调试真值只用于事后评估。运行器的 `observed` 只代表其所检查条件，不自动证明每一个采样流成功。

| 要求 | 当前证据 | 状态与剩余工作 |
|---|---|---|
| 从既有基线继续；不重做门/PWM/四实例基线 | `docs/17_runtime_validation_20261104.md`、Git 历史 | 复用原有基线；新增的是室外注入、风下 PWM、车辆、碰撞和不同并发范围。 |
| 强制进入 stage 4/5/6 或直接进入指定入口 | `experiments/stage_inject_{4,5,6}_20261104/`、`scripts/stage_inject.gdb` | 三 stage 的入口参数已注入，每 stage 三 seed 各两场均命中 `0x2fbac30`、`0x2fc77b0`、`0x2fb23b0`；车辆入口另通过调用目标重定向命中并返回。注入场不是合法赛程。 |
| 风：seed 123/456/2026 各重复，算法/参数/分布、动态原始数据 | `experiments/wind_offline_model_20261105/`、`experiments/wind_persistent_{4,5,6}_20261105/`、docs/15、17 | fresh 生成的全部 18 场关键帧由离线 float32 模型逐字节重建；持续注入的 18 场记录随时间变化的 ROS 矢量。正常赛程前随机流消耗、自然风区、时钟标定和留出 seed 尚未证明。单次状态写入场的风话题冻结 25 秒，失败样本见 `experiments/wind_transition_{4,5,6}_20261105/`。 |
| 粗/细线障：seed、算法、数量/位置/运动/碰撞 | `experiments/line_offsets_{4,5,6}_20261105/`、`experiments/obstacle_inventory_{4,5,6}_20261105/`、docs/14、15 | 内层候选的 LCG 和距离公式、三 seed 两次候选点/序列一致性已核验；候选不等于实际 spawn 或障碍间距，阶段 4 有大量 SpawnActor 碰撞失败。已识别粗/细 HISM 并记录全部实例提交变换与返回索引：stage 4 为 272/101、stage 5 为 594/524、stage 6 为 0/1434；同 seed 重复的变换哈希一致。实际碰撞对象及自然运动未验收。 |
| 车辆：seed、分支、路径、速度和动态轨迹 | `experiments/vehicle_redirect_4_20261105/`、`experiments/vehicle_forcegate_4_20261105/`、`experiments/vehicle_world_4_20261105/` | 入口/返回三 seed 两次；额外强制状态门槛后记录车辆五点路径、进度和组件世界坐标。seed 456 两次路径不同；自然时序、分支概率、路径重复性的原因、碰撞未确定。 |
| 碰撞：回调与动态物理 | `experiments/collision_inject_ignore{0,1}_20261105/`、`experiments/fall_collision_pilot_20261105/` | 合成回调在忽略标志 0/1 下三 seed 两次分别返回 state 12/7；另有开环 PWM 运行时回调。粗/细线夹具场已有三 seed 两次组件和候选实例索引匹配；字段独立类型验证、车辆接触、力和自然终止未确认；逐 tick 强制状态会覆盖终止。 |
| PWM：动作/输出/动力学，seed 重复 | `experiments/wind_pwm_step_{4,5,6}_20261105/`、`experiments/pwm_20261104/` | 三 stage、三 seed 各两次固定开环命令、PWM 输出、IMU/GPS 和事后位姿已保存；另有三 stage、三 seed 各两次的动态风与非零 PWM 近似重叠样本；扰动因果及可辨识动力学模型尚缺。header 首次匹配差不是已标定端到端延迟。 |
| 传感器：IMU、GPS、LiDAR、相机、时间/噪声 | `experiments/wind_persistent_{4,5,6}_20261105/`、`experiments/sensor_clock_rendered_20261104/` | 三 stage/seed/重复有 IMU、GPS、LiDAR 消息时钟；IMU 分量统计与原始流已保存，变化不能单独解释为噪声。NullRHI 相机为空，有渲染单场相机有数据；动态障碍点云、多 seed 图片与准确端到端延迟未验收。 |
| reset / 随机流重放 | `experiments/reset_semantics_20261104/`、`experiments/stage_reset_inject_retry_20261104/` | 三 seed 各两次官方 ROS reset 均返回 false；游戏 reset 入口的 FRandomStream 回到 seed，前后各 373 个线障提交变换的多重集合相同，但风数组均不同。成功 reset、各机制场景重放以及 libc 状态变化的确切原因未验证。 |
| 8 实例崩溃根因或稳定并发上限 | `experiments/concurrency_gdb_20261104/`、`experiments/concurrency_frequency_overlap/`、`experiments/concurrency_followup_20261104/` | 八实例曾崩溃，单次栈指向频率统计 deque，但根因未证；7 路两次各 300 秒存活，第二轮含 60 秒订阅，部分 LiDAR/源时钟变慢。该主机有限空载观测支持七路操作边界，但不同负载或长期的稳定上限及八路根因仍未知。 |
| 训练规格和逐类可复现脚本 | `docs/15_seed_dependency_matrix.md`、`docs/16_headless_requirements.md`、`experiments/training_host_20261105/`、`scripts/` | 硬件、镜像 ID、二进制和 settings 哈希、实际 settings 副本及已完成机制的脚本已记录；自定义训练环境完整等价和未完成类的验证尚缺。 |
| OL1 12 门和额外地图 | `experiments/runtime_trace_20261104_v2/`、`experiments/gates_ol2_20261104/` | OL1 三 seed 两次门变换一致；第二地图请求均实际加载 OL1，门洞物理尺寸与跨地图核验未完成。 |
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
