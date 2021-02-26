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

RouteClient::RouteClient(const rclcpp::NodeOptions & node_options)
: Node("route_client",node_options)
{
    route_service_client_=this->create_client<RouteOperator>("route_operator");
    while(!route_service_client_->wait_for_service(1s)){
        if(!rclcpp::ok()){
            RCLCPP_ERROR(this->get_logger(),"Interrupted while waiting for the service");
            return;
        }
        RCLCPP_INFO(this->get_logger(),"Service not available... waiting...");
    }
}

RouteClient::~RouteClient()
{

}


void RouteClient::send_request(std::string _start, std::string _end){

    auto request = std::make_shared<RouteOperator::Request>();
    request->start=_start;
    request->end=_end;

    auto response_received_callback =[this](rclcpp::Client<RouteOperator>::SharedFuture future) {
        auto response=future.get();
        RCLCPP_INFO(this->get_logger(),"Receiced time: ",response->stamp);
        result_WP_array=response->route;
        for(auto & cur:result_WP_array.waypoints){
            std::cout<<cur.lattitude<<std::endl;
        }
        return;
    };

    auto future_result=
        route_service_client_->async_send_request(request, response_received_callback);

}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto route_client_ = std::make_shared<RouteClient>();
  
  std::string start;
  std::string end;

  std::cout<<"type start end"<<std::endl;
  std::cin>>start>>end;

  
  while (rclcpp::ok()) {
    rclcpp::spin_some(route_client_);
    route_client_->send_request(start,end);
    break;
  }

  return 0;
}


