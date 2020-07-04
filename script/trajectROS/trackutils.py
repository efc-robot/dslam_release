# -*- coding: utf-8 -*- 
import numpy as np
from PIL import Image
import os
import rospy
from tf_conversions import posemath
from dslam_sp.msg import image_depth, PRrepresentor, TransformStampedArray, PoseStampedArray
import tf2_ros
from geometry_msgs.msg import TransformStamped, PoseStamped, Pose, PoseArray, Transform
from cv_bridge import CvBridge, CvBridgeError
import cv2
import time
import sys                                                              
import signal
import copy
import gtsam
 
COVAR_STR = "1.000000 0.000000 0.000000 0.000000 0.000000 0.000000   1.000000 0.000000 0.000000 0.000000 0.000000   1.000000 0.000000 0.000000 0.000000   1.000000 0.000000 0.000000   1.000000 0.000000   1.000000"


def quit(signum, frame):
    print ''
    print 'stop fusion'
    sys.exit()

def isrot(R, tol):
    if not ( R.shape[0] ==3 and R.shape[1] ==3 ):
        print("R is not 3x3")
        return False
    else :
        RTR = R*R.transpose()
        resmat = RTR - np.eye(3)
        res = np.linalg.norm(resmat)
        if res > tol:
            print("R is not orthonormal, i.e. ||(R''R-I)|| > {}".format(tol) )
            print(R)
            return False
        elif (np.linalg.det(R) < 0 ):
            print("R determinant < 0")
            return False
        return True


def np2Transform(nparray):
    npmat = nparray.reshape((3,4))
    R = npmat[0:3,0:3]
    assert ( isrot(R,10) )
    # T = npmat[0:3,3]
    posetmp = posemath.fromMatrix(npmat)
    transformtmp = posemath.toMsg(posetmp)
    return transformtmp

def gstamPose2Transform(gstamPose):
    npmat = np.zeros((3,4))
    npmat[0:3,0:3] = gstamPose.rotation().matrix()
    npmat[0,3] = gstamPose.translation().x()
    npmat[1,3] = gstamPose.translation().y()
    npmat[2,3] = gstamPose.translation().z()
    posetmp = posemath.fromMatrix(npmat)
    transformtmp = posemath.toMsg(posetmp)
    return transformtmp


def RTtxt2Posearray(txtfname, pose_array):
    posearrays = np.loadtxt(txtfname,dtype=np.float32)
    assert(posearrays.shape[1] == 12)
    index = 1
    for lines in posearrays:
        print(index)
        pose_array.poses.append(np2Transform(lines) )
        index += 1
    return pose_array


def RTtxt2PoseStampedarray(txtfname, posestamped_array):
    posearrays = np.loadtxt(txtfname,dtype=np.float32)
    assert(posearrays.shape[1] == 12)
    index = 1
    for lines in posearrays:
        print(index)
        posestampedtmp = PoseStamped()
        posestampedtmp.header.frame_id = str(index)
        posestampedtmp.pose = np2Transform(lines)
        posestamped_array.poses.append( posestampedtmp )
        index += 1
    return posestamped_array

def RTtxt2Transarray(txtfname, trans_array):
    posearrays = np.loadtxt(txtfname,dtype=np.float32)
    assert(posearrays.shape[1] == 12)
    index = 1
    for lines in posearrays:
        print(index)
        tramstamped = TransformStamped()
        tramstamped.transform = pose2trans( np2Transform(lines) )
        tramstamped.header.frame_id = str(index)
        tramstamped.child_frame_id = str(index - 1)
        trans_array.transformArray.append( tramstamped )
        index += 1
    return trans_array


def Posearray2G2O(g2ofname, pose_array):
    pointID = 0
    index = 1
    prev_pose = Pose()
    prev_pose.orientation.w = 1
    rel_pose = Pose()

    with open(g2ofname, "w") as g2ofile:
        for pose in pose_array.poses:
            if index == 1:
                pass
            else :
                rel_pose = posemath.toMsg ( (posemath.fromMsg(prev_pose).Inverse() * posemath.fromMsg(pose)) )
                # tmp = posemath.toMsg( posemath.fromMsg(prev_pose)*posemath.fromMsg(rel_pose) )
                prev_pose = pose
                print >> g2ofile, "EDGE_SE3:QUAT {id1} {id2} {tx} {ty} {tz} {rx} {ry} {rz} {rw}  {COVAR_STR}".format(id1=pointID-1,id2=pointID, tx =rel_pose.position.x , ty =rel_pose.position.y , tz =rel_pose.position.z, rx =rel_pose.orientation.x, ry =rel_pose.orientation.y, rz =rel_pose.orientation.z, rw =rel_pose.orientation.w, COVAR_STR=COVAR_STR)
            
            print >> g2ofile, "VERTEX_SE3:QUAT {pointID} {tx} {ty} {tz} {rx} {ry} {rz} {rw}".format(pointID=pointID,tx=pose.position.x,ty=pose.position.y,tz=pose.position.z, 
                rx = pose.orientation.x, ry = pose.orientation.y, rz = pose.orientation.z, rw = pose.orientation.w)

            
            pointID += 1
            index += 1


def PoseStampedarray2G2O(g2ofname, pose_array, add=False):
    if add:
        openmethod = "a"
    else:
        openmethod = "w"
    with open(g2ofname, openmethod) as g2ofile:
        for posestamped in pose_array.poseArray:
            print >> g2ofile, "VERTEX_SE3:QUAT {pointID} {tx} {ty} {tz} {rx} {ry} {rz} {rw}".format(pointID=posestamped.header.frame_id,tx=posestamped.pose.position.x,ty=posestamped.pose.position.y,tz=posestamped.pose.position.z, 
                rx = posestamped.pose.orientation.x, ry = posestamped.pose.orientation.y, rz = posestamped.pose.orientation.z, rw = posestamped.pose.orientation.w)

def AddTransArray2G2O(g2ofname, trans_array):
    with open(g2ofname, "a") as g2ofile:
        for transstamped in trans_array.transformArray:
            print >> g2ofile, "EDGE_SE3:QUAT {id1} {id2} {tx} {ty} {tz} {rx} {ry} {rz} {rw}  {COVAR_STR}".format(id1=transstamped.child_frame_id,id2=transstamped.header.frame_id, tx =transstamped.transform.translation.x , ty =transstamped.transform.translation.y , tz =transstamped.transform.translation.z, rx =transstamped.transform.rotation.x, ry =transstamped.transform.rotation.y, rz =transstamped.transform.rotation.z, rw =transstamped.transform.rotation.w, COVAR_STR=COVAR_STR)


def trans2pose(trans):
    pose = Pose()
    pose.orientation = trans.rotation
    pose.position = trans.translation
    return pose


def pose2trans(pose):
    trans = Transform()
    trans.rotation = pose.orientation
    trans.translation =pose.position
    return trans

def Transarray2G2O(g2ofname, trans_array):
    index = 1
    prev_pose = Pose()
    prev_pose.orientation.w = 1
    rel_pose = Pose()

    with open(g2ofname, "w") as g2ofile:
        for posetrans in trans_array.transformArray:
            pose = trans2pose(posetrans.transform)
            if index == 1:
                pass
            else :
                rel_pose = posemath.toMsg ( (posemath.fromMsg(prev_pose).Inverse() * posemath.fromMsg(pose)) )
                # tmp = posemath.toMsg( posemath.fromMsg(prev_pose)*posemath.fromMsg(rel_pose) )
                prev_pose = pose
                print >> g2ofile, "EDGE_SE3:QUAT {id1} {id2} {tx} {ty} {tz} {rx} {ry} {rz} {rw}  {COVAR_STR}".format(id1=posetrans.child_frame_id,id2=posetrans.header.frame_id, tx =rel_pose.position.x , ty =rel_pose.position.y , tz =rel_pose.position.z, rx =rel_pose.orientation.x, ry =rel_pose.orientation.y, rz =rel_pose.orientation.z, rw =rel_pose.orientation.w, COVAR_STR=COVAR_STR)
            
            # print >> g2ofile, "VERTEX_SE3:QUAT {pointID} {tx} {ty} {tz} {rx} {ry} {rz} {rw}".format(pointID=posetrans.header.frame_id,tx=pose.position.x,ty=pose.position.y,tz=pose.position.z, 
            #     rx = pose.orientation.x, ry = pose.orientation.y, rz = pose.orientation.z, rw = pose.orientation.w)

            index += 1




def AccReltrans2PoseStampedArray(reltrans_array, posestamped_array):
    poseinit = PoseStamped()
    poseinit.header.frame_id = reltrans_array.transformArray[0].child_frame_id
    poseinit.pose.orientation.w = 1
    nowframe = poseinit.header.frame_id
    posestamped_array.poseArray.append(poseinit)
    current_pose = Pose()
    current_pose.orientation.w = 1
    for trans in reltrans_array.transformArray:
        assert (trans.child_frame_id == nowframe)
        nowframe = trans.header.frame_id
        posetmp = PoseStamped()
        posetmp.header.frame_id = nowframe
        current_pose =  posemath.toMsg ( posemath.fromMsg(current_pose) *  posemath.fromMsg( trans2pose(trans.transform ) ) )
        posetmp.pose = current_pose
        posestamped_array.poseArray.append (posetmp)
    return posestamped_array

def StampedArray2PoseArray(posestamped_array, pose_array):
    for posestramped in posestamped_array.poseArray:
        pose_array.poses.append(posestramped.pose)
    return pose_array

def VOPoseArray2MapPoseArray(vo_pose_array, map_pose_array):
    init_pose = Pose()
    init_pose.orientation.x = 0.7071
    init_pose.orientation.w = 0.7071
    for pose in vo_pose_array.poses:
        map_pose_array.poses.append(posemath.toMsg( posemath.fromMsg( init_pose ) *  posemath.fromMsg( pose ) ))
    return map_pose_array


def Globalarray2Relarray(global_array, rel_array):
    prev_pose = Pose()
    prev_pose.orientation.w = 1
    current_pose = Pose()
    current_pose.orientation.w = 1
    rel_pose = Pose()
    posearray = PoseArray()
    index = 1
    RT_array = []
    for pose in global_array.poses:
        if index == 1:
            pass
        else :
            rel_pose = posemath.toMsg ( (posemath.fromMsg(prev_pose).Inverse() * posemath.fromMsg(pose)) )
            rel_array.poses.append(rel_pose)
            prev_pose = pose

        index+=1
    return rel_array
    
def appendTrans2TransArray(trans, trans_array, withassert = True):
    if not trans_array.transformArray:
        trans_array.transformArray.append(trans)
    elif not withassert:
        trans_array.transformArray.append(trans)
    else:
        print("match id1:{} ; id2:{}".format(trans_array.transformArray[-1].header.frame_id,trans.child_frame_id) )
        assert(trans_array.transformArray[-1].header.frame_id == trans.child_frame_id)
        trans_array.transformArray.append(trans)

def appendTrans2PoseStampedArray(trans, pose_array):
    if not pose_array.poseArray:
        posetmp = PoseStamped()
        posetmp.header.frame_id = trans.child_frame_id
        # posetmp.pose.orientation.x = 0.7071
        posetmp.pose.orientation.w = 1 #0.7071
        pose_array.poseArray.append(posetmp)
        
    match = (pose_array.poseArray[-1].header.frame_id == trans.child_frame_id)
    print("match id1:{} ; id2:{}".format(pose_array.poseArray[-1].header.frame_id,trans.child_frame_id) )
    assert(match)
    posetmp = PoseStamped()
    posetmp.header.frame_id = trans.header.frame_id
    posetmp.pose = posemath.toMsg( posemath.fromMsg( pose_array.poseArray[-1].pose ) *  posemath.fromMsg( trans2pose( trans.transform ) ) )
    pose_array.poseArray.append(posetmp)


def Global2Rel(globalfname, pose_array):
    global_array = PoseArray()
    RTtxt2Posearray(globalfname,global_array)
    Globalarray2Relarray(global_array, pose_array)
    return pose_array

def Global2Relfile(globalfname, relfname):
    pose_array = PoseArray()
    Global2Rel(globalfname, pose_array)
    with open(relfname, "w") as relfile:
        for pose in pose_array.poses:
            outarray = posemath.toMatrix( posemath.fromMsg(pose) )
            outonedim = outarray[0:3,:].reshape((12))
            for index in range(11):
                relfile.write("{} ".format(outonedim[index]) )
            relfile.write("{}\n".format(outonedim[11]) )
        relfile.close()

def Relarray2Globalarray(rel_array, global_array):
    current_pose = Pose()
    current_pose.orientation.w = 1
    global_array.poses.append(current_pose)
    for pose in rel_array.poses:
        current_pose = posemath.toMsg( posemath.fromMsg(current_pose) *  posemath.fromMsg(pose) )
        global_array.poses.append(current_pose)
    return global_array


def Rel2Global(relfname, pose_array):
    rel_array = PoseArray()
    RTtxt2Posearray(relfname, rel_array)
    Relarray2Globalarray(rel_array, pose_array)


def Rel2Globalfile(relfname, globalfname ):
    pose_array = PoseArray()
    Rel2Global(relfname, pose_array)
    with open(globalfname, "w") as relfile:
        for pose in pose_array.poses:
            outarray = posemath.toMatrix( posemath.fromMsg(pose) )
            outonedim = outarray[0:3,:].reshape((12))
            for index in range(11):
                relfile.write("{} ".format(outonedim[index]) )
            relfile.write("{}\n".format(outonedim[11]) )
        relfile.close()


def Loop2G2O(g2ofname, transform_loops):
    with open(g2ofname, "a") as g2ofile:
        for transform_loop in transform_loops:
            print >> g2ofile, "EDGE_SE3:QUAT {id1} {id2} {tx} {ty} {tz} {rx} {ry} {rz} {rw}  {COVAR_STR}".format(id1=transform_loop.child_frame_id,id2=transform_loop.header.frame_id, tx =transform_loop.transform.translation.x , ty =transform_loop.transform.translation.y , tz =transform_loop.transform.translation.z, rx =transform_loop.transform.rotation.x, ry =transform_loop.transform.rotation.y, rz =transform_loop.transform.rotation.z, rw =transform_loop.transform.rotation.w, COVAR_STR=COVAR_STR)

def G2O2Posearray(g2ofname,pose_array):
    with open(g2ofname) as g2ofile:
        for line in g2ofile:
            spltline = line.split(' ')
            # print(spltline)
            if (spltline[0] == 'VERTEX_SE3:QUAT'):
                nparray = np.array(spltline[2:9]).astype(float)
                tmppose = Pose()
                tmppose.position.x = nparray[0]
                tmppose.position.y = nparray[1]
                tmppose.position.z = nparray[2]
                tmppose.orientation.x = nparray[3]
                tmppose.orientation.y = nparray[4]
                tmppose.orientation.z = nparray[5]
                tmppose.orientation.w = nparray[6]
            pose_array.poses.append(tmppose)
    return pose_array


def vector6(x, y, z, a, b, c):
    """Create 6d double numpy array."""
    return np.array([x, y, z, a, b, c], dtype=np.float)

def gtsamOpt(inputfname, outputfname):

    graph, initial = gtsam.readG2o(inputfname, True)
        # Add Prior on the first key
    priorModel = gtsam.noiseModel_Diagonal.Variances(vector6(1e-6, 1e-6, 1e-6,
                                                            1e-4, 1e-4, 1e-4))

    print("Adding prior to g2o file ")
    graphWithPrior = graph
    firstKey = initial.keys().at(0)
    graphWithPrior.add(gtsam.PriorFactorPose3(firstKey, gtsam.Pose3(), priorModel))

    params = gtsam.GaussNewtonParams()
    params.setVerbosity("Termination")  # this will show info about stopping conds
    optimizer = gtsam.GaussNewtonOptimizer(graphWithPrior, initial, params)
    result = optimizer.optimize()
    print("Optimization complete")

    print("initial error = ", graphWithPrior.error(initial))
    print("final error = ", graphWithPrior.error(result))

    print("Writing results to file: ", outputfname)
    graphNoKernel, _ = gtsam.readG2o(inputfname, True)
    gtsam.writeG2o(graphNoKernel, result, outputfname)
    print ("Done!")

def gtsamOpt2Posearray(inputfname, pose_array):
    pointID = 0
    graph, initial = gtsam.readG2o(inputfname, True)
        # Add Prior on the first key
    priorModel = gtsam.noiseModel_Diagonal.Variances(vector6(1e-6, 1e-6, 1e-6,
                                                            1e-4, 1e-4, 1e-4))

    print("Adding prior to g2o file ")
    graphWithPrior = graph
    firstKey = initial.keys().at(0)
    graphWithPrior.add(gtsam.PriorFactorPose3(firstKey, gtsam.Pose3(), priorModel))

    params = gtsam.GaussNewtonParams()
    params.setVerbosity("Termination")  # this will show info about stopping conds
    optimizer = gtsam.GaussNewtonOptimizer(graphWithPrior, initial, params)
    result = optimizer.optimize()
    print("Optimization complete")

    print("initial error = ", graphWithPrior.error(initial))
    print("final error = ", graphWithPrior.error(result))


    resultPoses = gtsam.allPose3s(result)
    for i in range(pointID, pointID+resultPoses.size()):
        pose_array.poses.append(gstamPose2Transform(resultPoses.atPose3(i) ) )
    return pose_array


def gtsamOpt2PoseStampedarray(inputfname, pose_array):
    pointID = 0
    graph, initial = gtsam.readG2o(inputfname, True)
        # Add Prior on the first key
    priorModel = gtsam.noiseModel_Diagonal.Variances(vector6(1e-6, 1e-6, 1e-6,
                                                            1e-4, 1e-4, 1e-4))

    print("Adding prior to g2o file ")
    graphWithPrior = graph
    firstKey = initial.keys().at(0)
    graphWithPrior.add(gtsam.PriorFactorPose3(firstKey, gtsam.Pose3(), priorModel))

    params = gtsam.GaussNewtonParams()
    params.setVerbosity("Termination")  # this will show info about stopping conds
    optimizer = gtsam.GaussNewtonOptimizer(graphWithPrior, initial, params)
    result = optimizer.optimize()
    print("Optimization complete")

    print("initial error = ", graphWithPrior.error(initial))
    print("final error = ", graphWithPrior.error(result))


    resultPoses = gtsam.allPose3s(result)
    keys = resultPoses.keys()
    for i in range(keys.size()):
        posetmp = gstamPose2Transform(resultPoses.atPose3(keys.at(i) ) )
        posestampedtmp = PoseStamped()
        posestampedtmp.header.frame_id = str(keys.at(i) )
        posestampedtmp.pose = posetmp
        pose_array.poseArray.append(posestampedtmp)


def gtsamOpt2PoseStampedArraySet(inputfname, pose_array_set):
    #pose_array_set得是空的
    pointID = 0
    graph, initial = gtsam.readG2o(inputfname, True)
        # Add Prior on the first key
    priorModel = gtsam.noiseModel_Diagonal.Variances(vector6(1e-6, 1e-6, 1e-6,
                                                            1e-4, 1e-4, 1e-4))

    print("Adding prior to g2o file ")
    graphWithPrior = graph
    firstKey = initial.keys().at(0)
    graphWithPrior.add(gtsam.PriorFactorPose3(firstKey, gtsam.Pose3(), priorModel))

    params = gtsam.GaussNewtonParams()
    params.setVerbosity("Termination")  # this will show info about stopping conds
    optimizer = gtsam.GaussNewtonOptimizer(graphWithPrior, initial, params)
    result = optimizer.optimize()
    print("Optimization complete")

    print("initial error = ", graphWithPrior.error(initial))
    print("final error = ", graphWithPrior.error(result))


    resultPoses = gtsam.allPose3s(result)
    keys = resultPoses.keys()
    for i in range(keys.size()):
        posetmp = gstamPose2Transform(resultPoses.atPose3(keys.at(i) ) )
        posestampedtmp = PoseStamped()
        posestampedtmp.header.frame_id = str(keys.at(i) )
        current_robot_id = int(int(keys.at(i)) / 1e8)
        posestampedtmp.pose = posetmp
        if not (pose_array_set.has_key(str(current_robot_id))):
             pose_array_set[str(current_robot_id)] = PoseStampedArray()
        pose_array_set[str(current_robot_id)].poseArray.append(posestampedtmp)


def checkHeaderID(tragetID, posestampedarray):
    for index,posestamped in enumerate(posestampedarray.poseArray):
        if posestamped.header.frame_id == tragetID:
            return index
    return -1

def LoopPosearrayInitpose(transformstamped, posestampedarray_src, posestampedarray_target):
    tmp = 1
    id_src = checkHeaderID(transformstamped.header.frame_id, posestampedarray_src)
    id_target = checkHeaderID(transformstamped.child_frame_id, posestampedarray_target)
    # if ()
    if not ( id_src>0 and  id_target > 0):
        print("========================================checkHeaderID===========================")
        print("transformstamped.header.frame_id = {} :  posestampedarray_src = {}".format(transformstamped.header.frame_id, posestampedarray_src) )
        print("transformstamped.child_frame_id = {} :  posestampedarray_target = {}".format(transformstamped.child_frame_id, posestampedarray_target) )
        assert(False)
        return

    trans_r1_f1_findex = posemath.fromMsg(posestampedarray_src.poseArray[id_src].pose)
    trans_r1_r2_findex_findex = posemath.fromMsg(trans2pose(transformstamped.transform)).Inverse()
    trans_r1_r2_f1_finex = trans_r1_f1_findex*trans_r1_r2_findex_findex
    trans_r2_findex_f1 = posemath.fromMsg(posestampedarray_target.poseArray[id_target].pose).Inverse()
    trans_r1_r2_f1_f1 = trans_r1_r2_f1_finex*trans_r2_findex_f1

    return posemath.toMsg(trans_r1_r2_f1_f1)

def RotatePoserray(init_pose, posestampedarray):
    for index,posestamped in enumerate(posestampedarray.poseArray):
        posestampedarray.poseArray[index].pose = posemath.toMsg(posemath.fromMsg(init_pose)*posemath.fromMsg(posestamped.pose))
    return posestampedarray

def valid_loop(poseStampedArray_sets, tranStampedArray ):
    valid_loop_array = TransformStampedArray()
    for transinfo in tranStampedArray.transformArray:
        robot_current_ID = int(int(transinfo.header.frame_id) / 1e8)
        frame_current_ID = int(transinfo.header.frame_id)
        robot_child_ID = int(int(transinfo.child_frame_id) / 1e8)
        frame_child_ID = int(transinfo.child_frame_id)
        if frame_current_ID > int(poseStampedArray_sets[str(robot_current_ID)].poseArray[-1].header.frame_id):
            continue
        if frame_child_ID > int(poseStampedArray_sets[str(robot_child_ID)].poseArray[-1].header.frame_id):
            continue
        valid_loop_array.transformArray.append(transinfo)
    return valid_loop_array

if __name__ == '__main__':
    import pickle
    signal.signal(signal.SIGINT, quit)                                
    signal.signal(signal.SIGTERM, quit)

    infile = open('./script/trajectROS/testdata/data_robot2_0.pkl')
    LooptransArray_sets = pickle.load(infile)
    poseStampedArray_sets = pickle.load(infile)
    tranStampedArray_sets = pickle.load(infile)
    self_ID = 2


    g2ofilename = "/tmp/g2o_robot{}_test.g2o".format(self_ID)
    with open(g2ofilename, "w") as filetmp:
        pass
    #add pose 和 trans
    for key, looparray in LooptransArray_sets.items():
        if not (int(key) == self_ID):
            if len(looparray.transformArray) > 0 and len(tranStampedArray_sets[key].transformArray)>0 :
                posearray_target = PoseStampedArray()
                AccReltrans2PoseStampedArray(tranStampedArray_sets[key],posearray_target)
                target_init_pose = LoopPosearrayInitpose(LooptransArray_sets[key].transformArray[0], poseStampedArray_sets[str(self_ID)], posearray_target)
                poseStampedArray_sets[key] = RotatePoserray(target_init_pose, posearray_target)
                valid_loop_array = valid_loop(poseStampedArray_sets, looparray)
                if len(valid_loop_array.transformArray) > 0:
                    PoseStampedarray2G2O(g2ofilename,poseStampedArray_sets[key], True )
                    valid_loop_array = valid_loop(poseStampedArray_sets, tranStampedArray_sets[key])
                    AddTransArray2G2O(g2ofilename, valid_loop_array)
                    
        else : #ID 相等是自己 key == self_ID
            PoseStampedarray2G2O(g2ofilename,poseStampedArray_sets[key], True )
            valid_loop_array = valid_loop(poseStampedArray_sets, tranStampedArray_sets[key])
            AddTransArray2G2O(g2ofilename, valid_loop_array)
    
    for key, looparray in LooptransArray_sets.items():
        if not (int(key) == self_ID):
            if len(looparray.transformArray) > 0 and len(tranStampedArray_sets[key].transformArray)>0 :
                valid_loop_array = valid_loop(poseStampedArray_sets, looparray)
                if len(valid_loop_array.transformArray) > 0:
                    valid_loop_array = valid_loop(poseStampedArray_sets, looparray)
                    AddTransArray2G2O(g2ofilename, valid_loop_array)
        else :
            valid_loop_array = valid_loop(poseStampedArray_sets, looparray)
            AddTransArray2G2O(g2ofilename, valid_loop_array)




    # posearray_target = PoseStampedArray()
    # AccReltrans2PoseStampedArray(tranStampedArray_sets["2"],posearray_target)

    # target_init_pose = LoopPosearrayInitpose(LooptransArray_sets["2"].transformArray[0], poseStampedArray_sets["1"], posearray_target)
    # # if poseStampedArray_sets.has_key("2"):
    # #     poseStampedArray_sets["2"] = None
    # poseStampedArray_sets["2"] = RotatePoserray(target_init_pose, posearray_target)

    # PoseStampedarray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID),poseStampedArray_sets[str(self_ID)], False )
    # PoseStampedarray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID),poseStampedArray_sets["2"], True )
    # valid_loop_array = valid_loop(poseStampedArray_sets, tranStampedArray_sets["1"])
    # AddTransArray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID), valid_loop_array)
    # valid_loop_array = valid_loop(poseStampedArray_sets, tranStampedArray_sets["2"])
    # AddTransArray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID), valid_loop_array)
    # valid_loop_array = valid_loop(poseStampedArray_sets, LooptransArray_sets["1"])
    # AddTransArray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID), valid_loop_array)
    # valid_loop_array = valid_loop(poseStampedArray_sets, LooptransArray_sets["2"])
    # AddTransArray2G2O("/tmp/g2o_robot{}.g2o".format(self_ID), valid_loop_array)

    poseStampedArray_sets_result = {}
    gtsamOpt2PoseStampedArraySet(g2ofilename,poseStampedArray_sets_result)

    
    rospy.init_node('generate_track_py', anonymous=True)
    posearray_orig_pub = rospy.Publisher("posearray_orig",PoseArray, queue_size=3)
    posearray_optm_pub = rospy.Publisher("posearray_optm",PoseArray, queue_size=3)
    posearray = PoseArray()
    posearray.header.frame_id = "map"
    StampedArray2PoseArray(poseStampedArray_sets_result["1"], posearray)
    posearray_optm = PoseArray()
    posearray_optm.header.frame_id = "map"
    StampedArray2PoseArray(poseStampedArray_sets_result["2"], posearray_optm)
    while True:
        posearray_orig_pub.publish(posearray)
        posearray_optm_pub.publish(posearray_optm)
        time.sleep(1)
        print("12345")


    tmp = 1

    # relP_trans_array = TransformStampedArray()
    # RTtxt2Transarray('testdata/rel_09_s.txt',relP_trans_array)

    # globalposestamped = PoseStampedArray()
    # AccReltrans2PoseStampedArray(relP_trans_array, globalposestamped )
    
    # PoseStampedarray2G2O("./temp.g2o", globalposestamped)
    # AddTransArray2G2O("./temp.g2o", relP_trans_array)

    # transformloop = TransformStampedArray()
    # for index in range(1578,1579):
    #     tmpt = TransformStamped()
    #     tmpt.child_frame_id = index - 1578
    #     tmpt.header.frame_id = index
    #     tmpt.transform.rotation.w = 1
    #     transformloop.transformArray.append(tmpt)
    # AddTransArray2G2O("./temp.g2o", transformloop)

    # OptimizedPoststampedArray = PoseStampedArray()
    # gtsamOpt2PoseStampedarray("./temp.g2o",OptimizedPoststampedArray)
    # tmp = 1

    # posearray = PoseArray()
    # posearray.header.frame_id = "map"
    # StampedArray2PoseArray(globalposestamped, posearray)

    # posearray_optm = PoseArray()
    # posearray_optm.header.frame_id = "map"
    # StampedArray2PoseArray(OptimizedPoststampedArray, posearray_optm)
    



    # # Rel2Globalfile('testdata/scaled_09_rel.txt','testdata/scaled_09.txt')
    # # Global2Relfile('testdata/scaled_09.txt','testdata/scaled_09_rel.txt')

    # # transformloop = []
    # rospy.init_node('generate_track_py', anonymous=True)
    # posearray_orig_pub = rospy.Publisher("posearray_orig",PoseArray, queue_size=3)
    # posearray_optm_pub = rospy.Publisher("posearray_optm",PoseArray, queue_size=3)
    
    # # pose_trans_array = TransformStampedArray()
    # # RTtxt2Transarray('testdata/scaled_09.txt', pose_trans_array)

    # # # RTtxt2Posearray('testdata/scaled_09.txt',posearray)
    
    
    
    # # posearray_orig = copy.deepcopy(RTtxt2Posearray('testdata/scaled_09.txt',posearray) )
    # # posearray.poses = []
    # # pointID = 0
    
    # # for index in range(1578,1579):
    # #     tmpt = TransformStamped()
    # #     tmpt.child_frame_id = pointID+index - 1578
    # #     tmpt.header.frame_id = pointID+index
    # #     tmpt.transform.rotation.w = 1
    # #     transformloop.append(tmpt)


    # # Posearray2G2O("/tmp/tmplog.g2o",posearray_orig)
    # # Loop2G2O("/tmp/tmplog.g2o", transformloop )
    
    # # # gtsamOpt("./tmplog.g2o","./resultlog.g2o")


    # # posearray_optm = copy.deepcopy(gtsamOpt2Posearray("/tmp/tmplog.g2o",posearray) )


    # while True:
    #     posearray_orig_pub.publish(posearray)
    #     posearray_optm_pub.publish(posearray_optm)
    #     time.sleep(1)
    #     print("12345")
    
    # # rospy.spin()







    