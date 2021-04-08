
#include "drone/state_pub.hpp"


VehicleStatePublisher::VehicleStatePublisher(const rclcpp::NodeOptions & node_options)
: Node("VehicleStatus_listener",node_options)
{
    vehicle_status_=this->create_subscription<px4_msgs::msg::VehicleStatus>("VehicleStatus_PubSubTopic",10,
    [this](const px4_msgs::msg::VehicleStatus::UniquePtr msg) {
        std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        std::cout << "RECEIVED VEHICLE STATUS"    << std::endl;
        std::cout << "armstate " << msg->arming_state  << std::endl;
        std::cout << "component id " << msg->component_id  << std::endl;
        std::cout << "nav state " << msg->nav_state  << std::endl;
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