/*************************************************************************
	> File Name: src/rgbd2octomap_ros.cpp
	> Author: Gao Xiang
	> Mail: gaoxiang12@mails.tsinghua.edu.cn
	> Created Time: 2015年12月13日 星期日 14时37分05秒
 ************************************************************************/

#include <iostream>
#include <vector>

// octomap 
#include <octomap/octomap.h>
#include <octomap/ColorOcTree.h>
#include <octomap/math/Pose6D.h>

// opencv 用于图像数据读取与处理
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// 使用Eigen的Geometry模块处理3d运动
#include <Eigen/Core>
#include <Eigen/Geometry> 

// pcl
#include <pcl/common/transforms.h>
#include <pcl/point_types.h>

// boost.format 字符串处理
#include <boost/format.hpp>

#include <ros/ros.h>
#include <octomap_server/OctomapServer.h>
#include <nav_msgs/OccupancyGrid.h>
#include <dslam_sp/Pose_with_image.h>
#include <cv_bridge/cv_bridge.h>

using namespace std;

typedef octomap::ColorOcTree OcTreeT;

octomap::ColorOcTree tree( 0.25 ); //全局map
octomap::Pointcloud cloud_octo;
octomap_server::OctomapServer* server;
ros::Publisher pub;
ofstream myfile;

void Callback(const dslam_sp::Pose_with_image::ConstPtr &msg)
{
    Eigen::Quaterniond q( msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z );
    // q = Eigen::Quaterniond( 0.7071, 0.7071, 0, 0 ) * q;
    Eigen::Isometry3d pose(q);
    pose(0,3) = msg->pose.position.x; 
    pose(1,3) = msg->pose.position.y; 
    pose(2,3) = msg->pose.position.z;
    myfile << msg->header.stamp.sec << "." << msg->header.stamp.nsec << " " ;
    myfile << msg->pose.position.x << " " << msg->pose.position.y << " " << msg->pose.position.z << " ";
    myfile << msg->pose.orientation.x << " " << msg->pose.orientation.y << " " << msg->pose.orientation.z << " " << msg->pose.orientation.w << endl;

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
            ushort d = depth.ptr<ushort> (m) [n];
            if (d == 0)
                continue;
            float z = float(d) / camera_scale;
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
    // 将cloud旋转之后插入全局地图
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr temp( new pcl::PointCloud<pcl::PointXYZRGBA>() );
    pcl::transformPointCloud( cloud, *temp, pose.matrix() );

    for (auto p:temp->points)
    {
        cloud_octo.push_back( p.x, p.y, p.z );
        tree.insertRay(octomap::point3d( pose(0,3), pose(1,3), pose(2,3) ), octomap::point3d( p.x, p.y, p.z ));
        tree.integrateNodeColor( p.x, p.y, p.z, p.r, p.g, p.b );
    }

    server->handlePreNodeTraversal(ros::Time::now());

    // cout << "it.getSize() = " << server->m_octree->begin(server->m_maxTreeDepth).getSize() << endl;
    // server->m_occupancyMinZ = -0.1;
    // server->m_occupancyMaxZ = 0.1;
    server->m_filterSpeckles = true;
    for (OcTreeT::iterator it = server->m_octree->begin(server->m_maxTreeDepth), end = server->m_octree->end(); it != end; ++it)
    {
        double z = it.getZ();
        double half_size = it.getSize() / 2.0;
        if (z + half_size > server->m_occupancyMinZ && z - half_size < server->m_occupancyMaxZ)
        {
            if (server->m_octree->isNodeOccupied(*it))
            {
                double x = it.getX();
                double y = it.getY();

                // Ignore speckles in the map:
                if (server->m_filterSpeckles && (it.getDepth() == server->m_treeDepth +1) && server->isSpeckleNode(it.getKey())){
                    ROS_DEBUG("Ignoring single speckle at (%f,%f,%f)", x, y, z);
                    continue;
                } // else: current octree node is no speckle, send it out

                server->update2DMap(it, true);   
            }else{
                server->update2DMap(it, false);
            }
        }
    }
    cout<<"update2DMap"<<endl;
    pub.publish(server->m_gridmap);
 
}

int main( int argc, char** argv )
{
    ros::init(argc, argv, "octomap_demo");
    ros::NodeHandle nh;
    const ros::NodeHandle private_nh("~");

    pub = nh.advertise<nav_msgs::OccupancyGrid>("/octomap_demo/costmap", 1); //创建publisher，往话题上发布消息

    if (ros::names::remap("data") == "data") {
        ROS_WARN("rgbd2octomap Topic 'data' has not been remapped! ");
    }
    if (ros::names::remap("trajectory_dir") == "trajectory_dir") {
        ROS_WARN("rgbd2octomap trajectory_dir has not been remapped! ");
    }

    std::string topic = nh.resolveName("data");
    std::string trajectory_dir = nh.resolveName("trajectory_dir");

    myfile.open (trajectory_dir);
    myfile << "# timestamp tx ty tz qx qy qz qw" << endl;

    server = new octomap_server::OctomapServer(private_nh, nh);
    server->m_octree = &tree;
    server->m_publish2DMap = true;


    ros::Subscriber sub = nh.subscribe(topic, 1, Callback);  //设置回调函数Callback
    ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
    return 0;
}
