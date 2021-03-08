#include <vector>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include "routeclient.hpp"


#include "rclcpp/rclcpp.hpp"



#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/srv/route.hpp"


using namespace std::chrono_literals;


/*
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto route_client_ = std::make_shared<RouteClient>();
  
  std::string start;
  std::string end;

  

  
  while (rclcpp::ok()) {
    std::cout<<"type start end"<<std::endl;
    std::cin>>start>>end;
    route_client_->send_request(start,end);
    rclcpp::spin_until_future_complete(route_client_);
 
    
  }

  return 0;
}
*/


int main(int argc, char **argv)
{
  std::string start;
  std::string end;
  route_service_msgs::msg::WaypointArray result_WP_array;

  rclcpp::init(argc, argv);
  
  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("route_request");
  rclcpp::Client<route_service_msgs::srv::Route>::SharedPtr client =
    node->create_client<route_service_msgs::srv::Route>("route_operator");
  
  while (!client->wait_for_service(1s)) {
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
        return 0;
      }
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

  while(rclcpp::ok())
  {
    auto request = std::make_shared<route_service_msgs::srv::Route::Request>();

    std::cin>>start>>end;
    request->start = start;
    request->end =end;

    
    auto result = client->async_send_request(request);
    // Wait for the result.
    rclcpp::spin_until_future_complete(node, result); 
    result_WP_array=result.get()->route;
    for(auto & cur:result_WP_array.waypoints){
              std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                      cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
    }

  }
  //rclcpp::shutdown();
  return 0;
}
