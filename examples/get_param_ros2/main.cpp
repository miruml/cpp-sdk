// std
#include <iostream>

// miru
#include <miru/config/config.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>


// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/get_param_ros2/get_param_ros2)

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
    miru::query::ROS2 ros2_config = miru::query::ROS2(config);

    std::string config_slug = "example-config-slug";
    std::string param_name = config_slug + "." + "an_integer";

    // METHOD 1: get a parameter and throw an exception if it doesn't exist
    miru::params::Parameter param = ros2_config.get_parameter(param_name);
    std::cout << "Method 1 Result: " << param.as_int() << std::endl;


    // METHOD 2: pass in the return PARAMETER and return true if the parameter was able
    // to be stored in the result
    miru::params::Parameter result = miru::params::Parameter(param_name, 33);
    bool success = ros2_config.get_parameter(
        param_name,
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
    success = ros2_config.get_parameter(
        param_name,
        result_value
    );
    if (success) {
        std::cout << "Method 3 Result: " << result_value << std::endl;
    } else {
        std::cout << "Method 3 Result (should be 33): " << result_value << std::endl;
    }   


    // METHOD 4: provide a default value to return if the parameter cannot be retrieved
    int64_t default_value = 27;
    result_value = ros2_config.get_parameter_or(
        param_name,
        default_value
    );
    std::cout << "Method 4 Result: " << result_value << std::endl;


    // METHOD 5: pass in the return VALUE and a default. If the parameter was able to
    // be stored in the result, the result will be updated with the parameter value
    // and the function will return true. Otherwise, the result will be updated with
    // the default value and the function will return false.
    default_value = 46;
    success = ros2_config.get_parameter_or(
        param_name,
        result_value,
        default_value
    );
    if (success) {
        std::cout << "Method 5 Result: " << result_value << std::endl;
    } else {
        std::cout << "Method 5 Result (should be 46): " << result_value << std::endl;
    }
}

