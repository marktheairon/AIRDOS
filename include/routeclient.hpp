#ifndef ROUTECLIENT_HPP
#define ROUTECLIENT_HPP

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "route_service_msgs/srv/route.hpp"


class RouteClient : public rclcpp::Node
{
    public:
        using RouteOperator = route_service_msgs::srv::Route;

        explicit RouteClient(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~RouteClient();

        void send_request(std::string, std::string);

    private:
        rclcpp::Client<RouteOperator>::SharedPtr route_service_client_;
        route_service_msgs::msg::WaypointArray result_WP_array;
};

















#endif