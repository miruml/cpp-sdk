cmake_minimum_required(VERSION 3.16)

project(
        Miru_FromFileExample    
        VERSION 0.1
        DESCRIPTION "Miru From File Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(from-file main.cpp)
target_link_libraries(from-file PRIVATE miru)