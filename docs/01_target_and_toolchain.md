# 01 目标与工具链

## 1. 目标软件

官方模拟器包：

```text
https://sz-rm-rmua-dispatch-prod.oss-cn-shenzhen.aliyuncs.com/b39a0194e982f0d987153c6016feb325/simulator_12.0.0.5.zip
```

| 项目 | 值 |
|---|---|
| ZIP 大小 | 2,021,176,468 字节 |
| ZIP SHA-256 | `3FA858942D58DF1FD37D3779A2F1942333F61CD01E87EFC25AD216EEA988295E` |
| 解压后主目录 | `simulator_12.0.0.5/Build/LinuxNoEditor` |
| 主程序 | `RMUA/Binaries/Linux/RMUA-Linux-Shipping`，116,748,832 字节 |
| 主程序 SHA-256 | `5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800` |
| Pak | `RMUA/Content/Paks/RMUA-LinuxNoEditor.pak`，1,927,609,305 字节 |
| AirSim ROS 库 | `RMUA/Plugins/AirSimROS/Binaries/Linux/libairsimros.so`，3,678,624 字节 |
| libairsimros SHA-256 | `69d5ea3302bdd5f20b842ec3d3e84bafcb50ef3649ada42c4d346c9bcd389885` |

官方仓库（本次使用分支）：

- 模拟器：<https://github.com/RoboMaster/IntelligentUAVChampionshipSimulator>，分支 `RMUA2026-01`
- 基础示例：<https://github.com/RoboMaster/IntelligentUAVChampionshipBase>，分支 `RMUA2026`
- 规则中心：<https://bbs.robomaster.com/wiki/20204847/809873>

模拟器包里的 `.git` 只对应模拟器仓库的 README/脚本，`Build/` 是未跟踪的打包产物；仓库本身不提供 UE 工程源码或 `.debug` 文件。

## 2. 引擎与打包格式

- 引擎：**Unreal Engine 4.27**（UAssetAPI 使用 `EngineVersion.VER_UE4_27` 可解析）。
- Pak：V11（Fnv64BugFix），未加密。
- 压缩：**Oodle (Kraken) + Zlib**。
- `repak info`：

```text
mount point: ../../../
version: V11
encrypted index: false
compression: Oodle ,Zlib
2756 file entries
```

关键目录：

```text
RMUA/Config/DefaultEngine.ini
RMUA/Config/DefaultGame.ini
RMUA/Content/Configs/GameConfig.json
RMUA/Content/Configs/ROSConfig.json
RMUA/Content/Settings/settings_Game2024_1.json
RMUA/Content/Blueprints/BP_GameConfig2024_1.uasset/.uexp
RMUA/Content/Blueprints/BP_Gate.uasset/.uexp
RMUA/Content/Blueprints/GUI2024.uasset/.uexp
RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.umap/.uexp
RMUA/Content/Maps/RMUA2024OL/RMUA2024OL1..12.umap/.uexp
```

`DefaultEngine.ini` 关键项：

```ini
GameDefaultMap=/Game/Maps/RMUA2024OL/RMUA2024Lobby.RMUA2024Lobby
GameInstanceClass=/Script/AirSimROS.AirSimROSGameInstance
```

`DefaultGame.ini` 关键项：

```ini
+MapsToCook=(FilePath="/Game/Maps/RMUA2024OL/RMUA2024OL1")
...
+MapsToCook=(FilePath="/Game/Maps/RMUA2024OL/RMUA2024OL12")
+MapsToCook=(FilePath="/Game/Maps/RMUA2024OL/RMUA2024Lobby")
+DirectoriesToAlwaysStageAsNonUFS=(Path="Configs")
```

这解释了 `Content/Configs/GameConfig.json` 是外部可编辑文件。

## 3. 分析工具

| 工具 | 版本/用途 |
|---|---|
| REA | 6.4.0，MCP/CLI；本机 WSL 内使用 |
| Ghidra | 12.1.4 PUBLIC，Linux headless |
| JDK | Temurin 21 |
| UAssetAPI | 1.1.0（.NET 8） |
| repak | 0.2.3 |
| Python | 3.12（WSL Ubuntu 24.04） |
| capstone/pwntools | 辅助静态分析 |

REA 在 Windows 上只接受 PE，且全量分析 116 MB ELF 触发 330 秒超时；因此最终采用：

1. REA/`rea analyze` 处理 `libairsimros.so`（小型、可分析）；
2. 对主 ELF 使用 **Ghidra headless + 定向函数反编译**（`scripts/DumpAnchors.java`），只创建并反编译已知地址，13 秒完成；
3. UAssetAPI 解析 pak 解包后的 `.uasset/.uexp`；
4. `repak unpack` 负责 Oodle 解压；注意 `repak get` 对部分条目返回的是压缩字节，不能直接喂 UAssetAPI。

## 4. 证据与边界

- 函数地址、字符串地址、pak 路径、哈希均来自本次实际文件。
- 反编译输出是 Ghidra 伪代码，不等于原源码；函数体可能因无返回假设被截断。
- 没有官方 `.debug` 文件；没有 UE 工程源码。
- 静态分析不能证明运行时行为；涉及计分、ROS 频率、重置时序时，建议在真实模拟器上复核。
- 原始大文件（zip、pak、解包资源、2.4 GB UAssetAPI JSON）未入库，见 `docs/07_reproduction.md` 重生成。
