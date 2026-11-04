# Bounded beta VEL sensor-navigation probe

Seed 123, OL1, NullRHI, unmodified official simulator. The beta `route_follower` subscribes via ROS remapping to the diagnostic GPS position + IMU orientation bridge; the ROS graph snapshot shows no subscribers to `/debug/pose_gt`. Static cooked-map spline 1 provides XY and altitude guides; no recorded truth-derived route. Controller LiDAR backend is enabled, speed limited to 2 m/s for this run. The run stops after 60 wall-clock seconds (`timeout` exit 124); this is not race completion. Graph, pose source timestamps, wind, GDB state, controller output, source hashes and route provenance are retained.

The preceding `../beta_sensor_scout_20261104/` used an incorrect implicit `gate_z_uses_offset=true`, causing a 4.31 m startup height reference jump, and entered requested state 12 after roughly 2 s in state 1. The GDB state breakpoint has no reason code; the cause is not established. That failure is preserved. This corrected run sets `gate_z_uses_offset=false`, progressed to GPS x≈106.7 m, observed state 0→1 with no state 12 in the captured period, and ended on the time bound. `lidar_fresh_publication_check` here was the beta direct-node default false; the later 4 m/s run explicitly enables it. Neither run proves collision-free full-course navigation or stage 4/5/6.

Replay after choosing a fresh output directory and port:

```bash
python3 scripts/export_beta_static_route.py --output /tmp/beta-scout/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/beta-scout --seconds 60 --speed 2
```

Current script includes later fail-fast state monitoring and explicit LiDAR freshness. Earlier versions are preserved in experiment directories. Source stamp, ROS time, host monotonic and host wall clock are not interchangeable; there is no calibrated end-to-end latency measurement here.
