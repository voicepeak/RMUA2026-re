# 并发 SIGSEGV 的补充对照

二进制 SHA-256 与上轮相同（见各 pool.json）。原始的 `pool8` seed 42 Docker state 给出 ExitCode 139、OOMKilled false，持续约 60.16 墙钟秒；`pool8_repeat` 的 seed 2026/42/1005 记录了 SIGSEGV，回溯顶端都是 `0x26b6223`。这三份日志在致命错误之前各有 `ChangeState:12`，但日志 stdout/stderr 缓冲会改变出现顺序，这不是证明状态切换造成崩溃。`pool8_fast` 的 seed 456 也发生 signal 11。多个 seed、进程间 ROS master 端口互异，不能据此断定单一 seed 或端口造成问题。

`crash_disassembly.txt` 展示 `0x26b6223` 为 `cmpl $0x0,0x8(%r12,%r13,1)`，是一次通过基址加索引的 32 位整数内存读取，并非 SIMD 指令。Unreal 的通用崩溃提示提到 SIMD 不足以确立对齐错误；没有崩溃时寄存器/有效地址和对象生命周期，不能确定非法指针来源。OOMKilled false 排除了 Docker 将该次退出标记为 OOM kill，并不单独排除各种资源压力。NullRHI 仍启动了 X11 和 GPU 可见容器，不足以排除驱动因素。

单实例同 seed 42（ROS master 11720，NullRHI，无飞行控制）对照：`single_start.log` 和 `single_state.log` 为 Docker 墙钟观测；2026-11-03T18:22:35 UTC 启动，最后一次观测到存活为 18:23:58 UTC（约 83 s），其后主动清理。`single_monitor.jsonl` 是其中一段只读抽样，分别记录 Unix 墙钟和单调钟；停止后引擎 stdout 可能未刷新，不能据空日志推断赛段状态。该单次结果说明本次单进程、同 seed 在超过上一轮 seed 42 失败时间后仍存活，不能证明并发是根因。资源快照 `host_resources.txt` 是此轮测试时的主机快照，不代表上一轮崩溃瞬间。

复现：从仓库根目录执行 `experiments/concurrency_followup_20261104/reproduce_single.sh /tmp/rmua-single-seed42`（同主机路径和镜像，指定的输出目录和端口须空闲）；会在 80 秒监视后按 manifest 清理。单次成功与先前 4 实例 15 秒成功均不足以确定长期稳定并发上限。现阶段保守选择 4 路分批，使用 `scripts/monitor_pool_liveness.py` 检查每个 Docker 的真实状态和主机内存；若失败，保留日志、用 pool 脚本按 label 和 id 清理，再以未完成 seed 开始新的 4 路或更小批次。由于 reset 重放未核验，不要将中途失败的轨迹拼接为连续 episode。未完成的区分实验包括单实例重复、多实例一致的受控航迹、崩溃瞬间寄存器/地址和资源采样、端口隔离与驱动模式对照。

`run_pool_batches.py` 提供保守分批空载诊断：每次最多 4 路、15 墙钟秒就绪后观察；容器退出或启动失败则按 manifest 清理，换端口从头重试整批，保留每次启动、状态、清理日志；清理失败立即停止，不继续启动。它不是训练调度器，也不续跑失败 episode。例：

```bash
python3 scripts/run_pool_batches.py \
 --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 \
 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json \
 --seeds 123,456,2026,789,42,1005,1007,2024 --parallelism 4 --retries 1 \
 --seconds 15 --port 11800 --output /tmp/rmua-idle-batches --display :1
```

`scripts/test_pool_batches.py` 用模拟 Docker 调用验证失败后换端口重试、每次清理及清理失败阻止下次启动；此新增脚本还未做真实多实例运行，以免影响同时进行的航迹实验。15 秒健康检查不证明训练负载或更长时长的稳定性。
