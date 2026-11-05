# ROS publish hook development

The first probe was compiled on the Ubuntu 22.04 host and failed to load inside the Ubuntu 20.04 container because it required `GLIBC_2.34`; see `experiments/transport_probe_pilot_20261105/`. Build the same source inside the target image and run the forwarding/header toy self-test there:

```sh
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" -w /work --entrypoint bash rmua/re-probe:20261104 -c 'gcc -shared -fPIC -O2 -Wall -Wextra -Werror scripts/ros_publish_interpose.c -ldl -pthread -o experiments/transport_probe_development_20261105/probe_container.so && python3 scripts/test_ros_publish_interpose.py'
```

The binary is intentionally ignored by Git. Runner manifests record the actual library and probe hashes. Hooks are specific to this libairsimros.so symbol/ABI, write same-host monotonic and realtime clocks at function entry, and forward to the original function. The toy test is not proof of target ABI; target proof requires successful exact header-stamp matching and normal message delivery. These probes change scheduling, and they do not time physical acquisition or actuator response.
