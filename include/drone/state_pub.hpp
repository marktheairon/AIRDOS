#ifndef STATE_PUB_
#define STATE_PUB_


#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>

#include "px4_msgs/msg/vehicle_gps_position.hpp"
#include "px4_msgs/msg/vehicle_status.hpp"
#include "px4_msgs/msg/vehicle_land_detected.hpp"
#include "px4_msgs/msg/takeoff_status.hpp"
#include "px4_msgs/msg/vehicle_global_position.hpp"


#include "rclcpp/rclcpp.hpp"



class VehicleStatePublisher : public rclcpp::Node
{
        public:
                explicit VehicleStatePublisher(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
				virtual ~VehicleStatePublisher();
                
        
                

            private:
                rclcpp::Subscription<px4_msgs::msg::VehicleStatus>::SharedPtr vehicle_status_;
                rclcpp::Subscription<px4_msgs::msg::VehicleLandDetected>::SharedPtr vehicle_land_detected_;
                rclcpp::Subscription<px4_msgs::msg::VehicleGlobalPosition>::SharedPtr vehicle_global_position_;
                

};

















#endif