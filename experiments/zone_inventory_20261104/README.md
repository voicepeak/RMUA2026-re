# Runtime OutTime object inventory

A single seed 123 raised-route VEL diagnostic was repeated in OL1 using GPS+IMU and LiDAR, with unmodified simulator/rules and a read-only all-stop GDB breakpoint on the `0x2fb9830` predicate. On predicate changes the probe read raw object pointers, `UObject` FName fields, class pointer, actor root location and the specific compared fields. It also captured state/collision stack events, wind and pose clocks. The process hit state 12 and was stopped; no stage 4/5/6 was reached. Debugger inspection was observational and was not fed to the controller. Timing can be perturbed by breakpoints.

The first snapshot was matched against serialized OL1 root locations by `scripts/analyze_zone_inventory.py`, preserving all matches within 0.1 cm instead of guessing ties. Results: `array30` has 12 objects, eight uniquely matching a static route-spline root and four ambiguous due to identical serialized positions; index 0 uniquely matches spline `1_1`, exact root [706.734375,-5.68359375,8278.6328125] cm. `array40` has 12 unique matches to static `*_Port` root locations and `array60` has 12 unique matches to `*_Factory` roots. `array70` has 24 unmatched objects because the static comparison input does not include their locations; matching class indices to array30 suggests a shared runtime class, not a decoded name or confirmed type. FName fields are not decoded and class identity is not established by position alone.

During the raised-run transition events, predicate=0 snapshots had no passing entries among the four inspected arrays; in intermittent predicate=1 snapshots the exact `array30` index-0 object had signed `+0x288=2`, `+0x2b4=1`. Its runtime pointer and root position coincide with the initial inventory. The comparison is signed (`setg` in `0x2fc8d70`); the source of those integer values and detailed overlap/collision semantics remain unverified. A weak-reference path also exists in the predicate, so the array snapshot alone is not a complete behavioral reconstruction. This supports the hypothesis that leaving the relevant spline-related volume contributed to this specific run's cumulative OutTime; it does not prove the full allowed geometry or any result for other seeds.

`gdb.log`, `inventory.json`, raw GPS/IMU bridge output and wind, GDB wall/monotonic timestamps, executable/settings hashes, and source snapshots/hashes are retained. GPS arrival and GDB observation clocks are distinct from the ROS header source stamp and simulated time.

```bash
python3 scripts/export_beta_static_route.py --route candidate_1_3 --connector-lift-m 8 --output /tmp/rmua-zone/route
python3 scripts/run_beta_sensor_scout.py --output /tmp/rmua-zone --seconds 150 --speed 6 --inventory-trace
python3 scripts/analyze_zone_inventory.py /tmp/rmua-zone/gdb.log --output /tmp/rmua-zone/inventory.json
```
