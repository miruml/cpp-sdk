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

bool is_leaf(const ParameterValue& value) {
  if (value.is_nested_array()) {
    NestedArray nested_array = value.get<NestedArray>();
    for (const Parameter& param : nested_array) {
      if (!is_leaf(param)) {
        return false;
      }
    }
    return true;
  }
  return !(value.is_map() || value.is_map_array());
}

bool is_leaf(const Parameter& parameter) {
  return is_leaf(parameter.get_parameter_value());
}

bool parameter_exists_recursive(const Parameter& parameter,
                                const std::string& param_name, bool leaves_only) {
  // is a leaf
  if (is_leaf(parameter) || !leaves_only) {
    return parameter.get_name() == param_name;
  }

  // only continue if the parameter's name is a prefix of the param_name
  if (!utils::has_prefix(param_name, parameter.get_name())) {
    return false;
  }

  // not a leaf -> collect the subparameters
  std::vector<Parameter>::const_iterator iter_begin, iter_end;
  switch (parameter.get_type()) {
    case ParameterType::PARAMETER_NESTED_ARRAY:
      iter_begin = parameter.get_value<NestedArray>().begin();
      iter_end = parameter.get_value<NestedArray>().end();
      break;
    case ParameterType::PARAMETER_MAP:
      iter_begin = parameter.get_value<Map>().begin();
      iter_end = parameter.get_value<Map>().end();
      break;
    case ParameterType::PARAMETER_MAP_ARRAY:
      iter_begin = parameter.get_value<MapArray>().begin();
      iter_end = parameter.get_value<MapArray>().end();
      break;
    default:
      throw std::invalid_argument(
          "Cannot list subparameters for non-composite parameter");
  }

  // recursively collect all subparameters
  for (std::vector<Parameter>::const_iterator iter = iter_begin; iter != iter_end; ++iter) {
    if (parameter_exists_recursive(*iter, param_name, leaves_only)) {
      return true;
    }
  }
  return false;
}

//
std::vector<Parameter> get_parameters(const std::vector<Parameter>& parameters,
                                      const std::string& name) {
  std::vector<Parameter> result;
  for (const Parameter& param : parameters) {
    if (param.get_name() == name) {
      result.push_back(param);
    }
  }
  return result;
}

std::vector<Parameter> get_parameter(const std::vector<Parameter>& parameters,
                                     const std::string& name) {
  return get_parameters(parameters, name);
}

std::vector<std::reference_wrapper<const Parameter>> list_parameters_recursive(
    const Parameter& parameter, bool leaves_only) {
  std::vector<std::reference_wrapper<const Parameter>> result;

  // is a leaf
  if (is_leaf(parameter) || !leaves_only) {
    result.push_back(std::cref(parameter));
  }

  // not a leaf -> collect the subparameters
  std::vector<Parameter>::const_iterator iter_begin, iter_end;
  switch (parameter.get_type()) {
    case ParameterType::PARAMETER_NESTED_ARRAY:
      iter_begin = parameter.get_value<NestedArray>().begin();
      iter_end = parameter.get_value<NestedArray>().end();
      break;
    case ParameterType::PARAMETER_MAP:
      iter_begin = parameter.get_value<Map>().begin();
      iter_end = parameter.get_value<Map>().end();
      break;
    case ParameterType::PARAMETER_MAP_ARRAY:
      iter_begin = parameter.get_value<MapArray>().begin();
      iter_end = parameter.get_value<MapArray>().end();
      break;
    default:
      throw std::invalid_argument(
          "Cannot list subparameters for non-composite parameter");
  }

  // recursively collect all subparameters
  for (std::vector<Parameter>::const_iterator iter = iter_begin; iter != iter_end; ++iter) {
    auto recursive_results = list_parameters_recursive(*iter, leaves_only);
    result.insert(result.end(), recursive_results.begin(), recursive_results.end());
  }
  return result;
}

}  // namespace miru::params
