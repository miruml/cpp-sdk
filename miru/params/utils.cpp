// internal
#include <miru/params/parameter.hpp>
#include <miru/params/details/utils.hpp>

namespace miru::params {

bool is_leaf(const Parameter& parameter) {
  return is_leaf(parameter.get_parameter_value());
}

bool has_children(const Parameter& parameter) {
  return parameter.is_map() || parameter.is_map_array() || parameter.is_nested_array();
}

}  // namespace miru::params