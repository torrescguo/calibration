# Orbslam calibration

# Install required libs

Ubuntu 18.04

OpenCV 3.2

Eigen3

ROS1

Ceres --Cmake >> Cmake 3.11


# Build Steps

1.build imu_utils and code_utils

cd imu_utils_ws

catkin_make

source ./devel/setup.bash

# if build fail,try to mv imu_utils to a path,then catkin_make,if success,mv imu_utils to src/ ,continuing to catkin_make 

2.build kalibr

cd kalibr_ws

source /opt/ros/melodic/setup.bash

catkin init

catkin config --extend /opt/ros/melodic

catkin config --merge-devel # Necessary for catkin_tools >= 0.4.

catkin config --cmake-args -DCMAKE_BUILD_TYPE=Release

catkin build -DCMAKE_BUILD_TYPE=Release -j4

source ./devel/setup.bash


# Use Steps

source setEnv.sh








