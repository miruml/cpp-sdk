cmake_minimum_required(VERSION 3.16)

project(
        Miru_GetValueExample    
        VERSION 0.1
        DESCRIPTION "Miru Get Value Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(get-value main.cpp)
target_link_libraries(get-value PRIVATE miru)