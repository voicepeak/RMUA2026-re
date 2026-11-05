import json
import unittest
from pathlib import Path

from local_models import spline


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


class SplineModelTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.routes = spline.load_routes()
        cls.root = repo_root()

    def test_stage_route_mapping(self):
        expected = {4: (12, "2_2"), 5: (10, "4_2"), 6: (7, "1_2")}
        for stage, (route_index, name) in expected.items():
            self.assertEqual(spline.STAGE_ROUTE_INDEX[stage], route_index)
            self.assertEqual(spline.stage_spline(stage).actor, name)

    def test_stage_lengths_match_captured_reverse_bases(self):
        captured = {4: 65782.34375, 5: 64831.64844, 6: 69056.57812}
        for stage, base_cm in captured.items():
            with self.subTest(stage=stage):
                length = spline.stage_spline(stage).length_cm
                self.assertAlmostEqual(length - 100.0, base_cm, delta=0.1)

    def test_distance_endpoints_clamp(self):
        route = spline.stage_spline(4)
        first = route.world_at_key(route.points[0].in_val)
        last = route.world_at_key(route.points[-1].in_val)
        self.assertEqual(route.world_at_distance(-100.0), first)
        self.assertEqual(route.world_at_distance(route.length_cm + 100.0), last)
        self.assertEqual(route.key_at_distance(-1.0), route.points[0].in_val)
        self.assertEqual(route.key_at_distance(route.length_cm + 1.0), route.points[-1].in_val)

    def test_arc_length_is_monotonic(self):
        route = spline.stage_spline(5)
        previous = -1.0
        for step in range(11):
            distance = route.length_cm * step / 10.0
            self.assertGreaterEqual(distance, previous)
            previous = distance
        keys = [route.key_at_distance(route.length_cm * step / 10.0) for step in range(11)]
        self.assertEqual(keys, sorted(keys))

    def test_decompiled_artifacts_and_manifest_exist(self):
        rea = self.root / "artifacts" / "rea"
        manifest = json.loads((rea / "offline_model_functions.json").read_text(encoding="utf-8"))
        self.assertIn("0x2fc7620", manifest["functions"])
        self.assertTrue((rea / "spline_spawn_decomp.c").is_file())


if __name__ == "__main__":
    unittest.main()
