#!/usr/bin/env python
#coding=utf-8
import rospy
from cv_bridge import CvBridge, CvBridgeError
#导入自定义的数据类型
from dslam_sp.msg import image_depth

class Frame_Saver:

    def __init__(self):
        #Subscriber函数第一个参数是topic的名称，第二个参数是接受的数据类型 第三个参数是回调函数的名称
        self.image_sub = rospy.Subscriber("image_depth", image_depth, self.callback)
        self.bridge = CvBridge()

    #回调函数输入的应该是msg
    def callback(self, data):
        cv_image = self.bridge.imgmsg_to_cv2(data.image, desired_encoding="passthrough")
        cv_depth = self.bridge.imgmsg_to_cv2(data.depth, desired_encoding="passthrough")
        
        


def main():
    rospy.init_node('rgbd_to_pointcloud', anonymous=True)
    pcp = PointCloud_Publisher()
    rospy.spin()

if __name__ == '__main__':
    main()