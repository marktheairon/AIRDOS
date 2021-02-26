#ifndef ROUTE_SERVICE_HPP_
#define ROUTE_SERVICE_HPP_

#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include "rclcpp/rclcpp.hpp"

#include "route_service_msgs/msg/waypoint.hpp"
#include "route_service_msgs/msg/waypoint_array.hpp"
#include "route_service_msgs/srv/route.hpp"

#include "a_star_node.h"
#include "route.h"

class RouteService : public rclcpp::Node
{
public:
  using RouteRequest = route_service_msgs::srv::Route::Request;
  using RouteResponse = route_service_msgs::srv::Route::Response;
  using RouteOperator = route_service_msgs::srv::Route;
  using WaypointArray= route_service_msgs::msg::WaypointArray;

  explicit RouteService(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions());
  virtual ~RouteService();

private:
  WaypointArray calculate_route(const std::string &, const std::string & );


  rclcpp::Subscription<RouteRequest>::SharedPtr
    route_request_subscriber_;

  rclcpp::Service<RouteOperator>::SharedPtr
    route_response_server_;

    std::string start_;
    std::string end_;

    std::vector<A_star::Node> parsedlist_;                                                                    //main data pool, do not delete or free                  
    std::vector<A_star::Node> resultpath_;
    std::vector<Route> routelist_;
    WaypointArray resultroute_;

};
#endif  
