# 04 ROS 接口与观测/动作

## 1. 通信架构

- 模拟器是 UE4 + AirSim 插件；ROS 桥编译成 `libairsimros.so`，主程序通过导出的 C ABI 调用：
  - `airsim_ros::InitAirSimROS`
  - `AirSimROSFrame`
  - `AirSimROSLidar`
  - `AirSimROSGetStatus`
  - `AirSimROSPushImage` / `AirSimROSPushPointCloud`
  - `AirSimROSNotifyGameStatus`
  - `AirSimROSPushTreePoses` / `AirSimROSPushCirclePoses` / `AirSimROSPushPortsInfo`
- 官方基础示例在 `basic_dev/src/airsim_ros`，包含 msg/srv 定义。

> 话题名以**实际运行**的 `rostopic list` 为准；不同文档/示例存在命名差异。

## 2. 观测（订阅）

| 话题 | 类型 | 说明 |
|---|---|---|
| `/airsim_node/drone_1/front_left/Scene` | `sensor_msgs/Image` | 前左相机 960x720，FOV 60 |
| `/airsim_node/drone_1/front_right/Scene` | `sensor_msgs/Image` | 前右 |
| `/airsim_node/drone_1/back_left/Scene` | `sensor_msgs/Image` | 后左 |
| `/airsim_node/drone_1/back_right/Scene` | `sensor_msgs/Image` | 后右 |
| `/airsim_node/drone_1/front_left/Scene/camera_info` | `sensor_msgs/CameraInfo` | 相机内参 |
| `/airsim_node/drone_1/imu/imu` | `sensor_msgs/Imu` | IMU；文档建议用它做全局时钟 |
| `/airsim_node/drone_1/lidar` | `sensor_msgs/PointCloud2` | 32 线，10 Hz，200k 点/s，Range 30 m |
| `/airsim_node/drone_1/debug/pose_gt` | `geometry_msgs/PoseStamped` | 状态真值 |
| `/airsim_node/drone_1/gps` | `geometry_msgs/PoseStamped` | 带误差 GPS/姿态 |
| `/airsim_node/drone_1/debug/wind` | `airsim_ros/...` | 风 |
| `/airsim_node/drone_1/debug/rotor_pwm` | `airsim_ros/RotorPWM` | 实际电机 PWM |
| `/airsim_node/initial_pose` | `geometry_msgs/PoseStamped` | 起点 |
| `/airsim_node/end_goal` | `geometry_msgs/PoseStamped` | 终点 |
| `/airsim_node/custom_msgs` | 自定义 | 游戏状态/事件（实际类型以 runtime 为准） |

## 3. 动作（发布）

### 3.1 PWM 控制

话题：

```text
/airsim_node/drone_1/rotor_pwm_cmd
```

类型：`airsim_ros/RotorPWM`，4 个电机 PWM（0..1 或 0..1000，按官方 msg/README；以实际 msg 为准）。
电机顺序：

```text
0: 右前
1: 左后
2: 左前
3: 右后
```

主程序入口：`msr::airlib::MultirotorApiBase::moveByMotorPWMs`，地址 `0x612efc0`。
`REA_2fb3a50` 里的 `HISM_ThickLine/HISM_ThinLine` 是路线可视化，不是控制。

### 3.2 速度控制

README 写：

```text
/airsim_node/drone_1/vel_body_cmd
```

`basic_dev` 示例代码里写的是：

```text
airsim_node/drone_1/vel_cmd_body_frame
```

两者至少有一个是文档/示例不一致；**运行时用 `rostopic list | grep vel` 确认**。
消息字段：

```text
twist.linear.x/y/z   (m/s)
twist.angular.z      (yaw rate, deg/s)
acceleration         (m/s^2, 上限 8)
emergency_stop       (1 = 急停)
```

官方说明：速度控制只提供基础飞控；静止无风可悬停，但大幅速度变化会有姿态波动。需要稳定控制建议用 PWM。

### 3.3 服务

| 服务 | 说明 |
|---|---|
| `/airsim_node/drone_1/takeoff` | 起飞 |
| `/airsim_node/drone_1/land` | 降落 |
| `/airsim_node/reset` | 重置 |
| `/airsim_node/meter_report` | 工厂巡检数据上报，参数 `(index, value)`；index 0=中央枢纽前工厂，1=中央枢纽后工厂 |

`reset` 是轻量 episode 重置；是否需要重启进程取决于 seed/随机流处理。

## 4. 传感器配置

`settings_Game2024_1.json`：

- `SimMode: Multirotor`，`DefaultVehicleConfig: SimpleFlight`
- `EnableCollisions: true`，`EnableCollisionPassthrogh: false`
- Lidar：32 channels，10 rotations/s，200000 points/s，X/Y/Z=0/0/-0.05，Range 30，VerticalFOVUpper 40，VerticalFOVLower -19，HorizontalFOV -180..180，`DataFrame: SensorLocalFrame`
- 4 相机：960x720，FOV 60，位置：
  - front_left: (0.175,-0.15,0)
  - front_right: (0.175,0.15,0)
  - back_left: (-0.175,0.15,0)，yaw 180
  - back_right: (-0.175,-0.15,0)，yaw 180
- 图像噪声：`Enabled: true`，`RandContrib: 0.2`，`RandSpeed: 100000`，`RandSize: 1000`，`RandDensity: 6.5`，`HorzWaveStrength: 0.08` 等。

`ROSConfig.json`：

```json
"ROS": {
  "EnableDebug": true,
  "FrameTimeTolerance": 125
}
```

`FrameTimeTolerance: 125` 与 ROS 帧时间容差相关；建议用 IMU 时间戳做主时钟（README 明确说明模拟器时钟与本地有差异）。

## 5. 并行仿真注意事项

- ROS 节点名/话题名是固定的 `/airsim_node/drone_1/...`；同一 ROS master 下启动多个模拟器会冲突。
- 可行方案：
  1. 每个实例独立容器 + 独立 ROS master（推荐；官方本来就按容器评测）；
  2. 修改 `libairsimros.so`/主程序里的 node namespace（需要改代码/重新桥接）；
  3. 用多个 ROS master（不同 `ROS_MASTER_URI`/端口）并通过网桥隔离。
- 渲染关闭：`run_simulator_offscreen.sh` / `-RenderOffscreen`；在 `settings.json` 删除不用的相机会显著降低 GPU 负载。
- 多实例 GPU 显存是首要瓶颈；Lidar 32 线 + 4 相机开销大，训练时可按需关闭相机。

## 6. 证据

- `README.md`（官方模拟器）
- `artifacts/configs/settings_Game2024_1.json`
- `artifacts/configs/ROSConfig.json`
- `Base/basic_dev/src/airsim_ros/msg/*.msg`
- `artifacts/symbols/lib_game_symbols.txt`
- `artifacts/pseudocode/anchors_pseudo3.c` 的 `moveByMotorPWMs`
