# 06 Blueprint、UI 与 Lobby

## 1. BP_GameConfig2024_1

文件：

```text
RMUA/Content/Blueprints/BP_GameConfig2024_1.uasset/.uexp
```

UAssetAPI 解析出的关键属性：

```text
RouteConfig
RouteConfigs
RouteIndex
RouteSpline
SegmentInterval
RouteGateConfigs
FactoryConfigs
CheckPortConfigs
SideTrackConfigs
ArrowConfigs
VehiclePrefabs
LinePrefabs
VehicleRoot
Hover
Light
```

类默认对象（CDO）中的 `RouteConfigs` 是 12 个零模板（RouteIndex=0、RouteSpline=0、SegmentInterval=0）；真正的数据在每张地图的 **`BP_GameConfig2024_2`** 实例上：

```text
RouteConfigs[12]       RouteIndex 1..12, RouteSpline, SegmentInterval=2100
RouteGateConfigs[12]   RouteGateIndex 1..12 -> Gate actor
FactoryConfigs[12]     FactoryIndex 1..12 -> Factory actor
CheckPortConfigs[12]   PortIndex 1..12 -> Port actor
SideTrackConfigs[24]   SideTrackIndex 1..24 -> spline actor
ArrowConfigs[24]       ArrowIndex 1..24 -> arrow actor
```

`RouteConfig` 结构：

```text
RouteIndex       int
RouteSpline      object -> Spline actor
SegmentInterval  int
```

`RouteGateConfig` / `FactoryConfig` / `CheckPortConfig` 都是 `index + object` 的简单结构。
门/工厂/端口的具体世界坐标从对应 actor 的 root component transform 提取；路线点从 `RouteSpline` 的 `SplineCurves.Position.Points` 提取。

## 2. BP_Gate

文件：

```text
RMUA/Content/Blueprints/BP_Gate.uasset/.uexp
```

解析结果：

- `BP_Gate_C` 继承 `RouteGate2024`；
- 组件只有 `DefaultSceneRoot`、`StaticMesh`（`/Engine/EngineMeshes/Cube`）、`ConcreteSmall`（`/Game/Resources/Buildings/Junction/ConcreteSmall`）；
- **没有 Blueprint 事件图/触发逻辑**；门的触发/计数大概率在 C++ `ARouteGate2024_1` 或 GameState 中，Blueprint 只负责外观。

门的视觉 scale（每张图基本相同）：

```text
X=4.449821949005127
Y=1.8593449592590332
Z=2.4494340419769287
```

## 3. GUI2024

文件：

```text
RMUA/Content/Blueprints/GUI2024.uasset/.uexp
```

UAssetAPI 解析出的控件绑定：

| 控件 | 函数 | 文本 |
|---|---|---|
| `TextBlock_0` | `Get Flight Time` | `Time:{FlightTime}` |
| `TextBlock` | `Get Score` | `Score: {Score}` |
| `TextBlock_1` | `GetOverTime` | `OutTime:{OutTime}` |
| 其他 | `GetGameState` | `State:{GameState}` |
| 其他 | `GetStateTime` | `StateTime:{StateTime}` |

`Get Score` / `Get Flight Time` 的 `ResolvedOwner` 指向 import 6 = `GameConfig2024_1` 类，说明这些值来自 GameConfig 的 native 属性：

```text
Score        -> GameConfig + 0x2fc (计数器)
FlightTime   -> GameConfig + 0x2f8
OutTime      -> GameConfig + 0x2cc (默认 2.0)
CurStateTime -> GameConfig + 0x300
GameState    -> GameConfig + 0x308 (状态字符串)
```

GUI 默认预览文本：

```text
Score:300.0
Time:1800.000
OutTime:2.000
State:Pending1
StateTime:1800.000
```

这些是设计期占位值，不代表运行时真实分数/时间。

## 4. RMUA2024Lobby

文件：

```text
RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.umap/.uexp
```

UAssetAPI 解析出的关键点：

- `DefaultEngine.ini` 的 `GameDefaultMap` 是 Lobby。
- Lobby 的 `LevelExport.URL.Map` 为 `/Game/Maps/RMUA2024OL/RMUA2024OL1`；本地默认进入 OL1。
- Blueprint 变量/名称：
  - `InitialSeed`
  - `CmdParams`
  - `CmdParams_Value`
  - `Seed`
  - `ExecuteConsoleCommand`
  - `GetPlayerController`
  - `GameplayStatics`
  - `KismetSystemLibrary`
- `ExecuteUbergraph_RMUA2024Lobby` 的 Kismet 里包含：
  - 设置鼠标指针
  - 设置 CineCamera
  - `r.setRes 1280x720 w`
  - 没有 OL2–OL12 的地图名字符串数组。

结论：Lobby 只硬编码 OL1；12 张图的选择不在这里，也不在 `seed`。官方评测端应通过外部地图选择/启动参数/替换地图实现。

## 5. Kismet 解析边界

- `GUI2024.kismet3.txt`、`Lobby.kismet.txt` 是由 `scripts/kismet_summary.py` 从 UAssetAPI 的 `ScriptBytecode` token 生成的摘要。
- 已经能读出函数调用、字符串常量、控件属性；但没有把整个 Blueprint 事件图还原成 C++/Python 级别的控制流。
- 对比赛仿真真正重要的数据（路线、门、工厂、端口、Score 绑定、地图默认）已经从属性/字节码中提取；其余 UI 动画/按钮逻辑不影响训练。

## 6. 证据

- `artifacts/blueprints/BP_GameConfig2024_1.json`
- `artifacts/blueprints/BP_Gate.json`
- `artifacts/blueprints/GUI2024.json`
- `artifacts/blueprints/RMUA2024Lobby.json`
- `artifacts/blueprints/GUI2024.kismet3.txt`
- `artifacts/blueprints/Lobby.kismet.txt`
- `artifacts/pseudocode/anchors_pseudo3.c`
