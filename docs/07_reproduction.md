# 07 复现步骤

## 0. 目录约定

原分析机器使用：

```text
D:\RMUA2026-re\
  sim\                 # 解压后的模拟器
  extracted\           # pak 解包资源
  artifacts\           # 分析输出
  scripts\             # 分析脚本
  tools\               # Ghidra/JDK/repak
  wsl\                 # WSL 根文件系统（可选）
```

换设备后建议保持相同目录，或修改 `scripts/env.sh` 和脚本里的绝对路径。

## 1. 下载模拟器

```powershell
$url = "https://sz-rm-rmua-dispatch-prod.oss-cn-shenzhen.aliyuncs.com/b39a0194e982f0d987153c6016feb325/simulator_12.0.0.5.zip"
curl.exe -L --retry 5 --continue-at - -o D:\RMUA2026-re\simulator_12.0.0.5.zip $url
Get-FileHash D:\RMUA2026-re\simulator_12.0.0.5.zip -Algorithm SHA256
# 期望 3FA858942D58DF1FD37D3779A2F1942333F61CD01E87EFC25AD216EEA988295E
tar -xf D:\RMUA2026-re\simulator_12.0.0.5.zip -C D:\RMUA2026-re\sim
```

或直接使用 `repro/download_simulator.ps1`。

克隆官方仓库：

```powershell
git clone --depth 1 -b RMUA2026-01 https://github.com/RoboMaster/IntelligentUAVChampionshipSimulator
git clone --depth 1 -b RMUA2026    https://github.com/RoboMaster/IntelligentUAVChampionshipBase
```

## 2. Pak 解包

工具：repak 0.2.3（本仓库 `repro/` 不包含二进制，可从 GitHub 下载）。

```powershell
repak.exe info  D:\RMUA2026-re\sim\simulator_12.0.0.5\Build\LinuxNoEditor\RMUA\Content\Paks\RMUA-LinuxNoEditor.pak
repak.exe list  ... > artifacts\pak_files.txt
repak.exe unpack ... -o D:\RMUA2026-re\extracted\Maps -i "RMUA/Content/Maps/RMUA2024OL"
repak.exe unpack ... -o D:\RMUA2026-re\extracted\BP3 -i "RMUA/Content/Blueprints"
repak.exe unpack ... -o D:\RMUA2026-re\extracted\BP4 -i "RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.umap" -i "RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.uexp"
```

**关键坑**：`repak get` 对部分 Oodle 条目返回的是压缩字节（文件头 `ff fe`），UAssetAPI 会报 `File signature mismatch`。必须用 `repak unpack` 得到解压后的 `.uasset`（头 `c1 83 2a 9e`）。`repak unpack` 对 `.uexp` 也会写出可用数据。

## 3. UAssetAPI 解析

需要 .NET 8 SDK：

```bash
dotnet new console -o uasset_test
cd uasset_test
dotnet add package UAssetAPI --version 1.1.0
```

`scripts/Program.cs` 的核心：

```csharp
var asset = new UAsset(args[0], EngineVersion.VER_UE4_27);
File.WriteAllText(args[1], asset.SerializeJson(true));
```

批量导出 12 张地图：

```bash
bash scripts/dump_all_maps.sh
# 输出 artifacts/maps_json/RMUA2024OL1..12.json，每张约 209 MB
```

生成摘要/CSV：

```bash
python3 scripts/extract_all_maps.sh   # 内部使用 extract_map_summary2.py
python3 scripts/combine_maps.py
python3 scripts/export_map_tables.py
```

## 4. Ghidra 定向反编译

工具：Ghidra 12.1.4 + JDK 21。

```bash
export JAVA_HOME=/opt/jdk-21
export GHIDRA_INSTALL_DIR=/opt/ghidra_12.1.4_PUBLIC
export MAIN=/path/to/RMUA-Linux-Shipping
bash scripts/run_dump_anchors.sh
```

`DumpAnchors.java` 只对以下地址执行 `disassemble + createFunction + decompile`：

```text
0x2fa0e80  seed 初始化
0x2fa24a0  score/time/segment 初始化
0x2fa3190  路线初始化
0x2fa3dd0  FRandomStream 随机量
0x2fb3a50  GameConfig 读取
0x2fb4e80  GetGameConfig
0x2fb7110  碰撞处理
0x2fb7f90  report/state 分支
0x2fb8650  状态切换辅助
0x2fb8f40  GameState tick
0x2fc0020  hover/碰撞
0x2fcc5a0  其他状态逻辑
0x2fd29c0  HUD/结算文本
0x2fba830  FlightTime
0x2fba970  重置
0x2fb9960  状态切换
0x2fb9830  状态判断
0x2fb2050 / 0x2fb2130  风/计时辅助
0x612efc0  moveByMotorPWMs
0x612daf0  resetImplementation
```

输出：`/root/anchors_pseudo.c`，本仓库为 `artifacts/pseudocode/anchors_pseudo3.c`。

> 全量 Ghidra 自动分析在这个 116 MB UE4 Shipping 二进制上会被 `EntryPointAnalyzer`/Decompiler Parameter ID 拖到超时；定向反编译 13 秒完成，适合函数级分析。

## 5. REA

REA 6.4.0 已安装但主 ELF 全量分析受 Windows 限制（只接受 PE）和 330 秒超时影响。可用方式：

- WSL/Linux 内安装 REA + Ghidra + JDK；
- 对 `libairsimros.so` 做全量 REA 分析（3.6 MB，约 50 秒）；
- 主 ELF 使用上面的 Ghidra 定向脚本；
- 需要 REA MCP 会话时，`scripts/rea_manager.mjs` 是本次使用的 MCP 队列管理器。

## 6. 运行模拟器

遵循官方 README：

```bash
cd /path/to/IntelligentUAVChampionshipSimulator
wget https://sz-rm-rmua-dispatch-prod.oss-cn-shenzhen.aliyuncs.com/b39a0194e982f0d987153c6016feb325/simulator_12.0.0.5.zip
unzip simulator_12.0.0.5.zip
# 把仓库文件放到 Build 同级目录（不是 Build 内）
./run_simulator.sh 123
./run_simulator_offscreen.sh 123
```

Docker：

```bash
docker build -t simulator01 .
./run_docker_simulator.sh 123
```

需要 ROS Noetic + Nvidia Container Toolkit；Docker 仅支持后台模式。

## 7. 环境校验

```bash
source /opt/ros/noetic/setup.bash
rostopic list | grep airsim
```

确认 `vel` 话题实际名称（`vel_body_cmd` vs `vel_cmd_body_frame`）。

## 8. 大文件说明

未入库：

- `simulator_12.0.0.5.zip`（2.0 GB）
- `Build/LinuxNoEditor`（2.0 GB）
- 解包资源（约 4.3 GB，含地图 BuiltData 4.5 GB）
- `maps_json/`（2.4 GB）

均可由上面的 URL/命令重新生成。仓库只保留摘要、CSV、伪代码、Blueprint JSON 和证据。
