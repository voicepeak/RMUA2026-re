import json
import re
import unittest
from pathlib import Path

from local_models import vehicles
from local_models.ue_rng import FRandomStream, vehicle_bucket_from_state

DECISION_RE = re.compile(r"VEHICLE_DECISION state=(\d+) bucket=(\d+) direct=([01])")


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


class VehicleOuterFlowTests(unittest.TestCase):
    def test_branch_decisions_from_captured_runs(self):
        directory = repo_root() / "experiments" / "vehicle_branch_4_20261105"
        total = 0
        for log_path in sorted(directory.glob("seed*_repeat*/gdb.log")):
            matches = DECISION_RE.findall(log_path.read_text(errors="replace"))
            with self.subTest(run=log_path.parent.name):
                self.assertTrue(matches)
                for state_text, bucket_text, direct_text in matches:
                    state = int(state_text)
                    bucket = vehicle_bucket_from_state(state)
                    self.assertEqual(bucket, int(bucket_text))
                    self.assertEqual(bucket < 6, bool(int(direct_text)))
                    total += 1
        self.assertGreaterEqual(total, 6)

    def test_entry_arguments_forward_and_reverse(self):
        self.assertEqual(
            vehicles.entry_arguments(1000.0, True),
            (vehicles.f32(500.0), vehicles.f32(10000.0)),
        )
        self.assertEqual(
            vehicles.entry_arguments(1000.0, False),
            (vehicles.f32(1500.0), vehicles.f32(-8000.0)),
        )

    def test_run_dispatches_to_the_expected_branch(self):
        class Backend(vehicles.VehicleBackend):
            reverse = False
            speed_offset_26c = vehicles.f32(6.0)
            speed_offset_270 = vehicles.f32(7.0)

            def __init__(self):
                self.direct_call = None
                self.indirect_call = None

            def sample_point(self, route, distance_cm, tolerance_cm):
                return None

            def spawn_direct(self, route, start_cm, end_cm, tolerance_cm, speed_cm_s):
                self.direct_call = (start_cm, end_cm, tolerance_cm, speed_cm_s)

            def spawn_indirect(self, route, start_cm, end_cm, tolerance_cm, speed_cm_s):
                self.indirect_call = (start_cm, end_cm, tolerance_cm, speed_cm_s)

        for seed in (1, 2, 3, 4, 5, 6, 7, 8):
            stream = FRandomStream(seed)
            expected = vehicles.decide(FRandomStream(seed))
            backend = Backend()
            actual = vehicles.run(None, 1000.0, stream, backend)
            self.assertEqual(actual, expected)
            if expected.direct:
                self.assertIsNotNone(backend.direct_call)
                self.assertIsNone(backend.indirect_call)
                self.assertEqual(backend.direct_call[:3], (500.0, 10000.0, 2100.0))
                self.assertEqual(backend.direct_call[3], 700.0)
            else:
                self.assertIsNone(backend.direct_call)
                self.assertIsNotNone(backend.indirect_call)
                self.assertEqual(backend.indirect_call[:3], (500.0, 10000.0, 2100.0))
                self.assertEqual(backend.indirect_call[3], 600.0)


if __name__ == "__main__":
    unittest.main()
