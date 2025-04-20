// internal
#include <miru/params/parameter.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>

namespace miru::query {

bool ROS2StyleQuery::has_parameter(const std::string& parameter_name) {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(parameter_name);
  SearchParamFilters filters = builder.build();
  return miru::query::has_param(root_, filters);
}

Parameter ROS2StyleQuery::get_parameter(const std::string& name) const {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(name);
  SearchParamFilters filters = builder.build();
  return miru::query::get_param(root_, filters);
}

bool ROS2StyleQuery::get_parameter(const std::string& name, Parameter& parameter)
  const {
  SearchParamFiltersBuilder builder;
  builder.with_param_name(name);
  SearchParamFilters filters = builder.build();
  return miru::query::try_get_param(root_, filters, parameter);
}

std::vector<Parameter> ROS2StyleQuery::get_parameters(
  const std::vector<std::string>& names
) const {
  SearchParamFiltersBuilder builder;
  builder.with_param_names(names);
  SearchParamFilters filters = builder.build();
  return miru::query::get_params(root_, filters);
}

}  // namespace miru::query