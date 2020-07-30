#!/usr/bin/env python
# -*- coding: utf-8 -*- 

import torch
import torch.nn as nn
import torchvision
import torch.nn.functional as F
import torchvision.transforms as transforms
import torchvision.models as models

from std_msgs.msg import String

import numpy as np
from PIL import Image
import os
import rospy
from geometry_msgs.msg import TransformStamped
from dslam_sp.msg import image_depth, PRrepresentor, TransformStampedArray
from cv_bridge import CvBridge, CvBridgeError
import cv2
import threading
import sys, getopt 

meta = {'outputdim': 2048}


self_ID = 1
# frame 规则是 self_ID*1e8 + framecount

representors_record = {}
representors_locks = {}
representor_frameIDs = {} #没加锁 todo

frameIDstr = []
match_loop_pub = None
other_match_loop_pub = None

matched_frames = {} #展示和第i号机器人是否已经匹配，并且记录自己的第几帧和对方匹配。

looptrans_pub = None 

def matchrepresentor(query, matchs):
    results = np.matmul(matchs, query)
    return results

def loop_callback(data):
    #
    global representors_record, representors_locks, match_loop_pub, matched_frames
    imageHeader = data.imageHeader
    imageFrameID = imageHeader.frame_id

    current_robot_id = int(int(imageFrameID)/1e8)
    # assert(current_robot_id == self_ID)
    
    representor = data.representor
    #更新记录
    if not (representors_locks.has_key(str(current_robot_id)) ):
        representors_locks[str(current_robot_id)] = threading.Lock()
        representors_record[str(current_robot_id)] = np.zeros((0,meta['outputdim']),dtype=float)
        representor_frameIDs[str(current_robot_id)] = []
        matched_frames[str(current_robot_id)] = set()
    representors_locks[str(current_robot_id)].acquire()
    representors_record[str(current_robot_id)] = np.vstack(( representors_record[str(current_robot_id)] , np.array(representor) ) )
    representors_locks[str(current_robot_id)].release()
    
    #记录对应帧ID
    representor_frameIDs[str(current_robot_id)].append(imageFrameID)

    #处理match
    #和之前记录的值进行比较
    matchresults = {}
    if (int(current_robot_id) == self_ID): #如果是自己的图片需要和所有图片进行比较
        for robot_id, representor_id_record in representors_record.items():
            representors_locks[robot_id].acquire()
            matchresults[robot_id] = matchrepresentor(representor,representor_id_record )
            representors_locks[robot_id].release()

        for robot_id, representor_id_record in matchresults.items():
            if int(robot_id) == self_ID : #自己和自己比
                if len(matchresults[robot_id] > 30):
                    validresults = matchresults[robot_id][0:-30]
                    validmatch = validresults>0.95
                    if ( validmatch.any() ):
                        maxargs = np.argmax(validresults)

                        matchpair = "{id1} {id2}".format(id1 = representor_frameIDs[robot_id][maxargs], id2 = imageFrameID)
                        print (matchpair)
                        match_loop_pub.publish(matchpair) # to publish rel fnames  
            else : #新进来的帧和之前别人的图片能够匹配
                if len(matchresults[robot_id] > 1):
                    validresults = matchresults[robot_id][0:-1]
                    validmatch = validresults>0.95
                    if ( validmatch.any() ):
                        maxargs = np.argmax(validresults)
                        print(int(imageFrameID) )
                        if not ( int(imageFrameID) in matched_frames[str(robot_id)]):
                            matchpair = "{id1} {id2}".format(id1 = imageFrameID, id2 = representor_frameIDs[robot_id][maxargs])
                            print (matchpair)
                            match_loop_pub.publish(matchpair) # to publish rel fnames  
                            matched_frames[str(robot_id)] = matched_frames[str(robot_id)].union(set(range(max(0,int(imageFrameID) - 40 ), int(imageFrameID)+40  )))
    else : # 不是自己的图片，就只和自己进行比较
        representors_locks[str(self_ID)].acquire()
        matchresults[str(self_ID)] = matchrepresentor(representor,representors_record[str(self_ID)] )
        representors_locks[str(self_ID)].release()
        if len(matchresults[str(self_ID)] > 1):
            validresults = matchresults[str(self_ID)][0:-1]
            validmatch = validresults>0.95
            if ( validmatch.any() ):
                maxargs = np.argmax(validresults)
                print(representor_frameIDs[str(self_ID)][maxargs])
                if not ( int(representor_frameIDs[str(self_ID)][maxargs]) in matched_frames[str(current_robot_id)]):
                    matchpair = "{id1} {id2}".format(id1 = representor_frameIDs[str(self_ID)][maxargs], id2 = imageFrameID)
                    print (matchpair)
                    match_loop_pub.publish(matchpair) # to publish rel fnames  
                    matched_frames[str(current_robot_id)] = matched_frames[str(current_robot_id)].union(set(range(max(0,int(representor_frameIDs[str(self_ID)][maxargs]) - 40 ), int(representor_frameIDs[str(self_ID)][maxargs])+40  )))

    print(matched_frames)


def main(argv):
    global looptrans_pub, match_loop_pub, self_ID, representors_record, representors_locks, other_match_loop_pub, matched_frames
    opts, args = getopt.getopt(argv,"i:")
    for opt, arg in opts:
        if opt in ("-i"):
            self_ID = int(arg)


    representors_record[str(self_ID)] = np.zeros((0,meta['outputdim']),dtype=float)
    representors_locks[str(self_ID)] = threading.Lock()
    representor_frameIDs[str(self_ID)] = []
    matched_frames[str(self_ID)] = set()
    
    rospy.init_node('matcher', anonymous=True)
    rospy.Subscriber("PRrepresentor", PRrepresentor, loop_callback)
    # rospy.Subscriber("OtherPresentor", PRrepresentor, loop_callback)
    looptrans_pub = rospy.Publisher("looppose_deee",TransformStamped, queue_size=3)
    match_loop_pub = rospy.Publisher("loopfnames",String, queue_size=3)
    rospy.spin()



if __name__ == '__main__':
    main(sys.argv[1:])