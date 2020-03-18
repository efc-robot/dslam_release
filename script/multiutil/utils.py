# -*- coding: utf-8 -*- 

import numpy as np
from PIL import Image
import os
import rospy
from geometry_msgs.msg import TransformStamped
from dslam_sp.msg import image_depth, PRrepresentor, TransformStampedArray, MatchedFrame
from cv_bridge import CvBridge, CvBridgeError
import cv2
import sys, getopt 

def FramesfromFname():
    pass