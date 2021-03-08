#include <vector>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>



#include "rclcpp/rclcpp.hpp"


#include "route_service/routeclient.hpp"
namespace route_service_client_composition
{

RouteClient::RouteClient(const rclcpp::NodeOptions & node_options)
: Node("route_client",node_options)
{   
    using namespace std::chrono_literals;
    route_service_client_=this->create_client<route_service_msgs::srv::Route>("route_operator");
    while(!route_service_client_->wait_for_service(1s)){
        if(!rclcpp::ok()){
            RCLCPP_ERROR(this->get_logger(),"Interrupted while waiting for the service");
            return;
        }
        RCLCPP_INFO(this->get_logger(),"Service not available... waiting...");
    }
/*    
      auto response_received_callback =[this](rclcpp::Client<route_service_msgs::srv::Route>::SharedFuture future) {
          auto response=future.get();
          RCLCPP_INFO(this->get_logger(),"Receiced time: ",response->stamp);
          result_WP_array=response->route;
          
          for(auto & cur:result_WP_array.waypoints){
              std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                      cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
          }
          //return result_WP_array;
      };

      std::string start,end;
      auto request = std::make_shared<route_service_msgs::srv::Route::Request>();

      std::cin>>start>>end;
      request->start = start;
      request->end =end;

      auto future_result=
          route_service_client_->async_send_request(request, response_received_callback);
     */ 
}

RouteClient::~RouteClient()
{

}


route_service_msgs::msg::WaypointArray RouteClient::send_request(std::string _start, std::string _end){

    auto request = std::make_shared<route_service_msgs::srv::Route::Request>();
    request->start=_start;
    request->end=_end;


    auto response_received_callback =[this](rclcpp::Client<route_service_msgs::srv::Route>::SharedFuture future) {
        auto response=future.get();
        RCLCPP_INFO(this->get_logger(),"Receiced time: ",response->stamp);
        result_WP_array=response->route;
        
       
    };
     
    auto future_result=
        route_service_client_->async_send_request(request, response_received_callback);
    
    return result_WP_array;

}

}

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(route_service_client_composition::RouteClient)