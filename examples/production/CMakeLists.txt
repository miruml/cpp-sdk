cmake_minimum_required(VERSION 3.16)

project(
        Miru_ProductionExample    
        VERSION 0.1
        DESCRIPTION "Miru Production Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(production_example main.cpp)
target_link_libraries(production_example PRIVATE miru)