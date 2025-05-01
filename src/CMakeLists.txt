file(GLOB_RECURSE Miru_HEADERS CONFIGURE_DEPENDS ${Miru_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE Miru_SOURCES CONFIGURE_DEPENDS ${Miru_SOURCE_DIR}/src/*.cpp)

add_library(miru ${Miru_SOURCES} ${Miru_HEADERS})
add_library(miru::miru ALIAS miru)

target_include_directories(miru 
PUBLIC
    ${Miru_SOURCE_DIR}/include 
    ${Miru_BINARY_DIR}/include # needed for generated header files
PRIVATE
    ${Miru_SOURCE_DIR}/src
)
target_link_libraries(miru PRIVATE
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
    Boost::asio
    Boost::beast
)