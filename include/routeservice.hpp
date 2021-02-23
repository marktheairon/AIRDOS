#ifndef ROUTE_SERVICE_HPP_
#define ROUTE_SERVICE_HPP_

#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>

#include "rclcpp/rclcpp.hpp"

#include "route_service/msg/waypoint.hpp"
#include "route_service/msg/waypoint_array.hpp"
#include "route_service/srv/route.hpp"

#include "node.h"
#include "route.h"

class RouteService : public rclcpp::Node
{
public:
  using RouteRequest = route_service::srv::Route::Request;
  using RouteResponse = route_service::srv::Route::Response;
  using RouteOperator = route_service::srv::Route;
  using Waypoint= route_service::msg::WaypointArray;
  using WaypointArray= route_service::msg::WaypointArray;

  explicit RouteService(const rclcpp::NodeOptions & node_options = rclcpp::NodeOptions());
  virtual ~RouteService();

private:
  int calculate_route(const std::string &, const std::string & );


  rclcpp::Subscription<RouteRequest>::SharedPtr
    route_request_subscriber_;

  rclcpp::Service<RouteOperator>::SharedPtr
    route_response_server_;

    std::string start_;
    std::string end_;

    std::vector<A_star::Node> parsedlist;                                                                    //main data pool, do not delete or free                  
    std::vector<A_star::Node> resultpath;
    std::vector<Route> routelist;
    std::vector<Route> resultroute;

};
#endif  
