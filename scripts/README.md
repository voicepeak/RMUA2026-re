# 脚本说明

这些脚本是本次逆向的实际执行脚本，路径按分析机 `D:\RMUA2026-re` 编写；换设备后先改 `env.sh` 和各脚本里的绝对路径。

## 数据提取

| 脚本 | 作用 |
|---|---|
| `dump_all_maps.sh` | 用 UAssetAPI 把 12 张 `.umap` 导出为 JSON（约 209 MB/图） |
| `extract_map_summary2.py` | 从单张地图 JSON 提取 RouteConfigs/门/工厂/端口/样条点，输出 summary |
| `extract_all_maps.sh` | 对 12 张图批量执行上面的提取 |
| `combine_maps.py` | 合并 12 份 summary 为 `maps_summary_all.json` |
| `export_map_tables.py` | 生成 `gates.csv`/`factories.csv`/`ports.csv`/`routes.csv`/`check_ports.csv` |
| `query_map.py`、`inspect_map_exports.py`、`inspect_map_details.py` | 交互排查地图 JSON |
| `dump_gate_transform.py`、`inspect_spline_chain*.py` | 检查组件/actor 变换链 |
| `kismet_summary.py` | 把 UAssetAPI JSON 中的 Kismet bytecode 打印成可读 token |
| `Program.cs` | UAssetAPI 最小解析器（uasset/uexp -> JSON） |

## 二进制分析

| 脚本 | 作用 |
|---|---|
| `DumpAnchors.java` | Ghidra 定向反编译脚本（只处理已知函数地址） |
| `run_dump_anchors.sh` | 调用 Ghidra analyzeHeadless + DumpAnchors |
| `disasm_text.sh`、`disasm_game_region.sh` | 生成 objdump 反汇编 |
| `dump_config_func.sh`、`dump_flag_uses.sh`、`dump_route_init.sh`、`dump_seed_refs.sh` | 针对 GameConfig/seed/状态机的反汇编 |
| `find_utf16_keys.py`、`find_keys_files.py`、`find_seed_context.py`、`context_keys.py` | 在二进制/pak 资产中定位字符串 |
| `string_offsets.py`、`symbols_main.sh`、`inspect_lib.sh`、`grep_lib.sh` | 符号/字符串/偏移辅助 |
| `vtables2.py`、`vtable_summary.py` | 解析 vtable 目标 |
| `fde_lookup.py`、`parse_frames.py`、`parse_frames2.py` | 从 `.eh_frame` 找函数边界 |
| `grep_config_fields.sh` | 查 GameConfig 字段的使用点 |

## REA MCP

`rea_manager.mjs` 是一个长驻 MCP 客户端：读取 `mcp/queue/*.json` 请求，调用 `rea --mcp` 的 `tools/call`，把结果写到 `mcp/results/`。本次用于 REA 会话，但主 ELF 的全量 Ghidra 分析仍建议用 `DumpAnchors`。

## 注意

- 所有脚本只读原始文件；不会修改官方模拟器/pak。
- `repak get` 对部分 Oodle 条目返回压缩字节；请用 `repak unpack`。
- 路径中的中文/空格在不同机器可能导致问题；建议使用无空格路径。
