cmake_minimum_required(VERSION 3.16)

project(
        Miru_ListParamsExample    
        VERSION 0.1
        DESCRIPTION "Miru List Params Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(list-params main.cpp)
target_link_libraries(list-params PRIVATE miru)