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

#define KEEP_K_POINTS 1000
#define MATCHER "BF"
#define NN_thresh 0.7

using namespace cv;
using namespace std;

string g_window_name;
ros::Publisher pub;

vector<Point2f> points_prev;
Mat desc_prev;
cv::Mat depth_prev;
std_msgs::Header header_prev;

bool First_Frame;

Point2d pixel2cam ( const Point2d& p, const Mat& K )
{
    return Point2d
           (
               ( p.x - K.at<double> ( 0,2 ) ) / K.at<double> ( 0,0 ),
               ( p.y - K.at<double> ( 1,2 ) ) / K.at<double> ( 1,1 )
           );
}

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
        Mat K = ( Mat_<double> ( 3,3 ) << msg->P[0], 0, msg->P[2], 0, msg->P[5], msg->P[6], 0, 0, 1 );
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
        
        if (points_3d.size()<8) {
            ROS_INFO("stamp:%d.%d 3d-2d pairs:%d", msg->header.stamp.sec, msg->header.stamp.nsec, points_3d.size());
            
            points_prev = points_curr;
            desc_prev = desc_curr;
            depth_prev = depth_curr;
            header_prev = msg->header;
            
            First_Frame = false;
            
            return;
        }
        
        Mat r, t, R;
        solvePnP ( points_3d, points_2d, K, Mat(), r, t, false, cv::SOLVEPNP_EPNP ); // 调用OpenCV 的 PnP 求解，可选择EPNP，DLS等方法
        cv::Rodrigues ( r, R ); // r为旋转向量形式，用Rodrigues公式转换为矩阵
        
        bundleAdjustment ( points_3d, points_2d, K, R, t );
        
        Eigen::Matrix3d R_eigen;
        cv2eigen(R, R_eigen);
        Eigen::Quaterniond q = Eigen::Quaterniond(R_eigen); 
        
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