# 11 资料清单与未入库项

## 已入库（本仓库）

| 路径 | 内容/大小 |
|---|---|
| `README.md` | 总览、核心结论、使用建议 |
| `docs/01..11` | 目标/地图/seed/计时/ROS/AirSim/Blueprint/复现/边界/DRL/清单 |
| `artifacts/maps/maps_summary_all.json` | 12 张图的完整路线/门/工厂/端口/样条摘要（约 0.7 MB） |
| `artifacts/tables/*.csv` | gates/factories/ports/check_ports/routes（约 140 KB） |
| `artifacts/pseudocode/anchors_pseudo3.c` | Ghidra 定向反编译（约 169 KB） |
| `artifacts/blueprints/*.json` | BP_GameConfig/BP_Gate/GUI/Lobby 的 UAssetAPI JSON |
| `artifacts/blueprints/raw/` | 上述资产解包后的 `.uasset/.uexp` |
| `artifacts/blueprints/*.kismet*.txt` | Kismet token 摘要 |
| `artifacts/configs/` | GameConfig/ROSConfig/DefaultEngine/DefaultGame/settings 2024（以及 2023 参考） |
| `artifacts/disasm/` | 关键函数/游戏区反汇编（约 19 MB） |
| `artifacts/symbols/` | 主程序/libairsimros 动态符号、段表 |
| `artifacts/strings/` | 完整 ASCII/UTF-16 strings、匹配结果、pak 文件清单、anchors/seed 上下文 |
| `artifacts/vtables/` | vtable 目标与汇总 |
| `artifacts/extra/` | 其余小于 5 MB 的分析中间文件 |
| `scripts/` | 全部解析/反编译/导出脚本 |
| `repro/` | 下载模拟器、克隆官方仓库脚本 |

## 未入库（可重生成）

| 内容 | 大小 | 生成方式 |
|---|---:|---|
| `simulator_12.0.0.5.zip` | 2.0 GB | `repro/download_simulator.ps1` |
| `sim/Build/LinuxNoEditor` | 2.0 GB | 解压 zip |
| `extracted/`（pak 解包资源，含 BuiltData） | 约 4.3 GB | `repak unpack`；见 `docs/07` |
| `maps_json/RMUA2024OL1..12.json` | 约 2.4 GB | `scripts/dump_all_maps.sh`（UAssetAPI） |
| `main.text.disasm` | 约 724 MB | `scripts/disasm_text.sh` |
| WSL/Ghidra/JDK 工具链 | 约 7–10 GB | 自行安装；版本见 `docs/01` |

## 为什么不全传

- GitHub 单文件 100 MB 限制；12 张图的 UAssetAPI JSON 每张 209 MB，pak 2 GB，解包资源 4.3 GB。
- 这些大文件都可以由公开 URL/脚本重新生成，且复制到远端会显著增加克隆成本。
- 训练需要的门/工厂/端口/路线数据已经压缩进 `maps_summary_all.json` 和 CSV；原始资产只在需要重新解析细节时使用。

## 校验

```powershell
Get-FileHash D:\RMUA2026-re\simulator_12.0.0.5.zip -Algorithm SHA256
# 3FA858942D58DF1FD37D3779A2F1942333F61CD01E87EFC25AD216EEA988295E

Get-FileHash D:\RMUA2026-re\sim\simulator_12.0.0.5\Build\LinuxNoEditor\RMUA\Binaries\Linux\RMUA-Linux-Shipping -Algorithm SHA256
# 5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800
```

## 2026-11-04 追加

- `docs/12..16`：本机 seed/NullRHI 实测、RandomTree、车辆/线障、seed 总表、无头需求。
- `artifacts/rea/followup_native.c`、`followup_callers.c`、`followup_state.c`：门、RandomTree、风初始化、室外段、线障生成、状态切换的定点反编译。
- `scripts/read_elf_floats.py`：按 ELF 虚拟地址读取风/线障常量（含 SHA-256）。
