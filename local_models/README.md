# local_models: offline function-level models (12.0.0.5)

Status: 2026-11-05. Target binary SHA-256
`5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`.

This directory starts a local, function-level dynamic model. It is **not**
seed sweeping, distribution fitting, or a flight controller. Parameters and
constants come from the decompiled routines and the local simulator ELF; the
captured stage 4/5/6 experiments are used only as regression tests.

## Files

| File | Scope |
|---|---|
| `ue_rng.py` | UE4 `FRandomStream`: LCG `state = state*0x0bb38435 + 0x3619636b (mod 2^32)` and the exact `(bits>>9)|0x3f800000` float conversion used by line/vehicle code. |
| `glibc_rand.py` | glibc `srand()`/`rand()` TYPE_3 generator. Uses ctypes on Linux/glibc; otherwise a documented pure-Python fallback. |
| `wind.py` | Port of generator `0x2fb23b0`, samplers `0x2fb2050`/`0x2fb2130`, and stage parameters from `0x2fbac30`. Reads constants from the ELF. |
| `lines.py` | Outer control flow of `0x2fc77b0` with exact constants/LCG calls; inner spline/spawn helpers are explicit gaps. |
| `vehicles.py` | Outer control flow of `0x2fc8900` including the `bucket < 6` branch; spawn helpers are explicit gaps. |
| `tests/` | Regression tests against captured runtime data. |

## Run

From the repository root (`D:\RMUA2026-re\repo`):

```powershell
python -m unittest discover -s local_models/tests -t . -v
```

The same suite under local WSL Python (the local ELF path is auto-detected):

```bash
wsl -d RMUA-RE -- bash -lc "cd '/mnt/d/RMUA2026-re/repo' && python3 -m unittest discover -s local_models/tests -t . -v"
```

Generate keyframes:

```powershell
python -m local_models.wind --stage 4 --seed 123
python -m local_models.wind --stage 6 --seed 2026 --json
python -m local_models.wind --all
```

Override the binary path with `--elf PATH` or the `RMUA_ELF` environment
variable. The default Windows path is
`D:\RMUA2026-re\sim\simulator_12.0.0.5\Build\LinuxNoEditor\RMUA\Binaries\Linux\RMUA-Linux-Shipping`.

## Regression result

All **18** captured fresh stage-injection runs match byte-for-byte:

| Stage | Captured runs | Keyframes per run | Result |
|---|---:|---:|---|
| 4 | 6 (seeds 123/456/2026 x2) | 35 | exact |
| 5 | 6 | 37 | exact |
| 6 | 6 | 35 | exact |

The tests compare every 16-byte keyframe, both per-run SHA-256 values, and the
decoded time/speed/direction/transition fields in
`experiments/stage_inject_{4,5,6}_20261104/analysis.json`. No field is missing
in those captures. Same-seed repeats are identical; different seeds differ.

Additional checks:

- `ue_rng.py`: line offsets from all 9,780 captured `LINE_OFFSET` samples
  (`experiments/line_offsets_{4,5,6}_20261105/`) and vehicle buckets from
  `experiments/vehicle_branch_4_20261105/` are reproduced.
- `glibc_rand.py`: the pure fallback matches the host glibc ctypes sequence
  (verified under WSL glibc 2.39) and reproduces the captured glibc 2.35
  keyframes.
- Sampler: the first ROS wind vector of all 18 `wind_persistent_*` runs matches
  within `1e-6`; sampler trigonometry uses the host libm rounded to float32 and
  is not claimed byte-identical to glibc `sincosf`.

## Evidence sources

- `artifacts/rea/wind_generation_trace.c`: `0x2fb2040`, `0x2fb23b0`,
  `0x2fb2050`, `0x2fb2130`.
- `artifacts/rea/followup_callers.c`: `0x2fb8f40`, `0x2fbac30`, `0x2fc77b0`.
- `artifacts/rea/line_callers.c`: `0x2fc4980`, `0x2fc77b0`, `0x2fc7d70`.
- `artifacts/rea/hover_callers.c`: `0x2fc8900`, `0x2fdc2f0`;
  `artifacts/rea/hover_route.c`: `0x2fc93c0` and descendants.
- `artifacts/rea/followup_state.c`: state calls into `0x2fbac30` and
  `srand`/reset sequencing at `0x2fba970`.
- `docs/15_seed_dependency_matrix.md`, `docs/17_runtime_validation_20261104.md`,
  `docs/18_acceptance_ledger.md`.
- ELF constants read at `0x6c1440` (11 s), `0x6c163c` (16777215),
  `0x6c0d20` (8), `0x6c14d4` (2), `0x6c0fdc` (-0.01), `0x6c1a40` (2.4),
  `0x6c0c84` (-1.2), `0x6c0e50` (360), and the stage targets
  `0x6c0924`/`0x6c17bc`/`0x6c1860`.
- `0x2fbac30` stage parameters: 4 = `(200, 350, 2, 1.5)`,
  5 = `(220, 660, 4, 2)`, 6 = `(200, 1800, 15, 4)`. The first value is the
  default `GameConfig` limit at `+0x2bc/+0x2c0/+0x2c4` (docs/03), not an ELF
  float.

## Exactness status

- **UE LCG / float conversion**: exact for the implemented operations.
- **glibc rand**: exact TYPE_3 sequence; pure fallback verified against glibc.
- **Wind keyframes**: exact for all 18 fresh captures. Scope is fresh
  `srand(seed)` with no earlier libc `rand()` consumer. It does not cover reset
  or any hidden pre-consumption.
- **Wind sampler**: control flow and interpolation translated; trig differs at
  most by float rounding. The 25 s frozen/transition behavior observed in some
  injected runs is outside this model.
- **Lines**: outer loop, 10000 cm step, ±100 cm initial position, 1999 retries,
  `1500 + 7000u` cm offset, sign flip, and stage call parameters are
  translated. Actual spline geometry, spacing-array acceptance, spawn success,
  asset choice and collision are not.
- **Vehicles**: `±9000` cm lookup, `2100` cm tolerance, `(1000u converted) * 11`
  bucket, `bucket < 6` branch, and direct/indirect call argument construction
  are translated. The spawn implementations are not.

## Unresolved functions / blockers

- `0x2fc7620`: spline point lookup with distance repair (used by lines and
  vehicles). Needs spline component layout and tolerance semantics.
- `0x2fc4980`: line candidate spacing array and acceptance logic
  (`+0x250` array, `+0x260` point array, 0x2fc3db0 submission).
- `0x2fc3fc0`, `0x2fc3db0`, `0x2fc3f70`: line placement, component transform
  submission, and reset.
- `0x2fc93c0`: direct vehicle spawn; includes nearest-component search and
  `0x2fc98c0` / `0x2fbff60` path construction.
- `0x2fc9220`: indirect vehicle spawn / vector-field path.
- `0x2fc3270`, `0x2fdc2f0`: vehicle component attach and the four-scalar driver.
- `0x2fbff60` and later actor/tick/event plumbing (`0x5909b10`, `0x31d6030`).

These remain explicit `NotImplementedError` boundaries in `lines.py` and
`vehicles.py`; no inner spline or spawn behavior is fabricated.
