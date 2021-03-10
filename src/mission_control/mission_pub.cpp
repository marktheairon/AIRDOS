#include "mission_control/mission_pub.hpp"
#include "rclcpp/rclcpp.hpp"

#include <string>


MissionPublish::MissionPublish(const rclcpp::NodeOptions & node_options) 
: Node("mission_pub",node_options)
{
    route_command_publisher_ = this->create_publisher<route_service_msgs::msg::RouteCommand>("route_command",10);
    while (rclcpp::ok())
    {
        this->publish_keyboard_input();
    }
    
}


MissionPublish::~MissionPublish()
{
    
}


void MissionPublish::publish_keyboard_input()
{
    std::string start,end;
    int droneid;
    std::cout<<"Type  Start / End / TargetID"<<std::endl;
    std::cin>>start>>end>>droneid;

    auto message= route_service_msgs::msg::RouteCommand();
    message.startpoint=start;
    message.endpoint=end;
    message.droneid=droneid;

    RCLCPP_INFO(this->get_logger(), "Publishing: '%s    %s    %d'", message.startpoint,message.endpoint,message.droneid);
    route_command_publisher_->publish(message);

}


void MissionPublish::publish_app_api_input()
{

}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MissionPublish>());
  rclcpp::shutdown();
  return 0;
}