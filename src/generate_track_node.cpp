#include <ros/ros.h>   
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>   
#include <tf2/convert.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

string g_window_name;
tf2::Transform tf_base_to_prev(tf2::Quaternion(0,0,0,1));
Mat traj = Mat::zeros(600, 600, CV_8UC3);

void transform_Callback(const geometry_msgs::TransformStamped::ConstPtr &msg)
{  
    tf2::Transform tf_base_to_curr, tf_prev_to_curr;
    tf2::fromMsg(msg->transform, tf_prev_to_curr);
    tf_base_to_curr = tf_base_to_prev * tf_prev_to_curr;
    
    int x = int(tf_base_to_curr.getOrigin().x()*10) + 300;
    int z = int(tf_base_to_curr.getOrigin().z()*10) + 300;
    circle(traj, Point(x, z) ,1, CV_RGB(255,0,0), 2);
    
    imshow( g_window_name, traj );
    
    cv::waitKey(1); 
    
    tf_base_to_prev = tf_base_to_curr;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "generate_track", ros::init_options::AnonymousName);
  if (ros::names::remap("transform") == "transform") {
    ROS_WARN("Topic 'feature' has not been remapped! Typical command-line usage:\n"
             "\t$ rosrun visual_odometry visual_odometry_node feature:=<feature topic> [transport]");
  }

  ros::NodeHandle n;
  std::string topic = n.resolveName("transform");
  g_window_name = topic;
  
  // pub = n.advertise<geometry_msgs::TransformStamped>("/visual_odometry/transform_relative", 1); //创建publisher，往"transform_relative"话题上发布消息
  
  namedWindow( g_window_name, WINDOW_AUTOSIZE );// Create a window for display.
  
  ros::Subscriber sub = n.subscribe(topic, 1, transform_Callback);  //设置回调函数feature_Callback
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}