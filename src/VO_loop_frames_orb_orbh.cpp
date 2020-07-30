#include <ros/ros.h>
#include <geometry_msgs/TransformStamped.h>   
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/String.h>
#include <dslam_sp/image_depth.h>
#include <dslam_sp/Point2f.h>
#include <dslam_sp/KeyPoint.h>
#include <dslam_sp/KeyPoints_vector.h>
#include <dslam_sp/Descriptor.h>
#include <dslam_sp/Descriptors_vector.h>
#include <dslam_sp/EF_output.h>
#include <dslam_sp/top_k.h>
#include <dslam_sp/bundleAdjustment.h>
#include <dslam_sp/MatchedFrame.h>

#include <boost/make_shared.hpp>
#include <sstream>
#include <dslam_sp/Orb.hpp>
// #include <dslam_sp/SuperPoint.hpp>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <chrono>

using namespace cv;
using namespace std;

#define KEEP_K_POINTS 1000
#define MATCHER "BF"

string g_window_name;
// SuperPoint superpoint;
ros::Publisher pub;
sensor_msgs::CameraInfo camera_info;
int frame_id = 0;
char FileDir[200] = "/PATH/TO/DATASET/";

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


void fnames_Callback(const dslam_sp::MatchedFrame::ConstPtr &msg, Ptr<FeatureDetector> &detector, Ptr<DescriptorExtractor> &descriptor){
    

    cv_bridge::CvImageConstPtr cv_fr1_img_ptr, cv_fr1_deep_ptr, cv_fr2_img_ptr, cv_fr2_deep_ptr;

    sensor_msgs::ImagePtr img_ptr1 = boost::make_shared<::sensor_msgs::Image>(msg->frame1.image);
    sensor_msgs::ImagePtr dep_ptr1 = boost::make_shared<::sensor_msgs::Image>(msg->frame1.depth);

    sensor_msgs::ImagePtr img_ptr2 = boost::make_shared<::sensor_msgs::Image>(msg->frame2.image);
    sensor_msgs::ImagePtr dep_ptr2 = boost::make_shared<::sensor_msgs::Image>(msg->frame2.depth);
    
    // cout <<  "img.stamp:" << img_ptr->header.stamp << "  depth.stamp:" << dep_ptr->header.stamp << endl;
    // cout <<  "img.height:" << img_ptr->height << "  img.width:" << img_ptr->width << endl;
    
    //frame1 在selfloop中，表示先来的图片，在interloop中，表示对方匹配到的图片。
    cv_fr1_img_ptr = cv_bridge::toCvShare(img_ptr1, msg->frame1.image.encoding);
    const cv::Mat &Image1 = cv_fr1_img_ptr->image;
    cv_fr1_deep_ptr = cv_bridge::toCvShare(dep_ptr1, msg->frame1.depth.encoding);
    const cv::Mat &DImage1 = cv_fr1_deep_ptr->image;

    //frame2 在selfloop中，表示后来的图片，在interloop中，表示自己的图片。
    cv_fr2_img_ptr = cv_bridge::toCvShare(img_ptr2, msg->frame2.image.encoding);
    const cv::Mat &Image2 = cv_fr2_img_ptr->image;
    cv_fr2_deep_ptr = cv_bridge::toCvShare(dep_ptr2, msg->frame2.depth.encoding);
    const cv::Mat &DImage2 = cv_fr2_deep_ptr->image;
    cout <<  "msg->frame2.depth.encoding:" << msg->frame2.depth.encoding << endl;
    cout <<  "DImage2.type():" << DImage2.type() << endl;

    Mat image1,image2;
    Mat depth1,depth2;

    Size dsize = Size(640,480);
    std::vector<cv::KeyPoint> kpts1,kpts2;
    Mat descriptor_results1, descriptor_results2;
    sensor_msgs::CameraInfo camera_info_tmp;
    Mat depth_resized1, depth_resized2;
    Mat image_resized1, image_resized2;
    cout << "OKOK0" << endl;

    ORB_FeatureExtract_ROS(Image1, DImage1, camera_info, dsize, kpts1, descriptor_results1, detector, descriptor, camera_info_tmp,depth_resized1,image_resized1);
    cout << "OKOK00001" << endl;
    
    ORB_FeatureExtract_ROS(Image2, DImage2, camera_info, dsize, kpts2, descriptor_results2, detector, descriptor, camera_info_tmp,depth_resized2,image_resized2);

    cout << "OKOK1" << endl;


    std::vector<dslam_sp::KeyPoint> keypoints1,keypoints2;
    std::vector<dslam_sp::Descriptor> descriptors1,descriptors2;
    for(int i = 0; i < kpts1.size(); i++){ 
        
        dslam_sp::KeyPoint kp_ros;
        KeyPoint_cv2ros(kpts1[i],kp_ros);
        keypoints1.push_back(kp_ros);
        
        dslam_sp::Descriptor ds_ros;
        
        std::vector<float> dspts_tmp;
        dspts_tmp.resize(32);
        uchar* pData = descriptor_results1.ptr<uchar>(i);
        for (int j = 0; j < descriptor_results1.cols; j++){
            dspts_tmp[j] = (float)pData[j] ;
        }

        ds_ros.desc = dspts_tmp;
        descriptors1.push_back(ds_ros);
    }
    for(int i = 0; i < kpts2.size(); i++){ 
        
        dslam_sp::KeyPoint kp_ros;
        KeyPoint_cv2ros(kpts2[i],kp_ros);
        keypoints2.push_back(kp_ros);
        
        dslam_sp::Descriptor ds_ros;

                
        std::vector<float> dspts_tmp;
        dspts_tmp.resize(32);
        uchar* pData = descriptor_results2.ptr<uchar>(i);
        for (int j = 0; j < descriptor_results2.cols; j++){
            dspts_tmp[j] = (float)pData[j] ;
        }

        ds_ros.desc = dspts_tmp;
        descriptors2.push_back(ds_ros);
    }

    cout << "OKOK2" << endl;
    
    if(keypoints1.size() > KEEP_K_POINTS) {
        cout << "keypoints1.size()" << keypoints1.size() << endl;
        top_k(keypoints1, descriptors1, keypoints1.size(), KEEP_K_POINTS);
    }
    cout << "OKOK22" << endl;
    if(keypoints2.size() > KEEP_K_POINTS) {
        top_k(keypoints2, descriptors2, keypoints2.size(), KEEP_K_POINTS);
    }
    cout << "OKOK23" << endl;

    vector<Point2f> points_prev;
    Mat desc_prev;
    desc_prev.create( int(keypoints1.size()), int(descriptors1[0].desc.size()), CV_8UC1);
    for(int i=0; i<keypoints1.size(); i++)
    {
        points_prev.push_back(Point2f(keypoints1[i].pt.x, keypoints1[i].pt.y));
        uchar* pData = desc_prev.ptr<uchar>(i);   //第i+1行的所有元素  
        for(int j = 0; j < desc_prev.cols; j++){
            pData[j] = (uchar) descriptors1[i].desc[j];
        }
        
    }
    cout << "OKOK3" << endl;

    vector<Point2f> points_curr;
    Mat desc_curr;
    desc_curr.create( int(keypoints2.size()), int(descriptors2[0].desc.size()), CV_8UC1);
    for(int i=0; i<keypoints2.size(); i++)
    {
        points_curr.push_back(Point2f(keypoints2[i].pt.x, keypoints2[i].pt.y));
        uchar* pData = desc_curr.ptr<uchar>(i);   //第i+1行的所有元素  
        for(int j = 0; j < desc_curr.cols; j++){
            pData[j] = (uchar) descriptors2[i].desc[j];
        }
    }
    cout << "OKO4" << endl;

    
    cv::Mat depth_prev = depth_resized1;    
    cv::Mat depth_curr = depth_resized2;

    Eigen::Quaterniond q;
    Mat t;
    int matchpointNum = ORB_Match_VO(points_prev,points_curr, desc_prev, desc_curr, depth_prev, depth_curr, camera_info_tmp.P, q, t);
    cout << "OKO5 matchpointNum=" << matchpointNum << endl;


        
    geometry_msgs::TransformStamped TF_msg;
    TF_msg.header.frame_id = msg->frame2.header.frame_id; // frame2, 在selfloop中，表示后来的图片，在interloop中，表示自己的图片。
    TF_msg.child_frame_id = msg->frame1.header.frame_id; // frame1, 在selfloop中，表示先来的图片，在interloop中，表示对方的图片。
    TF_msg.transform.translation.x = t.at<double>(0);
    TF_msg.transform.translation.y = t.at<double>(1);
    TF_msg.transform.translation.z = t.at<double>(2);
    TF_msg.transform.rotation.x = q.x();
    TF_msg.transform.rotation.y = q.y();
    TF_msg.transform.rotation.z = q.z();
    TF_msg.transform.rotation.w = q.w();
    pub.publish(TF_msg);//发布msg
    
    cout << "child_frame_id:" << TF_msg.child_frame_id << " frame_id:" << TF_msg.header.frame_id << endl;
    ROS_INFO("stamp:%d.%d translation:%f %f %f rotation:%f %f %f %f ;", TF_msg.header.stamp.sec, TF_msg.header.stamp.nsec,
                TF_msg.transform.translation.x, TF_msg.transform.translation.y, TF_msg.transform.translation.z,
                TF_msg.transform.rotation.x, TF_msg.transform.rotation.y, TF_msg.transform.rotation.z, TF_msg.transform.rotation.w); //输出





}

int main(int argc, char **argv)
{
    int o;
    const char *optstring = "p:"; // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'p':
                strcpy(FileDir, optarg);
                break;
            default:
                break;
        }
    }
  Ptr<FeatureDetector> detector = ORB::create( 1000 );
  Ptr<DescriptorExtractor> descriptor = ORB::create();
  ros::init(argc, argv, "VO_loop", ros::init_options::AnonymousName);
  if (ros::names::remap("filenames") == "filenames") {
    ROS_WARN("Topic 'filenames' has not been remapped! Typical command-line usage:\n"
             "\t$ rosrun superpoint_EF superpoint_EF_node filenames:=<filenames topic> [transport]");
  }

  ros::NodeHandle n;
  std::string topic = n.resolveName("loopframes");
  g_window_name = topic;
  
  pub = n.advertise<geometry_msgs::TransformStamped>("VO_loop_relpose", 1); //创建publisher，往"featurepoints_descriptors"话题上发布消息
  
  // namedWindow( g_window_name, WINDOW_AUTOSIZE );// Create a window for display.
  
  ros::Subscriber sub_info = n.subscribe("/mynteye/left_rect/camera_info", 1, info_Callback);
  ros::Subscriber sub = n.subscribe<dslam_sp::MatchedFrame>(topic, 1, boost::bind(&fnames_Callback,_1, detector, descriptor)  );  //设置回调函数gpsCallback
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}