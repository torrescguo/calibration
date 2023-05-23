#!/bin/sh
# source ROS
chmod 777 setEnv.sh
source /opt/ros/melodic/setup.bash
export ROS_MASTER_URI=http://127.0.0.1:11311
export ROS_IP=127.0.0.1

# source kalibr and imu_utils

source ./kalibr_ws/devel/setup.bash
source ./imu_utils_ws/devel/setup.bash