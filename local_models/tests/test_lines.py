import json
import re
import unittest
from pathlib import Path

from local_models.engine import Frame, LineComponentState
from local_models.lines import (
    LINE_STAGE_CALLS,
    LineSpawnSink,
    LineStageCall,
    accept_candidate,
    component_candidate,
    place_component_candidates,
    place_spline_candidate,
    scaled_count,
)
from local_models.ue_rng import FRandomStream, f32, line_offset_from_state

LINE_OFFSET_RE = re.compile(
    r"LINE_OFFSET state=(\d+) base_cm=([-0-9.]+) offset_cm=([-0-9.]+) reverse=([01])"
)


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


class LineOuterFlowTests(unittest.TestCase):
    def test_stage_call_parameters(self):
        expected = {
            4: (120.0, 120.0, 20, 5, 20, 10, 1),
            5: (100.0, 120.0, 20, 15, 24, 24, 2),
            6: (80.0, 120.0, 0, 45, 0, 60, 3),
        }
        for stage, values in expected.items():
            call = LINE_STAGE_CALLS[stage]
            self.assertEqual(
                (
                    call.param_1,
                    call.param_2,
                    call.param_4,
                    call.param_5,
                    call.param_6,
                    call.param_7,
                    call.param_8,
                ),
                values,
            )

    def test_scaled_count_edges(self):
        self.assertEqual(scaled_count(10000.0, 20), 20)
        self.assertEqual(scaled_count(5000.0, 20), 10)
        self.assertEqual(scaled_count(0.0, 20), 0)
        self.assertEqual(scaled_count(10000.0, 0), 0)

    def test_all_captured_line_offsets_reconstruct(self):
        root = repo_root() / "experiments"
        total = 0
        for stage in (4, 5, 6):
            directory = root / f"line_offsets_{stage}_20261105"
            analysis = json.loads((directory / "offset_analysis.json").read_text(encoding="utf-8"))
            for run_name, run in analysis.items():
                log_path = directory / run_name / "gdb.log"
                matches = LINE_OFFSET_RE.findall(log_path.read_text(errors="replace"))
                with self.subTest(stage=stage, run=run_name):
                    self.assertEqual(len(matches), run["samples"])
                    for state_text, base_text, offset_text, reverse_text in matches:
                        state = int(state_text)
                        base = float(base_text)
                        observed = float(offset_text)
                        reverse = bool(int(reverse_text))
                        computed = line_offset_from_state(state, reverse)
                        self.assertAlmostEqual(computed, observed, delta=1e-4)
                        if reverse:
                            self.assertLessEqual(computed, 0.0)
                        else:
                            self.assertGreaterEqual(computed, 0.0)
                        self.assertLess(abs(computed), 8500.0)
                        total += 1
        self.assertEqual(total, 6 * (198 + 628 + 804))


if __name__ == "__main__":
    unittest.main()


class CountingRng:
    def __init__(self, value=0.5):
        self.value = value
        self.calls = 0

    def draw_fraction(self):
        self.calls += 1
        return self.value


class RecordingSink(LineSpawnSink):
    def __init__(self):
        self.calls = []

    def add_instance(self, selector, base, end):
        self.calls.append((selector, base, end))
        return len(self.calls) - 1


IDENTITY_FRAME = Frame(
    location=(0.0, 0.0, 0.0),
    forward=(1.0, 0.0, 0.0),
    right=(0.0, 1.0, 0.0),
    up=(0.0, 0.0, 1.0),
)


class LineCandidateTests(unittest.TestCase):
    def test_rng_draw_counts_by_mode(self):
        expected = {0: 2, 1: 4, 2: 5, 3: 4}
        for mode, count in expected.items():
            rng = CountingRng()
            component_candidate(rng, IDENTITY_FRAME, mode)
            self.assertEqual(rng.calls, count)

    def test_identity_frame_candidate_geometry(self):
        rng = CountingRng(0.5)
        candidate = component_candidate(rng, IDENTITY_FRAME, 0)
        self.assertEqual(candidate.base, (500.0, 0.0, -250.0))
        self.assertEqual(candidate.end, (500.0, 0.0, 0.0))

        rng = CountingRng(0.5)
        candidate = component_candidate(rng, IDENTITY_FRAME, 3)
        self.assertEqual(candidate.base, (500.0, 0.0, -250.0))
        for got, want in zip(candidate.end, (275.0, 0.0, 0.0)):
            self.assertAlmostEqual(got, want, delta=1e-3)

    def test_spacing_is_strictly_less_than_tolerance(self):
        candidate = component_candidate(CountingRng(0.5), IDENTITY_FRAME, 0)
        self.assertTrue(accept_candidate(candidate, [(620.0, 0.0, -250.0)], 120.0))
        self.assertFalse(accept_candidate(candidate, [(619.9, 0.0, -250.0)], 120.0))

    def test_component_placement_counts_and_selectors(self):
        sink = RecordingSink()
        state = LineComponentState(IDENTITY_FRAME)
        call = LINE_STAGE_CALLS[4]
        place_component_candidates(state, call, FRandomStream(123), sink, 120.0)
        self.assertEqual(state.count, call.param_4 + call.param_5)
        self.assertEqual([call[0] for call in sink.calls], [1] * call.param_4 + [0] * call.param_5)

    def test_spline_candidate_submits_once(self):
        sink = RecordingSink()
        state = LineComponentState(IDENTITY_FRAME)
        result = place_spline_candidate(state, FRandomStream(123), 1, sink, 120.0)
        self.assertEqual(result, 0)
        self.assertEqual(len(sink.calls), 1)
        self.assertEqual(state.count, 1)
