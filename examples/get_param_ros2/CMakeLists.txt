cmake_minimum_required(VERSION 3.16)

project(
        Miru_GetParamRos2Example    
        VERSION 0.1
        DESCRIPTION "Miru Get Param ROS2 Style Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(get-param-ros2 main.cpp)
target_link_libraries(get-param-ros2 PRIVATE miru)