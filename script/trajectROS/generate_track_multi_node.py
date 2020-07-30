#!/usr/bin/env python
# -*- coding: utf-8 -*- 


import numpy as np
from PIL import Image
import os
import rospy
import tf
from tf_conversions import posemath
import PyKDL as kdl
from dslam_sp.msg import TransformStampedArray, PoseStampedArray, TransformStamped_with_image, Pose_with_image
from dslam_sp.srv import *
from octomap_msgs.msg import Octomap
from octomap_msgs.srv import GetOctomap
import tf2_ros
from geometry_msgs.msg import TransformStamped, PoseStamped, Pose, PoseArray
from cv_bridge import CvBridge, CvBridgeError
import cv2
import trackutils
import time
import sys, getopt                                                      
import signal
import threading
import pickle
import queue

self_ID = 1
# frame 规则是 self_ID*1e8 + framecount

tranStampedArray_sets = {}
tranStampedArray_locks= {}

LooptransArray_sets = {}
LooptransArray_locks = {}

poseStampedArray_sets = {}
poseStampedArray_locks = {}

published_map = {}

posearray_pub = None
posearray_pub_test = None
pose_with_image_pub = None
map_pub = None
br = tf.TransformBroadcaster()

BackendRunning = False
NewLoop = False
withmap = True

count_save = 0

self_trans_queue = queue.Queue(maxsize=100)

def strip_leading_slash(s):
    return s[1:] if s.startswith("/") else s

def publish_tf_to_tree(tf_msg, var_frame_id, var_child_frame_id):
    global br, listener
    print("start publish tf")
    listener.waitForTransform(var_frame_id, tf_msg.header.frame_id, rospy.Time(), rospy.Duration(4.0))
    listener.waitForTransform(tf_msg.child_frame_id, var_child_frame_id, rospy.Time(), rospy.Duration(4.0))
    frame_tf_msg = listener._buffer.lookup_transform(strip_leading_slash(var_frame_id), strip_leading_slash(tf_msg.header.frame_id), rospy.Time(0))
    child_frame_tf_msg = listener._buffer.lookup_transform(strip_leading_slash(tf_msg.child_frame_id), strip_leading_slash(var_child_frame_id), rospy.Time(0))
    print("frame_tf_msg:"+str(frame_tf_msg))
    print("child_frame_tf_msg:"+str(child_frame_tf_msg))
    frame_tf = posemath.fromMsg(trackutils.trans2pose(frame_tf_msg.transform))
    print("frame_tf:"+str(frame_tf))
    child_frame_tf = posemath.fromMsg(trackutils.trans2pose(child_frame_tf_msg.transform))
    print("child_frame_tf:"+str(child_frame_tf))
    tf_kdl = posemath.fromMsg(trackutils.trans2pose(tf_msg.transform))
    print("tf_kdl:"+str(tf_kdl))
    tf_msg.transform = trackutils.pose2trans(posemath.toMsg( frame_tf * tf_kdl * child_frame_tf ))
    tf_msg.header.stamp = tf_msg.header.stamp
    tf_msg.header.frame_id = var_frame_id
    tf_msg.child_frame_id = var_child_frame_id
    print("publish:"+str(tf_msg))
    br.sendTransformMessage(tf_msg)

def maintain_tf_tree(odom_to_scene_before=None, odom_to_scene_after=None):
    global br, listener, self_ID
    base_to_scene = TransformStamped()
    base_to_scene.transform = trackutils.pose2trans(posemath.toMsg(kdl.Frame(kdl.Rotation.Quaternion(x=0.5, y=-0.5, z=0.5, w=-0.5))))
    base_to_scene.header.stamp = rospy.Time.now()
    base_to_scene.header.frame_id = "base_link{}".format(self_ID)
    base_to_scene.child_frame_id = "scene{}".format(self_ID)
    br.sendTransformMessage(base_to_scene)

    if odom_to_scene_before==None or odom_to_scene_after==None:
        map_to_odom = TransformStamped()
        map_to_odom.header.stamp = rospy.Time.now()
        map_to_odom.transform.rotation.w = 1.
        map_to_odom.header.frame_id = "/map{}".format(self_ID)
        map_to_odom.child_frame_id = "/odom{}".format(self_ID)
        print(map_to_odom)
        br.sendTransformMessage(map_to_odom) #发送map和odom的TF树
    elif odom_to_scene_before==odom_to_scene_after:
        listener.waitForTransform("map{}".format(self_ID), "odom{}".format(self_ID), rospy.Time(), rospy.Duration(4.0))
        map_to_odom = listener._buffer.lookup_transform("map{}".format(self_ID), "odom{}".format(self_ID), rospy.Time(0))
        map_to_odom.header.stamp = rospy.Time.now()
        br.sendTransformMessage(map_to_odom) #发送map和odom的TF树
    else:
        listener.waitForTransform("map{}".format(self_ID), "odom{}".format(self_ID), rospy.Time(), rospy.Duration(4.0))
        map_to_odom = listener._buffer.lookup_transform("map{}".format(self_ID), "odom{}".format(self_ID), rospy.Time(0))
        map_to_odom.header.stamp = rospy.Time.now()
        map_to_odom_kdl = posemath.fromMsg(trackutils.trans2pose(map_to_odom.transform))
        init_pose = kdl.Frame(kdl.Rotation.Quaternion(x=0.5, y=-0.5, z=0.5, w=-0.5))
        odom_to_scene_after_kdl = init_pose * posemath.fromMsg(odom_to_scene_after)
        odom_to_scene_before_kdl = init_pose * posemath.fromMsg(odom_to_scene_before)
        map_to_odom_kdl = map_to_odom_kdl * odom_to_scene_before_kdl * odom_to_scene_after_kdl.Inverse()
        map_to_odom.transform = trackutils.pose2trans(posemath.toMsg(map_to_odom_kdl))
        br.sendTransformMessage(map_to_odom) #发送map和odom的TF树


def self_track_pose_cb(data):
    global self_ID, self_transArray, poseStampedArray_sets, poseStampedArray_locks, posearray_pub,self_trans_queue
    self_trans_queue.put(data)
    # print(data)
    #child_frame_id表示先来的图片,或者是对方的图片。header.frame_id 表示后来的图片,或者自己的图片

def process_self():
    global self_ID, self_transArray, poseStampedArray_sets, poseStampedArray_locks, posearray_pub, self_trans_queue, pose_with_image_pub
    while(True):
        time.sleep(0.1)
        if self_trans_queue.empty():
            continue
        data = self_trans_queue.get()
        if( int(int(data.TF.child_frame_id)/1e8 ) == int(int(data.TF.header.frame_id)/1e8) ): #如果是自己的位姿
            current_robot_id = int(int(data.TF.child_frame_id)/1e8)
            print(data.TF)
            assert (current_robot_id == self_ID), str(data.TF) + " current_robot_id: " + str(current_robot_id) + " self_ID: " + str(self_ID)#验证确实是自己的位姿
            poseStampedArray_locks[ str(current_robot_id ) ].acquire()
            trackutils.appendTrans2PoseStampedArray(data.TF, poseStampedArray_sets[ str(current_robot_id ) ] )
            trackutils.appendTrans2TransArray(data.TF, tranStampedArray_sets[ str(self_ID)] )    
            poseStampedArray_locks[ str(current_robot_id) ].release()
            vo_posearray = PoseArray()
            map_posearray = PoseArray()
            map_posearray.header.stamp = data.TF.header.stamp
            map_posearray.header.frame_id = "odom{}".format(self_ID)

            poseStampedArray_locks[ str(current_robot_id ) ].acquire()
            trackutils.StampedArray2PoseArray(poseStampedArray_sets[ str(current_robot_id ) ], vo_posearray)
            poseStampedArray_locks[ str(current_robot_id ) ].release()
            trackutils.VOPoseArray2MapPoseArray(vo_posearray, map_posearray)
            posearray_pub.publish(map_posearray)

            pose_with_image = Pose_with_image()
            pose_with_image.header.stamp = data.TF.header.stamp
            pose_with_image.header.frame_id = "/scene{}".format(self_ID)
            pose_with_image.pose = map_posearray.poses[-1]
            pose_with_image.image = data.image
            pose_with_image.depth = data.depth
            pose_with_image.P = data.P
            pose_with_image_pub.publish(pose_with_image)

            tf_msg = TransformStamped()
            tf_msg.header.stamp = data.TF.header.stamp
            tf_msg.header.frame_id = "/odom{}".format(self_ID)
            tf_msg.child_frame_id = "/scene{}".format(self_ID)
            tf_msg.transform = trackutils.pose2trans(map_posearray.poses[-1])
            # tf_msg.transform.translation = map_posearray.poses[-1].position
            # tf_msg.transform.rotation = map_posearray.poses[-1].orientation
            print(tf_msg)
            # br.sendTransformMessage(tf_msg)
            publish_tf_to_tree(tf_msg, "/odom{}".format(self_ID), "/base_link{}".format(self_ID))

            maintain_tf_tree(1, 1)

            # for robot_id in poseStampedArray_sets.keys(): #发送多个机器人第一帧之间的TF树
            #     if ( int(robot_id) < self_ID):
            #         print("start")
            #         tf_msg = TransformStamped()
            #         tf_msg.header.frame_id = "/map{}".format(self_ID)
            #         tf_msg.child_frame_id = "/map{}".format(robot_id)
                    
            #         print("read map" + robot_id)
            #         poseStampedArray_locks[ robot_id ].acquire()
            #         print(poseStampedArray_sets[robot_id].poseArray[-1])
            #         tf_msg.header.stamp = rospy.Time.now()
            #         pose_map1 = posemath.fromMsg(poseStampedArray_sets[robot_id].poseArray[0].pose)
            #         poseStampedArray_locks[robot_id].release()
                    
            #         print("create init_pose")
            #         init_pose = kdl.Frame(kdl.Rotation.Quaternion(x=0.7071, y=0, z=0, w=0.7071))
            #         print("init_pose:" + str(init_pose))
            #         pose_map1 = init_pose * pose_map1 * init_pose.Inverse()
            #         tf_msg.transform.translation = posemath.toMsg(pose_map1).position
            #         tf_msg.transform.rotation = posemath.toMsg(pose_map1).orientation
            #         #2D# tf_msg.transform.translation.z = 0
            #         #2D# tf_msg.transform.rotation.x = 0
            #         #2D# tf_msg.transform.rotation.y = 0
            #         #2D# tf_msg.transform.rotation.z = pow(1-pow(tf_msg.transform.rotation.w, 2), 0.5) * tf_msg.transform.rotation.z / abs(tf_msg.transform.rotation.z)
            #         print(tf_msg)
            #         br.sendTransformMessage(tf_msg)

            #     if not published_map.has_key(robot_id) and not (int(robot_id) == self_ID):
            #         print ("start wait_for_service " + '/robot{}/octomap_binary'.format(robot_id))
            #         rospy.wait_for_service('/robot{}/octomap_binary'.format(robot_id) )
            #         print ("new handle")
            #         map_srvhandle = rospy.ServiceProxy('/robot{}/octomap_binary'.format(robot_id), GetOctomap)
            #         print ("get map")
            #         map_result = map_srvhandle()
            #         print ("gotten map")
            #         print("map_result.stamp:" + str(map_result.map.header.stamp) + "tf.stamp:" + str(tf_msg.header.stamp))
            #         map_pub.publish(map_result.map)
            #         print("published map")
            #         published_map[robot_id] = 1
    
            
    


def callback_loop(data):
    global LooptransArray_sets, LooptransArray_locks, NewLoop
    #child_frame_id表示先来的图片,或者是对方的图片。header.frame_id 表示后来的图片,或者自己的图片
    if (int(int(data.child_frame_id)/1e8 ) == int(int(data.header.frame_id)/1e8) ): #如果两个位姿相同
        current_robot_id = int(int(data.header.frame_id)/1e8)
        assert (current_robot_id == self_ID) #验证确实是自己的位姿
        LooptransArray_locks[str(self_ID)].acquire()
        trackutils.appendTrans2TransArray(data, LooptransArray_sets[str(self_ID)], False)
        LooptransArray_locks[str(self_ID)].release()
    else: #如果不是两个位姿不同
        other_robot_id = int(int(data.child_frame_id)/1e8)
        self_robot_id = int(int(data.header.frame_id)/1e8)
        print ("PreinterOKKKKKKKKKKKKKKKKKKKKKK")
        assert (self_robot_id == self_ID) #验证确实和自己相关
        print ("interOKKKKKKKKKKKKKKKKKKKKKK")
        if not (LooptransArray_sets.has_key(str(other_robot_id))):
            LooptransArray_sets[str(other_robot_id)] = TransformStampedArray()
            LooptransArray_locks[str(other_robot_id)] = threading.Lock()
        LooptransArray_locks[str(other_robot_id)].acquire()
        trackutils.appendTrans2TransArray(data,LooptransArray_sets[str(other_robot_id)],False)
        LooptransArray_locks[str(other_robot_id)].release()
    NewLoop = True
    print("NewLoop")
    print(data)

def BackendThread():
    global posearray_pub, transArray, poseStampedArray_sets, poseStampedArray_locks, LooptransArray_sets, LooptransArray_locks,BackendRunning, NewLoop, br, published_map, map_pub, withmap
    while True:
        time.sleep(2)
        print("BackendOpt..........running")

        print("Keys: ")
        print(poseStampedArray_sets.keys())

        #获取其他机器人的trans
        for key, _ in LooptransArray_sets.items():
            if not (int(key) == self_ID):
                print("loop find server!!!" + '/robot{}/transarray_srv'.format(key))
                rospy.wait_for_service('/robot{}/transarray_srv'.format(key) )
                print("server waited!!!" + '/robot{}/transarray_srv'.format(key))
                transarray_srvhandle = rospy.ServiceProxy('/robot{}/transarray_srv'.format(key), transarray_srv)
                transarray_result = transarray_srvhandle(str(key))
                print("loop get server!!!")
                print("transarray len: {} === OJBK".format(transarray_result.transarray) )
                if not tranStampedArray_sets.has_key(str(key)):
                    tranStampedArray_locks[str(key)] = threading.Lock()
                tranStampedArray_locks[key].acquire()
                tranStampedArray_sets[key] = transarray_result.transarray
                tranStampedArray_locks[key].release()

        SelfLastPoseBeforeOpt = None
        SelfLastPoseAfterOpt = None
        if (not BackendRunning) and (NewLoop):
            BackendRunning = True
            NewLoop = False
            poseStampedArray_locks[str(self_ID)].acquire()
            LastIndex = len(poseStampedArray_sets[str(self_ID)].poseArray)
            SelfLastPoseBeforeOpt = poseStampedArray_sets[str(self_ID)].poseArray[LastIndex-1].pose
            poseStampedArray_locks[str(self_ID)].release()
            BackendOpt() #这里面可能把NewLoop设置为True。主要是应对，回环已经检测到了，但是轨迹还没有来得及更新到的情况。
            poseStampedArray_locks[str(self_ID)].acquire()
            SelfLastPoseAfterOpt = poseStampedArray_sets[str(self_ID)].poseArray[LastIndex-1].pose
            poseStampedArray_locks[str(self_ID)].release()

            BackendRunning = False

        maintain_tf_tree(SelfLastPoseBeforeOpt, SelfLastPoseAfterOpt)
        for robot_id in poseStampedArray_sets.keys(): #发送多个机器人第一帧之间的TF树
            if ( int(robot_id) < self_ID):
                print("start")
                tf_msg = TransformStamped()
                tf_msg.header.frame_id = "/odom{}".format(self_ID)
                tf_msg.child_frame_id = "/odom{}".format(robot_id)
                
                print("read map" + robot_id)
                poseStampedArray_locks[ robot_id ].acquire()
                print(poseStampedArray_sets[robot_id].poseArray[-1])
                tf_msg.header.stamp = rospy.Time.now()
                pose_map1 = posemath.fromMsg(poseStampedArray_sets[robot_id].poseArray[0].pose)
                poseStampedArray_locks[robot_id].release()
                
                print("create init_pose")
                init_pose = kdl.Frame(kdl.Rotation.Quaternion(x=0.5, y=-0.5, z=0.5, w=-0.5))
                print("init_pose:" + str(init_pose))
                pose_map1 = init_pose * pose_map1 * init_pose.Inverse()
                tf_msg.transform = trackutils.pose2trans(posemath.toMsg(pose_map1))
                # tf_msg.transform.translation = posemath.toMsg(pose_map1).position
                # tf_msg.transform.rotation = posemath.toMsg(pose_map1).orientation
                #2D# tf_msg.transform.translation.z = 0
                #2D# tf_msg.transform.rotation.x = 0
                #2D# tf_msg.transform.rotation.y = 0
                #2D# tf_msg.transform.rotation.z = pow(1-pow(tf_msg.transform.rotation.w, 2), 0.5) * tf_msg.transform.rotation.z / abs(tf_msg.transform.rotation.z)
                print(tf_msg)
                # br.sendTransformMessage(tf_msg)
                publish_tf_to_tree(tf_msg, "/map{}".format(self_ID), "/map{}".format(robot_id))

            if not published_map.has_key(robot_id) and not (int(robot_id) == self_ID) and withmap:
                print ("start wait_for_service " + '/robot{}/octomap_binary'.format(robot_id))
                rospy.wait_for_service('/robot{}/octomap_binary'.format(robot_id) )
                print ("new handle")
                map_srvhandle = rospy.ServiceProxy('/robot{}/octomap_binary'.format(robot_id), GetOctomap)
                print ("get map")
                map_result = map_srvhandle()
                print ("gotten map")
                # print("map_result.stamp:" + str(map_result.map.header.stamp) + "tf.stamp:" + str(tf_msg.header.stamp))
                map_pub.publish(map_result.map)
                print("published map")
                published_map[robot_id] = 1

            

def BackendOpt():
    global posearray_pub, transArray, poseStampedArray_sets, poseStampedArray_locks, LooptransArray_sets, LooptransArray_locks, tranStampedArray_sets, tranStampedArray_locks, BackendRunning, NewLoop, self_ID, count_save, posearray_pub_test
    print("BackendOpt..........")
    # if len(LooptransArray_sets[str(3-self_ID)].transformArray) > 0:
    #     # print(LooptransArray_sets)
    print("BackendOpt..........valid")
    poseStampedArray_locks[str(self_ID)].acquire()
    for key, _ in LooptransArray_sets.items():
        LooptransArray_locks[key].acquire()
    for key, _ in tranStampedArray_sets.items():
        tranStampedArray_locks[key].acquire()
    # dumpname = '/tmp/data_robot{}_{}.pkl'.format(self_ID, count_save)
    # with open(dumpname, 'wb') as dumpoutfile:
    #     pickle.dump(LooptransArray_sets, dumpoutfile)
    #     pickle.dump(poseStampedArray_sets, dumpoutfile)
    #     pickle.dump(tranStampedArray_sets, dumpoutfile)
    #     count_save += 1

        # print("===================================LooptransArray_sets=======================================")
        # print( LooptransArray_sets )
        # print("===================================poseStampedArray_sets=======================================")
        # print( poseStampedArray_sets)
        # print("===================================tranStampedArray_sets=======================================")
        # print( tranStampedArray_sets)
        # print("===============================================================================================")
        # count_save += 1
    
    for key, _ in LooptransArray_sets.items():
        LooptransArray_locks[key].release()
    for key, _ in tranStampedArray_sets.items():
        tranStampedArray_locks[key].release()

    g2ofilename = "/tmp/g2o_robot{}_test.g2o".format(self_ID)
    with open(g2ofilename, "w") as filetmp:
        # pass
        print("show first_add_node_str")
        first_add_node_str = "VERTEX_SE3:QUAT {pointID} {tx} {ty} {tz} {rx} {ry} {rz} {rw}".format(pointID=0,tx=0,ty=0,tz=0, rx = 0, ry = 0, rz = 0, rw = 1)
        print (first_add_node_str)
        print >> filetmp, first_add_node_str
        print("shown first_add_node_str")
    #add pose 和 trans
    for key, looparray in LooptransArray_sets.items():
        if not (int(key) == self_ID):
            if not (tranStampedArray_locks.has_key(key)):
                tranStampedArray_locks[key] = threading.Lock()
                tranStampedArray_sets[key] = TransformStampedArray()
            tranStampedArray_locks[key].acquire()
            LooptransArray_locks[key].acquire()



            if len(LooptransArray_sets[key].transformArray) > 0 and len(tranStampedArray_sets[key].transformArray)>0 :
                

                posearray_target = PoseStampedArray()
                trackutils.AccReltrans2PoseStampedArray(tranStampedArray_sets[key],posearray_target)
                target_init_pose = trackutils.LoopPosearrayInitpose(LooptransArray_sets[key].transformArray[0], poseStampedArray_sets[str(self_ID)], posearray_target)

                if not target_init_pose:
                    NewLoop = True
                    break

                if not poseStampedArray_sets.has_key(key):
                    poseStampedArray_locks[key] = threading.Lock()
                poseStampedArray_locks[key].acquire()
                poseStampedArray_sets[key] = trackutils.RotatePoserray(target_init_pose, posearray_target)
                valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, LooptransArray_sets[key])
                if len(valid_loop_array.transformArray) > 0:
                    trackutils.PoseStampedarray2G2O(g2ofilename,poseStampedArray_sets[key], True )
                    valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, tranStampedArray_sets[key])
                    trackutils.AddTransArray2G2O(g2ofilename, valid_loop_array)
                poseStampedArray_locks[key].release()
                
            LooptransArray_locks[key].release()
            tranStampedArray_locks[key].release()

            
                    
        else : #ID 相等是自己 key == self_ID
            tranStampedArray_locks[key].acquire()
            trackutils.PoseStampedarray2G2O(g2ofilename,poseStampedArray_sets[key], True )
            with open(g2ofilename, "a") as g2ofile:
                print("show cons_str")
                cons_str = "EDGE_SE3:QUAT {id1} {id2} {tx} {ty} {tz} {rx} {ry} {rz} {rw}  {COVAR_STR}".format(id1=0,id2=poseStampedArray_sets[key].poseArray[0].header.frame_id, tx =0 , ty =0 , tz=0, rx =0, ry=0, rz =0, rw =1, COVAR_STR=trackutils.COVAR_STR)
                print(cons_str)
                print >> g2ofile, cons_str
                print("shown cons_str")
            valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, tranStampedArray_sets[key])
            trackutils.AddTransArray2G2O(g2ofilename, valid_loop_array)

            tranStampedArray_locks[key].release()
    
    for key, looparray in LooptransArray_sets.items():
        if not (int(key) == self_ID):
            tranStampedArray_locks[key].acquire()
            if len(LooptransArray_sets[key].transformArray) > 0 and len(tranStampedArray_sets[key].transformArray)>0 :
                valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, LooptransArray_sets[key])
                if len(valid_loop_array.transformArray) > 0:
                    valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, LooptransArray_sets[key])
                    trackutils.AddTransArray2G2O(g2ofilename, valid_loop_array)
            tranStampedArray_locks[key].release()

        else :
            tranStampedArray_locks[key].acquire()
            valid_loop_array = trackutils.valid_loop(poseStampedArray_sets, LooptransArray_sets[key])
            trackutils.AddTransArray2G2O(g2ofilename, valid_loop_array)
            tranStampedArray_locks[key].release()

    print("start Backend Optimization")
    poseStampedArray_sets_result = {}
    trackutils.gtsamOpt2PoseStampedArraySet(g2ofilename,poseStampedArray_sets_result)
    for key, _ in poseStampedArray_sets_result.items():
        poseStampedArray_sets[key] = poseStampedArray_sets_result[key]
    print("finish Backend Optimization")
    
    # posearray = PoseArray()
    # posearray.header.frame_id = "map{}".format(self_ID)
    # # poseStampedArray_locks[str(3-self_ID)].acquire()
    # trackutils.StampedArray2PoseArray(poseStampedArray_sets[str(3-self_ID)], posearray)
    # posearray_pub_test.publish(posearray)
    
    poseStampedArray_locks[str(self_ID)].release()
    
    # poseStampedArray_locks[str(3-self_ID)].release()


#     Max_id = int(poseStampedArray.poseArray[-1].header.frame_id)
#     for loopconstrain in LooptransArray.transformArray:
#         ValidLooptransArray = TransformStampedArray()
#         if ( int(loopconstrain.child_frame_id) < Max_id ) and ( int(loopconstrain.header.frame_id) < Max_id):
#             ValidLooptransArray.transformArray.append(loopconstrain)
#     if not len(ValidLooptransArray.transformArray) > 0:
#         return

#     trackutils.PoseStampedarray2G2O("./tem12.g2o", poseStampedArray)
#     trackutils.AddTransArray2G2O("./tem12.g2o", transArray )
#     trackutils.AddTransArray2G2O("./tem12.g2o", LooptransArray )
#     poseStampedArray.poseArray = []
#     trackutils.gtsamOpt2PoseStampedarray("./tem12.g2o",poseStampedArray)

#     posearray = PoseArray()
#     posearray.header.frame_id = "map"
#     # trackutils.StampedArray2PoseArray(poseStampedArray, posearray)
#     # posearray_pub.publish(posearray)

def handle_posearray_srv(req):
    print( "ID : {}".format(req.inputID) )
    poseStampedArray_locks[req.inputID].acquire()
    posearray_return = poseStampedArray_sets[req.inputID]
    poseStampedArray_locks[req.inputID].release()
    return posearray_srvResponse(posearray_return)


def handle_transarray_srv(req):
    print( "ID : {}".format(req.inputID) )
    tranStampedArray_locks[req.inputID].acquire()
    transarray_return = tranStampedArray_sets[req.inputID]
    tranStampedArray_locks[req.inputID].release()
    return transarray_srvResponse(transarray_return)

def interposecallback(msg):
    global poseStampedArray_sets, poseStampedArray_locks, self_ID,posearray_pub_test
    current_frame_id = int(msg.poseArray[1].header.frame_id)
    current_robot_id = int(current_frame_id / 1e8)
    assert(current_robot_id != self_ID)
    if not (poseStampedArray_sets.has_key(str(current_robot_id)) ):
        poseStampedArray_locks[str(current_robot_id)] = threading.Lock()
    poseStampedArray_locks[str(current_robot_id)].acquire()
    poseStampedArray_sets[str(current_robot_id)] = msg
    poseStampedArray_locks[str(current_robot_id)].release()
    # posearray = PoseArray()
    # posearray.header.frame_id = "map"

    # trackutils.StampedArray2PoseArray(poseStampedArray_sets[ str(current_robot_id ) ], posearray)
    # posearray_pub_test.publish(posearray)

def intertranscallback(msg):
    global self_ID, tranStampedArray_locks, tranStampedArray_sets, posearray_pub_test
    current_frame_id = int(msg.transformArray[0].child_frame_id)
    current_robot_id = int(current_frame_id / 1e8)
    assert(current_robot_id != self_ID)
    if not tranStampedArray_sets.has_key(str(current_robot_id)):
        tranStampedArray_locks[str(current_robot_id)] = threading.Lock()
    tranStampedArray_locks[str(current_robot_id)].acquire()
    tranStampedArray_sets[str(current_robot_id)]  =msg
    tranStampedArray_locks[str(current_robot_id)].release()


    # posestampedarray = PoseStampedArray()
    # posearray = PoseArray()
    # posearray.header.frame_id = "map"
    # trackutils.AccReltrans2PoseStampedArray(tranStampedArray_sets[str(current_robot_id)] ,posestampedarray)
    # trackutils.StampedArray2PoseArray(posestampedarray, posearray)
    # posearray_pub_test.publish(posearray)
    
    



def main(argv):
    global posearray_pub,self_ID,tranStampedArray_sets,poseStampedArray_sets,poseStampedArray_locks,LooptransArray_sets,LooptransArray_locks,posearray_pub_test,pose_with_image_pub, map_pub, withmap, br, listener
    opts, args = getopt.getopt(argv,"i:t")
    for opt, arg in opts:
        if opt in ("-i"):
            self_ID = int(arg)
        if opt in ("-t"):
            withmap = False
            
    
    tranStampedArray_sets[ str(self_ID)] =  TransformStampedArray()
    tranStampedArray_locks[str(self_ID)] = threading.Lock()
    poseStampedArray_sets[str(self_ID)] = PoseStampedArray()
    poseStampedArray_locks[str(self_ID)] = threading.Lock()
    LooptransArray_sets[str(self_ID)] = TransformStampedArray()
    LooptransArray_locks[str(self_ID)] = threading.Lock()
    
    signal.signal(signal.SIGINT, trackutils.quit)                                
    signal.signal(signal.SIGTERM, trackutils.quit)
    rospy.init_node('generate_track_py', anonymous=True)
    rospy.Subscriber("relpose", TransformStamped_with_image, self_track_pose_cb,queue_size=100)
    # srvhandle = rospy.Service('/robot{}/posearray_srv'.format(self_ID), posearray_srv, handle_posearray_srv)
    srvhandle = rospy.Service('/robot{}/transarray_srv'.format(self_ID), transarray_srv, handle_transarray_srv)
    rospy.Subscriber("looppose", TransformStamped, callback_loop)
    # rospy.Subscriber("/robot{}/loopinterpose".format(self_ID), PoseStampedArray, interposecallback )
    # rospy.Subscriber("/robot{}/loopintertrans".format(self_ID), TransformStampedArray, intertranscallback )
    posearray_pub = rospy.Publisher("posearray",PoseArray, queue_size=3)
    posearray_pub_test = rospy.Publisher("/robot{}/posearray_test".format(self_ID), PoseArray, queue_size=3)
    pose_with_image_pub = rospy.Publisher("pose_with_image", Pose_with_image, queue_size=3)
    map_pub = rospy.Publisher("/robot{}/octomap_in".format(self_ID), Octomap, queue_size=3)

    listener = tf.TransformListener()

    maintain_tf_tree()

    backt = threading.Thread(target=BackendThread)
    self_tran_t = threading.Thread(target=process_self)
    backt.setDaemon(True)
    self_tran_t.setDaemon(True)
    backt.start()
    self_tran_t.start()
    rospy.spin()


if __name__ == '__main__':
    main(sys.argv[1:])
#     # k1 = Pose()
#     # k1.orientation.w = 1
#     # k2 = Pose()
#     # k2.orientation.w = 1

#     # f = posemath.fromMsg(k1) * posemath.fromMsg(k2)
#     # print f

#     # [x, y, z, w] = f.M.GetQuaternion()
#     # print x,y,z,w

#     # pose = posemath.toMsg(f)
#     # print pose







    