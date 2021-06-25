
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

#include <chrono>
#include <iostream>

using namespace std::chrono;
using namespace std::chrono_literals;




#define DRONEID 1



RouteInjectionSub::RouteInjectionSub(const rclcpp::NodeOptions & node_options)
: Node("route_injection_sub",node_options)
{   
    subscription_ = this->create_subscription<airdos_msgs::msg::WaypointInjection>("wp_injection_pub",
                  10,std::bind(&RouteInjectionSub::msg_callback,this,std::placeholders::_1));
    waypoint_injector_client_ = this->create_client<airdos_msgs::srv::WaypointInjector>("waypoint_injector_server");
    vehicle_command_publisher_ = this->create_publisher<px4_msgs::msg::VehicleCommand>("VehicleCommand_PubSubTopic",10);
    timesync_sub_ = this->create_subscription<px4_msgs::msg::Timesync>("Timesync_PubSubTopic",
            10,
            [this](const px4_msgs::msg::Timesync::UniquePtr msg) {
                timestamp_.store(msg->timestamp);
            });
}

RouteInjectionSub::~RouteInjectionSub()
{

}

void RouteInjectionSub::msg_callback(airdos_msgs::msg::WaypointInjection::SharedPtr msg)
{
    auto request = std::make_shared<airdos_msgs::srv::WaypointInjector::Request>();
      request->injetionitem.waypoints=msg->waypoints;
      request->injetionitem.droneid=msg->droneid;
      
    auto response_received_callback =[this](rclcpp::Client<airdos_msgs::srv::WaypointInjector>::SharedFuture future) {
        auto response=future.get();
        
        std::cout<<"Waypoint injection Result : "<< response->sanity_check <<std::endl;
        if(!response->sanity_check)
        {
          std::cout<<"Waypoint injector service fail!"<<std::endl;
        } 
        else                                                                           //injection success check, set mission mode and arm
        {
          std::cout<<"Waypoint injection success, starting mission"<<std::endl;
          
          this->publish_vehicle_command(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_COMPONENT_ARM_DISARM, 1, 0); 
          
          rclcpp::sleep_for(std::chrono::milliseconds(1000));
          this->publish_vehicle_command(px4_msgs::msg::VehicleCommand::VEHICLE_CMD_MISSION_START, 0, 0);
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

void RouteInjectionSub::publish_vehicle_command(uint32_t command, float param1,
					      float param2) const {
	px4_msgs::msg::VehicleCommand msg{};
	msg.timestamp = timestamp_.load();
	msg.param1 = param1;
	msg.param2 = param2;
	msg.param3 = 0.0;
	msg.param4 = NAN;
	msg.param5 = 0.0;
	msg.param6 = 0.0;
	msg.param7 = 0.0;
	msg.command = command;
	msg.target_system = 1;
	msg.target_component = 1;
	msg.source_system = 1;
	msg.source_component = 1;
	msg.from_external = false;
	

	vehicle_command_publisher_->publish(msg);
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
