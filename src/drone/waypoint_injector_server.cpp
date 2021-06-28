#include <memory>
#include <sstream>
#include <string>

#include "ros/ros.h"
#include "airdos_msgs/WaypointInjector.h"

#include <mavros_msgs/WaypointPush.h>
#include <mavros_msgs/HomePosition.h>
#include <mavros_msgs/WaypointClear.h>
#include <mavros_msgs/CommandHome.h>
#include <mavros_msgs/Waypoint.h>

#include "airdos_msgs/WaypointInjection.h"
#include "airdos_msgs/WaypointArray.h"


#define DRONEID 1




class WaypointInjector
{
    public:

    WaypointInjector()
    {
        this->waypoint_injector_server= nh_.advertiseService("waypoint_injector_server",&WaypointInjector::service_callback,this);
        this->wp_clear_client = nh_.serviceClient<mavros_msgs::WaypointClear>("/mavros/mission/clear"); 
        this->wp_srv_client = nh_.serviceClient<mavros_msgs::WaypointPush>("/mavros/mission/push");
        this->set_home_client = nh_.serviceClient<mavros_msgs::CommandHome>("/mavros/cmd/set_home");
        update_check=false;
        ROS_INFO("injector Initializer activated");
    }

    ~WaypointInjector()
    {

    }

    bool service_callback(airdos_msgs::WaypointInjector::Request &req, airdos_msgs::WaypointInjector::Response &res)
    {   
        if(req.injetionitem.droneid==DRONEID)
        {
            waypoint_data={};
            waypoint_data = req.injetionitem;
            update_check=true;
            std::cout<<"waypoint received"<<std::endl;
        }

        res.sanity_check=waypoint_injection();

        return true;
    }

    bool waypoint_injection()
    {
        if(update_check==true){
                    wp_clear_srv.request = {};
                    while(!(bool)wp_clear_srv.response.success)
                    {
                            wp_clear_client.call(wp_clear_srv);
                            ros::Duration(1).sleep();
                            if((bool)wp_clear_srv.response.success)
                            {
                                    std::cout<<"WP clear success"<<std::endl;
                            }
                    }

                    set_home_srv.request.current_gps = true;
                    if (set_home_client.call(set_home_srv))
                    {
                            ROS_INFO("Home was set to new value ");
                    }
                    else
                    {
                            ROS_ERROR("Home position couldn't been changed");
                            return false;
                    }

                    wp_push_srv.request={};
                    for(auto & cur:waypoint_data.waypoints)
                    {
                            
                            mavros_msgs::Waypoint tmp;
                            tmp.frame=cur.frame;
                            tmp.command=cur.command;
                            tmp.autocontinue=cur.autocontinue;
                            tmp.param1=cur.param1;
                            tmp.param2=cur.param2;
                            tmp.param3=cur.param3;
                            tmp.param4=cur.param4;
                            tmp.x_lat=cur.lattitude;
                            tmp.y_long=cur.longitude;
                            tmp.z_alt=cur.altitude;


                            wp_push_srv.request.waypoints.push_back(tmp);
                    
                    }

                    std::cout <<"stitched waypoint"<<std::endl;
                    for(auto & cur:wp_push_srv.request.waypoints)
                    {
                            std::cout<<"LLA :  "<<cur.x_lat<<"  "<<cur.y_long<<"  "<<cur.z_alt<<std::endl;
                            std::cout<<"command:  "<<cur.command<<std::endl;
                    }


                    if (wp_srv_client.call(wp_push_srv))
                    {
                            ROS_INFO("Success:%d", (bool)wp_push_srv.response.success);
                    }
                    else
                    {
                            ROS_ERROR("Waypoint couldn't been sent");
                            ROS_INFO("Success:%d", (bool)wp_push_srv.response.success);
                            return false;
                    }
                    update_check=false;
                    return true;
            }
    }

    private:
        ros::NodeHandle nh_;
        ros::ServiceServer waypoint_injector_server;
        ros::ServiceClient wp_clear_client;
        ros::ServiceClient wp_srv_client;
        ros::ServiceClient set_home_client;
        mavros_msgs::WaypointPush wp_push_srv;
        mavros_msgs::CommandHome set_home_srv;
        mavros_msgs::WaypointClear wp_clear_srv;
        airdos_msgs::WaypointInjection waypoint_data;
        bool update_check;



};


int main(int argc, char **argv)
{
    ros::init(argc,argv,"waypoint_injector_server");
    
    WaypointInjector injector;
    ros::Rate r(10);
    while(ros::ok())
    {
        ros::spinOnce();
        r.sleep();

    }

    
    return 0;

}
