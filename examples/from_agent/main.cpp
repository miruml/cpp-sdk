// std
#include <iostream>

// miru
#include <miru/configs/instance.hpp>
#include <miru/query/query.hpp>


// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/from_agent/from_agent)

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
    // this is relative to the directory of resulting executable so we'll need to run
    // this from the root of the repository for these file paths to be properly defined.
    // In general we recommend using absolute paths.
    std::string schema_path = "./examples/config-schema.yaml";

    miru::config::ConfigInstance config_instance = miru::config::ConfigInstance::from_agent(
        schema_path
    );

    // list all the parameters in the config
    print_params(
        miru::query::list_params(config_instance),
        "Parameters from Agent"
    );
}