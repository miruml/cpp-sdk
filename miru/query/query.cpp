// std
#include <string>
#include <vector>
#include <string>
#include <vector>
#include <algorithm>

// internal
#include <miru/params/parameter.hpp>
#include <miru/params/value.hpp>
#include <miru/params/type.hpp>
#include <miru/params/utils.hpp>
#include <miru/query/query.hpp>
#include <miru/utils.hpp>

namespace miru::query {

// ================================ SEARCH FILTERS ================================ //
bool SearchParamFilters::matches(const Parameter& parameter) const {
  return (
    matches_name(parameter.get_name()) && 
    matches_prefix(parameter.get_name()) && 
    matches_leaves_only(parameter)
  );
}

bool SearchParamFilters::continue_search(const Parameter& parameter) const {
  return child_might_match_name(parameter) && child_might_match_prefix(parameter);
}

// matching operations
bool SearchParamFilters::matches_name(const std::string_view& param_name) const {
  if (!has_param_name_filter()) {
    return true;
  }
  return std::find(param_names.begin(), param_names.end(), param_name) != param_names.end();
}

bool SearchParamFilters::matches_prefix(const std::string_view& param_name) const {
  if (!has_prefix_filter()) {
    return true;
  }
  return utils::has_prefix(param_name, prefix);
}

bool SearchParamFilters::matches_leaves_only(const Parameter& parameter) const {
  return !leaves_only || miru::params::is_leaf(parameter);
}

// continue searching operations
bool SearchParamFilters::child_might_match_name(const Parameter& parameter) const {
  if (!has_param_name_filter()) {
    return true;
  }
  for (const auto& name : param_names) {
    if (utils::has_prefix(name, parameter.get_name())) {
      return true;
    }
  }
  return false;
}

bool SearchParamFilters::child_might_match_prefix(const Parameter& parameter) const {
  if (!has_prefix_filter()) {
    return true;
  }
  return utils::has_prefix(parameter.get_name(), prefix);
}

std::string to_string(const SearchParamFilters& filters) {
  std::stringstream ss;
  ss << "SearchParamFilters(";
  if (filters.has_param_name_filter()) {
    ss << "param_names: " << utils::to_string(filters.param_names) << ", ";
  }
  if (filters.has_prefix_filter()) {
    ss << "prefix: " << filters.prefix << ")";
  }
  return ss.str();
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_param_name(const std::string& param_name) {
  filters.param_names.push_back(param_name);
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_param_names(
  const std::vector<std::string>& param_names
) {
  filters.param_names.insert(
    filters.param_names.end(),
    param_names.begin(),
    param_names.end()
  );
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_prefix(const std::string& prefix) {
  filters.prefix = prefix;
  return *this;
}

// ================================ FIND PARAMETERS ================================ //
void find_parameters_recursive_helper(
  const Parameter& parameter,
  std::vector<const Parameter*>& result,
  const SearchParamFilters& filters
) {
  // check for a match
  if (filters.matches(parameter)) {
    result.push_back(&parameter);
  }

  // check if we should continue searching
  if (miru::params::is_leaf(parameter) || !filters.continue_search(parameter)) {
    return;
  }

  // find recursively
  for (const auto& item : get_children_view(parameter)) {
    find_parameters_recursive_helper(
      item, result, filters
    );
  }
}

std::vector<const Parameter*> find_parameters_recursive(
  const Parameter& root,
  const SearchParamFilters& filters
) {
  std::vector<const Parameter*> result;
  find_parameters_recursive_helper(root, result, filters);
  return result;
}

std::vector<const Parameter*> find_parameters_recursive(
  const std::vector<Parameter>& roots,
  const SearchParamFilters& filters
) {
  std::vector<const Parameter*> result;
  for (const auto& root : roots) {
    result.insert(result.end(), find_parameters_recursive(root, filters).begin(), find_parameters_recursive(root, filters).end());
  }
  return result;
}

std::vector<const Parameter*> find_parameters_recursive(
  const Map& map,
  const SearchParamFilters& filters
) {
  return find_parameters_recursive(std::vector<Parameter>(map.begin(), map.end()), filters);
}

std::vector<const Parameter*> find_parameters_recursive(
  const NestedArray& nested_array,
  const SearchParamFilters& filters
) {
  return find_parameters_recursive(std::vector<Parameter>(nested_array.begin(), nested_array.end()), filters);
}

std::vector<const Parameter*> find_parameters_recursive(
  const MapArray& map_array,
  const SearchParamFilters& filters
) {
  return find_parameters_recursive(std::vector<Parameter>(map_array.begin(), map_array.end()), filters);
}

// ================================ MIRU GETTERS =================================== //
std::vector<Parameter> get_parameters(
  const Parameter& root,
  const SearchParamFilters& filters
) {
  std::vector<Parameter> result;
  for (const auto& param : get_parameters(root, filters)) {
    result.push_back(param);
  }
  return result;
}

// ================================ ROS2 GETTERS =================================== //
Parameter get_parameter(
  const Parameter& root,
  const std::string& name
) {
  SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
  return get_parameter(root, filters);
}

bool get_parameter(
  const Parameter& root,
  const std::string& name,
  Parameter& result
) {
  SearchParamFilters filters = SearchParamFiltersBuilder().with_param_name(name).build();
  return get_parameter(root, filters, result);
}

std::vector<Parameter> get_parameters(
  const Parameter& root,
  const std::vector<std::string>& names
) {
  SearchParamFilters filters = SearchParamFiltersBuilder().with_param_names(names).build();
  return get_parameters(root, filters);
}

// =============================== LIST PARAMETERS ================================= //
std::vector<Parameter> list_parameters_recursive(
    const Parameter& parameter, bool leaves_only) {
  std::vector<Parameter> result;

  // is a leaf
  if (is_leaf(parameter) || !leaves_only) {
    result.push_back(std::cref(parameter));
  }

  // not a leaf -> collect the subparameters
  std::vector<std::pair<std::string, Parameter>>::const_iterator iter_begin, iter_end;
  switch (parameter.get_type()) {
    case ParameterType::PARAMETER_NESTED_ARRAY:
      for (const auto& item : parameter.get_value<NestedArray>()) {
        auto recursive_results = list_parameters_recursive(item, leaves_only);
        result.insert(result.end(), recursive_results.begin(), recursive_results.end());
      }
      break;
    case ParameterType::PARAMETER_MAP:
      for (const auto& param : parameter.get_value<Map>()) {
        auto recursive_results = list_parameters_recursive(param, leaves_only);
        result.insert(result.end(), recursive_results.begin(), recursive_results.end());
      }
      break;
    case ParameterType::PARAMETER_MAP_ARRAY:
      for (const auto& item : parameter.get_value<MapArray>()) {
        auto recursive_results = list_parameters_recursive(item, leaves_only);
        result.insert(result.end(), recursive_results.begin(), recursive_results.end());
      }
      break;
    default:
      throw std::invalid_argument(
          "Cannot list subparameters for non-composite parameter");
  }

  return result;
}

}  // namespace miru::params
