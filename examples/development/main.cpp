// std
#include <iostream>

// miru
#include <miru/version.hpp>
#include <miru/commit.hpp>
#include <miru/config/config.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>

int main() {

    miru::config::Config config = miru::config::Config::from_file(
        "path/to/schema.yaml",
        "path/to/config.yaml"
    );

    auto params = miru::query::list_params(config);
    std::cout << params << std::endl;
}