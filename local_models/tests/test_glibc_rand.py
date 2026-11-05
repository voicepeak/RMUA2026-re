import unittest

from local_models.glibc_rand import GlibcRand, host_is_glibc, make_glibc_rand

EXPECTED = {
    1: [1804289383, 846930886, 1681692777, 1714636915, 1957747793, 424238335, 719885386, 1649760492],
    123: [128959393, 1692901013, 436085873, 748533630, 776550279, 289134331, 807385195, 556889022],
    456: [1929505231, 1800653403, 1030306120, 2023122793, 1908338983, 350238183, 199311997, 1067924705],
    2026: [1199659537, 1872465372, 1381520923, 36027969, 1759202686, 784491491, 862550327, 1485565472],
}


class GlibcRandTests(unittest.TestCase):
    def test_known_glibc_sequences(self):
        for seed, expected in EXPECTED.items():
            stream = GlibcRand(seed)
            self.assertEqual([stream.rand() for _ in expected], expected)

    def test_seed_zero_is_one(self):
        zero = GlibcRand(0)
        one = GlibcRand(1)
        self.assertEqual([zero.rand() for _ in range(8)], [one.rand() for _ in range(8)])

    def test_reseed_restarts_sequence(self):
        stream = GlibcRand(123)
        first = [stream.rand() for _ in range(8)]
        stream.srand(123)
        self.assertEqual([stream.rand() for _ in range(8)], first)

    def test_auto_streams_are_independent_when_interleaved(self):
        first = make_glibc_rand(123)
        second = make_glibc_rand(456)
        expected_first = GlibcRand(123)
        expected_second = GlibcRand(456)
        for _ in range(64):
            self.assertEqual(first.rand(), expected_first.rand())
            self.assertEqual(second.rand(), expected_second.rand())

    @unittest.skipUnless(host_is_glibc(), "ctypes glibc comparison requires Linux/glibc")
    def test_pure_fallback_matches_host_glibc(self):
        for seed in (0, 1, 123, 456, 2026, 42, 0x7fffffff, 0x80000000,
                     0x80000001, 0xfffffffe, 0xffffffff, -1):
            pure = make_glibc_rand(seed, backend="pure")
            host = make_glibc_rand(seed, backend="ctypes")
            self.assertEqual(
                [pure.rand() for _ in range(64)],
                [host.rand() for _ in range(64)],
            )


if __name__ == "__main__":
    unittest.main()
