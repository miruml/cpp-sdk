// internal
#include <miru/params/parameter.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>

namespace miru::query {

bool ROS2StyleQuery::has_parameter(const std::string& parameter_name) {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(parameter_name).build();
    return miru::query::has_param(roots_, filters);
}

Parameter ROS2StyleQuery::get_parameter(const std::string& name) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::query::get_param(
        roots_,
        filters
    );
}

bool ROS2StyleQuery::get_parameter(const std::string& name, Parameter& parameter) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::query::try_get_param(
        roots_,
        filters,
        parameter
    );
}

std::vector<Parameter> ROS2StyleQuery::get_parameters(const std::vector<std::string>& names) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_names(names).build();
    return miru::query::get_params(
        roots_,
        filters
    );
}

}  // namespace miru::query