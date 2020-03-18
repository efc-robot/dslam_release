#ifndef _ORB_FEATURE_H_
#define _ORB_FEATURE_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <stdlib.h>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <sstream>

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
#include <boost/make_shared.hpp>
#include <sstream>
// #include <dslam_sp/SuperPoint.hpp>

#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <chrono>
#include <ros/ros.h>   
#include <geometry_msgs/TransformStamped.h>   
#include <dslam_sp/EF_output.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <std_msgs/Time.h>
#include <dslam_sp/top_k.h>
#include <dslam_sp/bundleAdjustment.h>

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <boost/make_shared.hpp>

using namespace cv;
using namespace std;

#define KEEP_K_POINTS 1000
#define MATCHER "BF"
#define NN_thresh 0.7


// #define KEEP_K_POINTS 1000
// #define MATCHER "BF"
// std::vector<dslam_sp::KeyPoint> keypoints1,keypoints2;
//     std::vector<dslam_sp::Descriptor> descriptors1,descriptors2;

Point2d pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}

void ORB_FeatureExtract_ROS(const cv::Mat &image_raw,const  cv::Mat &depth_raw,const  sensor_msgs::CameraInfo camera_info, Size dsize, std::vector<cv::KeyPoint>& keypoints, Mat& descriptor_results, Ptr<FeatureDetector> &detector, Ptr<DescriptorExtractor> &descriptor, sensor_msgs::CameraInfo& camera_info_tmp, Mat& depth_resized, Mat& image_resized);

int ORB_Match_VO(vector<Point2f>& points_prev, vector<Point2f>& points_curr, Mat& desc_prev, Mat& desc_curr, Mat& depth_prev, Mat& depth_curr, boost::array<double,12> camera_infoP,  Eigen::Quaterniond& q, Mat& t);


void ORB_FeatureExtract_ROS(const cv::Mat &image_raw,const  cv::Mat &depth_raw,const  sensor_msgs::CameraInfo camera_info, Size dsize, std::vector<cv::KeyPoint>& keypoints, Mat& descriptor_results, Ptr<FeatureDetector> &detector, Ptr<DescriptorExtractor> &descriptor, sensor_msgs::CameraInfo& camera_info_tmp, Mat& depth_resized, Mat& image_resized){
        //裁剪到dsize
    cout << "orbfile 001" << endl;
    Mat& image = image_resized;
    Mat& depth = depth_resized;
    int image_raw_height = image_raw.rows;
    int image_raw_width = image_raw.cols;
    camera_info_tmp = camera_info;
    cout << "orbfile 002" << endl;
    cout << "image_raw_height: " << image_raw_height << "  image_raw_width: " << image_raw_width << endl;
    if((float)image_raw_height/image_raw_width > 0.75) {
        cout << "orbfile 0021" << endl;
        int height_ = image_raw_width /4 *3;
        int margin = (image_raw_height - height_)/2;
        image = image_raw(Rect(0,margin,image_raw_width,height_));
        depth = depth_raw(Rect(0,margin,image_raw_width,height_));
        camera_info_tmp.P[6] = camera_info_tmp.P[6] - margin;
    }
    else if(image_raw_height/image_raw_width < 0.75) {
        cout << "orbfile 0022" << endl;
        int width_ = image_raw_height /3 *4;
        int margin = (image_raw_width - width_)/2;
        image = image_raw(Rect(margin,0,width_,image_raw_height));
        depth = depth_raw(Rect(margin,0,width_,image_raw_height));
        camera_info_tmp.P[2] = camera_info_tmp.P[2] - margin;
        // cout <<  "width_" << width_ << "  margin" << margin << endl;
    }
    cout << "orbfile 003" << endl;
    if(image.size() != dsize){
        resize(image, image, dsize);
        resize(depth, depth, dsize);
        camera_info_tmp.P[2] = camera_info_tmp.P[2] * (640.0/image.cols);
        camera_info_tmp.P[6] = camera_info_tmp.P[6] * (480.0/image.rows);
    }   

    detector->detect ( image,keypoints );
    cout << "orbfile 004" << endl;
    //NMS
    // selectMax(NMS_Threshold, keypoints);
    cout << "keypoints size:" << keypoints.size() << endl;
    
    // for(int i=0; i<min( KEEP_K_POINTS, int(keypoints.size()-1) ); i++) {
    //     for(int j=keypoints.size()-1; j>i; j--) {
    //         if(keypoints[j].response > keypoints[j-1].response) {
    //             swap(keypoints[j], keypoints[j-1]);
    //         }
    //     }
    // }
    // if(keypoints.size()>KEEP_K_POINTS) keypoints.resize(KEEP_K_POINTS, KeyPoint());

    //-- 第二步:根据角点位置计算 BRIEF 描述子
    descriptor->compute ( image, keypoints, descriptor_results );
    cout << "orbfile 005" << endl;

    cout << "descriptor_results: "<< descriptor_results.size() << endl;
    cout << "cols: "<< descriptor_results.cols << endl;
    cout << "rows: "<< descriptor_results.rows << endl;
    cout << "type: "<< descriptor_results.type() << endl;

    

    return;
}


int ORB_Match_VO(vector<Point2f>& points_prev, vector<Point2f>& points_curr, Mat& desc_prev, Mat& desc_curr, Mat& depth_prev, Mat& depth_curr, const boost::array<double,12> camera_infoP,  Eigen::Quaterniond& q, Mat& t){
    vector<DMatch> matches;
        if( MATCHER == "BF" ) {

            Ptr<DescriptorMatcher> matcher  = DescriptorMatcher::create ( "BruteForce-Hamming" );
            cout << "BF1" << endl;
            matcher->match ( desc_prev, desc_curr, matches );
            cout << "BF2" << endl;
            // BFMatcher matcher(NORM_L2, true);
            // matcher.match(desc_prev, desc_curr, matches);
        }
        else {
            FlannBasedMatcher matcher;
            matcher.match(desc_prev, desc_curr, matches);
        }
        
        //-- 匹配点对筛选
        double min_dist=10000, max_dist=0;

        //找出所有匹配之间的最小距离和最大距离, 即是最相似的和最不相似的两组点之间的距离
        for ( int i = 0; i < desc_prev.rows; i++ )
        {
            double dist = matches[i].distance;
            if ( dist < min_dist ) min_dist = dist;
            if ( dist > max_dist ) max_dist = dist;
        }

        printf ( "-- Max dist : %f \n", max_dist );
        printf ( "-- Min dist : %f \n", min_dist );



        //配对后的筛选
        vector <Point2f> RAN_KP1, RAN_KP2;
        for(int i=0; i<matches.size(); i++) {
            if ( matches[i].distance> max ( 3*min_dist, 30.0 ) )
                continue;
            // if ( abs(points1[matches[i].queryIdx].x-points2[matches[i].trainIdx].x)> Width/10 )
                // continue;
            // if ( abs(points1[matches[i].queryIdx].y-points2[matches[i].trainIdx].y)> Height/10 )
                // continue;
            RAN_KP1.push_back(points_prev[matches[i].queryIdx]);
            RAN_KP2.push_back(points_curr[matches[i].trainIdx]);
            //cout << matches[i].distance << endl;
            //RAN_KP1是要存储img01中能与img02匹配的点
        }
        cout <<  "RAN_KP1 size:" << RAN_KP1.size() << endl;
        
        //通过RansacStatus来删除误匹配点
        vector<uchar> RansacStatus;
        Mat Fundamental = findFundamentalMat(RAN_KP1, RAN_KP2, RansacStatus, FM_RANSAC);
        
        //2d to 3d的投射
        vector<Point3f> points_3d;
        vector<Point2f> points_2d;        //vectors to store the coordinates of the feature points
        Mat K = ( Mat_<double> ( 3,3 ) << camera_infoP[0], 0, camera_infoP[2], 0, camera_infoP[5], camera_infoP[6], 0, 0, 1 );
        Mat img_2 = Mat::zeros(480,640,CV_8UC3);
        for ( int i=0; i<RAN_KP1.size(); i++ )
        {
            if (RansacStatus[i] == 0)
                continue;
            ushort d = depth_prev.ptr<unsigned short> (int ( RAN_KP1[i].y )) [ int ( RAN_KP1[i].x ) ];
            if ( d == 0 )   // bad depth
                continue;
            float dd = d/1000.0;
            Point2d p1 = pixel2cam ( RAN_KP1[i], K );
            points_3d.push_back ( Point3f ( p1.x*dd, p1.y*dd, dd ) );
            points_2d.push_back ( RAN_KP2[i] );

            line(img_2, RAN_KP1[i], RAN_KP2[i], Scalar(0, 0, 255));
            circle(img_2, RAN_KP2[i], 4, cv::Scalar(0, 0, 255));
        }
        // cout<<"3d-2d pairs: "<<points_3d.size() <<endl;
        // imshow( g_window_name, img_2 );
        // waitKey(1);
        int  returnresult = points_3d.size();
        if (points_3d.size()<8) {
            // ROS_INFO("stamp:%d.%d 3d-2d pairs:%d", header.stamp.sec, header.stamp.nsec, points_3d.size());

            return returnresult;
        }
        
        Mat r, R;
        solvePnP ( points_3d, points_2d, K, Mat(), r, t, false, cv::SOLVEPNP_EPNP ); // 调用OpenCV 的 PnP 求解，可选择EPNP，DLS等方法
        cv::Rodrigues ( r, R ); // r为旋转向量形式，用Rodrigues公式转换为矩阵
        
        bundleAdjustment ( points_3d, points_2d, K, R, t );
        
        Eigen::Matrix3d R_eigen;
        cv2eigen(R, R_eigen);
        q = Eigen::Quaterniond(R_eigen); 
        return returnresult;
}

#endif
