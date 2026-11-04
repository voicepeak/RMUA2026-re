# Rendered interactive map selection attempt

`scripts/run_map_console_probe.py` launches the unmodified official binary under read-only GDB in a window, waits for `LoadMap` OL1, focuses the RMUA window, and sends keyboard events intended to open the Unreal console and execute `open /Game/Maps/RMUA2024OL/RMUA2024OL2`. The target map was not observed: `gdb.log` and `manifest.json` show only Lobby and OL1. No screenshot or independent confirmation that a console opened or accepted the command was captured, so this result does not establish whether the command is supported or disabled. GDB gate events remain OL1 and are not a cross-map result. All times in manifest are host Unix wall time.

Reproduce from the repository root with the configured simulator, Docker image and X display `:1` after choosing a fresh output directory in the script. It sends real keyboard input to the RMUA window, and should be run only on an isolated test display.
