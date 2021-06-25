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
#include "px4_msgs/msg/vehicle_command.hpp"
#include "px4_msgs/msg/timesync.hpp"


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
        rclcpp::Publisher<px4_msgs::msg::VehicleCommand>::SharedPtr vehicle_command_publisher_;
        rclcpp::Client<airdos_msgs::srv::WaypointInjector>::SharedPtr waypoint_injector_client_;
        airdos_msgs::msg::WaypointArray waypoints_;
        rclcpp::Subscription<px4_msgs::msg::Timesync>::SharedPtr timesync_sub_;
        std::atomic<unsigned long long> timestamp_;
        int droneid_;
        void publish_vehicle_command(uint32_t command, float param1 = 0.0,
				     float param2 = 0.0) const;
        

};

#endif