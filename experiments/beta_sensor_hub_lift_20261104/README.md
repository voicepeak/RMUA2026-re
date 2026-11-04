# Raised hub-connector candidate — ended before crossing

This bounded 6 m/s seed 123 OL1 VEL diagnostic retains the official simulator and rule settings. It uses the GPS/IMU bridge, LiDAR guards and static spline 1→3 candidate; the 147.53 m connector is lifted by at most 8 m with 80 m entry/exit ramps. The lift was motivated by limited prior LiDAR returns and is not a free-space or collision guarantee. ROS graph confirms the route controller subscribed to the diagnostic pose and the debug pose topic had no subscribers.

The state observer saw stage 1 and state 0→1→12; the runner observed state 12 and sent SIGINT to the controller. Controller exit code 0 reflects that shutdown, not an official success. The last logged GPS position was approximately [571.08,-110.93,-134.36] m. The state breakpoint does not report why the simulator ended; neither collision nor another cause is established. This did not reach the connector or stage 4/5/6. 6113 wind messages were captured, all zero. Two passive LiDAR scans near x≥560 and the control telemetry are retained, along with ROS graph, timing, route provenance, input hashes and source snapshots. GDB pauses can affect timing. Each `*.npz` cloud has sensor-frame points plus timestamps and the latest GPS/IMU; its transform is approximate and not diagnostic truth.

Reproduce only in an isolated development environment, with matching local image/simulator and an unused ROS port (the runner uses 11761):

```bash
python3 scripts/export_beta_static_route.py --route candidate_1_3 --connector-lift-m 8 --output /tmp/rmua-lift/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/rmua-lift --seconds 180 --speed 6 --hub-observer
python3 scripts/analyze_hub_lidar.py /tmp/rmua-lift/hub --output /tmp/rmua-lift/hub_analysis.json
```

This is a failed candidate, not an official score, a validated route, or an assertion that the cause of state 12 is known.
