# Use FetchContent for most dependencies
include(FetchContent)

# nlohmann/json #
# ------------- #
# https://github.com/nlohmann/json?tab=readme-ov-file#embedded-fetchcontent
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# yaml-cpp #
# --------- #
# https://github.com/jbeder/yaml-cpp?tab=readme-ov-file#how-to-integrate-it-within-your-project-using-cmake
FetchContent_Declare(
yaml-cpp
GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

# Threads #
# ------- #
# this should already be installed on the system (no need to fetch or otherwise import)
find_package(Threads REQUIRED) 

# Boost #
# ------ #
# include boost through the desired method
if(MIRU_FETCH_BOOST)
    message(STATUS "Miru: fetching Boost with FetchContent")
    FetchContent_Declare(
        boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
    )
    set(BOOST_INCLUDE_LIBRARIES asio beast)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_MakeAvailable(boost)
else ()
    message(STATUS "Miru: using system-installed Boost")
    find_package(Boost REQUIRED COMPONENTS asio beast)
endif()