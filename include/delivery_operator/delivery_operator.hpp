#ifndef DELIVERY_OPERATOR
#define DELIVERY_OPERATOR

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>



#include "rclcpp/rclcpp.hpp"
#include "route_service_msgs/msg/route_command.hpp"
#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/msg/waypoint_injection.hpp"
#include "route_service_msgs/srv/route.hpp"
#include "route_service/routeclient.hpp"
#include "std_msgs/msg/string.hpp"


class DeliveryOperator : public rclcpp::Node
{
    public:
        explicit DeliveryOperator(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~DeliveryOperator();

        void cmd_callback(route_service_msgs::msg::RouteCommand::SharedPtr msg);

    private:
        rclcpp::Subscription<route_service_msgs::msg::RouteCommand>::SharedPtr  subscription_;
        rclcpp::Publisher<route_service_msgs::msg::WaypointInjection>::SharedPtr wp_injection_pub_;
        rclcpp::Client<route_service_msgs::srv::Route>::SharedPtr route_service_client_;
        route_service_msgs::msg::WaypointArray result_WP_array_;
        route_service_msgs::msg::WaypointInjection wp_to_inject_;
};


















#endif 