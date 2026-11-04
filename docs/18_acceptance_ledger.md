# 动态机制验收清单

状态截至 2026-11-05；本表是逐项证据与未完成项，不是最终验收通过声明。目标版本为模拟器 12.0.0.5，主程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`。官方发布文件保持只读；标注为注入的场次在开发容器中临时更改寄存器或内存，调试真值只用于事后评估。运行器的 `observed` 只代表其所检查条件，不自动证明每一个采样流成功。

| 要求 | 当前证据 | 状态与剩余工作 |
|---|---|---|
| 从既有基线继续；不重做门/PWM/四实例基线 | `docs/17_runtime_validation_20261104.md`、Git 历史 | 复用原有基线；新增的是室外注入、风下 PWM、车辆、碰撞和不同并发范围。 |
| 强制进入 stage 4/5/6 或直接进入指定入口 | `experiments/stage_inject_{4,5,6}_20261104/`、`scripts/stage_inject.gdb` | 三 stage 的入口参数已注入，每 stage 三 seed 各两场均命中 `0x2fbac30`、`0x2fc77b0`、`0x2fb23b0`；车辆入口另通过调用目标重定向命中并返回。注入场不是合法赛程。 |
| 风：seed 123/456/2026 各重复，算法/参数/分布、动态原始数据 | `experiments/wind_offline_model_20261105/`、`experiments/wind_persistent_{4,5,6}_20261105/`、docs/15、17 | fresh 生成的全部 18 场关键帧由离线 float32 模型逐字节重建；持续注入的 18 场记录随时间变化的 ROS 矢量。正常赛程前随机流消耗、自然风区、时钟标定和留出 seed 尚未证明。单次状态写入场的风话题冻结 25 秒，失败样本见 `experiments/wind_transition_{4,5,6}_20261105/`。 |
| 粗/细线障：seed、算法、数量/位置/运动/碰撞 | `experiments/line_offsets_{4,5,6}_20261105/`、`experiments/obstacle_inventory_{4,5,6}_20261105/`、docs/14、15 | 内层候选的 LCG 和距离公式、三 seed 两次候选点/序列一致性已核验；候选不等于实际 spawn 或障碍间距，阶段 4 有大量 SpawnActor 碰撞失败。粗/细资产类型与实例变换、成功数、运动和实际碰撞未验收。 |
| 车辆：seed、分支、路径、速度和动态轨迹 | `experiments/vehicle_redirect_4_20261105/`、`experiments/vehicle_forcegate_4_20261105/`、`experiments/vehicle_world_4_20261105/` | 入口/返回三 seed 两次；额外强制状态门槛后记录车辆五点路径、进度和组件世界坐标。seed 456 两次路径不同；自然时序、分支概率、路径重复性的原因、碰撞未确定。 |
| 碰撞：回调与动态物理 | `experiments/collision_inject_ignore{0,1}_20261105/`、`experiments/fall_collision_pilot_20261105/` | 合成回调在忽略标志 0/1 下三 seed 两次分别返回 state 12/7；另有开环 PWM 运行时回调。碰撞对方、线障/车辆实际接触、力和自然终止未确认；逐 tick 强制状态会覆盖终止。 |
| PWM：动作/输出/动力学，seed 重复 | `experiments/wind_pwm_step_{4,5,6}_20261105/`、`experiments/pwm_20261104/` | 三 stage、三 seed 各两次固定开环命令、PWM 输出、IMU/GPS 和事后位姿已保存；这些短场风保持初始向量，动态风耦合和可辨识动力学模型尚缺。header 首次匹配差不是已标定端到端延迟。 |
| 传感器：IMU、GPS、LiDAR、相机、时间/噪声 | `experiments/wind_persistent_{4,5,6}_20261105/`、`experiments/sensor_clock_rendered_20261104/` | 三 stage/seed/重复有 IMU、GPS、LiDAR 消息时钟；IMU 分量统计与原始流已保存，变化不能单独解释为噪声。NullRHI 相机为空，有渲染单场相机有数据；动态障碍点云、多 seed 图片与准确端到端延迟未验收。 |
| reset / 随机流重放 | `experiments/reset_semantics_20261104/`、`experiments/stage_reset_inject_retry_20261104/` | 多 seed 中官方 ROS reset 响应 false；此前观察 FRandomStream 回到 seed，但三 seed 的注入风数组 reset 前后不同。成功 reset、各机制场景重放以及 libc 状态变化的确切原因未验证。 |
| 8 实例崩溃根因或稳定并发上限 | `experiments/concurrency_gdb_20261104/`、`experiments/concurrency_frequency_overlap/`、`experiments/concurrency_followup_20261104/` | 八实例曾崩溃，单次栈指向频率统计 deque，但根因未证；7 路两次各 300 秒存活，第二轮含 60 秒订阅，部分 LiDAR/源时钟变慢。该主机有限空载观测支持七路操作边界，但不同负载或长期的稳定上限及八路根因仍未知。 |
| 训练规格和逐类可复现脚本 | `docs/15_seed_dependency_matrix.md`、`docs/16_headless_requirements.md`、`experiments/training_host_20261105/`、`scripts/` | 硬件、镜像 ID、二进制和 settings 哈希、实际 settings 副本及已完成机制的脚本已记录；自定义训练环境完整等价和未完成类的验证尚缺。 |
| OL1 12 门和额外地图 | `experiments/runtime_trace_20261104_v2/`、`experiments/gates_ol2_20261104/` | OL1 三 seed 两次门变换一致；第二地图请求均实际加载 OL1，门洞物理尺寸与跨地图核验未完成。 |
| 失败样本、限制、原始数据与提交推送 | docs/09、15、16、17、Git 历史；各 `experiments/` | 各试验失败/原始流保留；逐阶段提交并推送。仍需在最终交付时重核远端 HEAD、当前文件和所有未完成项。 |

重要失败样本包括：`experiments/vehicle_call_pilot_20261104/`（GDB call 未完成）、`experiments/vehicle_trampoline_v2_pilot_20261105/`（错误返回标记后 SIGSEGV）、`experiments/stage_reset_inject_pilot_20261104/`（缺 ROS 服务类型，未发出调用）、`experiments/force_wind_tick_4_20261105/`（PWM 消息类型缺失，其他流保留）、`experiments/pwm_20261104/seed456_motor_pulses_failed/`（位置限制）及上述并发和地图失败。不要把失败场次的 `observed` 状态解释为全部机制通过。

剩余工作按证据缺口推进：实际粗/细线和车辆实例/碰撞对象及动态数据；自然或等价开发注入下的动态风与 PWM/传感器耦合和时钟标定；reset 成功及随机流重放；8 路根因或经明确持续时间、重复次数和负载限制界定的稳定上限；额外地图与最终逐项复核。若不能证明某项，保留为未完成，不用已有局部样本替代。

线障首场已区分粗/细组件并采集提交变换和非负返回索引；初始位置 LiDAR 原始帧已保存，动态风与固定 PWM 采样有时间重叠。三 stage/seed 重复正在运行，真实几何接触仍未证明。

stage 4 三 seed 各两次粗/细 HISM 组件提交变换及原始 LiDAR 帧已采样；不同帧的 LiDAR 字节差不能单独解释为 seed 差异。物理碰撞对象仍未证明。

三个 stage 的粗/细 HISM 组件提交变换与原始初始位置 LiDAR 各三 seed 两次完成；真实障碍接触、动态可见性及自然赛程仍未证明。
