cmake_minimum_required(VERSION 3.16)

project(
        Miru_FiltersExample    
        VERSION 0.1
        DESCRIPTION "Miru Filters Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(filters main.cpp)
target_link_libraries(filters PRIVATE miru)