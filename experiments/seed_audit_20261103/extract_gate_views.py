#!/usr/bin/env python3
"""Extract comparable historical scout images, with their actual debug poses."""
import json, sys
import numpy as np
import rosbag, cv2
from cv_bridge import CvBridge
from scipy.spatial import cKDTree

roots=['/workspace/alpha/alpha/runs/scout_123/20261102_225649_459968/scene.bag',
       '/workspace/alpha/alpha/runs/scout_456/20261102_230604_531614/scene.bag']
pose_topic='/airsim_node/drone_1/debug/pose_gt'
image_topic='/airsim_node/drone_1/front_left/Scene'
out='/analysis/experiments/seed_audit_20261103'
all_frames=[]
for path in roots:
    poses=[]; frames=[]
    with rosbag.Bag(path) as bag:
        for topic, msg, _ in bag.read_messages(topics=[pose_topic, image_topic]):
            if topic==pose_topic:
                p,q=msg.pose.position,msg.pose.orientation
                poses.append([msg.header.stamp.to_sec(),p.x,p.y,p.z,q.x,q.y,q.z,q.w])
            else: frames.append(msg.header.stamp.to_sec())
    poses=np.asarray(poses); frames=np.asarray(frames)
    idx=np.searchsorted(poses[:,0],frames).clip(1,len(poses)-1)
    idx-=np.abs(frames-poses[idx-1,0])<np.abs(frames-poses[idx,0])
    all_frames.append((frames,poses[idx]))
targets=[[12,0,-2],[32,0,-3],[52,0,-4],[90,0,-5],[150,0,-10],
         [300,None,None],[450,None,None],[600,None,None],[700,None,None]]
result=[]; picks=[{},{}]
for i,target in enumerate(targets):
    a=all_frames[0][1]
    j=(np.argmin(np.abs(a[:,1]-target[0])) if target[1] is None
       else np.argmin(np.linalg.norm(a[:,1:4]-target,axis=1)))
    b=all_frames[1][1]
    # Match both position and orientation; q and -q are equivalent.
    orient=np.minimum(np.linalg.norm(b[:,4:8]-a[j,4:8],axis=1),np.linalg.norm(b[:,4:8]+a[j,4:8],axis=1))
    cost=np.linalg.norm(b[:,1:4]-a[j,1:4],axis=1)+4*orient
    k=np.argmin(cost)
    row={'target':target,'pose123':a[j].tolist(),'pose456':b[k].tolist(),
         'position_difference_m':float(np.linalg.norm(a[j,1:4]-b[k,1:4])),
         'orientation_difference_rad':float(2*np.arccos(np.clip(abs(np.dot(a[j,4:8],b[k,4:8])),0,1)))}
    result.append(row); picks[0][all_frames[0][0][j]]=i; picks[1][all_frames[1][0][k]]=i
bridge=CvBridge()
for n,path in enumerate(roots):
    with rosbag.Bag(path) as bag:
        for _,msg,_ in bag.read_messages(topics=[image_topic]):
            t=msg.header.stamp.to_sec()
            if t in picks[n]:
                cv2.imwrite('%s/view_%s_seed%s.png' % (out,picks[n][t],[123,456][n]),bridge.imgmsg_to_cv2(msg,'bgr8'))
with open(out+'/historical_views.json','w') as f: json.dump(result,f,indent=2)
print(json.dumps(result,indent=2))
