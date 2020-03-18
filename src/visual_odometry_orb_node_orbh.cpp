#include <ros/ros.h>   
#include <geometry_msgs/TransformStamped.h>   
#include <dslam_sp/EF_output.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/Time.h>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <dslam_sp/Orb.hpp>
#include <boost/make_shared.hpp>

using namespace cv;
using namespace std;

string g_window_name;
ros::Publisher pub;

vector<Point2f> points_prev;
Mat desc_prev;
cv::Mat depth_prev;
std_msgs::Header header_prev;

bool First_Frame;

void feature_Callback(const dslam_sp::EF_output::ConstPtr &msg)
{  
    //读取当前帧的depth
    cv::Mat depth_curr;
    cv_bridge::CvImagePtr cv_ptr;
    sensor_msgs::ImagePtr dep_ptr = boost::make_shared<::sensor_msgs::Image>(msg->depth);
    cout << "PRE1" << endl;
    
    cv_ptr=cv_bridge::toCvCopy(dep_ptr, dep_ptr->encoding);
    cv_ptr->image.copyTo(depth_curr);
    cout << "PRE2" << endl;
    
    //读取当前帧的keypoint、descriptor
    std::vector<dslam_sp::KeyPoint> keypoints = msg->keypoints.keypoint;
    std::vector<dslam_sp::Descriptor> descriptors = msg->descriptors.descriptor;
    cout << "PRE3" << endl;
    
    // cout << "LOG start" << endl;
    // for ( auto keypoint1 : keypoints){
    //     cout << keypoint1 << endl;
    // }
    // for ( auto descriptor1 : descriptors){
    //     cout << descriptor1 << endl;
    // }

    //top k个特征点
    if(keypoints.size() > KEEP_K_POINTS) {
        top_k(keypoints, descriptors, keypoints.size(), KEEP_K_POINTS);
    }
    
    cout << "PRE4" << endl;
    //数据格式转换
    vector<Point2f> points_curr;
    Mat desc_curr;
    desc_curr.create( int(keypoints.size()), int(descriptors[0].desc.size()), CV_8UC1);
    for(int i=0; i<keypoints.size(); i++)
    {
        points_curr.push_back(Point2f(keypoints[i].pt.x, keypoints[i].pt.y));
        uchar* pData = desc_curr.ptr<uchar>(i);   //第i+1行的所有元素  
        for(int j = 0; j < desc_curr.cols; j++){
            pData[j] = (uchar) descriptors[i].desc[j];
        }
        
    }
    
    //判定是否有prev数据，第一帧不做match
    if(!First_Frame)
    {
        
        //match
        // cout << "LOG start" << endl;
        // for ( auto prev_point : points_prev){
        //     cout << prev_point << endl;
        // }
        // cout << desc_prev << endl;
        // cout << depth_prev << endl;

        // for ( auto curr_point : points_curr){
        //     cout << curr_point << endl;
        // }
        // cout << desc_curr << endl;
        // cout << depth_curr << endl;
        Eigen::Quaterniond q;
        Mat t;
        int matchpointNum = ORB_Match_VO(points_prev,points_curr, desc_prev, desc_curr, depth_prev, desc_curr, msg->P, q, t);


        
        
        geometry_msgs::TransformStamped TF_msg;
        TF_msg.header = msg->header;
        TF_msg.child_frame_id = header_prev.frame_id;
        TF_msg.transform.translation.x = t.at<double>(0);
        TF_msg.transform.translation.y = t.at<double>(1);
        TF_msg.transform.translation.z = t.at<double>(2);
        TF_msg.transform.rotation.x = q.x();
        TF_msg.transform.rotation.y = q.y();
        TF_msg.transform.rotation.z = q.z();
        TF_msg.transform.rotation.w = q.w();
        pub.publish(TF_msg);//发布msg
        
        cout << "child_frame_id:" << header_prev.frame_id << " frame_id:" << msg->header.frame_id << endl;
        ROS_INFO("stamp:%d.%d translation:%f %f %f rotation:%f %f %f", TF_msg.header.stamp.sec, TF_msg.header.stamp.nsec,
                    TF_msg.transform.translation.x, TF_msg.transform.translation.y, TF_msg.transform.translation.z,
                    TF_msg.transform.rotation.x, TF_msg.transform.rotation.y, TF_msg.transform.rotation.z, TF_msg.transform.rotation.w); //输出
    }
    
    points_prev = points_curr;
    desc_prev = desc_curr;
    depth_prev = depth_curr;
    header_prev = msg->header;
    
    First_Frame = false;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "visual_odometry", ros::init_options::AnonymousName);
  if (ros::names::remap("feature") == "feature") {
    ROS_WARN("Topic 'feature' has not been remapped! Typical command-line usage:\n"
             "\t$ rosrun visual_odometry visual_odometry_node feature:=<feature topic> [transport]");
  }

  ros::NodeHandle n;
  std::string topic = n.resolveName("feature");
  g_window_name = topic;
  
  pub = n.advertise<geometry_msgs::TransformStamped>("/visual_odometry/transform_relative", 1); //创建publisher，往"transform_relative"话题上发布消息
  
  namedWindow( g_window_name, WINDOW_AUTOSIZE );// Create a window for display.
  
  First_Frame = true;
  
  ros::Subscriber sub_point = n.subscribe(topic, 1, feature_Callback);  //设置回调函数feature_Callback
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}