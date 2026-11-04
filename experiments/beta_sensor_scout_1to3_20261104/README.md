# Candidate spline 1→3 connector probe

Seed 123, unmodified OL1, NullRHI, local VEL diagnostic; no debug truth input. GPS+IMU pose was connected through the diagnostic bridge; the ROS graph snapshot shows `/route_follower` as its subscriber and no subscriber to `/debug/pose_gt`. The path consists of static cooked spline 1, a 147.53 m interpolated straight candidate, then reversed static spline 3. The candidate connector is not verified free space. LiDAR backend, publication freshness check and bounded speed 6 m/s remained enabled; the user-visible event timer/score and collision settings were not changed.

The 180-wall-second bounded run reached approximately [615.04,-80.69,-133.36] m and remained near route station 689 m with controller log `COMMAND_BRAKING`. This is within the candidate connector, not a completed traversal. State breakpoints show 0→1 and stage 1 only; no wind/line/vehicle generation entry was hit. The 9155 `/debug/wind` messages were all zero in this observed range. No causal claim is made about the braking condition or geometry without additional inspection. Timeout exit 124 is the imposed diagnostic bound, not race completion. The complete pose, wind and GDB clock domains, source hashes, simulator/controller logs and failure/timeout are preserved.

Reproduce with an unused path and ROS port (the runner currently uses port 11761):

```bash
python3 scripts/export_beta_static_route.py --route candidate_1_3 --output /tmp/rmua-1to3/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/rmua-1to3 --seconds 180 --speed 6
```

The source clock, host Unix wall clock and monotonic arrival clock are separate; raw pose source stamps and observer clocks are recorded. Breakpoints affect scheduling. This does not establish competition equivalence or certify the candidate route.
