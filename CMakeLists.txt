cmake_minimum_required(VERSION 3.5)
project(airdos)


## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
# find dependencies

set(PYTHON_EXCUTABLE /usr/bin/python3)

find_package(catkin REQUIRED COMPONENTS 
      roscpp
      mavros
      mavros_msgs
      std_msgs
      std_srvs
      message_generation
      )

find_package(px4_msgs REQUIRED)

find_package(airdos_msgs REQUIRED)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS system)


catkin_package(
    INCLUDE_DIRS include
    LIBRARIES px4_ros_com
    CATKIN_DEPENDS px4_msgs airdos_msgs
    # DEPENDS ...
)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html



###################################
## catkin specific configuration ##
###################################

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES px4_ros_com
    CATKIN_DEPENDS px4_msgs airdos_msgs mavros_msgs
    # DEPENDS ...
)

###########
## Build ##
###########

## Add include dirs
include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${px4_msgs_INCLUDE_DIRS}
    ${airdos_msgs_INCLUDE_DIRS}
#    ${mavros_INCLUDE_DIRS}
#    ${mavros_msgs_INCLUDE_DIRS}
)

## Add library
add_library(${PROJECT_NAME}
#    src/drone/waypoint_injector.cpp
    src/drone/waypoint_injector_server.cpp
#   src/advertisers/debug_vect_advertiser.cpp
)
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})

## Build listener
#add_executable(
#    waypoint_injector
#    src/drone/waypoint_injector.cpp
#)
add_executable(
    waypoint_injector_server
    src/drone/waypoint_injector_server.cpp
)
add_dependencies(waypoint_injector_server ${catkin_EXPORTED_TARGETS})
target_link_libraries(waypoint_injector_server ${catkin_LIBRARIES})

## Build advertiser
# add_executable(
#     debug_vect_advertiser
#     src/advertisers/debug_vect_advertiser.cpp
# )
# add_dependencies(debug_vect_advertiser ${catkin_EXPORTED_TARGETS})
# target_link_libraries(debug_vect_advertiser ${catkin_LIBRARIES})

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(TARGETS ${PROJECT_NAME} waypoint_injector_server # debug_vect_advertiser
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

## Mark other files for installation (e.g. launch and bag files, etc.)
install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

## Install package.xml
install(
  FILES ${PROJECT_SOURCE_DIR}/package.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${PROJECT_NAME}
)

#############
## Testing ##
#############

if(CATKIN_ENABLE_TESTING)

#TODO

endif()

