#pragma once

// internal
#include <miru/config/config.hpp>
#include <miru/params/composite.hpp>
#include <miru/query/details/query.hpp>
#include <miru/params/details/iterator.hpp>
#include <miru/params/parameter.hpp>
#include <miru/query/errors.hpp>
#include <miru/query/filter.hpp>

namespace miru::query {

// =================================== EXISTS ==================================== //
template <typename rootT>
constexpr typename std::enable_if<details::is_parameter_root<rootT>::value, bool>::type
has_param(const rootT& root, const SearchParamFilters& filters) {
  return !details::find_all(root, filters).empty();
}

// ================================= GET PARAMS ==================================== //
template <typename rootT>
typename std::enable_if<details::is_parameter_root_v<rootT>, std::vector<Parameter>>::type
get_params(const rootT& root, const SearchParamFilters& filters) {
  std::vector<Parameter> result;
  for (const auto& param : details::find_all(root, filters)) {
    result.push_back(*param);
  }
  return result;
};

template <typename rootT>
typename std::enable_if<details::is_parameter_root_v<rootT>, std::vector<Parameter>>::type
get_params(const rootT& root, const std::vector<std::string>& param_names) {
  SearchParamFilters filters =
    SearchParamFiltersBuilder().with_param_names(param_names).build();
  std::vector<Parameter> result = get_params(root, filters);
  if (result.size() == param_names.size()) {
    return result;
  }

  // throw an error for errors specified in param names but not returned
  for (const auto& param_name : param_names) {
    if (std::find_if(result.begin(), result.end(), [&param_name](const auto& param) {
          return param.get_name() == param_name;
        }) == result.end()) {
      THROW_PARAMETER_NOT_FOUND(filters);
    }
  }
  return result;
};

template <typename rootT>
constexpr
  typename std::enable_if<details::is_parameter_root_v<rootT>, std::vector<Parameter>>::type
  list_params(const rootT& root) {
  return get_params(root, SearchParamFilters());
};

// ================================== GET PARAM ==================================== //
template <typename rootT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, Parameter>::type
get_param(const rootT& root, const SearchParamFilters& filters) {
  const Parameter* result = details::find_one(root, filters, true);
  if (result == nullptr) {
    THROW_PARAMETER_NOT_FOUND(filters);
  }
  return *result;
}

template <typename rootT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, Parameter>::type
get_param(const rootT& root, const std::string& param_name) {
  return get_param(
    root, SearchParamFiltersBuilder().with_param_name(param_name).build()
  );
}

template <typename rootT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, bool>::type
try_get_param(
  const rootT& root,
  const SearchParamFilters& filters,
  Parameter& param_result
) {
  details::ParameterPtr ptr_result = details::find_one(root, filters, false);
  if (ptr_result) {
    param_result = *ptr_result;
  }
  return ptr_result != nullptr;
}

template <typename rootT, typename ValueT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, bool>::type
try_get_param(
  const rootT& root,
  const SearchParamFilters& filters,
  ValueT& value_result
) {
  details::ParameterPtr ptr_result = details::find_one(root, filters, false);
  if (ptr_result) {
    value_result = ptr_result->as<ValueT>();
  }
  return ptr_result != nullptr;
}

template <typename rootT, typename ValueT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, ValueT>::type
get_param_or(
  const rootT& root,
  const SearchParamFilters& filters,
  const ValueT& default_value
) {
  details::ParameterPtr ptr_result = details::find_one(root, filters, false);
  if (ptr_result) {
    return ptr_result->as<ValueT>();
  } else {
    return default_value;
  }
}

template <typename rootT, typename ValueT>
constexpr typename std::enable_if<details::is_parameter_root_v<rootT>, bool>::type
try_get_param_or(
  const rootT& root,
  const SearchParamFilters& filters,
  ValueT& result,
  const ValueT& default_value
) {
  details::ParameterPtr ptr_result = details::find_one(root, filters, false);
  if (ptr_result) {
    result = ptr_result->as<ValueT>();
  } else {
    result = default_value;
  }
  return ptr_result != nullptr;
}

}  // namespace miru::query
