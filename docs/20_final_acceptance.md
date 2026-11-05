# 最终验收报告

状态日期：2026-11-06。目标版本：模拟器 12.0.0.5，主程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`。本报告是**研究/训练环境交付的收口**，不是官方比赛成绩或裁判端等价证明。逐场原始证据、失败样本和限制仍以各 `experiments/` 目录及 [验收台账](18_acceptance_ledger.md) 为准。

## 总体结论

**Conditional Go（有条件可用）**：可用于本地离线建模、seed/地图数据生成、单实例或最多 4 实例的 NullRHI 开发训练与诊断；所有 episode 必须使用新进程/容器初始化，不能依赖原生 reset 重放，不能把调试注入场当作官方赛程。

**No-Go（不可宣称）**：不能宣称官方裁判端计分等价、完整物理/车辆/自然碰撞重放、原生 reset 成功、8 实例稳定、OL2–OL12 可正常飞行、相机/动态障碍传感器等价，或 12 张图是 12 套独立门布局。

## 验收状态定义

- **Done**：在明确版本、配置和限制下已有可复查证据，能作为该项的交付结论。
- **Partial**：核心机制有证据，但缺少自然赛程、完整物理、独立类型/官方端等关键环节；只能按记录的范围使用。
- **Blocked**：需要当前发布二进制/本地环境缺失的引擎、符号、官方裁判端或重建能力；停止扩大实验，记录证据和下一步。

## 最终验收矩阵

| 项目 | 状态 | 结论与证据 | 剩余限制 / 下一步 |
|---|---|---|---|
| 版本、哈希、工具链、只读基线 | Done | `docs/01_target_and_toolchain.md`、`docs/11_artifact_manifest.md`、Git 历史；主程序/库/settings 哈希在各 manifest 中复用 | 无 |
| 12 图地图/路线/门/工厂/端口静态数据 | Done | `artifacts/maps/maps_summary_all.json`、`artifacts/tables/*.csv`、`docs/02_maps_and_routes.md` | 门洞物理尺寸未测 |
| 门布局跨图关系 | Done（静态） | `experiments/ol_gate_rigid_20261106/analysis.json`：12 图门间距差 ≤0.012 cm、刚性拟合残差 ≤0.009 cm、缩放一致；OL2 BeginPlay 6/6 场 12/12 门匹配 | 不能把 OL1–OL12 当独立门布局；环境资产/主题差异未证 |
| OL2 之外地图可飞行/跨图初始化 | Blocked | `experiments/ol2_direct_*`、`lobby_ol2_*`：直接加载缺 IMU/世界更新，经 Lobby 切图后无状态更新/风播种，GPS 全零/姿态异常 | 需要恢复游戏模式/ROS 节点初始化或重建桥接；当前发布二进制内未完成 |
| seed 对门/树/车辆/线障/风的影响 | Done（分类） | `docs/15_seed_dependency_matrix.md`、`docs/13_seed_randomtree_trace.md`、`docs/14_obstacle_asset_and_vehicle_trace.md`；门固定，树/车辆/线障/风受 seed 驱动 | 正常赛程前的全局随机流消耗未完全建模 |
| 风：fresh stage 4/5/6 关键帧 | Done（注入场） | `experiments/wind_offline_model_20261105/`、`local_models/wind.py`：18 场逐字节一致；40 项离线测试通过 | 正常赛程随机流、自然风区空间分布、时钟标定未证 |
| 风：持续动态/Reset 后随机流 | Partial | `experiments/wind_persistent_{4,5,6}_20261105/`、`reset_instances_20261105/wind_stream_analysis.json`；一次播种连续 54+54 次 `rand()` 可复现 false reset 前后数组 | 成功 reset 默认不重播种；完整重放需显式干预 |
| 线障：候选/实例/自然实例碰撞 | Partial | `line_offsets_*`、`line_instances_*`、`natural_line_component_20261106/`：3 seed 自然粗线实例组件+索引 0 命中，目标 hit 后 10 ms 内 state 12 请求 21/22/21 次 | 受控组件扫掠，不是自然飞行；碰撞力、独立 ABI、自然赛程终止未证 |
| 车辆：分支/路径/自然路径碰撞 | Partial | `vehicle_branch_4`、`vehicle_world_4`、`natural_vehicle_component_20261106/`：seed 123/2026 自然五点路径字节一致，各 27 个车辆 actor/子组件命中、23 个在 55 ms 内伴随 state 12 请求；seed 456 该设置未初始化 | 重定向+状态门+组件传送；自然飞行、碰撞力、seed 456 间接分支未证 |
| 碰撞：回调/状态机 | Partial | `collision_inject_ignore{0,1}`、`line_release_matrix`、`vehicle_release_state_matrix`、settlement 先导；7→12 写入与 `Finished` 文本链路有受控证据 | 物理冲量、独立对象类型、官方评分/完整结算未证 |
| PWM：动作/输出/传输 | Partial | `wind_pwm_step_{4,5,6}`、`pwm_20261104`、`transport_matrix_20261105`；p99 IMU 0.324–0.385 ms、LiDAR 0.561–0.655 ms、PWM 输出 0.241–0.335 ms、命令回传 0.338–0.431 ms | 物理电机响应、气动/机体辨识、执行器延迟未标定；长序列下降阶段仍有未跟随样本 |
| 传感器：IMU/GPS/LiDAR/相机/噪声 | Partial | `wind_persistent_*`、`sensor_clock_rendered_20261104`、四实例流观测；IMU ~100 Hz、LiDAR ~9.5–10.1 Hz | 采样至发布、噪声模型、多 seed 相机、动态障碍可见性未验收；NullRHI 相机为空 |
| reset：受控成功与部分重放 | Done（受控） | `reset_wind_interpose_{4,5,6}`：3 seed×2 均 true；线障提交与风数组前后重现（需 root/timer+重播种干预） | 不是原生行为；车辆/完整物理/自然赛程重放未证 |
| reset：原生成功与失败原因 | Blocked | `reset_native_component_20261106/`：固定 PWM 使物理 z 变化约 5 m，reset 检查组件仍 -37.9076614 cm 且 false；固定速度抬升同样 false | 检查组件不随该物理运动；需 GETPOSE_UE 组件/actor 身份或引擎侧修复；停止扩大实验 |
| 并发：4 实例稳定上限 | Done（有限负载） | `concurrency_4_pwm_300s_20261106`、`..._repeat2`：各约 300 s，IMU/LiDAR 重叠约 300.00–300.02 s，均清理 | 仅本机、固定 PWM、NullRHI；长期和真实训练负载未证 |
| 并发：8 实例根因 | Blocked | `concurrency_pwm_failure_summary_20261106`、`concurrency_8_pwm_gdb_180s_20261106`：6/8 各早期 SIGSEGV；GDB 捕获 `0x26b6223`（r12=-1，读地址 7）与 `rip=0` 空函数指针调用 | 至少两个现场，共同上游/概率未证；需重建/ASAN/source 或长期分层测试；当前按 4 实例 |
| 训练环境规格 | Partial（可交付草案） | `docs/19_training_environment_spec.md`：版本/隔离/观测/动作/episode/地图/并发/延迟约束已明确 | 尚无可验收的完整 episode 管理器；端到端训练器未在留出 seed/地图验证 |
| 失败样本与推送 | Done | `docs/09_evidence_index.md 与 docs/18_acceptance_ledger.md`、Git 历史；失败场次保留，所有结果已推送 | 仍须在最终交付时重核远端 HEAD |

## 高价值项收口

### 1. 原生 reset 失败原因

已确认的失败层级：reset 回调在 `AirSimROSData::Reset` 后查询 `GETPOSE_UE`，检查 7 个 float（四元数 + 位置）是否接近单位/原点。固定 PWM 场中物理 `pose_gt` z 从约 0.309 m 运动到 -4.82 m，但查询组件仍为 `-37.9076614 cm`，服务返回 false；固定速度抬升也未改变该组件。根高度写入可使其通过，说明**失败原因在检查组件/查询对象，而不是本次 PWM 动作未执行**。

未完成的是该组件的独立身份/类型和引擎侧修复路径。继续需要 `GETPOSE_UE` 处理函数的符号级跟踪或可重建的引擎/桥接层；在只读发布二进制上继续扫参数属于无限边界，已停止。下一步记录：用带符号的测试构建或在 `GETPOSE_UE` 回调处按 actor/component FName 记录对象，确认它是否是无人机 actor root、是否与 SimpleFlight 更新对象分离。

### 2. 自然车辆/自然赛程

线障：受控组件扫掠命中了自然生成的粗线实例（组件+索引 0），并在毫秒级触发 state 12；车辆：seed 123/2026 自然路径相同，组件传送后 27 次命中来源匹配车辆 actor/子组件。两者都不是自然飞行，也没有自然赛程的完整终止/评分。车辆 seed 456 在该重定向设置下未初始化，保留为失败样本。该线已达到“机制因果”而非“自然赛程”验收，后续若要继续，需要真实可达 stage 4 的控制器/航迹，当前不做。

### 3. 并发上限

在固定 PWM + 每路 IMU/LiDAR 订阅的本机 NullRHI 负载下，4 实例两轮 300 s 完成；6 实例和 8 实例各出现早期 SIGSEGV。GDB 8 实例场同时捕获两个不同现场：`0x26b6223` 的 `r12=-1` 无效读取（读取地址 7）和 `rip=0` 空函数指针调用。结论是**当前交付上限为 4 实例**，不是“8 实例根因已解决”。8 实例根因被列为 Blocked；下一步需要可重建构建/ASAN 或长期分层统计，而不是继续增加空载场次。

### 4. 最终训练环境

可交付内容：版本/哈希固定、地图与门表、seed 驱动的风/线/车辆离线模型、ROS 话题/服务清单、4 实例保守并发、普通 IMU 就绪、debug 真值仅事后、失败即终止 episode 的规格。未交付内容：原生 reset 重放、完整物理/碰撞力、官方计分、跨图飞行、相机/动态障碍训练、8 实例稳定。训练器实现必须以 `docs/19_training_environment_spec.md` 的约束为验收门槛，不能把本报告当成等价性证明。

## 复现与检查

```bash
# 离线模型（需要已校验的 ELF）
RMUA_ELF=/path/to/RMUA-Linux-Shipping python3 -m unittest discover -s local_models/tests -t . -v

# 门布局刚性复核
python3 scripts/verify_gate_rigid_layout.py

# 传输时延汇总
cat experiments/transport_latency_summary_20261106/summary.json

# 并发/失败上限
cat experiments/concurrency_pwm_failure_summary_20261106/summary.json

# 最终验收台账
sed -n '1,220p' docs/18_acceptance_ledger.md
```

## 失败样本索引

`docs/18` 已保留各阶段失败样本，包括：GDB call 未完成、trampoline 错误返回后 SIGSEGV、缺 ROS 服务类型、PWM 消息类型缺失、`POSITION_LIMIT`、OL2 直接加载/经 Lobby 切图失败、7/8 实例订阅与固定 PWM 失败、原生 reset false 等。失败场次的 `observed` 不解释为机制通过。

## 交付判定

- **可交付**：静态数据、离线模型、注入场机制证据、4 实例开发训练规格、失败样本和复现命令。
- **不可交付**：官方等价成绩、原生 reset/完整物理重放、8 实例稳定、跨图可飞行、物理碰撞力/官方计分。
- **停止线**：原生 reset 组件身份、8 实例共同根因、OL2+ 正常初始化均需要缺失的引擎/符号/重建能力；不再扩大实验，保留准确证据和下一步。
