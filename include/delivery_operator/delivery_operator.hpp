#ifndef DELIVERY_OPERATOR
#define DELIVERY_OPERATOR

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>



#include "rclcpp/rclcpp.hpp"
#include "airdos_msgs/msg/route_command.hpp"
#include "airdos_msgs/msg/waypoint.hpp"
#include "airdos_msgs/msg/waypoint_array.hpp"
#include "airdos_msgs/msg/waypoint_injection.hpp"
#include "airdos_msgs/srv/route.hpp"
#include "airdos_msgs/srv/waypoint_injector.hpp"

#include "route_service/routeclient.hpp"
#include "std_msgs/msg/string.hpp"


class DeliveryOperator : public rclcpp::Node
{
    public:
        explicit DeliveryOperator(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~DeliveryOperator();

        void cmd_callback(airdos_msgs::msg::RouteCommand::SharedPtr msg);

    private:
        rclcpp::Subscription<airdos_msgs::msg::RouteCommand>::SharedPtr  subscription_;
        rclcpp::Publisher<airdos_msgs::msg::WaypointInjection>::SharedPtr wp_injection_pub_;
        rclcpp::Client<airdos_msgs::srv::Route>::SharedPtr route_service_client_;
        airdos_msgs::msg::WaypointArray result_WP_array_;
        airdos_msgs::msg::WaypointInjection wp_to_inject_;
};


















#endif 