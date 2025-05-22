// std
#include <iostream>

// miru
#include <miru/config/instance.hpp>
#include <miru/params/type.hpp>
#include <miru/query/query.hpp>


// EXECUTE FROM THE ROOT OF THE REPOSITORY (./build/examples/get_value/get_value)

void print_param(
    const miru::params::Parameter& param,
    const std::string& title
) {
    std::cout << std::endl;
    std::cout << title << std::endl;
    std::cout << std::string(title.length(), '=') << std::endl;
    std::cout << "Name: " << param.get_name() << std::endl;
    std::cout << "Type: " << param.get_type() << std::endl;
}

void print_get_bool_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Boolean Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << std::boolalpha << "as_int(): " << param.as_bool() << std::endl;
    std::cout << std::boolalpha << "get_value<bool>(): " << param.get_value<bool>() << std::endl;
    std::cout << std::boolalpha << "as<bool>(): " << param.as<bool>() << std::endl;
    std::cout << std::boolalpha << "get_value<miru::params::PARAMETER_BOOLEAN>(): " << param.get_value<miru::params::PARAMETER_BOOL>() << std::endl;
}

void print_get_int_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Integer Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_int(): " << param.as_int() << std::endl;
    std::cout << "get_value<int>(): " << param.get_value<int>() << std::endl;
    std::cout << "get_value<int64_t>(): " << param.get_value<int64_t>() << std::endl;
    std::cout << "as<int>(): " << param.as<int>() << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_INTEGER>(): " << param.get_value<miru::params::PARAMETER_INTEGER>() << std::endl;
}

void print_get_double_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Double Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_double(): " << param.as_double() << std::endl;
    std::cout << "get_value<double>(): " << param.get_value<double>() << std::endl;
    std::cout << "get_value<float>(): " << param.get_value<float>() << std::endl;
    std::cout << "as<double>(): " << param.as<double>() << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_DOUBLE>(): " << param.get_value<miru::params::PARAMETER_DOUBLE>() << std::endl;
}

void print_get_string_value_variants(const miru::params::Parameter& param) {
    print_param(param, "String Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_string(): " << param.as_string() << std::endl;
    std::cout << "get_value<std::string>(): " << param.get_value<std::string>() << std::endl;
    std::cout << "as<std::string>(): " << param.as<std::string>() << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_STRING>(): " << param.get_value<miru::params::PARAMETER_STRING>() << std::endl;
}

void print_get_null_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Null Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_null(): " << param.as_null() << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_NULL>(): " << param.get_value<miru::params::PARAMETER_NULL>() << std::endl;
}

template <typename T>
std::string format_array_values(std::vector<T> array) {
    std::stringstream ss;
    for (auto i : array) {
        ss << std::boolalpha << i << " ";
    }
    return ss.str();
}

void print_get_boolean_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Boolean Array Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_bool_array(): " << format_array_values(param.as_bool_array()) << std::endl;
    std::cout << "get_value<std::vector<bool>>(): " << format_array_values(param.get_value<std::vector<bool>>()) << std::endl;
    std::cout << "as<std::vector<bool>>(): " << format_array_values(param.as<std::vector<bool>>()) << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_BOOL_ARRAY>(): " << format_array_values(param.get_value<miru::params::PARAMETER_BOOL_ARRAY>()) << std::endl;
}

void print_get_int_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Integer Array Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_int_array(): " << format_array_values(param.as_integer_array()) << std::endl;
    std::cout << "get_value<std::vector<long>>(): " << format_array_values(param.get_value<std::vector<long>>()) << std::endl;
    std::cout << "as<std::vector<int>>(): " << format_array_values(param.as<std::vector<int>>()) << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_INTEGER_ARRAY>(): " << format_array_values(param.get_value<miru::params::PARAMETER_INTEGER_ARRAY>()) << std::endl;

}

void print_get_double_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Double Array Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_double_array(): " << format_array_values(param.as_double_array()) << std::endl;
    std::cout << "get_value<std::vector<double>>(): " << format_array_values(param.get_value<std::vector<double>>()) << std::endl;
    std::cout << "as<std::vector<double>>(): " << format_array_values(param.as<std::vector<double>>()) << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_DOUBLE_ARRAY>(): " << format_array_values(param.get_value<miru::params::PARAMETER_DOUBLE_ARRAY>()) << std::endl;
}

void print_get_string_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "String Array Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    std::cout << "as_string_array(): " << format_array_values(param.as_string_array()) << std::endl;
    std::cout << "get_value<std::vector<std::string>>(): " << format_array_values(param.get_value<std::vector<std::string>>()) << std::endl;
    std::cout << "as<std::vector<std::string>>(): " << format_array_values(param.as<std::vector<std::string>>()) << std::endl;
    std::cout << "get_value<miru::params::PARAMETER_STRING_ARRAY>(): " << format_array_values(param.get_value<miru::params::PARAMETER_STRING_ARRAY>()) << std::endl;
}

void print_get_nested_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Nested Array Parameter");

    // retrieve the actual parameter value as a c++ type in different ways
    miru::params::NestedArray nested_array = param.as_nested_array();
    std::cout << "as_nested_array(): " << std::endl;
    int i = 0;
    for (auto& nested_param : nested_array) {
        std::cout << "Index " << i << ": " << format_array_values(nested_param.as_integer_array()) << std::endl;
        i++;
    }
}

void print_get_map_array_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Map Array Parameter");

    miru::params::MapArray map_array = param.as_map_array();
    std::cout << "as_map_array(): " << std::endl;
    int i = 0;
    for (auto& map_param : map_array) {
        std::cout << "Index " << i << ": " << map_param.value_to_string() << std::endl;
        i++;
    }
}

void print_get_map_value_variants(const miru::params::Parameter& param) {
    print_param(param, "Map Parameter");

    miru::params::Map map = param.as_map();
    std::cout << "as_map(): " << std::endl;
    for (auto& map_param : map) {
        std::cout << "  " << map_param.get_key() << ": " << map_param.value_to_string() << std::endl;
    }
}

int main() {
    // this is relative to the directory of resulting executable so we'll need to run
    // this from the root of the repository for these file paths to be properly defined.
    // In general we recommend using absolute paths.
    std::string config_schema_path = "./examples/config-schema.yaml";
    std::string config_instance_path = "./examples/config-instance.yaml";

    // retrieve a config instance from a file
    miru::config::ConfigInstance config_instance = miru::config::ConfigInstance::from_file(
        config_schema_path,
        config_instance_path
    );

    std::string config_type_slug = "example-config-type-slug";

    // boolean parameter
    miru::params::Parameter param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_boolean"
    );
    print_get_bool_value_variants(param);

    // integer parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "an_integer"
    );
    print_get_int_value_variants(param);

    // double parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_double"
    );
    print_get_double_value_variants(param);

    // string parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_string"
    );
    print_get_string_value_variants(param);

    // null parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_null_value"
    );
    print_get_null_value_variants(param);

    // boolean array parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_boolean_array"
    );
    print_get_boolean_array_value_variants(param);

    // integer array parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "an_integer_array"
    );
    print_get_int_array_value_variants(param);
    
    // double array parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_double_array"
    );
    print_get_double_array_value_variants(param);

    // string array parameter
    param = miru::query::get_param(
        config_instance     ,
        config_type_slug + "." + "a_string_array"
    );
    print_get_string_array_value_variants(param);

    // nested array parameter
    param = miru::query::get_param(
        config_instance,
        config_type_slug + "." + "a_nested_array"
    );
    print_get_nested_array_value_variants(param);

    // map array parameter
    miru::query::SearchParamFilters filters = miru::query::SearchParamFiltersBuilder()
        .with_param_name(config_type_slug + "." + "a_map_array")
        .with_leaves_only(false)
        .build();
    param = miru::query::get_param(
        config_instance,
        filters
    );
    print_get_map_array_value_variants(param);

    // map parameter
    filters = miru::query::SearchParamFiltersBuilder()
        .with_param_name(config_type_slug)
        .with_leaves_only(false)
        .build();
    param = miru::query::get_param(
        config_instance,
        filters
    );
    print_get_map_value_variants(param);
}

