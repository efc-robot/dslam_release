#!/usr/bin/env python
# -*- coding: utf-8 -*- 
import numpy as np
import rospy
from dslam_sp.msg import PRrepresentor
from dslam_sp.srv import keyframe_srv
import threading
import sys, getopt 

meta = {'outputdim': 2048} 

def matchrepresentor(query, matchs):
    results = np.matmul(matchs, query)
    return results

class Key_Frame:
    def __init__(self, self_ID):
        self.self_ID = self_ID
        self.key_frame_ids = []
        self.key_frame_representors = np.zeros((0,meta['outputdim']),dtype=float)
        self.key_frame_locks = threading.Lock()
        self.PR_sub = rospy.Subscriber("PRrepresentor", PRrepresentor, self.callback)
        self.srvhandle = rospy.Service('keyframe_srv'.format(self_ID), keyframe_srv, self.handle_keyframe_srv)

    def callback(self, data):
        imageHeader = data.imageHeader
        imageFrameID = imageHeader.frame_id
        current_robot_id = int(int(imageFrameID)/1e8)
        # assert(current_robot_id == self_ID)
        representor = data.representor
        
        self.key_frame_locks.acquire()
        if current_robot_id == self.self_ID:
            if len(self.key_frame_ids) == 0:
                self.key_frame_representors = np.vstack(( self.key_frame_representors , np.array(representor) ) )
                self.key_frame_ids.append(imageFrameID)
            else:
                last_keyframe_rep = self.key_frame_representors[-1]
                print("np.matmul(representor, last_keyframe_rep)")
                print(np.matmul(representor, last_keyframe_rep))
                print("np.matmul")
                if np.matmul(representor, last_keyframe_rep) < 0.95:
                    self.key_frame_representors = np.vstack(( self.key_frame_representors , np.array(representor) ) )
                    self.key_frame_ids.append(imageFrameID)
        self.key_frame_locks.release()

    def handle_keyframe_srv(req):
        ID_return = "None"
        print("frameID : {}".format(req.inputID))
        self.key_frame_locks.acquire()
        for rep in reversed(self.key_frame_representors):
            if int(rep)<=int(req.inputID):
                ID_return = rep
        self.key_frame_locks.release()
        print("key_frameID : {}".format(ID_return))
        return keyframe_srvResponse(ID_return)


def main(argv):
    self_ID = 1
    # frame 规则是 self_ID*1e8 + framecount
    opts, args = getopt.getopt(argv,"i:")
    for opt, arg in opts:
        if opt in ("-i"):
            self_ID = int(arg)
    
    rospy.init_node('key_frame', anonymous=True)
    kf = Key_Frame(self_ID)
    rospy.spin()

if __name__ == '__main__':
    main(sys.argv[1:])