#!/usr/bin/env python3
"""Test whether the cooked map is accepted as the executable's first argument."""
import run_map_gate_trace as trace
trace.LAUNCH=trace.LAUNCH.replace('RMUA seed "$RMUA_SEED"','"$RMUA_MAP" seed "$RMUA_SEED"').replace(' "-ini:Engine:[/Script/EngineSettings.GameMapsSettings]:GameDefaultMap=$RMUA_MAP"','')
if __name__=='__main__':trace.main()
