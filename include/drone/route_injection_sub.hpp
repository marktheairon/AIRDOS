#ifndef ROUTE_INJECTION_SUB
#define ROUTE_INJECTION_SUB

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


class RouteInjectionSub : public rclcpp::Node
{
    public:
        explicit RouteInjectionSub(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~RouteInjectionSub();

        void msg_callback(route_service_msgs::msg::WaypointInjection::SharedPtr msg);

    private:
        rclcpp::Subscription<route_service_msgs::msg::WaypointInjection>::SharedPtr  subscription_;
        route_service_msgs::msg::WaypointArray waypoints_;
        int droneid_;

};

#endif