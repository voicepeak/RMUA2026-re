# Same-object deque method interval probe

This development-only LD_PRELOAD tracks calls to the target library's dynamically bound `std::deque<unsigned long>` `push_back`, `pop_front` and `size` methods when nested inside any of the three interposed AirSimROS frequency functions. It uses the exact process-local deque address and atomic counters, excludes same-thread nesting, and does not lock the target. It records a monotonic timestamp and current Linux thread ID on each sampled/overlap record. Addresses identify objects only within a process; this version does not identify topic names or the other thread ID.

The method wrappers extend the measured interval, perturb scheduling, and may miss short overlaps. Even a positive wrapper overlap does not prove concurrent execution of specific underlying memory instructions. Getter iterator dereferences, inlined operations and other call paths are not covered. Periodic counters are lower bounds and `untracked` records slot exhaustion. The pool runner's `--frequency-guard` parameter loads this non-serializing probe; it is not a fix.

Build and test against a synthetic dynamically linked fixture in the same container:

```sh
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" -w /work --entrypoint bash rmua/re-probe:20261104 -c 'g++ -shared -fPIC -O2 -Wall -Wextra -Werror scripts/frequency_deque_overlap.cpp -ldl -pthread -o experiments/frequency_deque_probe_20261106/probe.so && python3 scripts/test_frequency_deque_overlap.py'
```

The generated `probe.so` is ignored by Git. Manifests record its SHA-256, source, target library, image, binary and settings; the fake test checks same-object read/write and write/write, a distinct-object control, forwarding and return values. The real simulator pilot validates that the three selected wrappers are actually entered, not complete coverage of the frequency data structure.
