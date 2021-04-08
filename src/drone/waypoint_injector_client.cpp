
#include <memory>
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>


#include "rclcpp/rclcpp.hpp"
#include "drone/route_injection_sub.hpp"


#define DRONEID 1



RouteInjectionSub::RouteInjectionSub(const rclcpp::NodeOptions & node_options)
: Node("route_injection_sub",node_options)
{   
    subscription_ = this->create_subscription<route_service_msgs::msg::WaypointInjection>("wp_injection_pub",
                  10,std::bind(&RouteInjectionSub::msg_callback,this,std::placeholders::_1));
    waypoint_injector_client_ = this->create_client<route_service_msgs::srv::WaypointInjector>("waypoint_injector_server");

}

RouteInjectionSub::~RouteInjectionSub()
{

}

void RouteInjectionSub::msg_callback(route_service_msgs::msg::WaypointInjection::SharedPtr msg)
{
    auto request = std::make_shared<route_service_msgs::srv::WaypointInjector::Request>();
      request->injetionitem.waypoints=msg->waypoints;
      request->injetionitem.droneid=msg->droneid;
      
    auto response_received_callback =[this](rclcpp::Client<route_service_msgs::srv::WaypointInjector>::SharedFuture future) {
        auto response=future.get();
        
        std::cout<<"Waypoint injection Result : "<< response->sanity_check <<std::endl;
        if(!response->sanity_check)
        {
          std::cout<<"Waypoint injector service fail!"<<std::endl;
        }

    };

    if(DRONEID==msg->droneid)
    {
      auto future_result=waypoint_injector_client_->async_send_request(request, response_received_callback); 
    }
    else
    {
      std::cout<<"Drone ID not match, ignore"<<std::endl;
    }
  

    


}



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  auto node1 = std::make_shared<RouteInjectionSub>();
  exec.add_node(node1);
  
  exec.spin();
  rclcpp::shutdown();
  return 0;
}
