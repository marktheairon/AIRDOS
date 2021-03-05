#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <boost/filesystem.hpp>
#include <cmath>
#include <limits.h>


#include "rapidjson/document.h"
#include "a_star_node.h"
#include "route.h"

#include "rclcpp/rclcpp.hpp"
#include "rcutils/cmdline_parser.h"

#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/srv/route.hpp"

#include "routeservice.hpp"



void print_help()
{
  printf("For ROS 2 topic subscriber, service server, action server rclcpp examples:\n");
  printf("calculator [-h]\n");
  printf("Options:\n");
  printf("\t-h Help           : Print this help function.\n");
}

int main(int argc, char * argv[])
{
  
  if (rcutils_cli_option_exist(argv, argv + argc, "-h")) {
    print_help();
    return 0;
  }

  rclcpp::init(argc, argv);

  auto routeservice = std::make_shared<RouteService>();

  rclcpp::spin(routeservice);

  rclcpp::shutdown();


  return 0;
}