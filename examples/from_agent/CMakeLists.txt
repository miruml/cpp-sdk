cmake_minimum_required(VERSION 3.16)

project(
        Miru_FromAgentExample    
        VERSION 0.1
        DESCRIPTION "Miru From Agent Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(from-agent main.cpp)
target_link_libraries(from-agent PRIVATE miru)