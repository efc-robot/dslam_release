#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <dslam_sp/image_depth.h>

#include <queue>

using namespace std;

std::queue <sensor_msgs::Image::ConstPtr> image_queue;
ros::Publisher pub;

void img_Callback(const sensor_msgs::Image::ConstPtr &msg)
{  
    //Convert to OpenCV native BGR color
    cout << "img.stamp:" << msg->header.stamp << endl;
    
    image_queue.push(msg);
}

void depth_Callback(const sensor_msgs::Image::ConstPtr &msg)
{  
    //Convert to OpenCV native BGR color
    cout << "depth.stamp:" << msg->header.stamp << endl;
    
    ros::Duration match_thresh_i_d(0.0);
    ros::Duration match_thresh_d_i(0.01);
    while(true) {
        if ( (! image_queue.empty() ) ) {
            //if队列不是空的
            if ( image_queue.front()->header.stamp >= msg->header.stamp - match_thresh_i_d ) {
                //if image[0]比depth晚或同时
                if ( image_queue.front()->header.stamp <= msg->header.stamp + match_thresh_d_i ) {
                    //if 时间差不超过阈值
                    dslam_sp::image_depth img_depth_msg;
                    img_depth_msg.image = *image_queue.front();
                    img_depth_msg.depth = *msg;
                    pub.publish(img_depth_msg);//发布msg
                    cout <<  "img.stamp:" << img_depth_msg.image.header.stamp << "depth.stamp:" << img_depth_msg.depth.header.stamp << endl;
                    return;
                } else {
                    //时间差超过阈值
                    return;
                }
            } else {
                // image[0]比depth早
                image_queue.pop();
            }
        } else {
            //队列是空的
            return;
        }
    }
                    
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "dslam_sp");

  ros::NodeHandle n;
  
  pub = n.advertise<dslam_sp::image_depth>("/merge/img_depth", 1); //创建publisher，往"/merge/img_depth"话题上发布消息
  
  ros::Subscriber sub_image = n.subscribe("/mynteye/left_rect/image_rect", 1, img_Callback);  //设置回调函数img_Callback
  ros::Subscriber sub_depth = n.subscribe("/mynteye/depth/image_raw", 1, depth_Callback);
  ros::spin(); //ros::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
  //而ros::spinOnce()只会去触发一次
  return 0;
}