#ifndef MISSION_PUB
#define MISSION_PUB

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>



#include "rclcpp/rclcpp.hpp"
#include "route_service_msgs/msg/route_command.hpp"



class MissionPublish : public rclcpp::Node
{
    public:
        explicit MissionPublish(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~MissionPublish();

        

    private:
        void publish_keyboard_input();
        void publish_app_api_input();

        rclcpp::Publisher<route_service_msgs::msg::RouteCommand>::SharedPtr  route_command_publisher_;
        /////add subscription for app api later
};


















#endif 