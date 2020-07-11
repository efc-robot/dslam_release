#!/usr/bin/env python  
# -*- coding:utf-8 -*-  
import rospy  
import math  
import tf   
if __name__ == '__main__':  
    frame_id1 = "map1"
    frame_id2 = "odom2"
    rospy.init_node('py_tf_turtle')
    listener = tf.TransformListener() #TransformListener创建后就开始接受tf广播信息，最多可以缓存10s  目前存在的问题，是四个数值的顺序我还有点问题
    rate = rospy.Rate(1.0)  
    # #1. 阻塞直到frame相通
    # print '1. 阻塞直到frame相通'  
    # listener.waitForTransform(frame_id1, frame_id2, rospy.Time(), rospy.Duration(4.0))
    while not rospy.is_shutdown():  
        #2. 判断两个frame是否相通
        print '2. 判断两个frame是否相通'
        if listener.canTransform(frame_id1, frame_id2, rospy.Time(0)) :
            print 'true'
        else :
            print 'false'
        try:  
        #3. 监听对应的tf,返回平移和旋转
            print '3. 监听对应的tf,返回平移和旋转'  
            (trans,rot) = listener.lookupTransform(frame_id1, frame_id2, rospy.Time(0)) #rospy.Time(0)不表示0时刻的tf，而是指最近一帧tf 
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):  
            rate.sleep()
            continue    
        rospy.loginfo('距离原点的位置: x=%f ,y= %f，z=%f \n 旋转四元数: w=%f ,x= %f，y=%f z=%f ',trans[0],trans[1],trans[2],rot[0],rot[1],rot[2],rot[3])
        (trans,rot) = (0, 0)
        rate.sleep()