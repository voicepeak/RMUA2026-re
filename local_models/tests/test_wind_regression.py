import hashlib
import json
import re
import unittest
from pathlib import Path

from local_models import wind


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


def seed_from_run(run_name: str) -> int:
    match = re.search(r"seed(\d+)_repeat", run_name)
    if not match:
        raise ValueError(f"cannot parse seed from {run_name!r}")
    return int(match.group(1))


class WindKeyframeRegressionTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.root = repo_root()
        cls.constants = wind.load_constants()
        cls.analyses = {}
        for stage in (4, 5, 6):
            path = cls.root / "experiments" / f"stage_inject_{stage}_20261104" / "analysis.json"
            cls.analyses[stage] = json.loads(path.read_text(encoding="utf-8"))

    def test_stage_parameters_from_call_sites(self):
        expected = {
            4: (200.0, 350.0, 2.0, 1.5),
            5: (220.0, 660.0, 4.0, 2.0),
            6: (200.0, 1800.0, 15.0, 4.0),
        }
        for stage, values in expected.items():
            self.assertEqual(
                wind.stage_parameters(stage, self.constants).as_tuple,
                values,
            )

    def test_all_18_captured_runs_match_byte_for_byte(self):
        total = 0
        for stage, analysis in self.analyses.items():
            parameters = wind.stage_parameters(stage, self.constants)
            for run_name, run in analysis["runs"].items():
                with self.subTest(stage=stage, run=run_name):
                    self.assertEqual(run["status"], "observed")
                    required = {"index", "time_s", "speed", "direction_deg", "transition", "raw_hex"}
                    observed_raw = []
                    for position, item in enumerate(run["wind_items"]):
                        missing = required - set(item)
                        if missing:
                            self.fail(
                                f"{run_name} item {position} is missing fields "
                                f"{sorted(missing)}; comparison not loosened"
                            )
                        self.assertEqual(item["index"], position)
                        raw = bytes.fromhex(item["raw_hex"])
                        self.assertEqual(len(raw), 16)
                        frame = wind.Keyframe.unpack(raw)
                        self.assertEqual(frame.time, item["time_s"])
                        self.assertEqual(frame.speed, item["speed"])
                        self.assertEqual(frame.direction, item["direction_deg"])
                        self.assertEqual(frame.transition, item["transition"])
                        observed_raw.append(raw)

                    seed = seed_from_run(run_name)
                    generated = wind.generate_keyframes(
                        parameters, seed=seed, constants=self.constants
                    )
                    generated_raw = [frame.pack() for frame in generated]
                    self.assertEqual(generated_raw, observed_raw)
                    self.assertEqual(
                        hashlib.sha256(b"".join(generated_raw)).hexdigest(),
                        run["wind_bytes_sha256"],
                    )
                    self.assertEqual(
                        hashlib.sha256(b"".join(observed_raw)).hexdigest(),
                        run["wind_bytes_sha256"],
                    )
                    total += 1
        self.assertEqual(total, 18)

    def test_same_seed_repeats_are_identical_and_seeds_differ(self):
        for stage, analysis in self.analyses.items():
            per_seed = {}
            for run_name, run in analysis["runs"].items():
                seed = seed_from_run(run_name)
                raw = [item["raw_hex"] for item in run["wind_items"]]
                per_seed.setdefault(seed, []).append(raw)
            for seed, repeats in per_seed.items():
                self.assertEqual(len(repeats), 2)
                self.assertEqual(repeats[0], repeats[1])
            unique = {tuple(values[0]) for values in per_seed.values()}
            self.assertEqual(len(unique), len(per_seed))

    def test_sampler_first_vector_matches_captured_ros_wind(self):
        for stage in (4, 5, 6):
            path = (
                self.root
                / "experiments"
                / f"wind_persistent_{stage}_20261105"
                / "analysis.json"
            )
            analysis = json.loads(path.read_text(encoding="utf-8"))
            for run_name, run in analysis["runs"].items():
                with self.subTest(stage=stage, run=run_name):
                    seed = seed_from_run(run_name)
                    frames = wind.generate_stage(stage, seed, constants=self.constants)
                    sample = wind.WindSeries(frames, self.constants).sample(0.0)
                    expected = run["wind_first"]
                    for got, want in zip(sample, expected):
                        self.assertAlmostEqual(got, want, delta=1e-6)


if __name__ == "__main__":
    unittest.main()
