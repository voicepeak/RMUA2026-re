"""Outer control-flow skeleton for vehicle selection at 0x2fc8900.

Translated exactly: the initial +-9000 cm spline lookup distance, the 2100 cm
tolerance, the FRandomStream step, the float fraction times 11.0f bucket, the
`bucket < 6` branch, and the direct/indirect call argument construction.

Not translated: 0x2fc7620 (spline lookup), 0x2fc3270 (component attach),
0x2fc93c0 (direct spawn), 0x2fc9220 (indirect spawn / vector-field path) and
the later actor/event plumbing.  Those remain documented gaps and the default
backend raises NotImplementedError.
"""

from __future__ import annotations

from dataclasses import dataclass

from .ue_rng import FRandomStream, f32

LOOKUP_HALF_SPAN_CM = f32(9000.0)  # _DAT_006c1864 / _DAT_006c12fc
LOOKUP_TOLERANCE_CM = f32(2100.0)  # _DAT_006c0f30
ENTRY_SHIFT_CM = f32(500.0)  # _DAT_006c2d28
SPEED_SCALE = f32(100.0)  # _DAT_006c1b34
DIRECT_BUCKET_LIMIT = 6


@dataclass(frozen=True)
class VehicleBranch:
    state: int
    fraction: float
    bucket: int
    direct: bool


def decide(rng: FRandomStream) -> VehicleBranch:
    """0x2fc8900: feed the route decision from one FRandomStream draw."""
    state = rng.step()
    fraction = rng.fraction()
    bucket = int(f32(fraction * f32(11.0)))
    return VehicleBranch(state=state, fraction=fraction, bucket=bucket, direct=bucket < DIRECT_BUCKET_LIMIT)


def entry_arguments(distance_cm: float, forward: bool) -> tuple[float, float]:
    """Return (start, end) used by both spawn branches."""
    shift = f32(-ENTRY_SHIFT_CM) if forward else ENTRY_SHIFT_CM
    span = LOOKUP_HALF_SPAN_CM if forward else f32(-LOOKUP_HALF_SPAN_CM)
    return f32(distance_cm + shift), f32(distance_cm + span)


class VehicleBackend:
    """Boundary for the unresolved route and spawn implementation."""

    reverse = False
    speed_offset_26c = f32(0.0)
    speed_offset_270 = f32(0.0)

    def sample_point(self, route, distance_cm: float, tolerance_cm: float):
        raise NotImplementedError("0x2fc7620 spline point lookup is not translated")

    def attach_initial_component(self, route, point) -> None:
        raise NotImplementedError("0x2fc3270 component attach is not translated")

    def spawn_direct(
        self,
        route,
        start_cm: float,
        end_cm: float,
        tolerance_cm: float,
        speed_cm_s: float,
    ) -> None:
        raise NotImplementedError("0x2fc93c0 direct vehicle spawn is not translated")

    def spawn_indirect(
        self,
        route,
        start_cm: float,
        end_cm: float,
        tolerance_cm: float,
        speed_cm_s: float,
    ) -> None:
        raise NotImplementedError("0x2fc9220 indirect vehicle spawn is not translated")


def run(
    route,
    distance_cm: float,
    rng: FRandomStream,
    backend: VehicleBackend,
) -> VehicleBranch:
    """Translate the outer flow of 0x2fc8900 for one route position."""
    forward = not bool(backend.reverse)
    lookup = f32(
        distance_cm + (LOOKUP_HALF_SPAN_CM if forward else f32(-LOOKUP_HALF_SPAN_CM))
    )
    point = backend.sample_point(route, lookup, LOOKUP_TOLERANCE_CM)
    if point is not None:
        backend.attach_initial_component(route, point)

    branch = decide(rng)
    start, end = entry_arguments(distance_cm, forward)
    if branch.direct:
        speed = f32(backend.speed_offset_270 * SPEED_SCALE)
        backend.spawn_direct(route, start, end, LOOKUP_TOLERANCE_CM, speed)
    else:
        speed = f32(backend.speed_offset_26c * SPEED_SCALE)
        backend.spawn_indirect(route, start, end, LOOKUP_TOLERANCE_CM, speed)
    return branch
