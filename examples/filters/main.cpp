// std
#include <iostream>

// miru
#include <miru/config/config.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>

// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/filters/filters)

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
    std::string concrete_config_path = "./examples/concrete-config.yaml";

    // retrieve the configs from the file system
    miru::config::Config config = miru::config::Config::from_file(
        config_schema_path,
        concrete_config_path
    );

    // define the filter
    std::string config_slug = "example-config-slug";
    auto filter = miru::query::SearchParamFiltersBuilder()
        .with_param_names({
            config_slug + "." + "a_boolean",
            config_slug + "." + "a_double",
        })
        .build();
        
    // check if the parameter exists
    bool has_param = miru::query::has_param(config, filter);
    std::cout << std::boolalpha << "Has param? (should be true): " << has_param << "\n" << std::endl;

    // retrieve the parameters which match the filter
    auto params = miru::query::get_params(config, filter);
    print_params(params, "Parameters which match the filter");
}

