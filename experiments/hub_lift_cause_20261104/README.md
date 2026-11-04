# Raised connector failure: read-only state/collision call-stack probe

This is a seed 123 repetition of the raised connector hypothesis, with official simulator and rule settings unchanged. The route controller consumed GPS/IMU bridge pose and LiDAR, not debug truth; `ros_graph_snapshot.txt` records this. GDB additionally logged calls to `0x2fb7110` and a short stack for each call requesting state 12. The diagnostic runner stopped the controller when it observed state 12. Source snapshots and hashes, sensor/control data, LiDAR, wind, state log and executable/settings hashes are retained.

Two `0x2fb7110` callbacks occurred in state 0 during startup. None was observed during state 1 in this run. At host wall 1793789022.0737 / monotonic 125167.7186, the first recorded request for state 12 observed old state 1 and `+0x1ac=2.000006`; its caller return address is `0x2fb9517`. The instruction listing `outtime_branch.objdump.txt` shows the immediately preceding `call 0x2fb9960` is reached when `GameState+0x180 == 0`, accumulating the tick delta in `+0x1ac`, comparing it to `GameConfig+0x2cc` (the configured OutTime, default 2 seconds), and checking `IgnoreOverTime==false`. This ties the observed state-12 request to that specific 2-second branch; it is not the ordinary 180-second stage timer branch. It does not by itself identify why `+0x180` was false, prove a collision did or did not occur elsewhere, or prove which spatial boundary was crossed. GDB alters scheduling and the state-entry trace records observation, not direct cause for uninstrumented earlier runs.

The route ended early and did not reach stage 4/5/6. Collision callback entry breakpoint coverage is for the fixed function address in the stated binary; there may be other collision mechanisms. The controller exit code 0 reflects the monitor sending SIGINT after observing state 12, not an official success. ROS header stamps, host wall and monotonic timestamps remain distinct.

To reproduce on the same local environment with unused output and ROS port 11761:

```bash
python3 scripts/export_beta_static_route.py --route candidate_1_3 --connector-lift-m 8 --output /tmp/rmua-lift-cause/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/rmua-lift-cause --seconds 150 --speed 6 --hub-observer --cause-trace
```
