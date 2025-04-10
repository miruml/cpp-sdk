#pragma once

// internal
#include <miru/params/parameter.hpp>

namespace miru::params {

// ============================== MIRU INTERFACES ================================= //

bool is_leaf(const ParameterValue& parameter);
bool is_leaf(const Parameter& parameter);

bool has_children(const Parameter& parameter);
ParametersView children(const Parameter& parameter);

// ================================ SEARCH FILTERS ================================ //
class SearchParamFilters {
public:
  SearchParamFilters() : param_names(), prefix() {}

  const std::vector<std::string_view>& get_param_names() const { return param_names; }
  const std::string_view& get_prefix() const { return prefix; }

  bool has_param_name_filter() const { return !param_names.empty(); }
  bool has_prefix_filter() const { return !prefix.empty(); }

  bool matches(const std::string_view& param_name) const;
  
  bool continue_search(const Parameter& parameter) const;

private:
  friend class SearchParamFiltersBuilder;

  std::vector<std::string_view> param_names;
  std::string_view prefix;

  // matching operations
  bool matches_name(const std::string_view& param_name) const;
  bool matches_prefix(const std::string_view& param_name) const;

  // continue searching operations
  bool child_might_match_name(const Parameter& parameter) const;
  bool child_might_match_prefix(const Parameter& parameter) const;
};

std::string to_string(const SearchParamFilters& filters);

class SearchParamFiltersBuilder {
public:
  SearchParamFiltersBuilder() : filters() {}

  SearchParamFiltersBuilder& with_param_name(const std::string_view& param_name);
  SearchParamFiltersBuilder& with_param_names(
    const std::vector<std::string_view>& param_names
  );
  SearchParamFiltersBuilder& with_param_names(
    const std::vector<std::string>& param_names
  );
  SearchParamFiltersBuilder& with_prefix(const std::string_view& prefix);
  SearchParamFilters build() const { return filters; }

private:
  SearchParamFilters filters;
};

// ================================ FIND PARAMETERS ================================ //
void find_parameters_recursive_helper(
  const Parameter& parameter,
  std::vector<const Parameter*>& result,
  const SearchParamFilters& filters
);

std::vector<const Parameter*> find_parameters_recursive(
  const Parameter& parameter,
  const SearchParamFilters& filters
);

std::vector<const Parameter*> find_parameters_recursive(
  const std::vector<Parameter>& roots,
  const SearchParamFilters& filters
);

std::vector<const Parameter*> find_parameters_recursive(
  const Map& map,
  const SearchParamFilters& filters
);

std::vector<const Parameter*> find_parameters_recursive(
  const NestedArray& nested_array,
  const SearchParamFilters& filters
);

std::vector<const Parameter*> find_parameters_recursive(
  const MapArray& map_array,
  const SearchParamFilters& filters
);

template<typename searchT>
typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  const Parameter*>::type
find_parameter_recursive(
  const searchT& search_parameter,
  const SearchParamFilters& filters
) {
  std::vector<const Parameter*> result;
  find_parameters_recursive_helper(search_parameter, result, filters);
  if (result.size() > 1) {
    throw std::invalid_argument("Multiple parameters found for filters: " + to_string(filters));
  }
  return result.empty() ? nullptr : result[0];
}

// ============================== CONTAINS PARAMETER =============================== //
template<typename searchT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  bool>::type
contains_parameter(
  const searchT& search_parameter,
  const SearchParamFilters& filters
) {
  return find_parameter_recursive(search_parameter, filters) != nullptr;
}

// ================================= MIRU GETTERS ================================== //
template<typename searchT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  Parameter>::type
get_parameter(
  const searchT& search_parameter,
  const SearchParamFilters& filters
) {
  const Parameter* result = find_parameter_recursive(
    search_parameter,
    filters
  );
  if (result == nullptr) {
    throw std::invalid_argument("Parameter not found");
  }
  return *result;
}

template<typename searchT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  bool>::type
get_parameter(
  const searchT& search_parameter,
  const SearchParamFilters& filters,
  Parameter& result
) {
  const Parameter* find_result = find_parameter_recursive(
    search_parameter,
    filters
  );
  if (find_result == nullptr) {
    return false;
  }
  result = *find_result;
  return true;
}

template<typename searchT, typename ValueT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  bool>::type
get_parameter(
  const searchT& search_parameter,
  const SearchParamFilters& filters,
  ValueT & value
) {
  const Parameter* result = find_parameter_recursive(
    search_parameter,
    filters
  );
  if (result == nullptr) {
    return false;
  }
  value = result->get_value<ValueT>();
  return true;
}

template<typename searchT, typename ValueT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  ValueT>::type
get_parameter_or(
  const searchT& search_parameter,
  const SearchParamFilters& filters,
  const ValueT & default_value
) {
  ValueT result;
  if (get_parameter(search_parameter, filters, result)) {
    return result;
  }
  return default_value;
}

template<typename searchT, typename ValueT>
constexpr typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  bool>::type
get_parameter_or(
  const searchT& search_parameter,
  const SearchParamFilters& filters,
  ValueT & value,
  const ValueT & default_value
) {
  if (get_parameter(search_parameter, filters, value)) {
    return true;
  }
  value = default_value;
  return false;
}

template<typename searchT>
typename std::enable_if<
  std::is_same<searchT, Parameter>::value ||
  std::is_same<searchT, Map>::value ||
  std::is_same<searchT, NestedArray>::value ||
  std::is_same<searchT, MapArray>::value,
  std::vector<Parameter>>::type
get_parameters(
  const searchT& search_parameter,
  const SearchParamFilters& filters
) {
  std::vector<Parameter> result;
  for (const auto& param : get_parameters(search_parameter, filters)) {
    result.push_back(param);
  }
  return result;
};

template<typename ParameterT>
bool get_parameters(
  const Parameter& root,
  const SearchParamFilters& filters,
  std::map<std::string, ParameterT>& values
) {
  for (const auto& param : get_parameters(root, filters)) {
    values[param.get_name()] = param.get_value<ParameterT>();
  }
  return true;
}

}  // namespace miru::params
