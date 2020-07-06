#!/usr/bin/env python
#coding=utf-8
import rospy
import struct
import tf
import numpy as np
from tf_conversions import posemath
import PyKDL as kdl
from cv_bridge import CvBridge, CvBridgeError
#导入自定义的数据类型
from dslam_sp.msg import Pose_with_image
from sensor_msgs.msg import PointCloud2, PointField
from geometry_msgs.msg import TransformStamped

class PointCloud_Publisher:

    def __init__(self):
        self.pointcloud_pub = rospy.Publisher("pointcloud_part_local", PointCloud2, queue_size=1)
        #Subscriber函数第一个参数是topic的名称，第二个参数是接受的数据类型 第三个参数是回调函数的名称
        self.image_sub = rospy.Subscriber("pose_with_image", Pose_with_image, self.callback)
        self.bridge = CvBridge()
        self.br = tf.TransformBroadcaster()

    #回调函数输入的应该是msg
    def callback(self, data):
        camera_cx    =   data.P[2]
        camera_cy    =   data.P[6]
        camera_fx     =   data.P[0]
        camera_fy     =   data.P[5]
        camera_scale  = 1000.0
        
        cv_image = self.bridge.imgmsg_to_cv2(data.image, desired_encoding="passthrough")
        cv_depth = self.bridge.imgmsg_to_cv2(data.depth, desired_encoding="passthrough")
        height = cv_depth.shape[0]
        weight = cv_depth.shape[1]

        buffer = []
        for row in range(height):            #遍历高
            for col in range(weight):         #遍历宽
                d = cv_depth[row, col]
                Luma = cv_image[row, col]
                if d == 0:
                    continue
                z = d / camera_scale
                x = (col - camera_cx) * z / camera_fx
                y = (row - camera_cy) * z / camera_fy
                if abs(y) > 0.1 :
                    continue
                buffer.append(struct.pack('ffff', x, y, z, Luma))
                # point = kdl.Frame(kdl.Vector(x, y, z))
                # point_global = posemath.fromMsg( data.pose ) * point
                # buffer.append(struct.pack('ffff', point_global.p.x(), point_global.p.y(), point_global.p.z(), Luma))

        ros_msg = PointCloud2()

        ros_msg.header = data.header
        # ros_msg.header.frame_id = "odom"
        print( ros_msg.header )

        ros_msg.height = 1
        ros_msg.width = len(buffer)

        ros_msg.fields.append(PointField(name="x", offset=0, datatype=PointField.FLOAT32, count=1))
        ros_msg.fields.append(PointField(name="y", offset=4, datatype=PointField.FLOAT32, count=1))
        ros_msg.fields.append(PointField(name="z", offset=8, datatype=PointField.FLOAT32, count=1))
        ros_msg.fields.append(PointField(name="rgb", offset=12, datatype=PointField.FLOAT32, count=1))

        ros_msg.is_bigendian = False
        ros_msg.point_step = 16
        ros_msg.row_step = ros_msg.point_step * ros_msg.width * ros_msg.height
        ros_msg.is_dense = False
        ros_msg.data = "".join(buffer)

        self.pointcloud_pub.publish(ros_msg)

        # tf_msg = TransformStamped()
        # tf_msg.header = data.header
        # tf_msg.header.frame_id = "odom"
        # tf_msg.child_frame_id = "map"
        # tf_msg.transform.translation = data.pose.position
        # tf_msg.transform.rotation = data.pose.orientation
        # print(tf_msg)
        # self.br.sendTransformMessage(tf_msg)


def main():
    rospy.init_node('rgbd_to_pointcloud', anonymous=True)
    pcp = PointCloud_Publisher()
    rospy.spin()

if __name__ == '__main__':
    main()