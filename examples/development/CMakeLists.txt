cmake_minimum_required(VERSION 3.16)

project(
        Miru_DevelopmentExample    
        VERSION 0.1
        DESCRIPTION "Miru Development Example"
        LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(development-example main.cpp)
target_link_libraries(development-example PRIVATE miru)