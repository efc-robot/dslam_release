#include <iostream>
#include <opencv2/opencv.hpp>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <dslam_sp/image_depth.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include "MatterSim.hpp"

using namespace mattersim;

#define WIDTH  640
#define HEIGHT 480
#define VFOV 60/180*3.14159265358979323846
#define HFOV VFOV*WIDTH/HEIGHT

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define ANGLEDELTA 5*M_PI/180

int main(int argc, char *argv[]) {

    ros::init(argc, argv, "publish_sim_node");
    ros::NodeHandle n;

    cv::namedWindow("C++ RGB");

    dslam_sp::image_depth img_depth_msg;
    sensor_msgs::CameraInfo camera_info;
    camera_info.P = {-3.35368, 1.27672, 21.592, 0, 0, -1,  0, 1, 0, 0.4, 0.326155, 1};
    ros::Publisher pub = n.advertise<dslam_sp::image_depth>("/merge/img_depth_file", 1); //创建publisher，往话题上发布消息
    ros::Publisher pub_info = n.advertise<sensor_msgs::CameraInfo>("/mynteye/left_rect/camera_info", 1); //创建publisher，往话题上发布消息

    Simulator sim;
    // Sets resolution. Default is 320X240
    sim.setCameraResolution(WIDTH,HEIGHT);
    sim.setDepthEnabled(false);

    // Initialize the simulator. Further camera configuration won't take any effect from now on.
    sim.initialize();

    sim.newRandomEpisode(std::vector<std::string>(1,"17DRP5sb8fy")); // Launches at a random location
    ros::Rate loop_rate(0.33);

    int i = 0;
    while(true) {
        // Get the state
        SimStatePtr state = sim.getState().at(0); // SimStatePtr is std::shared_ptr<SimState>

        // Which consists of:
        unsigned int n = state->step;
        cv::Mat rgb  = state->rgb; // OpenCV CV_8UC3 type (i.e. 8bit color rgb)
        cv::Mat depth  = state->depth; // OpenCV CV_16UC1 type (i.e. 16bit grayscale)
        //ViewpointPtr location = state->location; // Need a class to hold viewpoint id, and x,y,z location of a viewpoint
        float heading = state->heading;
        float elevation = state->elevation; // camera parameters
        std::vector<ViewpointPtr> reachable = state->navigableLocations; // Where we can move to,
        int locationIdx = 0; // Must be an index into reachable

        cv::Mat gray;
        cvtColor(rgb, gray, CV_RGB2GRAY);
        for(int i=0; i<reachable.size();i++) {
            ViewpointPtr location = reachable[i];
            double font_scale = 1.0/location->rel_distance;
            int id = location->ix;
            //std::cout<<"id:"<<id<<std::endl;
            int x = (int)(WIDTH/2 + location->rel_heading/HFOV*WIDTH);
            int y = (int)(HEIGHT/2 - location->rel_elevation/VFOV*HEIGHT);
            //int x = (int)WIDTH/2 + location->rel_heading;
            //int y = (int)HEIGHT/2 - location->rel_elevation;
            //std::cout<<location->rel_heading<<" "<<location->rel_elevation<<std::endl;
            if(id!=0) {
                cv::putText(rgb, std::to_string(i), cv::Point(x, y), cv::FONT_HERSHEY_SIMPLEX, font_scale, cv::Scalar(230, 40, 40), 3);
            }
        }

        double headingChange = 0;
        double elevationChange = 0;

        char k = cv::waitKey(1000);
        //std::cout<<k<<std::endl;
        if(k=='q')
            continue;
        else if (k >= '1' || k<='9') {
            locationIdx = k - '0';
            if (locationIdx >= reachable.size()) {
                locationIdx = 0;
            }
        }

        if (k == 81 || k == 'a') {
            headingChange = -ANGLEDELTA;
        }
        else if (k == 82 || k == 'w') {
            elevationChange = ANGLEDELTA;
        }
        else if (k == 83 || k == 'd') {
            headingChange = ANGLEDELTA;
        }
        else if (k == 84 || k == 's') {
            elevationChange = -ANGLEDELTA;
        }

        if(headingChange!=0 || elevationChange!=0) {
            sensor_msgs::ImagePtr ptr = cv_bridge::CvImage(std_msgs::Header(), "mono8", gray).toImageMsg();
            img_depth_msg.image = *ptr;
            ptr = cv_bridge::CvImage(std_msgs::Header(), "mono16", depth).toImageMsg();
            img_depth_msg.depth = *ptr;

            std::cout << "publish: " << state->location->viewpointId << std::endl;
            pub.publish(img_depth_msg);//以1Hz的频率发布msg
            pub_info.publish(camera_info);//以1Hz的频率发布msg
            loop_rate.sleep();
        }

        cv::imshow("C++ RGB", rgb);
        //cv::waitKey(10);

        sim.makeAction(std::vector<unsigned int>(1, locationIdx), 
                        std::vector<double>(1, headingChange), 
                        std::vector<double>(1, elevationChange));
        
        //std::cout<<headingChange<<" "<<elevationChange<<std::endl;
    }

    // It will be done automatically in destructor but after close you can init it again with different settings.
    sim.close();

    return 0;
}
