# Frequency measurement function overlap probe

`static_callers.txt` lists AirSimROS functions calling `record_frequency_data`. `probe_selftest.cc` and its log exercise same-key overlap on an independent toy process. `runtime_seed123` records the actual official simulator with all-stop GDB in NullRHI, no flight commands; see summary and gdb.log. The probe places breakpoints and does not modify program data. Scheduling perturbation prevents treating overlapping entry-to-return intervals as proof of a data race. No same-key overlap was observed in this sample; this is not evidence of absence.

```bash
python3 scripts/run_frequency_overlap_probe.py --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json --messages /home/tianbot/RUMA-by-helinjun/beta/rmua_ws --output /tmp/rmua-frequency --seeds 123 --repeats 1 --port 11691 --sample-seconds 10 --display :1
```
