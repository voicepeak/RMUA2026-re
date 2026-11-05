import json
import re
import unittest
from pathlib import Path

from local_models.lines import LINE_STAGE_CALLS, scaled_count
from local_models.ue_rng import line_offset_from_state

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
