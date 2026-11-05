import json
import re
import unittest
from pathlib import Path

from local_models import vehicles
from local_models.engine import EngineBoundaryError, Frame, distance
from local_models.ue_rng import FRandomStream, vehicle_bucket_from_state

DECISION_RE = re.compile(r"VEHICLE_DECISION state=(\d+) bucket=(\d+) direct=([01])")


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


class VehicleOuterFlowTests(unittest.TestCase):
    def test_distance_rounds_intermediate_float32_operations(self):
        # Independent C fixture and target SSE instruction excerpt are retained
        # in experiments/offline_distance_20261106/ and artifacts/rea/.
        actual = distance((29.600000381469727, 34.5, 23.100000381469727), (0, 0, 0))
        self.assertEqual(actual, float.fromhex('0x1.97ec54p+5'))

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


class FakeSpline:
    def __init__(self, points):
        self.points = points
        self.index = 0

    def world_at_distance(self, distance):
        point = self.points[self.index]
        self.index += 1
        return point


class FakeRoute:
    def __init__(self, spline):
        self.spline = spline


class FakeProvider(vehicles.RouteComponentProvider):
    def __init__(self, components):
        self.components_list = components

    def components(self, route):
        return self.components_list

    def initial_component_point(self, route):
        return (0.0, 0.0, 0.0)

    def transform_component_point(self, component, local):
        return component.frame.location


FRAME = Frame(
    location=(0.0, 0.0, 0.0),
    forward=(1.0, 0.0, 0.0),
    right=(0.0, 1.0, 0.0),
    up=(0.0, 0.0, 1.0),
)


class VehiclePathTests(unittest.TestCase):
    def test_path_sample_distances_forward_and_reverse(self):
        self.assertEqual(
            vehicles.path_sample_distances(10000.0, 500.0),
            [10000.0, 7900.0, 5800.0, 3700.0, 1600.0],
        )
        self.assertEqual(
            vehicles.path_sample_distances(500.0, 10000.0),
            [500.0, 2600.0, 4700.0, 6800.0, 8900.0],
        )

    def test_nearest_component_strict_tie_keeps_first(self):
        first = vehicles.RouteComponent("first", Frame((0.0, 0.0, 0.0), FRAME.forward, FRAME.right, FRAME.up))
        second = vehicles.RouteComponent("second", Frame((0.0, 0.0, 0.0), FRAME.forward, FRAME.right, FRAME.up))
        self.assertIs(vehicles.nearest_component((0.0, 0.0, 0.0), [first, second]), first)

    def test_build_path_dedups_by_component_identity(self):
        locations = [
            (10000.0, 0.0, 0.0),
            (7900.0, 0.0, 0.0),
            (5800.0, 0.0, 0.0),
            (3700.0, 0.0, 0.0),
            (1600.0, 0.0, 0.0),
        ]
        components = [
            vehicles.RouteComponent(index, Frame(location, FRAME.forward, FRAME.right, FRAME.up))
            for index, location in enumerate(locations)
        ]
        route = FakeRoute(FakeSpline(list(locations)))
        path = vehicles.build_path(route, 10000.0, 500.0, FakeProvider(components))
        self.assertEqual(path, locations)

    def test_tick_progress_and_boundary(self):
        state = vehicles.PathState(0.0)
        state.begin(667.0, [(0.0, 0.0, 0.0)])
        state.spline_length_cm = 100.0
        self.assertTrue(state.tick(0.05))
        self.assertAlmostEqual(state.progress_cm, 33.35, delta=1e-4)
        while state.progress_cm < 100.0:
            self.assertTrue(state.tick(0.05))
        self.assertFalse(state.tick(0.05))

    def test_tick_requires_spline_length(self):
        state = vehicles.PathState(667.0)
        state.begin(667.0, [])
        with self.assertRaises(EngineBoundaryError):
            state.tick(0.05)


class FakeFourScalarSource(vehicles.FourScalarSource):
    def __init__(self, values):
        self.values = values
        self.advanced = 0

    def read_four(self):
        return self.values

    def advance(self):
        self.advanced += 1


class VehicleDescriptorTests(unittest.TestCase):
    def test_indirect_descriptor_fields(self):
        descriptor = vehicles.indirect_descriptor(1.0, 2.0, 3.0, 4.0, "owner", "path")
        self.assertEqual(descriptor.vtable, 0xCD88C8)
        self.assertEqual(
            (descriptor.param_1, descriptor.param_2, descriptor.param_3, descriptor.param_4),
            (1.0, 2.0, 3.0, 4.0),
        )
        self.assertEqual(descriptor.owner, "owner")
        self.assertEqual(descriptor.path_source, "path")

    def test_four_scalar_driver_reads_advances_and_builds_call(self):
        source = FakeFourScalarSource((10000.0, 500.0, 2100.0, 667.0))
        result = vehicles.four_scalar_driver(source, "route")
        self.assertEqual(source.advanced, 1)
        self.assertEqual(result["route"], "route")
        self.assertEqual(result["function"], "0x2fc93c0")
        self.assertEqual(
            result["call"],
            {
                "end_distance_cm": 10000.0,
                "start_distance_cm": 500.0,
                "tolerance_cm": 2100.0,
                "speed_cm_s": 667.0,
            },
        )
