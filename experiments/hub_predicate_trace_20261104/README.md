# Raised connector: predicate and out-time trace

Seed 123, OL1, unmodified simulator, GPS/IMU/LiDAR development VEL, 6 m/s. This repeats the raised connector and adds all-stop GDB entry/finish instrumentation to `0x2fb9830`; it records only predicate transitions, raw checked fields, and object array members whose predicates pass. GDB changes timing; breakpoints are observations and do not change program data. The route controller subscribed to the diagnostic pose; no debug truth subscribers appear in the ROS graph. No stage 4/5/6 was reached.

The predicate returned 1 at startup, then changed 1→0 near GPS x≈555.62 m, and fluctuated as x progressed through ≈570.66 m. At each 0 transition the probed arrays `+0x30/+0x40/+0x60/+0x70` had no passing element. During intermittent 1 intervals, `array30` index 0 had `+0x288=2 > +0x2b4=1`. This does not exclude an unprobed weak-reference branch or prove the full semantics or identity of these objects. The latest GPS callback is paired by host monotonic arrival, not interpolated; offsets are in `summary.json`.

`out_timer` read from GameState+0x1ac was 0, 0.4, 1.3, 1.9 seconds across the successive excursions: it remained at its earlier value while the predicate was 1, rather than resetting. The first logged state 12 request occurred with old state 1, timer≈2.000005 and return address `0x2fb9517`; this confirms accumulation across the observed zero intervals in this run. The earlier static branch in `artifacts/rea/followup_callers.c` is consistent: when stored flag +0x180 is zero the tick delta is added; otherwise there is no corresponding reset in that block. More complete object/volume typing and an independent timing run are needed before generalizing. The 35000 constant in `0x2fb9830` is a raw float; its physical meaning has not been established.

All source snapshots, binary/config hashes, raw point clouds, pose, wind, telemetry and GDB logs are retained. GDB observed wall and monotonic clocks, ROS message source stamps and GameState accumulator units must not be conflated. This is a failed diagnostic route, not an official score.

```bash
python3 scripts/export_beta_static_route.py --route candidate_1_3 --connector-lift-m 8 --output /tmp/rmua-predicate/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/rmua-predicate --seconds 150 --speed 6 --hub-observer --predicate-trace
```
