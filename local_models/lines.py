"""Outer control-flow skeleton for outdoor line generation at 0x2fc77b0.

This module deliberately stops at the spline/spawn boundary.  The outer loop,
FRandomStream calls, sign flip, 10000 cm step, 1999-attempt retry limit and the
stage 4/5/6 call parameters are translated exactly.  The helpers implemented at
0x2fc7620 (spline point lookup), 0x2fc4980 (candidate spacing/acceptance),
0x2fc3fc0 (candidate placement), 0x2fc3f70 (component reset) and the component
array at route+0x240 are not translated; a backend must supply them or the
default backend raises NotImplementedError.
"""

from __future__ import annotations

from dataclasses import dataclass

from .ue_rng import FRandomStream, f32

INITIAL_FORWARD_CM = f32(100.0)  # _DAT_006c1b34
INITIAL_REVERSE_CM = f32(-100.0)  # _DAT_006c17b8
SAMPLE_TOLERANCE_CM = f32(2100.0)  # _DAT_006c0f30
STEP_CM = f32(10000.0)  # _DAT_006c110c
MAX_ATTEMPTS = 1999
ZERO = f32(0.0)  # _DAT_006c0fa8


@dataclass(frozen=True)
class LineStageCall:
    """One 0x2fc77b0 call site from 0x2fbac30.

    Fields keep the decompiled argument names.  param_6 controls how many
    candidates use the inequality branch; param_7 the other branch; param_8 is
    forwarded unchanged to 0x2fc3fc0.
    """

    param_1: float
    param_2: float
    param_4: int
    param_5: int
    param_6: int
    param_7: int
    param_8: int


LINE_STAGE_CALLS = {
    4: LineStageCall(f32(120.0), f32(120.0), 0x14, 5, 0x14, 10, 1),
    5: LineStageCall(f32(100.0), f32(120.0), 0x14, 0x0F, 0x18, 0x18, 2),
    6: LineStageCall(f32(80.0), f32(120.0), 0, 0x2D, 0, 0x3C, 3),
}


def _near_int(value: float) -> int:
    """`cvtss2si` with the default nearest-even MXCSR mode."""
    return int(round(f32(value)))


def scaled_count(remaining_cm: float, count: int) -> int:
    """Translate the `cvtss2si`/`sar`/`neg` count adjustment at 0x2fc795e."""
    scaled = f32(f32(f32(remaining_cm) * f32(float(count))) / STEP_CM)
    rounded = _near_int(f32(f32(-0.5) - f32(scaled + scaled)))
    return -((rounded) >> 1)


class LineBackend:
    """Boundary for the unresolved spline and spawn implementation."""

    reverse = False

    def clear_components(self) -> None:
        raise NotImplementedError("0x2fc3f70 component reset is not translated")

    def spline_length(self) -> float:
        raise NotImplementedError("0x052effa0 spline length is not translated")

    def sample_point(self, distance_cm: float, tolerance_cm: float):
        raise NotImplementedError("0x2fc7620 spline point lookup is not translated")

    def place_initial(self, call: LineStageCall, point) -> bool:
        raise NotImplementedError("0x2fc4980 initial candidate placement is not translated")

    def place_candidate(self, point, before_first: bool, param_8: int) -> int:
        raise NotImplementedError("0x2fc3fc0 candidate placement is not translated")

    def report_failure(self, call: LineStageCall) -> None:
        pass


def generate(call: LineStageCall, backend: LineBackend, rng: FRandomStream) -> None:
    """Run the outer loop of 0x2fc77b0 until both spline directions end."""
    backend.clear_components()
    reverse = bool(backend.reverse)
    length = f32(backend.spline_length())
    distance = f32(INITIAL_REVERSE_CM + length) if reverse else INITIAL_FORWARD_CM

    while True:
        if reverse:
            if distance <= ZERO:
                return
        elif f32(backend.spline_length()) <= distance:
            return

        point = backend.sample_point(distance, SAMPLE_TOLERANCE_CM)
        if point is not None and not backend.place_initial(call, point):
            backend.report_failure(call)
            return

        if reverse:
            remaining = distance
        else:
            remaining = f32(backend.spline_length() - distance)

        if remaining < STEP_CM:
            first_count = scaled_count(remaining, call.param_6)
            second_count = scaled_count(remaining, call.param_7)
        else:
            first_count = call.param_6
            second_count = call.param_7
        total = first_count + second_count

        for column in range(total):
            attempts = 0
            result = 0
            while True:
                rng.step()
                offset = rng.line_offset_cm(reverse)
                target = f32(distance + offset)
                length = f32(backend.spline_length())
                clamped = length if length <= target else target
                if target < ZERO:
                    clamped = ZERO
                point = backend.sample_point(clamped, SAMPLE_TOLERANCE_CM)
                result = 0
                if point is not None:
                    result = backend.place_candidate(
                        point, column < first_count, call.param_8
                    )
                attempts += 1
                if MAX_ATTEMPTS <= attempts or (result & 1):
                    break

        distance = f32(distance + (-STEP_CM if reverse else STEP_CM))
