# Frequency call overlap interposer

Development-only LD_PRELOAD wraps the three exported frequency functions and forwards to the actual implementation without taking a mutex. It uses C++ atomics for 128 slots indexed by a 64-bit FNV-1a topic hash and thread-local nesting counts. It records a possible cross-thread conflict when a writer (`record_frequency_data`) and another writer or reader (`get_topic_frequency[_one_sec]`) with the same hash have overlapping wrapper lifetimes. Multiple readers alone and same-thread nesting are not classified as conflicts. A hash collision may be misclassified; no raw topic names are logged. Slot exhaustion increments `untracked`. Counter samples are periodic and the final tail may be unreported on process termination. The interposer changes scheduling and can miss overlaps during short sampling races. A positive means wrapper intervals intersect, not necessarily that underlying deque operations overlap or that they lack higher-level synchronization. Direct non-exported accesses are not covered.

Build and check against synthetic libraries in the same container ABI:

```sh
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/work" -w /work --entrypoint bash rmua/re-probe:20261104 -c 'g++ -shared -fPIC -O2 -Wall -Wextra -Werror scripts/frequency_overlap_interposer.cpp -ldl -pthread -o experiments/frequency_overlap_interposer_20261106/probe.so && python3 scripts/test_frequency_overlap_interposer.py'
```

`probe.so` is generated locally and ignored by Git; the pilot manifest records its hash and the simulator, target DSO, source, runner, settings and image identities. The synthetic test verifies forwarding, float return values, simultaneous same-key writer/reader classification and the distinct-key control; it cannot validate the target simulator or prove a crash mechanism.
