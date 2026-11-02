# 10 面向 DRL 的仿真使用笔记

## 1. 可用的训练信号

从静态分析可确认的仿真状态：

- `pose_gt` / `gps` / `imu`：位姿、速度、姿态；
- 4 相机 + LiDAR：视觉/点云；
- `/airsim_node/debug/rotor_pwm`：实际 PWM；
- `GameConfig.Score`：航段计数器（每进入 case 2 自增）；
- `GameConfig.FlightTime`：六段飞行时间累计；
- `GameConfig.CurStateTime`：当前状态时间；
- `GameConfig.GameState`：状态字符串；
- `/airsim_node/debug/wind`：风；
- `/airsim_node/initial_pose`、`end_goal`；
- `meter_report` 服务/`custom_msgs`：工厂巡检/游戏事件（以 runtime 为准）。

## 2. Reward 建议

不要直接拟合官方分数；官方分数在裁判端。可组合：

```text
r = w_gate * gate_passed
  + w_seg  * segment_advanced
  + w_port * port_or_meter_event
  - w_time * delta_flight_time
  - w_col  * collision
  - w_dev  * deviation_from_route_spline
  + w_speed * progress_along_spline
```

- `segment_advanced`：用 `GameState`/`Score` 变化检测；
- `gate_passed`：用 `pose_gt` 穿过门平面/门中心距离，或运行时事件；
- `deviation`：用 `routes.csv` 的样条点计算最近点/横向误差；
- 训练早期可用 `IgnoreAllHitCollision`/`IgnoreOverTime` 降低终止；正式评估恢复 false。

## 3. 域随机化

- 同图换 `seed`：改变 `FRandomStream` 随机量（风等）；扫 seed 1..N。
- 换图：OL2–OL12 需要地图切换（见 `docs/02`），不要只换 seed。
- 传感器噪声：`settings_Game2024_1.json` 已开启图像噪声；训练时可按需保留/关闭。
- 风：`IgnoreWind` 只用于消融/调试；训练应保留。

## 4. 并行与吞吐

- 同一 ROS master 不能多开固定节点名的模拟器。
- 推荐：每实例一个容器 + 独立 ROS master/端口；GPU 足够时并行 2–4 个。
- 降低负载：`-RenderOffscreen`、删除不用的相机、降低相机分辨率/帧率。
- LiDAR 32 线/200k 点/s 很重；纯视觉策略可尝试关闭 LiDAR，但官方评测可能要求使用全部传感器，需遵守规则。

## 5. Episode 重置

- `/airsim_node/reset` 重置 Score/计时/状态；
- 如需改变 seed，优先重启进程/容器；
- 重置后等待 IMU/pose_gt 稳定再开始 step，避免第一帧异常。

## 6. 重要合规提醒

- `GameConfig` 的 5 个未文档化字段、改地图、改传感器配置都属于**调试/训练环境**操作；正式提交镜像/成绩必须遵守当季规则和评测端配置。
- 不要修改碰撞/计时后声称正式成绩；文档和实验应区分“训练环境”和“官方配置”。
