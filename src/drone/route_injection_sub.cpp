
#include <memory>
#include <chrono>
#include <cstdlib>

#include "drone/route_injection_sub.hpp"

#include "rclcpp/rclcpp.hpp"


#define DRONEID 1



RouteInjectionSub::RouteInjectionSub(const rclcpp::NodeOptions & node_options)
: Node("route_injection_sub",node_options)
{   
    subscription_ = this->create_subscription<route_service_msgs::msg::WaypointInjection>("wp_injection_pub",
                  10,std::bind(&RouteInjectionSub::msg_callback,this,std::placeholders::_1));


}

RouteInjectionSub::~RouteInjectionSub()
{

}

void RouteInjectionSub::msg_callback(route_service_msgs::msg::WaypointInjection::SharedPtr msg)
{
    waypoints_.waypoints=msg->waypoints;
    droneid_=msg->droneid;
    if(droneid_==DRONEID)
    {
        for(auto &cur:waypoints_.waypoints)
        {
            std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                      cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
        }
    }

}



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::executors::SingleThreadedExecutor exec;
  auto node1 = std::make_shared<RouteInjectionSub>();
  exec.add_node(node1);
  
  exec.spin();
  rclcpp::shutdown();
  return 0;
}