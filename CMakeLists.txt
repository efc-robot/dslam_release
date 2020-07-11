cmake_minimum_required(VERSION 2.8.3)
project(dslam_sp)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

SET(CPU_ARCH x86_64)
##SP## SET(CAFFE_DIR /home/yujc/robotws/caffe-ssd/) #选择caffe路径
SET(G2O_CMAKE_MODULES /home/xuzhl/git_workspace/g2o/cmake_modules/)
##SP## SET(SUPERPOINT_LIB ${PROJECT_SOURCE_DIR}/superpointlib/lib/libsuperpoint.so)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(Matter_LIBS /home/yujc/robotws/DSLAM_release/src/ROS-DSLAM/mattersim/build/libMatterSim.so)
# set(Matter_INCLUDE ./mattersim/include)

##SP## SET(GLOG_BOOST_LIBS /usr/lib/${CPU_ARCH}-linux-gnu/libglog.so /usr/lib/${CPU_ARCH}-linux-gnu/libboost_system.so) #caffe执行过程中的依赖库

LIST(APPEND CMAKE_MODULE_PATH CMAKE_MODULE_PATH  ${G2O_CMAKE_MODULES})
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
  sensor_msgs
  geometry_msgs
  cv_bridge
  octomap_server
  tf
  tf2_geometry_msgs
)
find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(G2O REQUIRED)
find_package(CSparse REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )
add_message_files(
  FILES
  image_depth.msg
  Point2f.msg
  KeyPoint.msg
  KeyPoints_vector.msg
  Descriptor.msg
  Descriptors_vector.msg
  EF_output.msg
  PRrepresentor.msg
  TransformStampedArray.msg
  TransformStamped_with_image.msg
  PoseStampedArray.msg
  Pose_with_image.msg
  MatchedFrame.msg
  InterMatch.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  posearray_srv.srv
  transarray_srv.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )
generate_messages(
  DEPENDENCIES
  std_msgs
  sensor_msgs
  geometry_msgs
)


################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES dslam_sp
#  CATKIN_DEPENDS roscpp std_msgs
   CATKIN_DEPENDS roscpp rospy std_msgs sensor_msgs message_runtime tf tf2_geometry_msgs octomap_server
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
  ${G2O_INCLUDE_DIR}
  ${CSPARSE_INCLUDE_DIR}
#  ${Matter_INCLUDE}
  ${CAFFE_DIR}/include/
  /usr/local/include 
  /usr/local/cuda/include
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/dslam_sp.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(merge_img_depth src/merge_img_depth_node.cpp)
# add_executable(publish_sim src/publish_sim_node.cpp)
add_executable(publish_merge src/publish_merge_node.cpp)
add_executable(publish_merge_tum src/publish_merge_node_tum.cpp)
##SP## add_executable(vo_loop_fnames src/VO_loop_filenames.cpp)
add_executable(vo_loop_fnames_orb src/VO_loop_filenames_orb_orbh.cpp)
add_executable(vo_loop_frames_orb src/VO_loop_frames_orb_orbh.cpp)
##SP## add_executable(superpoint_EF src/superpoint_EF_node.cpp)
add_executable(orb_EF src/orb_EF_node_orbh.cpp)
add_executable(sift_EF src/sift_EF_node.cpp)
##SP## add_executable(visual_odometry src/visual_odometry_node.cpp)
add_executable(visual_odometry_orb src/visual_odometry_orb_node_orbh.cpp)
add_executable(visual_odometry_sift src/visual_odometry_sift_node.cpp)
add_executable(generate_track src/generate_track_node.cpp)
add_executable(rgbd2octomap_ros src/rgbd2octomap_ros.cpp)
add_executable(rgbd2pointcloud src/rgbd2pointcloud_ros.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
add_dependencies(merge_img_depth ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# add_dependencies(publish_sim ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
##SP## add_dependencies(superpoint_EF ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(orb_EF ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(sift_EF ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(publish_merge ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(publish_merge_tum ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
##SP## add_dependencies(vo_loop_fnames ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(vo_loop_fnames_orb ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(vo_loop_frames_orb ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
##SP## add_dependencies(visual_odometry ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(visual_odometry_orb ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(visual_odometry_sift ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(generate_track ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(rgbd2octomap_ros ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(rgbd2pointcloud ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
target_link_libraries(merge_img_depth
  ${catkin_LIBRARIES}
)
target_link_libraries(publish_merge
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
target_link_libraries(publish_merge_tum
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
# target_link_libraries(publish_sim
#  ${catkin_LIBRARIES}
#  ${OpenCV_LIBRARIES}
#  ${Matter_LIBS}
#)
##SP## target_link_libraries(superpoint_EF
##SP##   ${catkin_LIBRARIES}
##SP##   ${OpenCV_LIBRARIES}
##SP##   ${CAFFE_DIR}/build/lib/libcaffe.so
##SP##   ${GLOG_BOOST_LIBS}
##SP##   ${SUPERPOINT_LIB}
##SP## )
SET(G2O_LIBS g2o_cli g2o_ext_freeglut_minimal g2o_simulator g2o_solver_slam2d_linear g2o_types_icp g2o_types_slam2d g2o_core g2o_interface g2o_solver_csparse g2o_solver_structure_only g2o_types_sba g2o_types_slam3d g2o_csparse_extension g2o_opengl_helper g2o_solver_dense g2o_stuff g2o_types_sclam2d g2o_parser g2o_solver_pcg g2o_types_data g2o_types_sim3 cxsparse )
## Specify libraries to link a library or executable target against
##SP## target_link_libraries(visual_odometry
##SP##   ${catkin_LIBRARIES}
##SP##   ${OpenCV_LIBRARIES}
##SP##   ${G2O_LIBS}
##SP## )
target_link_libraries(orb_EF
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${GLOG_BOOST_LIBS}
  ${G2O_LIBS}
)
target_link_libraries(visual_odometry_orb
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${G2O_LIBS}
)
target_link_libraries(sift_EF
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${GLOG_BOOST_LIBS}
  ${G2O_LIBS}
)
target_link_libraries(visual_odometry_sift
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${G2O_LIBS}
)
##SP## target_link_libraries(vo_loop_fnames
##SP##   ${catkin_LIBRARIES}
##SP##   ${OpenCV_LIBRARIES}
##SP##   ${CAFFE_DIR}/build/lib/libcaffe.so
##SP##   ${GLOG_BOOST_LIBS}
##SP##   ${SUPERPOINT_LIB}
##SP##   ${G2O_LIBS}
##SP## )
target_link_libraries(vo_loop_fnames_orb
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${G2O_LIBS}
)
target_link_libraries(vo_loop_frames_orb
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
  ${G2O_LIBS}
)
target_link_libraries(generate_track
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
target_link_libraries(rgbd2octomap_ros
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)
target_link_libraries(rgbd2pointcloud
  ${catkin_LIBRARIES}
  ${OpenCV_LIBRARIES}
)


##SP## ADD_SUBDIRECTORY(superpointlib)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_dslam_sp.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
