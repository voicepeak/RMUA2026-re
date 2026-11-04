# 逆向验收进度与证据边界

本表是持续更新的检查表，不是验收通过声明。正式目标为完整交付；所有训练诊断不得改变官方模拟器、规则或计分，调试真值仅用于事后评估。

| 要求 | 当前证据 | 状态与缺口 |
|---|---|---|
| stage 4/5/6，seed 123/456/2026 各两次；风时间序列及指定四个函数命中参数 | `experiments/stage_probe_20261104/`；`experiments/runtime_trace_20261104_v2/` | 未达成。只到 stage 1，室外三个函数未命中，六次均没有室外风/车辆/线障。静态路线与合法时限内的安全导航仍需解决。 |
| OL1 12 门，跨 seed ID/位置/姿态/尺寸 | `experiments/runtime_trace_20261104_v2/gate_comparison.json` | 部分：三个 seed 各重复两次的 12 门 BeginPlay 位置、四元数、组件缩放一致。FName 索引与地图静态表位置配对，不是已解码的稳定名称；组件缩放不等于门洞物理尺寸。 |
| 至少另一图逐门比较 | `experiments/gates_ol2_20261104/actual_maps_summary.json`；`experiments/map_console_20261104/` | 未达成。多种参数以及一次未确认控制台接收输入的键盘尝试均未观测 OL2，所有观测最终为 OL1。 |
| SIGSEGV 根因或可复现稳定并发上限和降级 | `experiments/concurrency_gdb_20261104/`；`experiments/concurrency_frequency_overlap/`；`scripts/run_pool_batches.py` | 部分：一次崩溃定位到 AirSimROS 频率记录 deque 释放调用链，不同线程调用生命周期可重叠，但没有证据证明是该崩溃根因。8 路不稳定，4 路 15 秒、7 路约 89 秒样本均不能证明长期稳定上限。保守分批重试脚本是诊断手段。 |
| reset 与 seed / 随机流重放的成功和失败 | `experiments/reset_semantics_20261104/` | 部分：seed123/456 观测随机流重置回 seed，但两次 ROS 响应 false；一次 reset 窗口未命中 glibc srand。未证实动态障碍/风序列重放、reset 成功场次。 |
| PWM 到姿态和位置响应 | `experiments/pwm_20261104/` | 部分：一次 PWM 闭环参考阶跃原始数据，开环脉冲触发 POSITION_LIMIT；未完成可信系统辨识或参数标定。 |
| LiDAR/IMU/相机频率、时间戳、延迟及钟域 | `experiments/sensor_clock_rendered_20261104/` | 部分：单次有渲染场景的消息到达频率及头时间戳、墙钟、单调钟；源时间语义/同步和端到端延迟未标定。 |
| 文档、原始数据提交并 push | docs/09、15、16、17 和 Git 记录 | 部分：截至 2026-11-04 18:09 CST，`682f93c` 和 `0d29746` 已提交本地；远端实际 main 为 `ce3fedb`，本机 SSH 公钥被拒绝、HTTPS 无凭据，尚未推送新提交。待认证恢复后以 `git push` 和远端 HEAD 校验。 |

运行时样本、复现命令和时间域以各实验目录的 manifest 与日志为准；任何单场诊断均不能推广为比赛等价性或最终稳定性。
