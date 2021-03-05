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



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto route_client_ = std::make_shared<RouteClient>();
  
  std::string start;
  std::string end;

  

  
  while (rclcpp::ok()) {
    std::cout<<"type start end"<<std::endl;
    std::cin>>start>>end;
    rclcpp::spin_some(route_client_);
    route_client_->send_request(start,end);
    
  }

  return 0;
}


