#ifndef ROUTE_INJECTION_SUB
#define ROUTE_INJECTION_SUB

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
//#include "px4_msgs/msg/vehicle_command.hpp"



class RouteInjectionSub : public rclcpp::Node
{
    public:
        explicit RouteInjectionSub(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~RouteInjectionSub();

        void msg_callback(airdos_msgs::msg::WaypointInjection::SharedPtr msg);

    private:
        rclcpp::Subscription<airdos_msgs::msg::WaypointInjection>::SharedPtr  subscription_;
        //rclcpp::Publisher<px4_msgs::msg::VehicleCommand>::SharedPtr cmd_publisher_;
        rclcpp::Client<airdos_msgs::srv::WaypointInjector>::SharedPtr waypoint_injector_client_;
        //px4_msgs::msg::VehicleCommand vehiclecommand_;
        airdos_msgs::msg::WaypointArray waypoints_;
        int droneid_;

};

#endif