#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <dslam_sp/image_depth.h>
#include <dslam_sp/Point2f.h>
#include <dslam_sp/KeyPoint.h>
#include <dslam_sp/KeyPoints_vector.h>
#include <dslam_sp/Descriptor.h>
#include <dslam_sp/Descriptors_vector.h>
#include <dslam_sp/EF_output.h>

#include <boost/make_shared.hpp>

#include <cv_bridge/cv_bridge.h>
#include <dslam_sp/top_k.h>
#include <dslam_sp/bundleAdjustment.h>
#include <dslam_sp/Sift.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <chrono>

using namespace cv;
using namespace std;

string g_window_name;
// SuperPoint superpoint;
ros::Publisher pub;
sensor_msgs::CameraInfo camera_info;
int seq = 0;
// #ifndef NMS_Threshold
// #define NMS_Threshold 4
// #endif
// #ifndef KEEP_K_POINTS
// #define KEEP_K_POINTS 300
// #endif

void info_Callback(const sensor_msgs::CameraInfo::ConstPtr &msg)
{  
    camera_info.P = msg->P;
}

void KeyPoint_cv2ros(cv::KeyPoint cv_kp, dslam_sp::KeyPoint& ros_kp)
{
    ros_kp.angle = cv_kp.angle;
    ros_kp.class_id = cv_kp.class_id;
    ros_kp.octave = cv_kp.octave;
    ros_kp.pt.x = cv_kp.pt.x;
    ros_kp.pt.y = cv_kp.pt.y;
    ros_kp.response = cv_kp.response;
    ros_kp.size = cv_kp.size;
}

// //局部极大值抑制，这里利用fast特征点的响应值做比较
// void selectMax(int r, std::vector<KeyPoint> & kp){

//     //r是局部极大值抑制的窗口半径
//     if (r != 0){
//         //对kp中的点进行局部极大值筛选
//         for (int i = 0; i < kp.size(); i++){
//             for (int j = i + 1; j < kp.size(); j++){
//                 //如果两个点的距离小于半径r，则删除其中响应值较小的点
//                 if (abs(kp[i].pt.x - kp[j].pt.x)<=r && abs(kp[i].pt.y - kp[j].pt.y)<=r){
//                     if (kp[i].response < kp[j].response){
//                         std::vector<KeyPoint>::iterator it = kp.begin() + i;
//                         kp.erase(it);
//                         i--;
//                         break;
//                     }
//                     else{
//                         std::vector<KeyPoint>::iterator it = kp.begin() + j;
//                         kp.erase(it);
//                         j--;
//                     }
//                 }
//             }
//         }
//     }

// }


void img_Callback(const dslam_sp::image_depth::ConstPtr &msg, Ptr<FeatureDetector> &detector, Ptr<DescriptorExtractor> &descriptor)
{  
    // Convert to OpenCV native BGR color
    // std::chrono::steady_clock::time_point//timepoint[5];
    // int timeindex = 0;
    cv_bridge::CvImageConstPtr cv_ptr1, cv_ptr2;
    sensor_msgs::ImagePtr img_ptr = boost::make_shared<::sensor_msgs::Image>(msg->image);
    sensor_msgs::ImagePtr dep_ptr = boost::make_shared<::sensor_msgs::Image>(msg->depth);
    // cout <<  "img.stamp:" << img_ptr->header.stamp << "  depth.stamp:" << dep_ptr->header.stamp << endl;
    // cout <<  "img.height:" << img_ptr->height << "  img.width:" << img_ptr->width << endl;
    
    cv_ptr1=cv_bridge::toCvShare(img_ptr, msg->image.encoding);
    const cv::Mat &image_raw = cv_ptr1->image;
    cv_ptr2=cv_bridge::toCvShare(dep_ptr, msg->depth.encoding);
    const cv::Mat &depth_raw = cv_ptr2->image;
    
    
    // cout <<  "img_raw.size:" << image_raw.size() << "  depth_raw.size:" << depth_raw.size() << endl;
    // imshow( g_window_name, image_raw );
    
   //timepoint[timeindex++] = std::chrono::steady_clock::now();

    Size dsize = Size(640,480);
    std::vector<cv::KeyPoint> kpts;
    Mat descriptor_results;
    sensor_msgs::CameraInfo camera_info_tmp;
    Mat depth_resized;
    Mat image_resized;
    
    SIFT_FeatureExtract_ROS(image_raw, depth_raw, camera_info, dsize, kpts, descriptor_results, detector, descriptor, camera_info_tmp,depth_resized,image_resized);
    
    dslam_sp::EF_output feature_msg;
    feature_msg.header = msg->header;
    // feature_msg.header.seq = std::to_string(seq++);
    feature_msg.header.seq = msg->header.seq;
    feature_msg.header.stamp.sec = msg->header.stamp.sec;
    feature_msg.header.stamp.nsec = msg->header.stamp.nsec;
    feature_msg.P = camera_info_tmp.P;
    cout << "feature_msg.header.seq:" << feature_msg.header.seq << endl;
    cout << "feature_msg.header.stamp:" << feature_msg.header.stamp.sec << " . " << feature_msg.header.stamp.nsec << endl;
    
    sensor_msgs::ImagePtr dp_ptr = cv_bridge::CvImage(std_msgs::Header(), msg->depth.encoding, depth_resized).toImageMsg();
    feature_msg.depth = *dp_ptr;
    sensor_msgs::ImagePtr im_ptr = cv_bridge::CvImage(std_msgs::Header(), msg->image.encoding, image_resized).toImageMsg();
    feature_msg.image = *im_ptr;
    
    cout << "descriptor_results.type: " << descriptor_results.type() << endl;
    for(int i = 0; i < kpts.size(); i++)
    { 
        // cv::Point p(kpts[i].pt.x, kpts[i].pt.y);
        // cv::circle(image, p , 5, (0, 255, 0), -1);
        
        dslam_sp::KeyPoint kp_ros;
        KeyPoint_cv2ros(kpts[i],kp_ros);
        feature_msg.keypoints.keypoint.push_back(kp_ros);
        
        dslam_sp::Descriptor ds_ros;
        std::vector<float> dspts_tmp;
        dspts_tmp.resize(descriptor_results.cols);
        float* pData = descriptor_results.ptr<float>(i);
        for (int j = 0; j < descriptor_results.cols; j++){
            dspts_tmp[j] = (float)pData[j] ;
        }
        ds_ros.desc = dspts_tmp;
        feature_msg.descriptors.descriptor.push_back(ds_ros);
    }
   //timepoint[timeindex++] = std::chrono::steady_clock::now();

    // for (int index = 0; index < 4; index ++){
    //     std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(timepoint[index+1] -//timepoint[index]);
    //     cout << "timedurarion: " << index << " , consumes " << time_span.count() << " seconds.";
    // }
    // cout << "dspts[0][0]:" << dspts[0][0] << endl;
    // cout << "feature_msg.keypoints.keypoint[0].response:" << feature_msg.keypoints.keypoint[0].response << endl;
    
    pub.publish(feature_msg);//发布msg
    
    // imshow( g_window_name, image );
    // waitKey(1);
}


int main(int argc, char **argv)
{
//   Ptr<FeatureDetector> detector = FeatureDetector::create ( "SIFT" );
//   Ptr<DescriptorExtractor> descriptor = DescriptorExtractor::create ( "SIFT" );
  Ptr<FeatureDetector> detector = xfeatures2d::SIFT::create( KEEP_K_POINTS,  3, 0.04, 3, 1.6 );
  Ptr<DescriptorExtractor> descriptor = xfeatures2d::SIFT::create();

  ros::init(argc, argv, "sift_EF", ros::init_options::AnonymousName);
  if (ros::names::remap("image") == "image") {
    ROS_WARN("Topic 'image' has not been remapped! Typical command-line usage:\n"
             "\t$ rosrun sift_EF sift_EF_node image:=<image topic> [transport]");
  }

  ros::NodeHandle n;
  std::string topic = n.resolveName("image");
  g_window_name = topic;
  
  pub = n.advertise<dslam_sp::EF_output>("/sift_EF/featurepoints_descriptors", 1); //创建publisher，往"featurepoints_descriptors"话题上发布消息
  
//   namedWindow( g_window_name, WINDOW_AUTOSIZE );// Create a window for display.
  
  ros::Subscriber sub_info = n.subscribe("/mynteye/left_rect/camera_info", 1, info_Callback);
  ros::Subscriber sub = n.subscribe<dslam_sp::image_depth>(topic, 1, boost::bind(&img_Callback,_1, detector, descriptor) );  //设置回调函数gpsCallback
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}