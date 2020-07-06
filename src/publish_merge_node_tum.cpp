#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <dslam_sp/image_depth.h>
#include <string>

#include<fstream>  

#include <sstream>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace std;
using namespace cv;

int self_ID = 1;
int framestart = 1;
int frameend = 3000;
// frame 规则是 self_ID*1e8 + framecount

bool get_Name_and_Scale( char* listFileDir, int seq, char* rgbname, char* depthname, char* time)	{
  string line;
  char listname[200];
  int i = 1;
//   char listFileDir[200] = "/home/linaro/dataset/tum/rgbd_dataset_freiburg2_desk/";
  sprintf(listname, "%smatch.txt", listFileDir);
  
  //cout << listname << endl;
  ifstream myfile(listname);
  char tmp[30];
  if (myfile.is_open())
  {
    while (( getline (myfile,line) ) && (i<=seq))
    {
      std::istringstream in(line);
      //cout << line << '\n';
      for (int j=0; j<4; j++)  {
        in >> tmp ;
        //cout << "tmp is " << tmp << endl;
        if (j==3) sprintf(depthname, "%s%s", listFileDir, tmp);
        if (j==1) sprintf(rgbname, "%s%s", listFileDir, tmp);
        if (j==0) sprintf(time, "%s", tmp);
        //cout << "rgbname is " << rgbname << endl;
      }
      
      i++;
    }
    myfile.close();
  }
  else {
    cout << "Unable to open file";
  }
  return i>seq;
}

int find_camera_info(char* listFileDir)
{
    char* Dir = listFileDir;
    while( strlen(Dir)>=9 )
    {
        if (Dir[0]=='f'&&Dir[1]=='r'&&Dir[2]=='e'&&Dir[3]=='i'&&Dir[4]=='b'&&Dir[5]=='u'&&Dir[6]=='r'&&Dir[7]=='g') //freiburg
        {
            int info = Dir[8] - '0';
            return info;
        }
        Dir++;
    }
    return -1;
}

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
                cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> self_ID;
                break;
            case 's':
                tmpss.clear();
                cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> framestart;
                break;
            case 'e':
                tmpss.clear();
                cout<< optarg << endl;
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

  int K_FREIBURG = find_camera_info(FileDir);
  cout << " freiburg: " << K_FREIBURG << endl;
  if ( K_FREIBURG == 1 )
    camera_info.P = {517.3, 0, 318.6, 0, 0, 516.5, 255.3, 0, 0, 0, 1, 0};//freiburg1
  else if  ( K_FREIBURG == 2 )
    camera_info.P = {520.9, 0, 325.1, 0, 0, 521.0, 249.7, 0, 0, 0, 1, 0};//freiburg2
  else
    camera_info.P = {535.4, 0, 320.1, 0, 0, 539.2, 247.6, 0, 0, 0, 1, 0};//freiburg3
  
  ros::Publisher pub = n.advertise<dslam_sp::image_depth>("/merge/img_depth_file", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_info = n.advertise<sensor_msgs::CameraInfo>("/mynteye/left_rect/camera_info", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_image = n.advertise<sensor_msgs::Image>("/mynteye/left_rect/image", 1); //创建publisher，往话题上发布消息
  ros::Publisher pub_depth = n.advertise<sensor_msgs::Image>("/mynteye/left_rect/depth", 1); //创建publisher，往话题上发布消息
  ros::Rate loop_rate1(0.5);   //定义发布的频率，20HZ 
  loop_rate1.sleep();//根据前面的定义的loop_rate,设置1s的暂停
  ros::Rate loop_rate(6);   //定义发布的频率，20HZ 
  
  while (ros::ok() && tmp < frameend)   //循环发布msg
  {
    char depth_name[200],image_name[200],time_stamp[200];

    if(!get_Name_and_Scale(FileDir, tmp, image_name, depth_name, time_stamp)) break;
    
    Mat Image;
    Mat Image_c = imread ( image_name, CV_LOAD_IMAGE_UNCHANGED );
    Mat Depth = imread ( depth_name, CV_LOAD_IMAGE_UNCHANGED );
    Depth = Depth * 0.2;
    // we work with grayscale images
    cvtColor(Image_c, Image, COLOR_BGR2GRAY);
    // cout << "Image.type(): " << Image.type() << endl;
    // cout << "imshow" << image_name << endl;
    // cv::imshow(FileDir, Image);
    // cv::waitKey(1); 
    
    // cout << "CvImage.toImageMsg" << endl;
    sensor_msgs::ImagePtr ptr = cv_bridge::CvImage(std_msgs::Header(), "mono8", Image).toImageMsg();
    img_depth_msg.image = *ptr;
    ptr = cv_bridge::CvImage(std_msgs::Header(), "mono16", Depth).toImageMsg();
    img_depth_msg.depth = *ptr;

    img_depth_msg.header.frame_id = "odom";
    img_depth_msg.header.seq = int(self_ID*1e8 + tmp);

    char* t_stamp = strtok(time_stamp, ".");
    img_depth_msg.header.stamp.sec = atoi(t_stamp);
    t_stamp = strtok(NULL, ".");
    img_depth_msg.header.stamp.nsec = atoi(t_stamp);
    cout << "img_depth_msg.header.stamp:" << img_depth_msg.header.stamp.sec << " . " << img_depth_msg.header.stamp.nsec << endl;

    tmp++;
    
    cout << "publish: " << tmp << endl;
    // if ( tmp %4 == 0){
      pub.publish(img_depth_msg);//以1Hz的频率发布msg
      pub_image.publish(img_depth_msg.image);
      pub_depth.publish(img_depth_msg.depth);
      pub_info.publish(camera_info);//以1Hz的频率发布msg
      loop_rate.sleep();//根据前面的定义的loop_rate,设置1s的暂停
    // }
    
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