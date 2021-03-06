cmake_minimum_required(VERSION 2.8.3)
project(door_passing)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  genmsg
  roscpp
  rospy
  std_msgs
  geometry_msgs
  nav_msgs
  gateway_detection
  robot_distance_monitor
  robot_heading_monitor
  gateway_msgs
  heading_control
  motion_control
  actionlib_msgs
  actionlib
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)

add_action_files(
  DIRECTORY action
  FILES
  DoorPassing.action
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  nav_msgs
  gateway_msgs
  heading_control
  actionlib_msgs
)

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES gateway_detection
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_library(door_passing src/door_passing.cpp)
target_link_libraries(door_passing ${catkin_LIBRARIES})

add_library(door_passing_ros src/door_passing_ros.cpp)
target_link_libraries(door_passing_ros door_passing ${catkin_LIBRARIES})
add_dependencies(door_passing_ros door_passing_generate_messages_cpp)

add_executable(door_passing_node src/door_passing_node.cpp)
target_link_libraries(door_passing_node door_passing_ros ${catkin_LIBRARIES})

include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(include ${EIGEN3_INCLUDE_DIRS})

