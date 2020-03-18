#!/usr/bin/env python
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

meta = {'outputdim': 2048}

representors_a1 = np.zeros((0,meta['outputdim']),dtype=float)
frameIDstr = []
looptrans_pub = None 
looptrans_name_pub = None 

def matchrepresentor(query, matchs):
    results = np.matmul(matchs, query)
    return results

def callback(data):
    global representors_a1,looptrans_name_pub,looptrans_pub
    transarray = TransformStampedArray()
    imageHeader = data.imageHeader
    imageFrameID = imageHeader.frame_id
    representor = data.representor
    matchresults = matchrepresentor (representor, representors_a1)
    # print ("matchresults")
    # print (matchresults)
    # print ("np.array(representor)")
    # print (np.array(representor))
    representors_a1 = np.vstack((representors_a1 , np.array(representor) ) )
    frameIDstr.append(imageFrameID)
    if ( len(matchresults) > 30):
        # print ("array > 30")
        validresults = matchresults[0:-30]
        validmatch = validresults>0.95
        if ( validmatch.any() ):
            # print ("array valid")
            maxargs = np.argmax(validresults)
            # match_relpose = TransformStamped()
            # match_relpose.header.frame_id = imageFrameID
            # match_relpose.child_frame_id = frameIDstr[maxargs]
            # match_relpose.transform.rotation.w = 1 
            # match_relpose.transform.rotation.x = 0 
            # match_relpose.transform.rotation.y = 0 
            # match_relpose.transform.rotation.z = 0 
            # match_relpose.transform.translation.x = 0
            # match_relpose.transform.translation.y = 0
            # match_relpose.transform.translation.z = 0
            # transarray.transformArray.append(match_relpose)

            matchpair = "{id1} {id2}".format(id1 = frameIDstr[maxargs], id2 = imageFrameID)
            # looptrans_pub.publish(match_relpose) # to publish relpose
            print (matchpair)
            looptrans_name_pub.publish(matchpair) # to publish rel fnames
            tmp = 1
    
    tmp = 1
    # print( representor )
    # print( type(representor) )
    


def main():
    global looptrans_pub, looptrans_name_pub
    rospy.init_node('matcher', anonymous=True)
    rospy.Subscriber("PRrepresentor", PRrepresentor, callback)
    looptrans_pub = rospy.Publisher("looppose_deee",TransformStamped, queue_size=3)
    looptrans_name_pub = rospy.Publisher("loopfnames",String, queue_size=3)
    rospy.spin()



if __name__ == '__main__':
    main()