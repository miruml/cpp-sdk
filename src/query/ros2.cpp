// internal
#include <configs/instance_impl.hpp>
#include <miru/params/parameter.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>

namespace miru::query {

bool ROS2NodeI::has_parameter(const std::string& parameter_name) {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(parameter_name);
  SearchParamFilters filters = builder.build();
  return has_param(root_, filters);
}

Parameter ROS2NodeI::get_parameter(const std::string& name) const {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(name);
  SearchParamFilters filters = builder.build();
  return get_param(root_, filters);
}

bool ROS2NodeI::get_parameter(const std::string& name, Parameter& parameter) const {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(name);
  SearchParamFilters filters = builder.build();
  return try_get_param(root_, filters, parameter);
}

std::vector<Parameter> ROS2NodeI::get_parameters(const std::vector<std::string>& names
) const {
  return get_params(root_, names);
}

}  // namespace miru::query