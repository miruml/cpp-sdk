// internal
#include <miru/params/parameter.hpp>
#include <miru/params/utils.hpp>
#include <miru/params/details/value.hpp>

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

ParametersView get_children_view(const Parameter& parameter) {
  switch (parameter.get_type()) {
    case ParameterType::PARAMETER_MAP:
      return ParametersView(parameter.as_map().begin(), parameter.as_map().end());
    case ParameterType::PARAMETER_MAP_ARRAY:
      return ParametersView(
        parameter.as_map_array().begin(), parameter.as_map_array().end()
      );
    case ParameterType::PARAMETER_NESTED_ARRAY:
      return ParametersView(
        parameter.as_nested_array().begin(), parameter.as_nested_array().end()
      );
    default:
      return ParametersView(
        std::vector<Parameter>::const_iterator(),
        std::vector<Parameter>::const_iterator()
      );
  }
}

}  // namespace miru::params