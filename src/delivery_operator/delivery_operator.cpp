#include <memory>

#include "route_service/routeclient.hpp"
#include "rclcpp/rclcpp.hpp"













//////////////////////////////////////////////Route su
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