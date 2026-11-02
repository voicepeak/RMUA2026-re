# 02 地图、路线与门/工厂/端口

## 1. 地图清单

Pak 内包含：

```text
RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.umap
RMUA/Content/Maps/RMUA2024OL/RMUA2024OL1.umap
...
RMUA/Content/Maps/RMUA2024OL/RMUA2024OL12.umap
```

12 张图都是独立地图，几何/门/工厂位置不同；不是同一张图只换 seed。

- 本地默认加载 `GameDefaultMap=RMUA2024Lobby`。
- Lobby 的 `LevelExport.URL.Map` 为 `/Game/Maps/RMUA2024OL/RMUA2024OL1`，所以本地启动默认进入 **OL1**。
- 官方评测/服务器可以通过换图或启动参数使用 OL2–OL12；客户端 Lobby 中没有 OL2–OL12 的选择表，也没有“seed 选图”的字符串/逻辑。

## 2. 每张图的结构

每张图（含 OL1）解析出：

| 配置 | 数量 | 说明 |
|---|---:|---|
| `RouteConfigs` | 12 | 路线段；`RouteIndex`、`RouteSpline`、`SegmentInterval` |
| `RouteGateConfigs` | 12 | 门；`RouteGateIndex` -> `1_1_Gate..6_2_Gate` |
| `FactoryConfigs` | 12 | 工厂；`FactoryIndex` -> `1_1_Factory..6_2_Factory` |
| `CheckPortConfigs` | 12 | 检查端口；`PortIndex` -> `1_1_Port..6_2_Port` |
| `SideTrackConfigs` | 24 | `1_1_I/O`、`1_2_I/O` 等侧线 |
| `ArrowConfigs` | 24 | 对应箭头 |

`SegmentInterval` 在 OL1 全部为 **2100**（配置整数；推测为 21.00 m 或 2100 cm，需在引擎里按实际坐标复核）。

### 路线顺序

- **OL1**：`1_1 → 2_1 → 3_1 → 4_1 → 5_1 → 6_1 → 1_2 → 6_2 → 5_2 → 4_2 → 3_2 → 2_2`
- **OL2–OL12**：`1_1 → 1_2 → 2_1 → 2_2 → 3_1 → 3_2 → 4_1 → 4_2 → 5_1 → 5_2 → 6_1 → 6_2`

也就是说 OL1 是“先绕完 6 条去程，再沿 6_2→2_2 回程”，其余图是逐段往返。不要把 OL1 的顺序套到其他图上。

## 3. 坐标与导出

### Gate / Factory / Port

门、工厂、端口 actor 的根组件变换已经导出为**世界坐标**（UE4 单位 cm）：

- `artifacts/tables/gates.csv`
- `artifacts/tables/factories.csv`
- `artifacts/tables/ports.csv`
- `artifacts/tables/check_ports.csv`

字段：

```text
map,name,x,y,z,yaw,scale_x,scale_y,scale_z
```

门示例（OL1）：

```text
RMUA2024OL1,1_1_Gate,62349.5859375,-8776.669921875,13507.50390625,-83.16549682617188,4.449821949005127,1.8593449592590332,2.4494340419769287
```

门的 `scale` 是视觉/碰撞体 scale，不是位置。门中心用 root location，朝向用 yaw。

工厂/端口同理；`Building_Factory_Factory` 之类的静态网格名会被过滤，当前 `factories.csv` 只包含 `N_N_Factory` actor。

### Route Spline

`artifacts/tables/routes.csv`：

```text
map,route_index,spline,point,local_x,local_y,local_z,world_x,world_y,world_z,seg_interval
```

- `local_*` 是 Spline 组件局部空间点。
- `world_*` 按 `component.relative_location + R_yaw(local)` 计算；该组件 scale 在解析到的 12 张图中多为默认 1，未参与计算。
- 如果要做精确训练，建议用 UE 的 `SplineComponent::GetLocationAtSplinePoint` 或运行时 `/airsim_node/debug/pose_gt` 校准一次；CSV 是静态重建，不是引擎运行时输出。
- 每条路线有 4–7 个样条点；门在样条附近但不一定与首尾点重合。

### 原始结构化数据

`artifacts/maps/maps_summary_all.json` 包含每张图的：

```text
map
config_object
config.RouteConfigs[]        (含 spline 的 location/rotation/points)
config.RouteGateConfigs[]
config.FactoryConfigs[]
config.CheckPortConfigs[]    (含 transform)
config.SideTrackConfigs[]
config.ArrowConfigs[]
gates{}                      (每个门的 transform)
factories{}
ports{}
player_starts[]
```

它由 `scripts/extract_map_summary2.py` 从 UAssetAPI 的 209 MB/图 JSON 中提取；原始 JSON 未入库。

## 4. 地图切换方法（本地）

静态证据只能确认默认是 OL1。可选方案（按风险从低到高）：

1. **启动参数/URL 覆盖**：UE4 支持通过 URL/`-map=`/`open` 类命令覆盖地图；需要实测当前 build 是否接受。最稳妥的是在容器里用控制台/`-ExecCmds` 打开 `/Game/Maps/RMUA2024OL/RMUA2024OLn`。
2. **改包内 Lobby 的 Level URL**：把 `RMUA2024Lobby.umap` 的 URL.Map 改为目标图；需要重新打包/替换 pak。
3. **改 `DefaultEngine.ini` 的 `GameDefaultMap`**：直接把默认地图设为 `RMUA2024OLn`；同样需要改 pak 或使用非 UFS 覆盖。
4. **官方服务器**：评测端可能直接为每轮指定地图；本地脚本只传 `seed`，所以不要把本地默认图当成官方轮次映射。

## 5. 证据

- `artifacts/maps/maps_summary_all.json`
- `artifacts/tables/*.csv`
- `artifacts/configs/DefaultEngine.ini`
- `artifacts/configs/DefaultGame.ini`
- `scripts/query_map.py`、`scripts/extract_map_summary2.py`、`scripts/export_map_tables.py`
- `artifacts/strings/pak_files.txt`
