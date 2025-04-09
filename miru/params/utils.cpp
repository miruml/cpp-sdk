// std
#include <string>
#include <vector>

// internal
#include <miru/params/parameter.hpp>
#include <miru/params/utils.hpp>
#include <miru/utils.hpp>

// external
#include <functional>
#include <string>
#include <vector>


namespace miru::params {


bool is_leaf(const ParameterValue & value) {
    if (value.is_nested_array()) {
        NestedArray nested_array = value.get<NestedArray>();
        for (const Parameter & param: nested_array.get_items()) {
            if (!is_leaf(param)) {
                return false;
            }
        }
        return true;
    }
    return !(value.is_object() || value.is_object_array());
}

bool is_leaf(const Parameter & parameter) {
    return is_leaf(parameter.get_parameter_value());
}

bool parameter_exists_recursive(
    const Parameter & parameter,
    const std::string & param_name,
    bool leaves_only
) {
    // is a leaf
    if (is_leaf(parameter) || !leaves_only) {
        return parameter.get_name() == param_name;
    }

    // only continue if the parameter's name is a prefix of the param_name
    if (!utils::has_prefix(param_name, parameter.get_name())) {
        return false;
    }

    // not a leaf -> collect the subparameters
    std::vector<Parameter> one_level_subparameters;
    switch (parameter.get_type()) {
        case ParameterType::PARAMETER_NESTED_ARRAY:
            one_level_subparameters = parameter.get_value<NestedArray>().get_items();
            break;
        case ParameterType::PARAMETER_OBJECT:
            one_level_subparameters = parameter.get_value<Object>().get_fields();
            break;
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            one_level_subparameters = parameter.get_value<ObjectArray>().get_items();
            break;
        default:
            throw std::invalid_argument("Cannot list subparameters for non-composite parameter");
    }

    // recursively collect all subparameters
    for (const Parameter & param: one_level_subparameters) {
        if (parameter_exists_recursive(param, param_name, leaves_only)) {
            return true;
        }
    }
    return false;
}


// 
std::vector<Parameter> get_parameters(
    const std::vector<Parameter> & parameters,
    const std::string & name
) {
    std::vector<Parameter> result;
    for (const Parameter & param: parameters) {
        if (param.get_name() == name) {
            result.push_back(param);
        }
    }
    return result;
}

std::vector<Parameter> get_parameter(
    const std::vector<Parameter> & parameters,
    const std::string & name
) {
    return get_parameters(parameters, name);
}

std::vector<std::reference_wrapper<const Parameter>> list_parameters_recursive(
    const Parameter & parameter,
    bool leaves_only
) {
    std::vector<std::reference_wrapper<const Parameter>> result;

    // is a leaf
    if (is_leaf(parameter) || !leaves_only) {
        result.push_back(std::cref(parameter));
    }

    // not a leaf -> collect the subparameters
    std::vector<Parameter> one_level_subparameters;
    switch (parameter.get_type()) {
        case ParameterType::PARAMETER_NESTED_ARRAY:
            one_level_subparameters = parameter.get_value<NestedArray>().get_items();
            break;
        case ParameterType::PARAMETER_OBJECT:
            one_level_subparameters = parameter.get_value<Object>().get_fields();
            break;
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            one_level_subparameters = parameter.get_value<ObjectArray>().get_items();
            break;
        default:
            throw std::invalid_argument("Cannot list subparameters for non-composite parameter");
    }

    // recursively collect all subparameters
    for (const Parameter & param: one_level_subparameters) {
        auto recursive_results = list_parameters_recursive(param, leaves_only);
        result.insert(
            result.end(),
            recursive_results.begin(),
            recursive_results.end()
        );
    }
    return result;
}

} // namespace miru::params

