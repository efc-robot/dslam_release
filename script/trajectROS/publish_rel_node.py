#!/usr/bin/env python

import trackutils
import rospy
from dslam_sp.msg import TransformStampedArray
from geometry_msgs.msg import TransformStamped, PoseStamped, Pose, PoseArray
import time

import sys                                                              
import signal


def main():
    signal.signal(signal.SIGINT, trackutils.quit)                                
    signal.signal(signal.SIGTERM, trackutils.quit)

    rospy.init_node('publish_rel_node', anonymous=True)
    relpose_pub = rospy.Publisher("relpose",TransformStamped, queue_size=3)
    looptrans_pub = rospy.Publisher("looppose",TransformStamped, queue_size=3)
    rel_pose_array = PoseArray()
    trackutils.RTtxt2Posearray("./testdata/rel_09_s.txt",rel_pose_array)
    from_id = 1
    to_id = 2
    for pose in rel_pose_array.poses:
        time.sleep(0.05)
        transformtmp = TransformStamped()
        transformtmp.header.frame_id = str(to_id)
        transformtmp.child_frame_id = str(from_id)
        from_id = to_id
        to_id += 1
        transformtmp.transform.rotation = pose.orientation
        transformtmp.transform.translation = pose.position
        relpose_pub.publish(transformtmp)

    for i in range(6):
        print(6-i)
        time.sleep(1)
        
    tmpt = TransformStamped()
    tmpt.child_frame_id = str(1)
    tmpt.header.frame_id = str(1+1578)
    tmpt.transform.rotation.w = 1

    looptrans_pub.publish(tmpt)


    for i in range(6):
        print(6-i)
        time.sleep(1)    

    




if __name__ == "__main__":
    main()