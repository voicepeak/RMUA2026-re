# Offline sensor vs truth alignment on an existing flight

This analysis reads the already recorded `experiments/pwm_20261104/seed123_closed_loop` logs. The `scripts/evaluate_sensor_pose_alignment.py` tool pairs each recorded GPS/IMU message with the nearest independent evaluation truth sample by the recorded source stamp, rejecting matches beyond 20 ms. Quaternions are interpreted as wxyz and compared modulo sign. Truth is used only after the flight, never by the bridge or controller. Input hashes and every match are retained.

170/170 GPS samples matched; position error norm RMSE was 0.0728 m, p95 0.1000 m. 1698/1698 IMU samples matched; orientation angle RMSE was 0.0000876 rad, p95 0.0000000730 rad, max 0.00294 rad. Truth motion ranges were only about 0.95 m, 0.21 m, 1.02 m on the three axes, from one seed and one short PWM closed-loop run. The data support a limited local comparison; they do not independently establish clock synchronization, frame correctness in the full course, sensor behavior under high speed or online transport/control latency.

Reproduce:

```bash
python3 scripts/evaluate_sensor_pose_alignment.py experiments/pwm_20261104/seed123_closed_loop --output /tmp/rmua-pose-alignment
```
