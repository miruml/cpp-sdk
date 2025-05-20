// std
#include <iostream>

// miru
#include <miru/config/config.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>

// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/from_file/from_file)

void print_params(const std::vector<miru::params::Parameter>& params, const std::string& title) {
    std::cout << title << std::endl;
    std::cout << std::string(title.length(), '=') << std::endl;
    for (auto param : params) {
        std::cout << "Name: " << param.get_name() << std::endl;
        std::cout << "Key: " << param.get_key() << std::endl;
        std::cout << "Type: " << param.get_type() << std::endl;
        std::cout << "Value: " << param.get_parameter_value() << "\n" << std::endl;
    }
}

int main() {
    // this is relative to the directory of resulting executable. so we'll need to run
    // this from the root of the repository for these file paths to be properly defined.
    // In general we recommend using absolute paths.
    std::string config_schema_path = "./examples/config-schema.yaml";
    std::string config_instance_path = "./examples/config-instance.yaml";

    // retrieve a config instance from a file
    miru::config::Config config = miru::config::Config::from_file(
        config_schema_path,
        config_instance_path
    );

    print_params(
        miru::query::list_params(config),
        "Printing Parameters From File"
    );
}