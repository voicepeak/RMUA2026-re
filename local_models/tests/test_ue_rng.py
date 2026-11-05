import unittest

from local_models.ue_rng import (
    FRandomStream,
    fraction_from_state,
    line_offset_from_state,
    step_state,
    vehicle_bucket_from_state,
)


class FRandomStreamTests(unittest.TestCase):
    def test_reset_state_252_steps_from_seed_123(self):
        state = 123
        for _ in range(252):
            state = step_state(state)
        self.assertEqual(state, 214098071)

    def test_second_runtime_consumed_state_from_seed_123(self):
        state = 123
        for _ in range(3078):
            state = step_state(state)
        self.assertEqual(state, 2071961785)

    def test_vehicle_buckets_from_captured_updated_states(self):
        cases = {
            997704449: 2,
            2925953427: 7,
            276274103: 0,
        }
        for state, bucket in cases.items():
            self.assertEqual(vehicle_bucket_from_state(state), bucket)

    def test_line_offsets_from_captured_updated_states(self):
        cases = [
            (3813234266, True, -7714.86426),
            (300017425, True, -1988.97229),
            (2384819588, True, -5386.81299),
            (982241873, False, 3100.87207),
            (1040831743, False, 3196.3623),
            (4057258973, False, 8112.57861),
        ]
        for state, reverse, observed in cases:
            computed = line_offset_from_state(state, reverse)
            self.assertAlmostEqual(computed, observed, delta=1e-4)

    def test_draw_fraction_steps_before_reading(self):
        stream = FRandomStream(123)
        expected_state = step_state(123)
        fraction = stream.draw_fraction()
        self.assertEqual(stream.state, expected_state)
        self.assertEqual(fraction, fraction_from_state(expected_state))

    def test_reset_restores_initial_state(self):
        stream = FRandomStream(456)
        stream.step()
        stream.step()
        self.assertEqual(stream.reset(), 456)
        self.assertEqual(stream.fraction(), fraction_from_state(456))


if __name__ == "__main__":
    unittest.main()
