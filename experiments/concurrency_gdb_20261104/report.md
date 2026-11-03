# 8 路 GDB SIGSEGV 复现

2026-11-03 18:31 UTC，使用 `scripts/run_gdb_pool_repro.py`、目标 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`、OL1 空载 NullRHI，独立 ROS master 11900–11907，8 个 seed 123/456/2026/789/42/1005/1007/2024，各自进程由 GDB 启动；没有飞行指令，GDB 可能改变时序。80 多秒的墙钟监控不代表训练稳定性。

`pool/pool.json` 显示 8 路均达到 IMU 首次观测就绪，9.1–10.0 秒（墙钟，含调度及检查延迟）。`liveness.jsonl` 有 70 次采样，首尾单调钟跨度 88.98 秒，每次包含 Docker 容器状态、OOM 标志、模拟器主机 PID 的 CPU ticks 和 RSS、主机 MemAvailable、全部 TCP listener 和各 ROS master 端口。`summary.json` 是由原始数据重算的汇总：每路 CPU 约 109.2–110.1%（100%=单核，monotonic 秒 / CLOCK_TICKS），进程观测 RSS 峰值 约 684–765 MiB；监控期间宿主 MemAvailable 最低 29,524,340 KiB，最高 35,104,584 KiB。0–6 号容器全部观测时保持运行；7 号 seed 2024 在 70 次里仅 60 次被观察为运行。容器退出后其 ROS master 11907 不再监听，其余 11900–11906 在末次采样时仍监听。各端口不同且此前均可监听，这个样本未显示固定 ROS master 端口冲突；不能排除其余网络或资源竞争。

seed 2024 容器在 18:31:40.213 UTC 启动、18:32:58.377 UTC 退出，相差 78.16 墙钟秒，`OOMKilled: false`。原始 `pool/rmua-re-0457708b23e7-7.log` 第 604 行开始：GDB 捕获线程 134 的 `SIGSEGV`，`rip=0x3053eaf` 指向 `mov (%r12),%r12`，此时 `r12=0x1e3100200`。栈显示 UE `operator delete` ← libairsimros.so `std::deque<unsigned long>::pop_front` ← `airsim_ros::record_frequency_data` ← `AirSimROSFrame`。可确认这一次崩溃发生在 AirSimROS 频率记录 deque 的释放路径和 UE 内存分配器中；不能仅由栈认定谁破坏内存、是驱动、资源或竞态。线程 133 在同一 allocator 附近等待 pthread mutex，证明有其他线程经过分配器，不能证明同时访问同一个 deque。

REA/Ghidra 证据 `artifacts/rea/frequency_rea.json`（`ev_fe7c671abf4c39cbe2ce1ee0910971e4896a508aa01021ba546f7c9d45042b5f`）反编译了库 `record_frequency_data` @ `0x184b87`：记录 ROS 时间（`ros::Time::now()`，转纳秒）到 map 内 deque，push 后长度至少 1001 时 pop，函数体未见显式锁。`AirSimROSFrame` 的直接调用前也未见锁，但上层调用可能持锁，当前快照不能证明 deque 数据竞争。旧 `pool8_repeat` 的 `0x26b6223` 回溯只显示主程序未知符号和 libc，无 AirSimROSFrame 或 libairsimros 帧，因此不能认定与本次相同的故障路径。

`run.json` 中启动、监控、清理三项退出码均为 0，只说明对应工具完成；GDB 截获致命信号后也可能以 0 退出，不能将它误判为模拟器成功。当前脚本已改成发现 SIGSEGV 或容器在监控中退出时返回非零；原始 `run.json` 不作事后改写。所有容器均已按 manifest 清理。

复现（依赖本机 `rmua/re-probe:20261104` 镜像、空闲端口、GDB，目录需不存在）：

```bash
python3 scripts/run_gdb_pool_repro.py \
 --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 \
 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json \
 --output /tmp/rmua-8gdb --port 11900 --seconds 90 --display :1
python3 scripts/summarize_pool_liveness.py /tmp/rmua-8gdb/liveness.jsonl --output /tmp/rmua-8gdb/summary.json
```

当前建议是失败留样、按 manifest 清理后从 episode 开始重试，并使用保守 4 路以下分批；已有 4 路样本仅 15 秒，不足以保证长期稳定。本次只测到 7 路在约 89 秒采样期内未退出，不能把 7 定义为可靠并发上限。要识别真正根因还需更多崩溃现场及同代码路径无 GDB 对照，也不能据此修改官方库或比赛规则。
