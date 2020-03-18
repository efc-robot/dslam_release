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

#include <caffe/caffe.hpp>
#include <dslam_sp/SuperPoint.hpp>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

using namespace cv;
using namespace std;
using namespace caffe;  // NOLINT(build/namespaces)

string g_window_name;
// SuperPoint superpoint;
ros::Publisher pub;
sensor_msgs::CameraInfo camera_info;
int frame_id = 0;

char ProFileDir[200] = "/PATH/TO/PROTOTXT/";
char ModelFileDir[200] = "/PATH/TO/CAFFEMODEL/";

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

void img_Callback(const dslam_sp::image_depth::ConstPtr &msg, SuperPoint &superpoint)
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


    //裁剪到640x480
    Mat image;
    Mat depth;
    sensor_msgs::CameraInfo camera_info_tmp = camera_info;
    if((float)msg->image.height/(float)msg->image.width > 0.75) {
        int height_ = msg->image.width /4 *3;
        int margin = (msg->image.height - height_)/2;
        image = image_raw(Rect(0,margin,msg->image.width,height_));
        depth = depth_raw(Rect(0,margin,msg->image.width,height_));
        camera_info_tmp.P[6] = camera_info_tmp.P[6] - margin;
    }
    else if((float)msg->image.height/(float)msg->image.width < 0.75) {
        int width_ = msg->image.height /3 *4;
        int margin = (msg->image.width - width_)/2;
        image = image_raw(Rect(margin,0,width_,msg->image.height));
        depth = depth_raw(Rect(margin,0,width_,msg->image.height));
        camera_info_tmp.P[2] = camera_info_tmp.P[2] - margin;
        // cout <<  "width_" << width_ << "  margin" << margin << endl;
    }
    // cout <<  "img.size:" << image.size() << "  depth.size:" << depth.size() << endl;

    
    
    Size dsize = Size(640,480);
    if(image.size() != dsize){
        resize(image, image, dsize);
        resize(depth, depth, dsize);
        camera_info_tmp.P[2] = camera_info_tmp.P[2] * (640.0/image.cols);
        camera_info_tmp.P[6] = camera_info_tmp.P[6] * (480.0/image.rows);
    }   
    // cout <<  "img.size:" << image.size() << "  depth.size:" << depth.size() << endl;
   //timepoint[timeindex++] = std::chrono::steady_clock::now();
    
    std::vector<cv::KeyPoint> kpts;
    std::vector<std::vector<float> > dspts;
    
    superpoint.ExactSP(image, kpts, dspts);
   //timepoint[timeindex++] = std::chrono::steady_clock::now();
    
    dslam_sp::EF_output feature_msg;
    feature_msg.header = msg->image.header;
    // feature_msg.header.frame_id = std::to_string(frame_id++);
    feature_msg.header.frame_id = msg->header.frame_id;
    feature_msg.P = camera_info_tmp.P;
    cout << "feature_msg.header.frame_id:" << feature_msg.header.frame_id << endl;
    
    sensor_msgs::ImagePtr dp_ptr = cv_bridge::CvImage(std_msgs::Header(), msg->depth.encoding, depth).toImageMsg();
    feature_msg.depth = *dp_ptr;
    
    for(int i = 0; i < kpts.size(); i++)
    { 
        cv::Point p(kpts[i].pt.x, kpts[i].pt.y);
        cv::circle(image, p , 5, (0, 255, 0), -1);
        
        dslam_sp::KeyPoint kp_ros;
        KeyPoint_cv2ros(kpts[i],kp_ros);
        feature_msg.keypoints.keypoint.push_back(kp_ros);
        
        dslam_sp::Descriptor ds_ros;
        ds_ros.desc = dspts[i];
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
    int o;
    const char *optstring = "P:W:"; // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'P':
                strcpy(ProFileDir, optarg);
                break;
            case 'W':
                strcpy(ModelFileDir, optarg);
                break;
            default:
                break;
        }
    }
  Caffe::set_mode(Caffe::GPU);
  SuperPoint superpoint = SuperPoint(ProFileDir, ModelFileDir, 200);
  ros::init(argc, argv, "superpoint_EF", ros::init_options::AnonymousName);
  if (ros::names::remap("image") == "image") {
    ROS_WARN("Topic 'image' has not been remapped! Typical command-line usage:\n"
             "\t$ rosrun superpoint_EF superpoint_EF_node image:=<image topic> [transport]");
  }

  ros::NodeHandle n;
  std::string topic = n.resolveName("image");
  g_window_name = topic;
  
  pub = n.advertise<dslam_sp::EF_output>("/superpoint_EF/featurepoints_descriptors", 1); //创建publisher，往"featurepoints_descriptors"话题上发布消息
  
  // namedWindow( g_window_name, WINDOW_AUTOSIZE );// Create a window for display.
  
  ros::Subscriber sub_info = n.subscribe("/mynteye/left_rect/camera_info", 1, info_Callback);
  ros::Subscriber sub = n.subscribe<dslam_sp::image_depth>(topic, 1, boost::bind(&img_Callback,_1, superpoint) );  //设置回调函数gpsCallback
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}
