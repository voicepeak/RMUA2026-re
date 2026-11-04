# First executable argument map selection attempt

Runs invoked the unmodified executable with the OL2 cooked map path replacing the usual `RMUA` first argument, followed by `seed 123` or `seed 456`. Both instances reported `requested_map_not_observed`; the actual `LogLoad: LoadMap` sequence was Lobby then OL1, as retained in both `gdb.log` files and the manifest. This experiment does not show any OL2 runtime gate transforms. It does not establish whether another launch convention or interactive method could load OL2.

From the repo root (after choosing an unused output path and ports):

```bash
python3 scripts/run_map_firstarg_probe.py --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --messages /home/tianbot/RUMA-by-helinjun/beta/rmua_ws --output /tmp/rmua-firstarg --seeds 123,456 --repeats 1 --port 11741 --sample-seconds 2 --display :1
```
