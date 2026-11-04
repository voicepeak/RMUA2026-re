# 无头/高并发训练的逆向需求清单

日期：2026-11-04。本页把“官方 `-nullrhi` 能不能用”和“自定义批量环境要还原什么”分开，避免把开发诊断当成比赛等价。

## 已实测的官方 `-nullrhi`

来自 `experiments/seed_audit_20261103/`，固定版本 12.0.0.5、同一 `settings.json`、seed 123/456：

| 项目 | 观察 | 结论 |
|---|---|---|
| 前视/环视图像 | 0×0、0 字节 | NullRHI 不能提供相机感知 |
| LiDAR | 有非零点；静止单帧约 15,745 点 | 可用于几何/避障诊断，但未验证逐点精度和动态障碍 |
| IMU | 可达约 100 Hz（8 秒约 801 条） | 可用；仍需测时间戳、延迟和抖动 |
| PWM | `rotor_pwm_cmd` 能驱动悬停；真值约上升 1 m | 可用作动力学对照；未认证 PWM 归一化/延迟 |
| 两实例并发 | 不同 ROS master 下各约 109.7% CPU、766 MiB RSS | 可并发启动，但不是吞吐上限 |
| `ClockSpeed=2` | IMU 时间/墙钟约 1.659 | 可试探加速；未验证事件和动力学保真 |
| reset/seed | 未完成端到端验证 | 不能假设 reset 会重放同一 seed 场景 |

`scripts/headless_pool.py` 已实现隔离启动、独立 ROS master、IMU 就绪检查和按 manifest 清理；默认明确标注“No lockstep or reset guarantee”“Camera images are empty in NullRHI”“Diagnostics only”。

## seed 对场景的影响（必须先建模）

由 `docs/15_seed_dependency_matrix.md`：

- 门：地图静态，不由 seed 改位置；换 seed 不会随机化门。
- 树：`ARandomTree` 由 seed/LCG 拒绝采样生成。
- 车辆：`HoverVehicle2024_1` 由 seed/LCG 分支初始化，沿 `RouteSpline` 推进。
- 线障：stage 4/5/6 通过 `0x2fc77b0` 使用 seed/LCG 生成粗/细线；碰撞凸网格已提取。
- 风：`GameState2024_1` 初始化时 `srand(seed)`，stage 4/5/6 生成随机风序列；不是固定风，也不是与 seed 无关。

因此，官方 NullRHI 可以用来做**同版本对照**，但不能仅靠“空渲染 + 多开”就宣称等价于官方比赛；自定义训练环境必须显式建模上述四类对象。

## 自定义批量环境的最低要求

1. **路线/门**：使用 `artifacts/maps/maps_summary_all.json` + `artifacts/tables/*.csv` 的 12 条路线、12 门、工厂/端口/检查点；门位置固定，按路线顺序激活。
2. **动态障碍**：粗/细线使用 `artifacts/maps/obstacle_collision_meshes.json` 的凸网格；车辆使用 `BP_Hover_1` 的样条/速度参数，至少保留“沿路线推进 + 由 seed 选择分支”的统计。
3. **风**：按 stage 4/5/6 的分段风序列建模；保留 `srand(seed)`/`rand()` 的分布特征，至少拟合基准风速、采样间隔、方向变化；先按 `/airsim_node/drone_1/debug/wind` 实测校准。
4. **PWM/动力学**：保留 PWM 输入、4 个电机、刚体积分、气动阻力、碰撞；用官方 `rotor_pwm_cmd` 阶跃/闭环数据标定，不要只拟合位置轨迹。
5. **传感器**：LiDAR/IMU 的采样率、噪声、时间戳；相机若用于端到端策略，必须使用有渲染模式或另建图像渲染，NullRHI 的空图像不能训练视觉策略。
6. **延迟**：分别记录传感器采样、ROS 传输、策略推理、PWM 发布、仿真步进；官方 `FrameTimeTolerance`、ROS 队列、`ClockSpeed` 都要作为配置记录，不能用固定延时一把替代。
7. **并发**：每个实例独立 ROS master、独立端口、独立配置/seed；用 manifest 记录二进制、settings、容器/镜像、seed、启动时间，支持幂等清理。
8. **验证**：留出 seed 和地图；比较门通过、车辆/线障相遇时间、风序列、PWM 响应、LiDAR 命中、碰撞事件和成功率；明确报告误差和失败样本。

## 下一步实验

- 在官方 `-nullrhi` 与有渲染模式下，用同一条 stage 4/5/6 航迹分别采样 `/debug/wind`，验证本文的 `srand(seed)` 推断和风速量级。
- 用 GDB 只读断点记录 `0x2fc77b0`（线障生成）和 `0x2fc8900`（车辆分支）在多个 seed 下的调用次数/位置；不要在比赛控制路径使用调试信息。
- 对 `-nullrhi` 做 reset/双 seed 重放测试，确认 FRandomStream 与 libc `rand()` 是否被重置；这决定批量环境能否复用进程。
- 把 `scripts/headless_pool.py` 的启动/清理测试扩展到 4/8 实例，测 CPU、RSS、启动时延和 topic 抖动；当前只有两实例证据。

## 2026-11-04 四/八实例实验更新

四实例、15 秒短测中，每个模拟器约 109.45–110.24% 单核 CPU、763.5–764.5 MiB 进程 RSS，IMU 约 100 Hz、LiDAR 约 10.08 Hz，IMU 到达间隔 p99 10.17–10.23 ms；首个 IMU 观测就绪约 8.36–11.54 秒。这是特定主机、短时间空载，不是训练吞吐或稳定性承诺。

8 实例测试中有进程出现 SIGSEGV；一次 8 路就绪后仅 5 路完成测量，一次在就绪前失败。保存了失败和部分观测，原因尚未确定，不能声称 8 路稳定。旧的“当前只有两实例证据”已被本节更新取代。reset 响应为 false，且未完成随机数消费后的回放核验；NullRHI/有渲染同轨迹动态障碍、风、碰撞对照也尚未完成。方法、复现命令、原始文件和具体限制见 `docs/17_runtime_validation_20261104.md`。

## SIGSEGV 补充对照与批处理

此前 8 路的 seed 42 在约 60.16 墙钟秒后退出 139、Docker 标记非 OOMKilled；同 seed 单进程、相同 NullRHI 空载对照运行至少约 83 秒后仍存活，再由实验脚本主动清理。这是一例对照，不能证明并发就是崩溃原因或给出长期稳定并发上限。重复实验的崩溃顶帧 `0x26b6223` 对应普通整数内存读取，UE 的 SIMD 字样不是对齐错误的确证；缺少崩溃时寄存器与有效地址。`pool8_fast` 没有可用的崩溃栈，不能确定与该故障相同。详情和原始日志在 `experiments/concurrency_followup_20261104/`。

`scripts/run_pool_batches.py` 提供空载诊断的保守 4 路分批、独立端口重试、失败留档和按 manifest 清理；它不是训练 episode 管理器，尚不能从这一短测推断 4 路长期稳定。清理失败会停止后续重试；不应将失败前后轨迹拼成一个 episode。运行命令及边界见上述报告。

8 路带 GDB 的空载复现中（每路独立 ROS master），8 路均就绪，约 89 秒监测内 7 路存活，seed 2024 在启动约 78.16 秒后 SIGSEGV。其寄存器和完整调用栈定位到 `AirSimROSFrame → record_frequency_data → deque::pop_front → operator delete` 路径中读取无效地址；这定位了本次崩溃现场，尚未证明内存为何损坏。独立 master 均曾监听，监测期间宿主 MemAvailable 最低约 28.2 GiB，Docker 未标记该次为 OOM；不能据此排除所有网络、资源或驱动因素。旧崩溃栈不同，不能认定同因。`experiments/concurrency_gdb_20261104/` 包含完整现场与复现命令；`artifacts/rea/frequency_rea.json` 是该频率记录函数的 REA 静态证据。函数内未见显式锁并不单独证明数据竞争。GDB 改变时序，7 路 89 秒存活不能视为长期稳定上限；推荐仍为有监测、失败留档的保守 4 路或更小分批。

单实例只读 GDB 频率统计入口/返回探针观测到多线程调用生命周期跨 frame、imu、lidar、rotor_pwm 重叠（343 次，未观测到同 topic 重叠）；GDB all-stop 会影响调度，不能单凭此确认竞争或将其等同先前崩溃根因。见 `experiments/concurrency_frequency_overlap/` 和 docs/17。

注入式室外探针已能在 NullRHI 初始路线设置时触发 stage 4 生成，见 `experiments/stage_inject_pilot_20261104/` 和 `scripts/run_stage_injection.py`。场景被调试器改变，且无人机仍在起点；生成器数组可供校准，不能据此宣称官方赛程或传感器在室外等价。

stage 4 注入风关键帧在每个 seed 的两场逐字一致；该场中许多 SpawnActor 碰撞失败，训练障碍生成器不能用入口调用次数代替实际成功实例数。具体样本和限制见 docs/17。

stage 5 注入风序列的三 seed 重复数据与脚本见 `experiments/stage_inject_5_20261104/`；线障入口尝试次数并不能替代真实障碍实例、运动和碰撞采样。

三个 seed 的一次 ROS reset 均返回 false，但 reset 路径重新触发注入后的 stage 4 生成，风数组与 reset 前不同；勿将此响应或场景当作已验证成功的 episode 重置。详见 `experiments/stage_reset_inject_retry_20261104/`。

车辆注入试场已证明可在游戏线程上调用并返回生成入口，但不能替代车辆实例和运动标定。线障内部候选点数组并不等于成功 spawn 或实际碰撞体；详见 docs/17。

stage 4 注入获得可重复的线障内部候选点集合，但仍需确认资产实例和实际碰撞；车辆重定向六场均返回，初始化和运动尚未观测，见 docs/17。

强制车辆路径的种子 123 场提供了组件世界位置、路径点、速度与进度样本，可用于后续运动模型对照；GDB 暂停和额外状态分支注入限制了外推，见 docs/17。

车辆注入在 seed 456 的两次路径字节不同，采样时序与分支是待控变量；训练复现不能仅凭输入 seed 保证相同动态对象。线障 stage 6 的内部候选点复现边界见 docs/17。

强制风状态的 NullRHI 试场获得了非零 ROS 风向量；这是开发注入，尚不是自然风区等价性证明。车辆世界位置样本覆盖三个 seed，但 seed 456 重复路径不同。

风状态注入六场均有非零风与 IMU/GPS/LiDAR 时钟采样；初版 PWM 订阅失败，修复试场使用原始消息并校验类型与 MD5。数据和边界见 docs/17。

强制 stage 4 风的六场中 ROS 原始 PWM、IMU、GPS 和 LiDAR 时钟均有采样；另有单 seed 固定开环 PWM 序列。需继续测其他 seed 与 stage、延迟和受风时动力学，见 docs/17。

stage 5 的三 seed 强制风采样已包含原始 PWM 和传感器流；合成碰撞回调证明一次忽略标志为 0 时的 state 12 分支，尚未测真实几何碰撞。

stage 4 的固定开环 PWM 序列在三 seed 两次均采到对应输出，header 首次匹配差约 1.9–40.1 ms，不是经校准的端到端延迟。stage 6 强制风及传感器采样已覆盖三 seed 两次。

stage 5 固定开环 PWM 三 seed 两次已采集。单次风状态注入 25 秒话题不变，训练风时间演化不能依此认定已校准，见 docs/17。

开发容器持续逐 tick 强制室外状态后，风索引和 ROS 向量可动态变化；按此采集存在调试器调度偏差。碰撞回调忽略标志的三 seed 对照已运行，但不替代物理接触测试。

stage 4/5/6 固定 PWM 步进均已覆盖三 seed 各两次，动作不依赖真值。当前短场风向量不变，需在持续变化风中复测以及标定延迟。

持续注入单场已用风关键帧的速度/角度插值近似重建 ROS 风矢量；误差含调试暂停及钟域不确定性，尚需跨 seed 校验。

风关键帧可由 `scripts/wind_offline_model.py` 重建；fresh 注入的 18 个 seed/stage/重复样本与原始内存逐字节相等。必须保留 libc 随机流调用顺序、低 24 位与 float32 运算顺序，正常赛程和 reset 后不保证直接从 seed 开始。实测动态矢量对照和钟域限制见 docs/17。

线障内层候选沿样条偏移服从 `1500+7000u` cm 的随机表达式、基准按 10000 cm 步进，反向、裁剪和拒绝/生成失败必须建模；这不是最终相邻障碍间距。首场状态与偏移对照见 docs/15。

持续注入的 stage 4/5 动态风三 seed 各两次可供矢量模型对照，但不能以 GDB 干预下的近似时间误差代替严格钟域或动态等价验证。

## 2026-11-05 开发场次主机与可复现配置

本机 Ubuntu 22.04.5、x86-64、i7-12700H（20 逻辑 CPU）、内存 40,874,916 kB、无 swap，RTX 3060 Laptop GPU 6144 MiB、驱动 595.99.02。模拟器主程序 SHA-256 为 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`；实验 Docker 镜像 `rmua/re-probe:20261104` 的不可变 ID、kernel、Docker 版本和完整 CPU 信息在 `experiments/training_host_20261105/host.json`。实际 settings 文件副本为 `experiments/training_host_20261105/settings.json`，SHA-256 `0d72886f92b091cbc5ba2ae58d25b4b4bc589045d4e725f483603efdc26c6c17`。这是实验硬件快照，不是训练环境等价或并发上限承诺；官方包与原始设置文件未修改。

使用已有同版本模拟器、含 ROS 消息的 catkin 工作区、Docker 镜像和 X 显示，可参照：

```bash
python3 scripts/run_stage_force_wind_persistent.py --stage 4 --simulator "$SIM_DIR" --settings experiments/training_host_20261105/settings.json --messages "$ROS_WS" --output /tmp/rmua-wind-4 --seeds 123,456,2026 --repeats 2 --sample-seconds 25 --port 12220
python3 scripts/analyze_persistent_wind.py /tmp/rmua-wind-4
python3 scripts/wind_offline_model.py --output /tmp/rmua-wind-offline
```

`SIM_DIR` 指向解包后的模拟器目录，`ROS_WS` 指向已构建的 ROS 工作区；其依赖、镜像构建与复原步骤仍须结合 docs/07 和 docs/17。第一个命令会在开发容器的内存中注入阶段与状态，记录的真值仅供事后分析。实际建模须分别处理模拟时钟、传感器和物理时间、通信队列以及随机流消费顺序。
