#ifndef DELIVERY_OPERATOR
#define DELIVERY_OPERATOR

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>



#include "rclcpp/rclcpp.hpp"
#include "route_service_msgs/srv/route.hpp"


class DeliveryOperator : public rclcpp::Node
{
    public:
        explicit DeliveryOperator(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~DeliveryOperator();

        void cmd_callback(route_service_msgs::msg::RouteCommand::SharedPtr msg);

    private:
        rclcpp::Subscription<route_service_msgs::msg::RouteCommand> subscription_;
        rclcpp::Client<route_service_msgs::srv::Route>::SharedPtr route_service_client_;
        route_service_msgs::msg::WaypointArray result_WP_array;
};


















#endif 