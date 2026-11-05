# local_models: offline function-level models (12.0.0.5)

Status: 2026-11-05. Target binary SHA-256
`5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800`.

This directory is a local, function-level dynamic model. It is **not** seed
sweeping, distribution fitting, or a flight controller. Parameters and constants
come from decompiled routines and the local simulator ELF; captured stage 4/5/6
experiments are regression data only.

## Files

| File | Scope |
|---|---|
| `ue_rng.py` | UE4 `FRandomStream`: LCG `state*0x0bb38435 + 0x3619636b` and the exact `(bits>>9)|0x3f800000` float conversion. |
| `glibc_rand.py` | glibc `srand()`/`rand()` TYPE_3 generator; isolated pure-Python streams by default, opt-in process-global ctypes comparison. |
| `wind.py` | Byte-exact port of `0x2fb23b0`, samplers `0x2fb2050`/`0x2fb2130`, and `0x2fbac30` stage parameters; reads ELF constants. |
| `spline.py` | Offline route splines from `artifacts/maps/maps_summary_all.json`: cubic Hermite, arc length, distance lookup, stage route mapping. |
| `engine.py` | Explicit `EngineBoundaryError` interfaces and the engine-supplied `Frame`/line-state types. |
| `lines.py` | `0x2fc77b0` outer loop, `0x2fc4980`/`0x2fc3fc0` RNG/placement math, spacing test, and HISM sink boundary. |
| `vehicles.py` | `0x2fc8900` branch, `0x2fc98c0` distance walk/path dedup, `0x2fbff60`/`0x2fc0180` progress state, `0x2fc9220`/`0x2fdc2f0` descriptors. |
| `tests/` | Byte-exact and structural regression tests. |

Decompiled evidence added this milestone: `artifacts/rea/spline_spawn_decomp.c`
and `artifacts/rea/offline_model_functions.json` (address-to-file manifest).
Existing `line_native.c`, `hover_route.c`, `hover_callers.c`, and
`wind_generation_trace.c` cover the other translated routines.

## Run

From the repository root:

```powershell
python -m unittest discover -s local_models/tests -t . -v
```

Local WSL (ctypes glibc comparison included):

```bash
wsl -d RMUA-RE -- bash -lc "cd '/mnt/d/RMUA2026-re/repo' && python3 -m unittest discover -s local_models/tests -t . -v"
```

Linux with a separately installed simulator (point to the executable, not its
directory):

```bash
RMUA_ELF=/path/to/RMUA-Linux-Shipping python3 -m unittest discover -s local_models/tests -t . -v
```

Generate wind keyframes:

```powershell
python -m local_models.wind --stage 4 --seed 123
python -m local_models.wind --stage 6 --seed 2026 --json
python -m local_models.wind --all
```

Override the ELF with `--elf PATH` or `RMUA_ELF`; override the map summary with
`RMUA_MAPS_SUMMARY`.

`make_glibc_rand(..., backend="auto")` creates an independent pure stream.
The explicit `backend="ctypes"` requires a glibc host and accesses libc's process-global state: another
`srand`/`rand` user can change its sequence, so it is for isolated comparison,
not parallel episode streams.  High-bit 32-bit seeds are converted to signed
initialization words and use C's truncation-toward-zero division.

## Regression results

Wind keyframes: all **18** captured fresh runs match byte-for-byte (stage 4:
6 x 35, stage 5: 6 x 37, stage 6: 6 x 35). Tests compare every 16-byte
keyframe, per-run SHA-256, and decoded time/speed/direction/transition fields.

Line offsets: all **9,780** captured `LINE_OFFSET` state/offset pairs across
stages 4/5/6 reconstruct within text precision.

Spline route mapping/length: stage 4/5/6 use route 12/10/7 (`2_2`, `4_2`,
`1_2`). The offline length minus 100 cm matches the captured reverse line base
within **0.1 cm** for all three stages:

| Stage | Route | Offline base | Captured base | Difference |
|---|---|---|---:|---:|
| 4 | 2_2 | 65782.35 cm | 65782.34 cm | ~0.004 cm |
| 5 | 4_2 | 64831.74 cm | 64831.65 cm | ~0.088 cm |
| 6 | 1_2 | 69056.50 cm | 69056.58 cm | ~0.074 cm |

Vehicle samples: the `0x2fc98c0` distance walk `10000, 7900, 5800, 3700, 1600`
matches the five captured vehicle path points for the direct stage-4 fixture;
branch buckets match all six `vehicle_branch_4` captures.

## Exactness matrix

| Area | Status | Evidence / remaining limit |
|---|---|---|
| UE LCG state/float conversion | **exact** | `0x2fc77b0`, `0x2fc4980`, `0x2fc8900`; line offsets and vehicle buckets. |
| glibc `rand()` | **exact** | TYPE_3 pure fallback == host glibc ctypes; captured glibc 2.35 keyframes. |
| Wind keyframes | **exact** | 18/18 fresh captures; scope excludes prior `rand()` consumers and reset. |
| Wind sampler | structurally exact | Interpolation/RNG order translated; trig is host libm rounded to float32, not glibc `sincosf`. |
| Stage route selection and length | **near-exact** | Route mapping and lengths within 0.1 cm; UE `ReparamTable` sampling is approximated by a dense arc-length table. |
| Spline point/tangent lookup | structurally modeled | Cubic Hermite from serialized points; `USplineComponent` eval and `0x2fc6590` reparam behavior are engine details. |
| Line offset/step/retry | **exact** | `1500+7000u`, sign flip, 10000 cm step, 1999 retries, 9,780 captures. |
| Line candidate jitter/orientation | structurally modeled | `0x2fc4980`/`0x2fc3fc0` RNG order and constants; needs component basis vectors and transform. |
| Line HISM submission | blocked-on-engine | `0x2fc3db0`/`0x054775d0`: HISM component and AddInstance transform. |
| Vehicle bucket/branch/dispatch | **exact** | `0x2fc8900`, `0x2fc8bb0`, captured states 997704449/2925953427/276274103. |
| Vehicle path distance walk/dedup | structurally exact | `0x2fc98c0` step +/-2100 cm and component-change dedup; five-point fixture. |
| Vehicle component array/transform | needs-runtime | route+0x240 component list, quaternions, translation/scale at +0x1c0/+0x1d0. |
| Vehicle attach/indirect spawn | blocked-on-engine | `0x2fc3270`, `0x2fc9220`, `0x2fc9f30`: actor allocation and component attach. |
| Vehicle tick progress | structurally exact | `0x2fc0180` adds `dt*speed`; spline length and movement target from UE are boundaries. |
| Collision/event outcomes | blocked-on-engine | `0x2fb7110` and PhysX contact data are not in the offline model. |

## Engine boundaries

`engine.py` raises `EngineBoundaryError(function, required_input)` for:

- `0x2fc7620` / `0x052f01a0` / `0x052f1190` / `0x2fc6590`: live spline
  component location, world-location update and reparam table.
- `0x2fc4980` / `0x2fc3fc0`: component forward/right/up vectors and transforms
  for route+0x240 elements.
- `0x2fc3db0` / `0x054775d0`: HISM thick/thin component and AddInstance.
- `0x2fc98c0` / `0x2fc96b0`: route component array, quaternion transform and
  initial component vector.
- `0x2fc3270` / `0x2fc9220` / `0x2fc9f30`: UE actor allocation, attach and
  descriptor copy.
- `0x2fbff60` / `0x2fc0180`: spline rebuild, `GetSplineLength`,
  `GetLocationAtDistance`, `GetTangentAtDistance`, movement component update.
- `0x2fdc2f0`: the four engine accessors and +0x20 counter.

No inner spline or spawn behavior is fabricated; the offline spline is the only
structural substitute, and it is validated against captured route lengths.
