// internal
#include <miru/params/parameter.hpp>
#include <miru/params/query.hpp>
#include <miru/params/ros2.hpp>

namespace miru::params {

bool ROS2StyleParameterQuery::has_parameter(const std::string& parameter_name) {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(parameter_name).build();
    return miru::params::contains_parameter(root_, filters);
}

Parameter ROS2StyleParameterQuery::get_parameter(const std::string& name) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::params::get_parameter(
        root_,
        filters
    );
}

bool ROS2StyleParameterQuery::get_parameter(const std::string& name, Parameter& parameter) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
    return miru::params::get_parameter(
        root_,
        filters,
        parameter
    );
}

std::vector<Parameter> ROS2StyleParameterQuery::get_parameters(const std::vector<std::string>& names) const {
    SearchParamFilters filters = SearchParamFiltersBuilder().with_param_names(names).build();
    return miru::params::get_parameters(root_, filters);
}

}  // namespace miru::params