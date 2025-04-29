# Use FetchContent for most dependencies
include(FetchContent)

# https://github.com/nlohmann/json?tab=readme-ov-file#embedded-fetchcontent
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# https://github.com/jbeder/yaml-cpp?tab=readme-ov-file#how-to-integrate-it-within-your-project-using-cmake
FetchContent_Declare(
yaml-cpp
GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

# fetch boost from system by default but allow FetchContent use if desire
option(MIRU_USE_SYSTEM_BOOST "Use system-installed Boost instead of FetchContent" ON)
if(MIRU_USE_SYSTEM_BOOST)
    find_package(Boost REQUIRED COMPONENTS asio beast)
else()
    FetchContent_Declare(
        boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
    )
    set(BOOST_INCLUDE_LIBRARIES asio beast)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_MakeAvailable(boost)
endif()