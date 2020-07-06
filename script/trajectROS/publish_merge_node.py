#!/usr/bin/env python
#coding=utf-8
import rospy
from dslam_sp.msg import image_depth

class PointCloud_Publisher:

    def __init__(self):
        self.merge_pub = rospy.Publisher("pointcloud_part_global",image_depth)
        #Subscriber函数第一个参数是topic的名称，第二个参数是接受的数据类型 第三个参数是回调函数的名称
        self.bridge = CvBridge()

def main():
    rospy.init_node('publish_merge', anonymous=True)
    pub = Merge_Publisher()
    rospy.spin()

if __name__ == '__main__':
    main()