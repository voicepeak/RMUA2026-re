#!/usr/bin/env python3
"""Summarize stage probe artifacts without treating missing breakpoints as proof."""
import argparse,collections,json,re
from pathlib import Path

def readrows(path):
    if not path.exists():return []
    result=[]
    for line in path.open():
        try: result.append(json.loads(line))
        except json.JSONDecodeError: pass
    return result

def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('path',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    path=a.path;manifest=json.loads((path/'manifest.json').read_text())
    trajectory=readrows(path/'scout_trajectory.jsonl');wind=readrows(path/'wind.jsonl')
    records=[z for z in trajectory if 'position' in z];events=[];clock=None
    for line in (path/'gdb.log').read_text(errors='replace').splitlines():
        if 'RMUA_TRACE_CLOCK ' in line:
            m=re.search(r'wall_unix=([0-9.]+) monotonic=([0-9.]+)',line)
            if m:clock={'wall_unix':float(m[1]),'monotonic':float(m[2])}
        for tag in ('RMUA_RANDOM_INIT','RMUA_SRAND','RMUA_STATE_ENTER','RMUA_STAGE_INIT','RMUA_WIND_GENERATE','RMUA_LINE','RMUA_VEHICLE_BRANCH'):
            if tag+' ' in line:
                events.append({'tag':tag,'clock':clock,'line':line[line.index(tag):]});clock=None;break
    result={'scope':'read-only GDB and ROS observer; diagnostic not competition-equivalent','seed':manifest['seed'],
            'run_status':manifest['status'],'scout_exit_code':manifest.get('scout_exit_code'),
            'clock_domains':{'wall_unix':'host Unix epoch seconds','monotonic':'host monotonic seconds',
                             'ros_header_stamp':'ROS message header source stamp; mapping to simulation time is not yet calibrated',
                             'trajectory.wall':'host monotonic seconds (legacy field name)',
                             'timer_1ac':'GameState field accumulates a game update delta per static code; calibration pending'},
            'gps_records':len(records),'wind_records':len(wind),'event_counts':dict(collections.Counter(x['tag'] for x in events)),
            'gdb_events':events,'wind_distinct_vectors':[list(x) for x in sorted({tuple(x['wind']) for x in wind})]}
    if records:
        result['first_trajectory']=records[0];result['last_trajectory']=records[-1]
        result['max_waypoint_index']=max(x['waypoint'] for x in records)
    if wind:
        result['wind_first']=wind[0];result['wind_last']=wind[-1]
    if (path/'scout_result.json').exists(): result['scout_result']=json.loads((path/'scout_result.json').read_text())
    a.output.write_text(json.dumps(result,indent=2)+'\n')
if __name__=='__main__':main()
