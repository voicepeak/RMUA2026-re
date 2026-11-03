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
