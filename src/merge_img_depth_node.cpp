#include <ros/ros.h>
#include <unistd.h>
#include <sensor_msgs/Image.h>
#include <dslam_sp/image_depth.h>

#include <queue>

using namespace std;

int self_ID = 1;

std::queue <sensor_msgs::Image::ConstPtr> image_queue;
ros::Publisher pub;

void img_Callback(const sensor_msgs::Image::ConstPtr &msg)
{  
    //Convert to OpenCV native BGR color
    cout << "img.stamp:" << msg->header.stamp << endl;
    if (msg->width==0 || msg->height==0){
        cout << "image.width:" << msg->width << " image.height:" << msg->height << endl;
        return;
    }
    
    image_queue.push(msg);
}

void depth_Callback(const sensor_msgs::Image::ConstPtr &msg)
{  
    //Convert to OpenCV native BGR color
    cout << "depth.stamp:" << msg->header.stamp << endl;
    if (msg->width==0 || msg->height==0){
        cout << "depth.width:" << msg->width << " depth.height" << msg->height << endl;
        return;
    }
    
    ros::Duration match_thresh_i_d(0.1);
    ros::Duration match_thresh_d_i(0.4);
    while(true) {
        if ( (! image_queue.empty() ) ) {
            //if队列不是空的
            if ( image_queue.front()->header.stamp >= msg->header.stamp - match_thresh_i_d ) {
                //if image[0]比depth新或同时
                if ( image_queue.front()->header.stamp <= msg->header.stamp + match_thresh_d_i ) {
                    //if 时间差不超过阈值 image[0]比depth没有新太多
                    dslam_sp::image_depth img_depth_msg;
                    img_depth_msg.image = *image_queue.front();
                    img_depth_msg.depth = *msg;
                    img_depth_msg.header.stamp = img_depth_msg.image.header.stamp;
                    img_depth_msg.header.frame_id = std::to_string(int(self_ID*1e8 + img_depth_msg.image.header.seq%int(1e8)) );
                    pub.publish(img_depth_msg);//发布msg
                    cout <<  "img.stamp:" << img_depth_msg.image.header.stamp << "depth.stamp:" << img_depth_msg.depth.header.stamp << endl;
                    return;
                } else {
                    //时间差超过阈值 image[0]比depth新太多，返回，等待最新的depth
                    return;
                }
            } else {
                // image[0]比depth早，扔掉image[0]，换一个新的image
                image_queue.pop();
            }
        } else {
            //队列是空的
            cout << "image empty:" << endl;
            ros::Duration(0.02).sleep();
        }
    }
                    
}

int main(int argc, char **argv)
{
    int o;
    stringstream tmpss;
    const char *optstring = "i:"; // 有三个选项-abc，其中c选项后有冒号，所以后面必须有参数
    while ((o = getopt(argc, argv, optstring)) != -1) {
        switch (o) {
            case 'i':
                tmpss.clear();
                // cout<< optarg << endl;
                tmpss << optarg;
                tmpss >> self_ID;
                break;
            default:
                break;
        }
    }
  
    ros::init(argc, argv, "dslam_sp");

    ros::NodeHandle n;
    
    pub = n.advertise<dslam_sp::image_depth>("merge/img_depth", 1); //创建publisher，往"/merge/img_depth"话题上发布消息
    
    ros::Subscriber sub_image = n.subscribe("mynteye/left_rect/image_rect", 1, img_Callback);  //设置回调函数img_Callback
    ros::Subscriber sub_depth = n.subscribe("mynteye/depth/image_raw", 1, depth_Callback);
    ros::MultiThreadedSpinner spinner(2); // Use 2 threads
    spinner.spin(); // spin() will not return until the node has been shutdown
    //ros::spin(); //ros;::spin()用于调用所有可触发的回调函数，将进入循环，不会返回，类似于在循环里反复调用spinOnce() 
    //而ros::spinOnce()只会去触发一次
    return 0;
}