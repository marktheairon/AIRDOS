cmake_minimum_required(VERSION 3.5)
project(airdos)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(rcutils REQUIRED)
find_package(Boost 1.62.0 REQUIRED COMPONENTS system filesystem)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(airdos_msgs REQUIRED)
find_package(px4_msgs REQUIRED)

# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)





include_directories(include)


set(node_plugins "")


add_executable(routeservice 
	src/route_service/server/routeservice.cpp 
	src/route_service/route.cpp 
	src/route_service/a_star_node.cpp)
ament_target_dependencies(routeservice
  airdos_msgs
  rclcpp
)

target_link_libraries(routeservice
   ${Boost_FILESYSTEM_LIBRARY}
   ${Boost_SYSTEM_LIBRARY}
   ${Boost_LIBRARIES}
)


add_library(routeclient SHARED
  src/route_service/client/routeclient.cpp)
target_compile_definitions(routeclient
  PRIVATE "COMPOSITION_BUILDING_DLL")
ament_target_dependencies(routeclient
  airdos_msgs
  class_loader
  rclcpp
)
rclcpp_components_register_nodes(routeclient "route_service_client_composition::RouteClient")
set(node_plugins "${node_plugins}route_service_client_composition::RouteClient;$<TARGET_FILE:routeclient>\n")



add_executable(delivery_operator
  src/delivery_operator/delivery_operator.cpp)
ament_target_dependencies(delivery_operator
   airdos_msgs
   rclcpp)


add_executable(mission_pub
  src/mission_control/mission_pub.cpp)
ament_target_dependencies(mission_pub
   airdos_msgs
   rclcpp)

add_executable(waypoint_injector_client
  src/drone/waypoint_injector_client.cpp)

ament_target_dependencies(waypoint_injector_client
   px4_msgs
   airdos_msgs
   rclcpp)
   
add_executable(state_pub
  src/drone/state_pub.cpp)
  
ament_target_dependencies(state_pub
   px4_msgs
   airdos_msgs
   rclcpp)


install(TARGETS
  
  routeclient
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)


install(TARGETS
  
  delivery_operator
  routeservice
  waypoint_injector_client
  mission_pub
  state_pub
  DESTINATION lib/${PROJECT_NAME})


# Install launch files
install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)






if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
ament_export_dependencies(class_loader)
ament_package()
