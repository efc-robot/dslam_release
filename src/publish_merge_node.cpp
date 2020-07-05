#include <ros/ros.h>
#include <ros/time.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <dslam_sp/image_depth.h>
#include <string>

#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

int self_ID = 1;
int framestart = 1;
int frameend = 3000;
// frame 规则是 self_ID*1e8 + framecount

int main(int argc, char **argv)
{
    int o;
    stringstream tmpss;
    char FileDir[200] = "/PATH/TO/DATASET/";
    const char *optstring = "p:i:s:e:"; // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'p':
                strcpy(FileDir, optarg);
                break;
            case 'i':
                tmpss.clear();
                // cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> self_ID;
                break;
            case 's':
                tmpss.clear();
                // cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> framestart;
                break;
            case 'e':
                tmpss.clear();
                // cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> frameend;
                break;
            default:
                break;
        }
    }

  ros::init(argc, argv, "publish_merge_node");

  cout << " ID: " << self_ID << ". start: " << framestart << ". end: " << frameend << endl;
  ros::NodeHandle n;
  
  int tmp = framestart;
  // cv::namedWindow(FileDir);
  
  dslam_sp::image_depth img_depth_msg;
  sensor_msgs::CameraInfo camera_info;
  camera_info.P = {367.5128813981231, 0.0, 391.8034210205078, 0.0, 0.0, 367.5128813981231, 246.19532012939453, 0.0, 0.0, 0.0, 1.0, 0.0};
  
  ros::Publisher pub = n.advertise<dslam_sp::image_depth>("/merge/img_depth_file", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_info = n.advertise<sensor_msgs::CameraInfo>("/mynteye/left_rect/camera_info", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_image = n.advertise<sensor_msgs::Image>("/mynteye/left_rect/image", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_depth = n.advertise<sensor_msgs::Image>("/mynteye/left_rect/depth", 1); //创建publisher，往话题上发布消息
  ros::Rate loop_rate1(0.5);   //定义发布的频率，20HZ 
  loop_rate1.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  ros::Rate loop_rate(1);   //定义发布的频率，20HZ 
  
  while (ros::ok() && tmp < frameend)   //循环发布msg
  {
    char depth_name[200],image_name[200];
    
    sprintf(image_name, "%s/left_%d.png", FileDir, tmp);
    sprintf(depth_name, "%s/depth_%d.png", FileDir, tmp);
    
    Mat Image = imread ( image_name, CV_LOAD_IMAGE_UNCHANGED );
    Mat Depth = imread ( depth_name, CV_LOAD_IMAGE_UNCHANGED );
    // cout << "Image.type(): " << Image.type() << endl;
    // cout << "imshow" << image_name << endl;
    // cv::imshow(FileDir, Image);
    // cv::waitKey(1); 
    
    // cout << "CvImage.toImageMsg" << endl;
    sensor_msgs::ImagePtr ptr = cv_bridge::CvImage(std_msgs::Header(), "mono8", Image).toImageMsg();
    img_depth_msg.image = *ptr;
    ptr = cv_bridge::CvImage(std_msgs::Header(), "mono16", Depth).toImageMsg();
    img_depth_msg.depth = *ptr;

    img_depth_msg.header.frame_id = std::to_string(int(self_ID*1e8 + tmp) );
    // img_depth_msg.header.seq = int(self_ID*1e8 + tmp);
    img_depth_msg.header.stamp = ros::Time::now();
    tmp++;
    
    if ( tmp %4 == 0){
        cout << "publish: seq=" << img_depth_msg.header.seq << " frame_id=" << img_depth_msg.header.frame_id << " stamp=" << img_depth_msg.header.stamp.sec << " . " << img_depth_msg.header.stamp.nsec << endl;
        pub.publish(img_depth_msg);//以1Hz的频率发布msg
        pub_image.publish(img_depth_msg.image);
        pub_depth.publish(img_depth_msg.depth);
        pub_info.publish(camera_info);//以1Hz的频率发布msg
        loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
    }
    
    // cv_bridge::CvImageConstPtr cv_ptr1;
    // sensor_msgs::ImagePtr img_ptr = boost::make_shared<::sensor_msgs::Image>(img_depth_msg.image);
    // cout << img_depth_msg.image.encoding << endl;
    // cv_ptr1=cv_bridge::toCvShare(img_ptr, img_depth_msg.image.encoding);
    // const cv::Mat &image_raw = cv_ptr1->image;
    // cv::imshow(FileDir, image_raw);
    // cv::waitKey(1); 
  }
  
  return 0;
}