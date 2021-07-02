#! /bin/bash

source /opt/ros/melodic/setup.bash
source /home/jetson/catkin_ws/install/setup.bash

roslaunch mavros px4.launch fcu_url:="/dev/ttyACM0" gcs_url:="udp://:14445@192.168.255.10:14550"
