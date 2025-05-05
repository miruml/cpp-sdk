// std
#include <iostream>

// miru
#include <miru/config/config.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>

// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/get_param/get_param)

int main() {
    // this is relative to the directory of resulting executable. so we'll need to run
    // this from the root of the repository for these file paths to be properly defined.
    // In general we recommend using absolute paths.
    std::string config_schema_path = "./examples/config-schema.yaml";
    std::string concrete_config_path = "./examples/concrete-config.yaml";

    // retrieve a concrete config from a file
    miru::config::Config config = miru::config::Config::from_file(
        config_schema_path,
        concrete_config_path
    );

    std::string config_slug = "example-config-slug";
    std::string param_name = config_slug + "." + "an_integer";
    miru::query::SearchParamFilters filters = miru::query::SearchParamFiltersBuilder()
        .with_param_name(param_name)
        .with_leaves_only(false)
        .build();


    // METHOD 1: get a parameter and throw an exception if it doesn't exist
    miru::params::Parameter param = miru::query::get_param(
        config,
        filters
    );
    std::cout << "Method 1 Result: " << param.as_int() << std::endl;


    // METHOD 2: pass in the return PARAMETER and return true if the parameter was able
    // to be stored in the result
    miru::params::Parameter result = miru::params::Parameter(param_name, 33);
    bool success = miru::query::try_get_param(
        config,
        filters,
        result
    );
    if (success) {
        std::cout << "Method 2 Result: " << result.as_int() << std::endl;
    } else {
        std::cout << "Method 2 Result (should be 33): " << result.as_int() << std::endl;
    }


    // METHOD 3: pass in the return VALUE and return true if the parameter was able to
    // be stored in the result
    int64_t result_value = 33;
    success = miru::query::try_get_param(
        config,
        filters,
        result_value
    );
    if (success) {
        std::cout << "Method 3 Result: " << result_value << std::endl;
    } else {
        std::cout << "Method 3 Result (should be 33): " << result_value << std::endl;
    }   


    // METHOD 4: provide a default value to return if the parameter cannot be retrieved
    int64_t default_value = 27;
    result_value = miru::query::get_param_or(
        config,
        filters,
        default_value
    );
    std::cout << "Method 4 Result: " << result_value << std::endl;


    // METHOD 5: pass in the return VALUE and a default. If the parameter was able to
    // be stored in the result, the result will be updated with the parameter value
    // and the function will return true. Otherwise, the result will be updated with
    // the default value and the function will return false.
    default_value = 46;
    success = miru::query::try_get_param_or(
        config,
        filters,
        result_value,
        default_value
    );
    if (success) {
        std::cout << "Method 5 Result: " << result_value << std::endl;
    } else {
        std::cout << "Method 5 Result (should be 46): " << result_value << std::endl;
    }
}

