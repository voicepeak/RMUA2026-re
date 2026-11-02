# 05 AirSim 动力学与车辆参数

## 1. 车辆类型

`settings_Game2024_1.json`：

```json
"SimMode": "Multirotor",
"DefaultVehicleConfig": "SimpleFlight",
"Vehicles": {
  "drone_1": {
    "VehicleType": "SimpleFlight",
    "DefaultVehicleState": "Armed",
    "EnableCollisions": true,
    "AllowAPIAlways": true
  }
}
```

因此动力学主体是 **AirSim SimpleFlight + FastPhysicsEngine**，不是 RMUA 自研的飞控/物理引擎。

## 2. 二进制中的对应模块

`RMUA-Linux-Shipping` 动态符号中包含：

- `msr::airlib::MultirotorApiBase`：
  - `moveByMotorPWMs` @ `0x612efc0`
  - `moveByVelocity` @ `0x6130680`
  - `moveByVelocityZ` @ `0x6130ac0`
  - `moveByAngleRatesZ` @ `0x6130000`
  - `moveByVelocityBodyFrame` @ `0x612e430`
  - `resetImplementation` @ `0x612daf0`
- `msr::airlib::SimpleFlightApi` vtable
- `msr::airlib::FastPhysicsEngine` vtable
- `MultirotorPawnSimApi` vtable
- `msr::airlib::AirSimSimpleFlightBoard` / `AirSimSimpleFlightCommLink` / `AirSimSimpleFlightEstimator` vtable

`moveByMotorPWMs` 本身主要做命令排队/互斥/等待（`waitForFunction`），实际 PWM→电机转速→推力/力矩在 SimpleFlight 板级与 FastPhysicsEngine 内完成。

## 3. 官方给出的车辆参数

README 明确给出（不是从二进制猜的）：

| 参数 | 值 |
|---|---:|
| 质量 | 0.9 kg |
| 轴距（电机到机体中心） | 0.18 m |
| Ixx | 0.0046890742 |
| Iyy | 0.0069312 |
| Izz | 0.010421166 |
| 电机升力系数 | 0.000367717 |
| 电机反扭力系数 | 4.888486266072161e-06 |
| 最大转速 | 11079.03 RPM |

这些是 AirSim SimpleFlight 的机体参数；若要做高保真 RL，可用 AirSim 源码中的 `SimpleFlightQuadXParams` 与这些值对齐。

## 4. 控制与物理

- PWM 模式：4 路 PWM -> 电机转速 -> 升力/反扭 -> 刚体六自由度；`FastPhysicsEngine` 做碰撞/重力/积分。
- 速度模式：飞控内部闭环到目标速度；官方说明加速度上限 8 m/s²，急停会瞬间归零但有姿态变化。
- 碰撞：`EnableCollisions=true`；RMUA 的 `IgnoreAllHitCollision`/`IgnoreHoverCollision` 是游戏层调试开关，不改变 AirSim 物理开关。
- 风：`AAirSimROSWindZone`/`AirSimROSTransformWind`；`IgnoreWind` 在游戏层把风置零；`/airsim_node/drone_1/debug/wind` 可观测。

## 5. 时间与同步

- UE4 物理帧不是固定 dt；`ROSConfig.FrameTimeTolerance=125` 是桥接层容差。
- 官方建议用 IMU 时间戳作为全局时钟（README Q&A）。
- 若做 RL step，建议：
  - 以 IMU/`pose_gt` 的仿真时间戳计算 dt；
  - 不要假设 1 ROS 消息 = 固定物理步长；
  - 用 `/airsim_node/reset` 重置后重新等待 IMU 时间稳定。

## 6. 未完成/边界

- 没有逐行验证 AirSim 内部积分器常数；这部分是开源 AirSim 的标准实现，且官方参数已给出。
- 没有验证 `moveByMotorPWMs` 的 PWM 归一化范围（0..1 vs 0..1000）；以 `RotorPWM.msg` 和运行时为准。
- 没有验证自定义风场的时间/空间相关模型；`seed` 初始化的 `FRandomStream` 是主要随机源。
- 没有验证碰撞体与门的精确触发半径；门的视觉 scale 已导出，触发逻辑可能在 C++ gate 类/碰撞体。

## 7. 证据

- `artifacts/configs/settings_Game2024_1.json`
- `artifacts/symbols/main.symbols.demangled`
- `artifacts/pseudocode/anchors_pseudo3.c`
- `README.md` 官方参数/说明
