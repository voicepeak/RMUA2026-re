# 03 seed、GameConfig、计时、状态机与 Score

## 1. 命令行 seed

2026-11-03 补充：`REA_2fcc5a0` 还会把命令行 seed 初始化到 `0x2fcc220` 所返回单例的 `+0x19c/+0x1a0`；后者在路线触发时被 LCG 更新，参与是否调用车辆初始化路径的分支。静态证据和限制见 `docs/14_obstacle_asset_and_vehicle_trace.md`。以下不同对象的偏移不能混同为同一个随机流。

相关函数（Ghidra 地址）：

- `REA_2fa0e80`：GameState/GameConfig 初始化入口，函数体 `0x2fa0e80..0x2fa1335`。
- 默认 `seed = 1`。
- 调用命令获取函数 `func_0x0311a7c0`（命令行对象），然后：
  `func_0x0315c920(commandLine, "seed", &seed)`。
- 解析结果写入：
  - `GameState + 0x50`（int）
  - `GameState + 0xc`（64 位值，低/高 32 位都为 seed）
- 随后调用：
  - `REA_2fa24a0`：初始化计时/分数/段表
  - `REA_2fa3190`：初始化路线/段
  - `REA_2fa3dd0`：用 `FRandomStream` 生成随机量

`REA_2fa3dd0` 内含 UE4 `FRandomStream` 的 LCG（乘数/增量常量可在伪代码中看到），并计算随机位置。2026-11-03 的进一步静态追踪将这条路径关联到 `RandomTree` 类；参见 `docs/13_seed_randomtree_trace.md`。此前将该路径归因于风的表述证据不足，已撤回。

2026-11-04 补充：seed 对各类对象的影响已拆开验证。门是地图静态；随机树、车辆、线障和室外风各有独立路径；风由 `0x2fb8f40` 调 `0x2fb2040`（`srand(seed)`）后交给 `0x2fb23b0` 的 `rand()` 序列生成。总表和证据见 `docs/15_seed_dependency_matrix.md`。

结论：

- `seed` 是“随机流种子”，不是地图编号。
- 默认 seed=1；不传 seed 时仍可运行。
- `run_simulator.sh 123` / `run_simulator_offscreen.sh 123` 中的 123 就是该 seed。

## 2. GameConfig.json 的 7 个实际字段

文件路径：

```text
RMUA/Content/Configs/GameConfig.json
```

读取函数：`REA_2fb3a50`（函数体 `0x2fb3a50..0x2fb4e7a`），路径字符串 `Configs/GameConfig.json`
在 `.rodata` 的地址约为 `0x75c4fe`（UTF-16）。

构造函数把单例写入全局 `0x719f540`；后续通过 `REA_2fb4e80` 取回。

| 字段 | GameConfig 偏移 | 默认 | 使用位置/行为 |
|---|---:|---|---|
| `ChargeGlobally` | `+0x2d0` | false | `0x2fb9195` 充电状态逻辑 |
| `IgnoreHoverCollision` | `+0x2d1` | false | `0x2fc003a` hover/接触碰撞分支 |
| `AllowAllReports` | `+0x2d2` | false | `0x2fb7fe9`、`0x2fb8090` 汇报/状态分支；`0x2fb8257` 可置 1 |
| `IgnoreAllHitCollision` | `+0x2d3` | false | `0x2fb712e`：为 true 直接返回，跳过撞击结算 |
| `IgnoreOverTime` | `+0x2d4` | false | `0x2fb92d3`、`0x2fb9410`、`0x2fb94ff`：为 true 跳过超时结算 |
| `IgnoreWind` | `+0x2d5` | false | `0x2fb948d`：为 true 时把风向量置零 |
| `AllowChangeState` | `+0x2d6` | false | `0x2fb812e`、`0x2fb816e`、`0x2fb86dc`、`0x2fb88fb` 状态切换门控 |

字段名是从二进制中的 UTF-16 常量按存储顺序解码出来的：

```text
ChargeGlobally
IgnoreHoverCollision
AllowAllReports
IgnoreAllHitCollision
IgnoreOverTime
IgnoreWind
AllowChangeState
```

README 只公开了 `IgnoreAllHitCollision` 和 `IgnoreOverTime`；其余是未文档化调试/规则字段。
官方文件当前只包含：

```json
{
    "IgnoreAllHitCollision": false,
    "IgnoreOverTime": false
}
```

其余字段缺省为 false。

### 调试建议

- 采集数据/长时训练：可临时 `IgnoreAllHitCollision=true`、`IgnoreOverTime=true`、`IgnoreWind=true`。
- 不要用这些字段伪造正式比赛成绩；正式提交前应恢复默认。
- `AllowChangeState`/`AllowAllReports` 会影响状态推进/汇报，不建议在不了解状态机时随意打开。

## 3. 计时

### 3.1 默认上限常量

`REA_2fb3a50` 构造函数初始化（float）：

| 偏移 | 默认值 | 用途（来自 tick 分支） |
|---|---:|---|
| `+0x2b0` | 180.0 | 默认/状态分支上限 |
| `+0x2b4` | 180.0 | 状态 3 上限 |
| `+0x2b8` | 180.0 | 状态 5 上限 |
| `+0x2bc` | 200.0 | 状态 7 上限 |
| `+0x2c0` | 220.0 | 状态 9 上限 |
| `+0x2c4` | 200.0 | 状态 0xb 上限 |
| `+0x2c8` | 30.0 | 30 秒计时/阶段检查 |
| `+0x2cc` | 2.0 | OutTime（GUI 默认 `OutTime:2.000`） |

### 3.2 GameState tick

函数 `REA_2fb8f40`（`0x2fb8f40..0x2fb96ab`）：

- 状态枚举：`GameState + 0xd0`。
- 当前状态计时：`GameState + 0x1a8`。
- 总计时/飞行累计：`GameState + 0x1ac`。
- 读取 GameConfig 的单例（`0x719f540`）。
- 根据状态选择 `+0x2b0/+0x2b4/+0x2b8/+0x2bc/+0x2c0/+0x2c4` 之一作为当前段上限；超过且 `IgnoreOverTime==false` 时调用状态切换（`0xc`，结算）。
- `+0x2c8` 的 30 秒检查；`+0x2cc` 的 2 秒检查同样受 `IgnoreOverTime` 影响。
- 将 `REA_2fba830` 的飞行时间写入 `GameConfig+0x2f8`。
- 将总计时写入 `GameConfig+0x300`。

### 3.3 FlightTime

`REA_2fba830`（`0x2fba830..0x2fba963`）返回：

```text
GameState + 0x400
+ GameState + 0x194
+ GameState + 0x198
+ GameState + 0x19c
+ GameState + 0x1a0
+ GameState + 0x1a4
```

即六段历史时间 + 当前段时间的累计。GUI `Get Flight Time` 绑定 GameConfig 的 `FlightTime` 属性。

## 4. 状态机与 Score

### 4.1 状态切换函数

`REA_2fb9960`（`0x2fb9960..0x2fba82c`）：

- 参数 `(GameState*, newState, bool)`。
- 写 `GameState+0xd0 = newState`。
- 把状态名字符串写入 `GameConfig+0x308`（GUI 的 `GameState` 文本）。
- `case 2` 时：
  ```c
  if (GameState->state != 2)
      GameConfig->score_counter += 1;   // GameConfig + 0x2fc
  ```
  这说明 `+0x2fc` 是仿真内部的 `Score` 计数器；进入 case 2 且原状态不为 2 时自增。
- `case 12` 保存原状态到 `GameState+0xd4`，写入 `+0xd0 = 12`，记录时间；第三参数非零时还会计算一段时间并使 `Score` 自增，零时则不执行该自增。碰撞场是否执行到后续完整结算仍需动态证据，不能仅凭状态写入推断。证据：`artifacts/rea/followup_state.c` 的 `case 0xc`。
- `REA_2fba970` 重置时把 `GameConfig+0x2fc = 0`、总计时 `+0x1ac = 0`、历史段时间清零，并重置状态字符串为 `normal`/`Pending` 等。

### 4.2 GUI Score

`GUI2024` 的函数：

- `Get Score` 使用 `Score: {Score}`
- `Get Flight Time` 使用 `Time:{FlightTime}`
- `GetOverTime` 使用 `OutTime:{OutTime}`
- `GetGameState` 使用 `State:{GameState}`
- `GetStateTime` 使用 `StateTime:{StateTime}`

这些属性的 `ResolvedOwner = -7`，对应 import 索引 6：`GameConfig2024_1` 类。因此 GUI 读的是 GameConfig 的 native 属性：

```text
Score
FlightTime
OutTime
CurStateTime
GameState
```

其中：

- `FlightTime` = `GameConfig+0x2f8`
- `CurStateTime` = `GameConfig+0x300`
- `OutTime` = `GameConfig+0x2cc`（默认 2.0）
- `Score` = `GameConfig+0x2fc` 的计数器（case 2 有条件自增，case 12 的第三参数非零时也自增）
- `GameState` = `GameConfig+0x308` 的状态字符串

### 4.3 重要边界

- 这是**模拟器内部**的 Score/计时，不等于官方裁判端的最终分数。
- 官方评测服务器可能根据 gate/factory/meter/time 上报另行计分；客户端没有完整的官方积分公式。
- 如果要设计 RL reward，建议不要只依赖 `Score` 计数器，而是组合：门通过、段推进、工厂/端口事件、飞行时间、碰撞、风扰动。

## 5. 重置

- ROS 服务：`/airsim_node/reset`（见 `docs/04_ros_interface.md`）。
- 游戏状态重置逻辑：`REA_2fba970`（`0x2fba970..0x2fbac29`）。
- 静态重置路径会清零 `+0x2fc`、`+0x1ac`、六段历史时间、状态字符串等；服务成功不代表完整物理状态或随机流已重放，当前不能据此把 reset 作为已验收的 episode 边界，见 `docs/18_acceptance_ledger.md`。
- 重置是否保留 seed 的随机流状态需要运行时验证；若要每 episode 改变 seed，建议重启进程/容器。

## 6. 证据

- `artifacts/pseudocode/anchors_pseudo3.c`：
  - `REA_2fa0e80`、`REA_2fa24a0`、`REA_2fa3190`、`REA_2fa3dd0`
  - `REA_2fb3a50`、`REA_2fb4e80`
  - `REA_2fb7110`、`REA_2fb7f90`、`REA_2fb8650`、`REA_2fb8f40`
  - `REA_2fb9960`、`REA_2fba830`、`REA_2fba970`
  - `REA_2fc0020`、`REA_2fcc5a0`、`REA_2fd29c0`
  - `moveByMotorPWMs`、`resetImplementation`
- `artifacts/disasm/flag_uses.disasm`、`config_func.disasm`、`seed_refs.disasm`、`route_init.disasm`
- `artifacts/configs/GameConfig.json`
- `artifacts/blueprints/GUI2024.json`、`GUI2024.kismet3.txt`
