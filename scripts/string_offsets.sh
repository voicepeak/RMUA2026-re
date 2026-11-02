#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
strings -a -t x "$MAIN" > /root/main.strings.off
for s in Configs/GameConfig.json RouteGateConfigs RouteGateIndex RouteGateConfig AGameConfig2024_1 GameState2024_1 ARMUAGameModeBase RenderOffScreen ResetGameEvent; do
  echo "==== $s"
  grep -F -- "$s" /root/main.strings.off | head -5
done
