#! /bin/bash

source /opt/ros/foxy/install/setup.bash ;
source /home/jetson/px4_ros_com_ros2/install/setup.bash ;

ros2 run airdos waypoint_injector_client &

sleep 3

micrortps_agent -d /dev/ttyTHS0 -b 921600 &

sleep 3

ros2 run ros1_bridge daynamic_bridge &
