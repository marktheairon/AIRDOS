// Copyright 2014 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"


// Create a Listener class that subclasses the generic rclcpp::Node base class.
// The main function below will instantiate the class as a ROS node.
class Listentest : public rclcpp::Node
{
public:
  explicit Listentest(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions())
  : Node("listener",node_options)
  {
    // Create a callback function for when messages are received.
    // Variations of this function also exist using, for example UniquePtr for zero-copy transport.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
    auto callback =
      [this](const std_msgs::msg::String::SharedPtr msg) -> void
      {
        RCLCPP_INFO(this->get_logger(), "I heard: [%s]", msg->data.c_str());
      };
    // Create a subscription to the topic which can be matched with one or more compatible ROS
    // publishers.
    // Note that not all publishers on the same topic with the same type will be compatible:
    // they must have compatible Quality of Service policies.
    sub_ = create_subscription<std_msgs::msg::String>("chatter", 10, callback);
  }

  virtual ~Listentest()
  {   
  }
private:
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_;
};




int main(int argc, char *argv[])
{
	std::cout << "Starting vehicle_status listener node..." << std::endl;
	rclcpp::init(argc, argv);
    rclcpp::executors::SingleThreadedExecutor exec;
	auto listenertest=std::make_shared<Listentest>();
    exec.add_node(listenertest);
    exec.spin();
	rclcpp::shutdown();
	return 0;
}

  // namespace demo_nodes_cpp

