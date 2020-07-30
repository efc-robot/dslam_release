#!/usr/bin/env python
#coding=utf-8
import rospy
import os
import cv2
import sys, getopt
from pathlib import Path
from cv_bridge import CvBridge, CvBridgeError
#导入自定义的数据类型
from dslam_sp.msg import image_depth

class Frame_Saver:

    def __init__(self, FileDir):
        self.FileDir = Path(FileDir)
        if not self.FileDir.exists():
            os.makedirs(self.FileDir)
        #Subscriber函数第一个参数是topic的名称，第二个参数是接受的数据类型 第三个参数是回调函数的名称
        self.image_sub = rospy.Subscriber("image_depth", image_depth, self.callback)
        self.bridge = CvBridge()

    #回调函数输入的应该是msg
    def callback(self, data):
        cv_image = self.bridge.imgmsg_to_cv2(data.image, desired_encoding="passthrough")
        cv_depth = self.bridge.imgmsg_to_cv2(data.depth, desired_encoding="passthrough")
        image_name = Path(self.FileDir, "left_{}.png".format(int(int(data.header.frame_id)%1e8)))
        depth_name = Path(self.FileDir, "depth_{}.png".format(int(int(data.header.frame_id)%1e8)))
        print("save "+str(image_name))
        print("cv_depth.type:" + str(type(cv_depth)))
        print("cv_depth.dtype:" + str(cv_depth.dtype))
        print("cv_depth" + str(cv_depth))
        if cv_depth.dtype!="int":
            cv_depth = cv_depth * 1000
            cv_depth = cv_depth.astype("uint16")
        print("cv_depth.dtype:" + str(cv_depth.dtype))
        print("cv_depth" + str(cv_depth))
        cv2.imwrite(str(image_name),cv_image)
        cv2.imwrite(str(depth_name),cv_depth)
        print("save finish")



def main(argv):
    FileDir = "/PATH/TO/DATASET/"
    opts, args = getopt.getopt(argv,"p:")
    for opt, arg in opts:
        if opt in ("-p"):
            FileDir = arg
    rospy.init_node('save_frame_file', anonymous=True)
    fs = Frame_Saver(FileDir)
    rospy.spin()

if __name__ == '__main__':
    main(sys.argv[1:])