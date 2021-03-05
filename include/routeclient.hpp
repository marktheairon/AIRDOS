#ifndef ROUTECLIENT_HPP
#define ROUTECLIENT_HPP

#include <chrono>
#include <memory>
#include <string>
#include <utility>
#include <random>

#include "rclcpp/rclcpp.hpp"
#include "route_service_msgs/srv/route.hpp"


class RouteClient : public rclcpp::Node
{
    public:
        using RouteOperator = route_service_msgs::srv::Route;

        explicit RouteClient(const rclcpp::NodeOptions & node_options =rclcpp::NodeOptions());
        virtual ~RouteClient();

        void send_request(std::string, std::string);

    private:
        rclcpp::Client<RouteOperator>::SharedPtr route_service_client_;
        route_service_msgs::msg::WaypointArray result_WP_array;
};



RouteClient::RouteClient(const rclcpp::NodeOptions & node_options)
: Node("route_client",node_options)
{   
    using namespace std::chrono_literals;
    route_service_client_=this->create_client<RouteOperator>("route_operator");
    while(!route_service_client_->wait_for_service(1s)){
        if(!rclcpp::ok()){
            RCLCPP_ERROR(this->get_logger("rclcpp"),"Interrupted while waiting for the service");
            return;
        }
        RCLCPP_INFO(this->get_logger("rclcpp"),"Service not available... waiting...");
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
            std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                    cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
        }
        return;
    };

    auto future_result=
        route_service_client_->async_send_request(request, response_received_callback);

}
















#endif