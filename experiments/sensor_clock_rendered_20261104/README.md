# RenderOffscreen sensor clock probe

20 seconds, seed 123, one idle local scene. The observer subscribes to IMU, LiDAR and both rendered cameras, recording source ROS header stamp, callback ROS now, host wall and monotonic arrival; image data are not retained, only payload sizes. The four clock fields must not be assumed synchronized or interpreted as calibrated transport latency. These are diagnostic results, not competition validation.

```bash
python3 scripts/run_rendered_sensor_probe.py --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --output /tmp/rmua-render-sensors --port 11681 --seconds 20 --display :1
python3 scripts/summarize_sensor_clock.py /tmp/rmua-render-sensors/samples.jsonl --output /tmp/rmua-render-sensors/summary.json
```

Requires the locally installed Docker image and simulator listed in `manifest.json`. Observer queueing, ROS publisher behavior, simulated sensor time and rendering load affect measured rates and apparent age; zero observed sequence gaps only describes this subscriber.
