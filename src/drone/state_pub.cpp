
#include "drone/state_pub.hpp"

#define DRONEID 1


VehicleStatePublisher::VehicleStatePublisher(const rclcpp::NodeOptions & node_options)
: Node("VehicleStatus_listener",node_options)
{   
    vehicle_status_=this->create_subscription<px4_msgs::msg::VehicleStatus>("VehicleStatus_PubSubTopic",10,
    [this](const px4_msgs::msg::VehicleStatus::UniquePtr msg) {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "RECEIVED VEHICLE STATUS"    << std::endl;
        std::cout << "armstate " << (int)msg->arming_state  << std::endl;
        std::cout << "component id " << (int)msg->component_id  << std::endl;
        std::cout << "nav state " << (int)msg->nav_state  << std::endl;
        std::cout << "nav state " << (uint64_t)msg->timestamp  << std::endl;

        

        });

    vehicle_land_detected_=this->create_subscription<px4_msgs::msg::VehicleLandDetected>("VehicleLandDetected_PubSubTopic",10,
    [this](const px4_msgs::msg::VehicleLandDetected::UniquePtr msg) {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "RECEIVED VEHICLE STATUS"    << std::endl;
        std::cout << "ground contact " << msg->ground_contact  << std::endl;
        std::cout << "landed  " << msg->landed  << std::endl;
        std::cout << "maybe landed " << msg->maybe_landed  << std::endl;
        });
    
    vehicle_global_position_=this->create_subscription<px4_msgs::msg::VehicleGlobalPosition>("VehicleGlobalPosition_PubSubTopic",10,
    [this](const px4_msgs::msg::VehicleGlobalPosition::UniquePtr msg) {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "RECEIVED VEHICLE STATUS"    << std::endl;
        std::cout << "vehicle lattitude " << msg->lat  << std::endl;
        std::cout << "vehicle longitude  " << msg->lon  << std::endl;
        std::cout << "vehicle altitude " << msg->alt  << std::endl;
        });

        

}

VehicleStatePublisher::~VehicleStatePublisher()
{
    
}



int main(int argc, char *argv[])
{
	std::cout << "Starting vehicle_status listener node..." << std::endl;
	rclcpp::init(argc, argv);
	auto statesub=std::make_shared<VehicleStatePublisher>();
    rclcpp::spin(statesub);

	rclcpp::shutdown();
	return 0;
}