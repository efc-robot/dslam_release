# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "dslam_sp: 15 messages, 3 services")

set(MSG_I_FLAGS "-Idslam_sp:/home/xuzhl/catkin_ws_d/src/dslam_release/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/melodic/share/sensor_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(dslam_sp_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" "dslam_sp/Point2f"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" "sensor_msgs/Image:geometry_msgs/TransformStamped:geometry_msgs/Vector3:geometry_msgs/Transform:std_msgs/Header:geometry_msgs/Quaternion"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" "sensor_msgs/Image:dslam_sp/image_depth:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" "sensor_msgs/Image:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" ""
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" "dslam_sp/PoseStampedArray:geometry_msgs/Pose:std_msgs/Header:geometry_msgs/Point:geometry_msgs/PoseStamped:geometry_msgs/Quaternion"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" "geometry_msgs/TransformStamped:dslam_sp/TransformStampedArray:geometry_msgs/Vector3:geometry_msgs/Transform:std_msgs/Header:geometry_msgs/Quaternion"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" ""
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" "dslam_sp/Descriptor"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" "geometry_msgs/Pose:geometry_msgs/Point:sensor_msgs/Image:geometry_msgs/Quaternion:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" "dslam_sp/KeyPoints_vector:sensor_msgs/Image:dslam_sp/KeyPoint:dslam_sp/Descriptor:dslam_sp/Point2f:dslam_sp/Descriptors_vector:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" ""
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" "geometry_msgs/Vector3:geometry_msgs/TransformStamped:geometry_msgs/Transform:geometry_msgs/Quaternion:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" "dslam_sp/KeyPoint:dslam_sp/Point2f"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" ""
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" "sensor_msgs/Image:dslam_sp/image_depth:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" "geometry_msgs/Pose:geometry_msgs/Quaternion:geometry_msgs/Point:geometry_msgs/PoseStamped:std_msgs/Header"
)

get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_custom_target(_dslam_sp_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dslam_sp" "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" "std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_msg_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)

### Generating Services
_generate_srv_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_srv_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)
_generate_srv_cpp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
)

### Generating Module File
_generate_module_cpp(dslam_sp
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(dslam_sp_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(dslam_sp_generate_messages dslam_sp_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_cpp _dslam_sp_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dslam_sp_gencpp)
add_dependencies(dslam_sp_gencpp dslam_sp_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dslam_sp_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_msg_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)

### Generating Services
_generate_srv_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_srv_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)
_generate_srv_eus(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
)

### Generating Module File
_generate_module_eus(dslam_sp
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(dslam_sp_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(dslam_sp_generate_messages dslam_sp_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_eus _dslam_sp_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dslam_sp_geneus)
add_dependencies(dslam_sp_geneus dslam_sp_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dslam_sp_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_msg_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)

### Generating Services
_generate_srv_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_srv_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)
_generate_srv_lisp(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
)

### Generating Module File
_generate_module_lisp(dslam_sp
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(dslam_sp_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(dslam_sp_generate_messages dslam_sp_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_lisp _dslam_sp_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dslam_sp_genlisp)
add_dependencies(dslam_sp_genlisp dslam_sp_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dslam_sp_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_msg_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)

### Generating Services
_generate_srv_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_srv_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)
_generate_srv_nodejs(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
)

### Generating Module File
_generate_module_nodejs(dslam_sp
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(dslam_sp_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(dslam_sp_generate_messages dslam_sp_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_nodejs _dslam_sp_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dslam_sp_gennodejs)
add_dependencies(dslam_sp_gennodejs dslam_sp_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dslam_sp_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg;/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/sensor_msgs/cmake/../msg/Image.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_msg_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)

### Generating Services
_generate_srv_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TransformStamped.msg;/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Transform.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_srv_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)
_generate_srv_py(dslam_sp
  "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv"
  "${MSG_I_FLAGS}"
  "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/PoseStamped.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
)

### Generating Module File
_generate_module_py(dslam_sp
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(dslam_sp_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(dslam_sp_generate_messages dslam_sp_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoint.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStamped_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchedFrame.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/image_depth.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/keyframe_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/posearray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/srv/transarray_srv.srv" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Descriptors_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Pose_with_image.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/EF_output.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/MatchStamp.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/TransformStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/KeyPoints_vector.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/Point2f.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/InterMatch.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PoseStampedArray.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/xuzhl/catkin_ws_d/src/dslam_release/msg/PRrepresentor.msg" NAME_WE)
add_dependencies(dslam_sp_generate_messages_py _dslam_sp_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dslam_sp_genpy)
add_dependencies(dslam_sp_genpy dslam_sp_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dslam_sp_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dslam_sp
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(dslam_sp_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(dslam_sp_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(dslam_sp_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dslam_sp
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(dslam_sp_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(dslam_sp_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(dslam_sp_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dslam_sp
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(dslam_sp_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(dslam_sp_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(dslam_sp_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dslam_sp
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(dslam_sp_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(dslam_sp_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(dslam_sp_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dslam_sp
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(dslam_sp_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(dslam_sp_generate_messages_py sensor_msgs_generate_messages_py)
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(dslam_sp_generate_messages_py geometry_msgs_generate_messages_py)
endif()
