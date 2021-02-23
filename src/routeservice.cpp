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
#include "node.h"
#include "route.h"

#include "rclcpp/rclcpp.hpp"
#include "route_service/msg/waypoint.hpp"
#include "route_service/msg/waypoint_array.hpp"
#include "route_service/srv/route.hpp"

#include "routeservice.hpp"


RouteService::RouteService(const rclcpp::NodeOptions & node_options)
: Node("route_service_server", node_options)
{
  RCLCPP_INFO(this->get_logger(), "Run route_service_server");

  this->declare_parameter("qos_depth", 10);
  int8_t qos_depth = 0;
  this->get_parameter("qos_depth", qos_depth);

  const auto QOS_RKL10V =
    rclcpp::QoS(rclcpp::KeepLast(qos_depth)).reliable().durability_volatile();

  route_request_subscriber_ = this->create_subscription<RouteRequest>(
    "route_request",
    QOS_RKL10V,
    [this](const RouteRequest::SharedPtr msg) -> void
    {
      start_= msg->start;
      end_= msg->end;

      RCLCPP_INFO(
        this->get_logger(),
        "Subscribed at: sec %ld nanosec %ld",
        msg->stamp.sec,
        msg->stamp.nanosec);

      RCLCPP_INFO(this->get_logger(), "Subscribed start : %s", start_);
      RCLCPP_INFO(this->get_logger(), "Subscribed end : %s", end_);
    }
  );

  auto route_calculation =
    [this](
    const std::shared_ptr<RouteRequest> request,
    std::shared_ptr<RouteResponse> response) -> void
    {
        
        this->calculate_route(request->start,request->end);
        
      
        RCLCPP_INFO(this->get_logger(), "%s", argument_formula_.c_str());
    };

  route_response_server_=
    create_service<RouteOperator>("route_operator", route_calculation);

}

RouteService::~RouteService()
{
}

int RouteService::calculate_route(const std::string &_start, const std::string &_end)
{
    
    A_star::parse_node_file("node.txt",parsedlist_);
  
    A_star::parse_connection_file(parsedlist_);
    
    A_star::parse_route_file(parsedlist_,routelist_);

    A_star::a_star_path(parsedlist_,resultpath_,_start,_end);
    
    resultroute_=A_star::waypoint_assembly(resultpath_,routelist_);

    

  return 1;
}


