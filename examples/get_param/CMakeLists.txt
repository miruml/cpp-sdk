cmake_minimum_required(VERSION 3.16)

project(
        Miru_GetParamExample    
        VERSION 0.1
        DESCRIPTION "Miru Get Param Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(get-param main.cpp)
target_link_libraries(get-param PRIVATE miru)