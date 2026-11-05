# Frequency statistics guard: development probe

This diagnostic interposer serializes only three exported `libairsimros.so`
frequency functions, identified by symbol and checked library SHA-256. It
preserves C++ exceptions using RAII. It does not protect unknown direct map
accesses, establish native data races, or constitute an official patch. The
mutex and logging alter scheduling. The counter reports try-lock contention in
the interposed run, not an unmodified-run race.

Build and test in the same Ubuntu 20.04 image used by the simulator:

```sh
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" -w /work --entrypoint bash rmua/re-probe:20261104 -c 'g++ -shared -fPIC -O2 -Wall -Wextra -Werror scripts/frequency_guard.cpp -ldl -pthread -o experiments/frequency_guard_development_20261106/guard.so && python3 scripts/test_frequency_guard.py'
```

The generated shared library is ignored by Git; `metadata.json` and run
manifests record its SHA-256. The fake-library test checks return values,
exception unlock, forwarding and serialization; it does not validate the real
simulator ABI or crash cause. A single simulator pilot and its input/output
records are in `experiments/frequency_guard_single_pilot_20261106/`.
