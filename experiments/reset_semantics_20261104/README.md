# Reset 语义调查

目标程序 SHA-256 `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`；`libairsimros.so` SHA-256 `69d5ea3302bdd5f20b842ec3d3e84bafcb50ef3649ada42c4d346c9bcd389885`。所有断点仅观察并继续，不写目标进程。这里不是比赛成绩。

- `Reset.srv` 是空请求，响应只有布尔 success，不能通过这个服务直接指定 seed。
- REA 证据 `artifacts/rea/reset_bridge_rea.json`（evidence ID `ev_6d3c5d98928a718cb159369093295752204f64739f9e49dd1cb5868abe4e990a`）和 `reset_callback.objdump.txt`：`RESET_Callback` 先检查 node instance `+0x2954`；值不为 1 则直接返回 false。若为 1，则发 `CMD_RESET`，重置桥接数据，在调用线程睡眠 1 秒，发 `CMD_GETPOSE_UE` 并对返回结果七个 float 进行阈值检查，结果作为 bool 返回。因此服务通信正常并不证明场景重置成功；检查的输出也不是 seed/RNG 一致性校验。具体哪一分支或哪一个分量导致 false，需要运行时探针确认。
- `random_reset_helper.objdump.txt` 显示 helper `0x2fced20` 的参数为 `0xffffffff` 时，从单例 `+0x198` 读取配置值，并同时写入 `+0x19c` 和 `+0x1a0`。`artifacts/rea/followup_callers.c` 中 `0x2fb8f40` 初始化路径会调用该 helper 与 `srand` 后进入 `0x2fba970`。`artifacts/rea/followup_state.c` 所见 `0x2fba970` 代码没有对 helper 的直接调用；不排除间接调用，也不能仅凭这条路径得出 reset 一定不重置随机流。
- 六个原有起点场次 `experiments/runtime_trace_20261104_v2/` 的服务调用都返回 false；初始值相同不足以证明流被消耗后可复位。新进程可以通过命令行设置 seed，但同 seed 的 stage 4/5/6 障碍/风序列重放还没验证。

新增探针 `scripts/reset_probe.py`/`.gdb` 和 `scripts/run_reset_diagnostic.py`：在 ROS 回调入口根据此二进制固定偏移捕捉 inactive 分支或 GETPOSE_UE 返回后的七个 32 位 float；还捕捉 RNG helper 命中，原 trace 捕捉 srand。探针记录 debugger Unix 墙钟和 monotonic 时刻；风 CSV 的 `header.stamp` 是 ROS 消息时间戳，三者不能直接当作相同的仿真时间或用其差值宣称端到端延迟。旧 trace 的断点输出没有各自时间标签。

复现（需本机相同镜像、ROS 消息、可用 ROS master 端口和显示服务）：

```bash
python3 scripts/run_reset_diagnostic.py --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --messages /home/tianbot/RUMA-by-helinjun/beta/rmua_ws --output /tmp/reset-probe --seeds 123 --repeats 1 --port 11621 --sample-seconds 2 --display :1 --reset
```

## 单次运行时结果

`run_seed123/` 是 123 seed、NullRHI、新进程、起点场景一次请求。`gdb.log` 中调试器墙钟 `1793730422.419` 进入 ROS 回调，实际进入发 `CMD_RESET` 的分支；0.341 墙钟秒后 helper `0x2fced20` 命中，参数为 `0xffffffff`，配置/初始/当前值为 `123 / 123 / 214098071`。其机器码在该参数下将 `+0x198` 的 123 写入初始及当前两个字段；随后 `RMUA_GAME_RESET` 入口读取到 `initial=123 current=123`。这是本场该随机流在实际已消费后被恢复到 seed 的证据。注意 `RMUA_GAME_RESET` 输出没有时间戳，此处是日志中的事件顺序；断点暂停可能影响墙钟间隔。

`1793730423.723` 在 `CMD_GETPOSE_UE` 的回调返回后（ROS 回调内部，距请求入口约 1.304 墙钟秒），按偏移 `00,04,08,0c,10,14,18` 读取的七个 float 是 `0,0,0,0,0,0,-37.9084167`。REA 代码检查 `abs(offset+0x18)<0.2`，该次检查失败，所以 `success: False` 有具体观测原因。不把该内存字段未经进一步确认地命名成某个坐标或赋单位；结果仅适用于本版本、本场配置，不能解释所有历史失败。`reset.log` 保存服务响应；rostopic timeout 退出码 124 表示采样时间结束，非复位成功。

本次 ROS reset 过程中没有第二次命中 `0x2fb2040`（对应的初始化包装调用），只在进程初始化命中一次；不能排除 libc `srand` 通过其他入口调用，也未跟踪 rand 内部状态。单次 FRandomStream 初值复位不代表场景完全重放：调用顺序、wind/libc、动态障碍物均未比较。`manifest.json` 的 `reset_failed` 指 ROS 响应 false，不意味着 helper 没有运行。

## 直接 libc srand 运行时补充

`run_seed456_libc/` 使用 seed 456 在独立起点进程内再次调用官方 reset。针对镜像中的 `/lib/x86_64-linux-gnu/libc-2.31.so`，探针以 `/proc/<pid>/maps` 的文件零偏移映射基址和已核对的 SHA-256 `53b909551f8e581120ddc2d24cfb5df76c52e3c2e2a5dac0d202b25638b3e902`，在 glibc 的 `srand` / `srandom` 共享入口 ELF 相对地址 `0x475c0` 设置只读断点。`libc_srand_installed` 证明安装成功；探针在回调入口时安装，从安装到采样结束期间没有产生 `libc_srand` 命中。没有观察启动阶段的 libc srand（断点当时未安装），此前独立的 `RMUA_SRAND` 记录证明了对应包装入口在启动期间命中。此结论限定这一场、这一段时间和此 libc 入口；它不能排除 `srandom_r` 等其他途径修改随机状态，也不能证明后续 rand 序列复现或不复现。

该场 `0x2fced20` 的配置/初始/当前值为 `456 / 456 / 1410508404`，随后 `RMUA_GAME_RESET` 入口为 `456 / 456`，与 helper 指令的恢复语义一致。`CMD_GETPOSE_UE` 返回 `offset +0x18=-37.9077873`，大于 0.2 阈值，响应再次为 `success: False`。这是两个 seed 的两次起点观察，依旧不能断言任意场景或 stage 的完整同 seed 重放。探针墙钟与 monotonic 时刻、ROS 消息时间戳分别保留；仿真时间尚未对齐。

第一场使用的原始探针源码快照保存在 `run_seed123/reset_probe_v1.py`，其哈希与同场 `reset_probe_metadata.json` 一致。第二场哈希见 `run_seed456_libc/reset_probe_metadata.json`，对应目前的 `scripts/reset_probe.py`。
