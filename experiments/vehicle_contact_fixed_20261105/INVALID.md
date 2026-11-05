# Interrupted diagnostic batch

The runner gated probe startup on receipt of the debug wind topic. It did not use wind values as actions, but the scheduling dependency is stricter than the user's posthoc-only debug-truth requirement. The batch was interrupted and must not be used for acceptance. The replacement runner gates readiness on the normal IMU topic. Any unfinished run is not evidence of a vehicle collision or its absence.
