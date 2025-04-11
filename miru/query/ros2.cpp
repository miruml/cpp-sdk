// internal
#include <miru/params/parameter.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>

namespace miru::query {

bool ROS2StyleQuery::has_parameter(const std::string& parameter_name) {
    SearchParamFiltersBuilder builder;
    builder.with_param_name(parameter_name);
    SearchParamFilters filters = builder.build();
    return miru::query::has_param(roots_, filters);
}

Parameter ROS2StyleQuery::get_parameter(const std::string& name) const {
    SearchParamFiltersBuilder builder;
    builder.with_param_name(name);
    SearchParamFilters filters = builder.build();
    return miru::query::get_param(
        roots_,
        filters
    );
}

bool ROS2StyleQuery::get_parameter(const std::string& name, Parameter& parameter) const {
    SearchParamFiltersBuilder builder;
    builder.with_param_name(name);
    SearchParamFilters filters = builder.build();
    return miru::query::try_get_param(
        roots_,
        filters,
        parameter
    );
}

std::vector<Parameter> ROS2StyleQuery::get_parameters(const std::vector<std::string>& names) const {
    SearchParamFiltersBuilder builder;
    builder.with_param_names(names);
    SearchParamFilters filters = builder.build();
    return miru::query::get_params(
        roots_,
        filters
    );
}

}  // namespace miru::query