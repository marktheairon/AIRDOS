#include <memory>
#include <chrono>
#include <cstdlib>

#include "delivery_operator/delivery_operator.hpp"
#include "route_service_msgs/msg/route_command.hpp"
#include "route_service/routeclient.hpp"
#include "std_msgs/msg/string.hpp"
#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;


DeliveryOperator::DeliveryOperator(const rclcpp::NodeOptions & node_options)
: Node("delivery_operator",node_options)
{   
    subscription_ = this->create_subscription<route_service_msgs::msg::RouteCommand>("route_command",
                  10,std::bind(&DeliveryOperator::cmd_callback,this,std::placeholders::_1));
    route_service_client_=this->create_client<route_service_msgs::srv::Route>("route_operator");
    while(!route_service_client_->wait_for_service(1s)){
        if(!rclcpp::ok()){
            RCLCPP_ERROR(this->get_logger(),"Interrupted while waiting for the service");
            return;
        }
        RCLCPP_INFO(this->get_logger(),"Service not available... waiting...");
    }
}

void DeliveryOperator::cmd_callback(route_service_msgs::msg::RouteCommand::SharedPtr msg)
{

  auto request = std::make_shared<route_service_msgs::srv::Route::Request>();
    request->start=msg->startpoint;
    request->end=msg->endpoint;


    auto response_received_callback =[this](rclcpp::Client<route_service_msgs::srv::Route>::SharedFuture future) {
        auto response=future.get();
        RCLCPP_INFO(this->get_logger(),"Receiced time: ",response->stamp);
        result_WP_array=response->route;
        for(auto & cur:result_WP_array.waypoints)
        {
          std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                      cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
        }
    };
     
    auto future_result=
        route_service_client_->async_send_request(request, response_received_callback);  

}

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<route_service_msgs::msg::RouteCommand>("route_command", 10);
      std::bind()
    }

  private:
    void timer_callback()
    {
      auto message = route_service_msgs::msg::RouteCommand();
      message.startpoint = "Hello, world! " + std::to_string(count_++);
      RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}



//////////////////////////////////////////////composition test
/*
int main(int argc, char * argv[])
{
  // Force flush of the stdout buffer.
  setvbuf(stdout, NULL, _IONBF, BUFSIZ);

  // Initialize any global resources needed by the middleware and the client library.
  // This will also parse command line arguments one day (as of Beta 1 they are not used).
  // You must call this before using any other part of the ROS system.
  // This should be called once per process.
  rclcpp::init(argc, argv);

  // Create an executor that will be responsible for execution of callbacks for a set of nodes.
  // With this version, all callbacks will be called from within this thread (the main one).
  rclcpp::executors::SingleThreadedExecutor exec;
  rclcpp::NodeOptions options;
  
  
  // Add some nodes to the executor which provide work for the executor during its "spin" function.
  // An example of available work is executing a subscription callback, or a timer callback.
  auto routeclient = std::make_shared<route_service_client_composition::RouteClient>(options);
  exec.add_node(routeclient);
  std::string start,end;
  std::cin>>start>>end;
  route_service_msgs::msg::WaypointArray route;
  route=routeclient->send_request(start,end);
  for(auto & cur:route.waypoints)
  {
    std::cout<<cur.command<<"    lla:  "<<cur.lattitude<<"   "<<cur.longitude<<"   "<<cur.altitude<<"   "<<"param 1 2 3 4: "<<
                      cur.param1<<"   "<<cur.param2<<"   "<<cur.param3<<"   "<<cur.param4<<std::endl;
  }
  exec.spin();

  
  // spin will block until work comes in, execute work as it becomes available, and keep blocking.
  // It will only be interrupted by Ctrl-C.

  rclcpp::shutdown();

  return 0;
}
*/

