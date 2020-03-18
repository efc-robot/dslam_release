#!/usr/bin/env python

import numpy as np
from PIL import Image
import os
import rospy
from tf_conversions import posemath
from dslam_sp.msg import TransformStampedArray, PoseStampedArray
import tf2_ros
from geometry_msgs.msg import TransformStamped, PoseStamped, Pose, PoseArray
from cv_bridge import CvBridge, CvBridgeError
import cv2
import trackutils
import time
import sys                                                              
import signal
import threading

transArray = TransformStampedArray()
LooptransArray = TransformStampedArray()
poseStampedArray = PoseStampedArray()
posearray_pub = None
poseStampedArraylock = threading.Lock()

def callback(data):
    global posearray_pub, transArray, poseStampedArray, poseStampedArraylock
    print(data)
    poseStampedArraylock.acquire()
    trackutils.appendTrans2PoseStampedArray(data, poseStampedArray)
    trackutils.appendTrans2TransArray(data, transArray)    
    poseStampedArraylock.release()
    posearray = PoseArray()
    posearray.header.frame_id = "map"

    trackutils.StampedArray2PoseArray(poseStampedArray, posearray)
    posearray_pub.publish(posearray)

BackendRunning = False
NewLoop = False

def callback_loop(data):
    global LooptransArray, NewLoop
    print ("loop detected")

    trackutils.appendTrans2TransArray(data, LooptransArray, False)
    NewLoop = True

def BackendThread():
    global posearray_pub, transArray, poseStampedArray, LooptransArray,BackendRunning, NewLoop, poseStampedArraylock
    while True:
        time.sleep(3)
        if (not BackendRunning) and (NewLoop):
            poseStampedArraylock.acquire()
            BackendRunning = True
            NewLoop = False
            BackendOpt(transArray,poseStampedArray,LooptransArray,posearray_pub)
            BackendRunning = False
            poseStampedArraylock.release()

    # trackutils.PoseStampedarray2G2O("./tem12.g2o", poseStampedArray)
    # trackutils.AddTransArray2G2O("./tem12.g2o", transArray )
    # trackutils.AddTransArray2G2O("./tem12.g2o", LooptransArray )
    # poseStampedArray.poseArray = []
    # trackutils.gtsamOpt2PoseStampedarray("./tem12.g2o",poseStampedArray)

    # posearray = PoseArray()
    # posearray.header.frame_id = "map"
    # trackutils.StampedArray2PoseArray(poseStampedArray, posearray)
    # posearray_pub.publish(posearray)
    

def BackendOpt(transArray,poseStampedArray,LooptransArray,posearray_pub):

    Max_id = int(poseStampedArray.poseArray[-1].header.frame_id)
    for loopconstrain in LooptransArray.transformArray:
        ValidLooptransArray = TransformStampedArray()
        if ( int(loopconstrain.child_frame_id) < Max_id ) and ( int(loopconstrain.header.frame_id) < Max_id):
            ValidLooptransArray.transformArray.append(loopconstrain)
    if not len(ValidLooptransArray.transformArray) > 0:
        return

    trackutils.PoseStampedarray2G2O("./tem12.g2o", poseStampedArray)
    trackutils.AddTransArray2G2O("./tem12.g2o", transArray )
    trackutils.AddTransArray2G2O("./tem12.g2o", LooptransArray )
    poseStampedArray.poseArray = []
    trackutils.gtsamOpt2PoseStampedarray("./tem12.g2o",poseStampedArray)

    posearray = PoseArray()
    posearray.header.frame_id = "map"
    # trackutils.StampedArray2PoseArray(poseStampedArray, posearray)
    # posearray_pub.publish(posearray)

def main():
    global posearray_pub
    signal.signal(signal.SIGINT, trackutils.quit)                                
    signal.signal(signal.SIGTERM, trackutils.quit)
    rospy.init_node('generate_track_py', anonymous=True)
    rospy.Subscriber("relpose", TransformStamped, callback)
    rospy.Subscriber("looppose", TransformStamped, callback_loop)
    posearray_pub = rospy.Publisher("posearray",PoseArray, queue_size=3)
    backt = threading.Thread(target=BackendThread)
    backt.setDaemon(True)
    backt.start()
    rospy.spin()


if __name__ == '__main__':
    main()
    # k1 = Pose()
    # k1.orientation.w = 1
    # k2 = Pose()
    # k2.orientation.w = 1

    # f = posemath.fromMsg(k1) * posemath.fromMsg(k2)
    # print f

    # [x, y, z, w] = f.M.GetQuaternion()
    # print x,y,z,w

    # pose = posemath.toMsg(f)
    # print pose







    