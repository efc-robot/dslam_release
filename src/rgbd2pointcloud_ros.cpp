/*************************************************************************
	> File Name: src/rgbd2pointcloud_ros.cpp
	> Author: Gao Xiang
	> Mail: gaoxiang12@mails.tsinghua.edu.cn
	> Created Time: 2015年12月13日 星期日 14时37分05秒
 ************************************************************************/

#include <iostream>
#include <vector>

// opencv 用于图像数据读取与处理
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// pcl
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

// boost.format 字符串处理
#include <boost/format.hpp>

#include <ros/ros.h>
#include <dslam_sp/Pose_with_image.h>
#include <sensor_msgs/PointCloud2.h>
#include <cv_bridge/cv_bridge.h>

using namespace std;

ros::Publisher pub,pub2;

void Callback(const dslam_sp::Pose_with_image::ConstPtr &msg)
{
    cv_bridge::CvImageConstPtr cv_ptr1, cv_ptr2;
    sensor_msgs::ImagePtr img_ptr = boost::make_shared<::sensor_msgs::Image>(msg->image);
    sensor_msgs::ImagePtr dep_ptr = boost::make_shared<::sensor_msgs::Image>(msg->depth);
    
    cv_ptr1=cv_bridge::toCvShare(img_ptr, msg->image.encoding);
    const cv::Mat &image = cv_ptr1->image;
    cv_ptr2=cv_bridge::toCvShare(dep_ptr, msg->depth.encoding);
    const cv::Mat &depth = cv_ptr2->image;

// 相机矩阵
    float camera_cx    =   msg->P[2];
    float camera_cy    =   msg->P[6];
    float camera_fx     =   msg->P[0];
    float camera_fy     =   msg->P[5];
    float camera_scale  = 1000;

    pcl::PointCloud<pcl::PointXYZRGBA> cloud; 

    for ( int m=0; m<depth.rows; m++ ){
        for ( int n=0; n<depth.cols; n++ )
        {
            float z;
            if(depth.type()==CV_32FC1){
                z = depth.ptr<float> (m) [n];
            }
            else if(depth.type()==CV_16UC1){
                ushort d = depth.ptr<ushort> (m) [n];
                z = float(d) / camera_scale;
            }
            if (z == 0)
                continue;
            float x = (n - camera_cx) * z / camera_fx;
            float y = (m - camera_cy) * z / camera_fy;
            pcl::PointXYZRGBA p;
            p.x = x; p.y = y; p.z = z;

            const uchar* rgbdata = &image.ptr<uchar>(m)[n];
            uchar b = rgbdata[0];
            uchar g = rgbdata[0];
            uchar r = rgbdata[0];

            p.r = r; p.g = g; p.b = b;

            if( y<0.1 && y>-0.1 )
            {
                cloud.points.push_back( p ); 
            }
        }
    }
    
    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(cloud, output);
    output.header = msg->header;
    pub.publish(output);
    pub2.publish(output);
}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "rgbd2pointcloud");
    ros::NodeHandle nh;

    pub = nh.advertise<sensor_msgs::PointCloud2>("pointcloud_part_local", 3); //创建publisher，往话题上发布消息
    pub2 = nh.advertise<sensor_msgs::PointCloud2>("pointcloud_part_local_forGlobalMap", 3); //创建publisher，往话题上发布消息

    if (ros::names::remap("data") == "data") {
        ROS_WARN("rgbd2octomap Topic 'data' has not been remapped! ");
    }

    std::string topic = nh.resolveName("data");

    ros::Subscriber sub = nh.subscribe(topic, 1, Callback);  //设置回调函数Callback
    ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
    return 0;
}
