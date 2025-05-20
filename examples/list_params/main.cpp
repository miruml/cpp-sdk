// std
#include <iostream>

// miru
#include <miru/config/config.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>

// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/list_params/list_params)

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
    std::string yaml_config_schema_path = "./examples/config-schema.yaml";
    std::string yaml_config_instance_path = "./examples/config-instance.yaml";
    std::string json_config_schema_path = "./examples/config-schema.json";
    std::string json_config_instance_path = "./examples/config-instance.json";

    // retrieve the configs from the file system
    miru::config::Config config_from_yaml = miru::config::Config::from_file(
        yaml_config_schema_path,
        yaml_config_instance_path
    );
    miru::config::Config config_from_json = miru::config::Config::from_file(
        json_config_schema_path,
        json_config_instance_path
    );

    // list all the parameters in the config
    // notice how yaml gives 'scalar' parameter types because of the yaml parser
    // but json gives 'integer', 'number', 'boolean', etc. because of the json parser
    print_params(
        miru::query::list_params(config_from_yaml),
        "YAML Config Instance File"
    );
    print_params(
        miru::query::list_params(config_from_json),
        "JSON Config Instance File"
    );
}

