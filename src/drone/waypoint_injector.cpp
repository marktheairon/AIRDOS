/****************************************************************************
 *
 * Copyright 2018 PX4 Pro Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @brief Sensor Combined uORB topic listener example
 * @file sensor_combined_listener.cpp
 * @addtogroup ros1_examples
 * @author Nuno Marques <nuno.marques@dronesolutions.io>
 */

#include "ros/ros.h"

#include <mavros_msgs/WaypointPush.h>
#include <mavros_msgs/HomePosition.h>
#include <mavros_msgs/WaypointClear.h>
#include <mavros_msgs/CommandHome.h>
#include <mavros_msgs/Waypoint.h>

#include "route_service_msgs/WaypointInjection.h"
#include "route_service_msgs/WaypointArray.h"


#define DRONEID 1
static route_service_msgs::WaypointInjection waypoint_data;
bool   update_check=false;




void wp_callback(const route_service_msgs::WaypointInjection::ConstPtr& msg)
{
        
        if(msg->droneid==DRONEID)
        {
                /*for(auto & cur:msg->waypoints)
                {
                        std::cout<<"LLA :  "<<cur.lattitude<<"  "<<cur.longitude<<"  "<<cur.altitude<<std::endl;
                        std::cout<<"command:  "<<cur.command<<std::endl;
                }*/
                waypoint_data={};
                waypoint_data = *msg;
                update_check=true;
                std::cout<<"waypoint received"<<std::endl;
        }

	fflush(stdout);

        // republish data in another topic. Can be converted to a std ROS msg instead
}



int main(int argc, char **argv)
{
        // init the node
	ros::init(argc, argv, "wp_injection_pub_listener_node");

        // create the node handler
        ros::NodeHandle nh;
        
        // set the publisher
       // _sensor_combined_pub = nh.advertise<px4_msgs::SensorCombined>(kDefaultSensorCombinedPubTopic, 100);

        // set the subscriber
        ros::Subscriber wp_sub = nh.subscribe("wp_injection_pub", 100, wp_callback);
        ros::ServiceClient wp_clear_client = nh.serviceClient<mavros_msgs::WaypointClear>("/mavros/mission/clear"); 
        ros::ServiceClient wp_srv_client = nh.serviceClient<mavros_msgs::WaypointPush>("/mavros/mission/push");
        ros::ServiceClient set_home_client = nh.serviceClient<mavros_msgs::CommandHome>("/mavros/cmd/set_home");
        mavros_msgs::WaypointPush wp_push_srv;
        mavros_msgs::CommandHome set_home_srv;
        mavros_msgs::WaypointClear wp_clear_srv;
        
        
        ros::Rate rate(20.0);
        
        while(ros::ok())
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
                        }
                        update_check=false;

                }

                ros::spinOnce();
                rate.sleep();
        }
        


	return 0;
}
