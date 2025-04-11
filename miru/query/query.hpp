#pragma once

// internal
#include "miru/params/composite.hpp"
#include "miru/query/exceptions.hpp"
#include <miru/query/filter.hpp>
#include <miru/params/parameter.hpp>

namespace miru::query {

using Parameter = miru::params::Parameter;
using ParameterType = miru::params::ParameterType;
using Map = miru::params::Map;
using NestedArray = miru::params::NestedArray;
using MapArray = miru::params::MapArray;
using ParametersView = miru::params::ParametersView;

using ParameterPtr = const Parameter*;
using ParameterPtrs = std::vector<ParameterPtr>;

// Define type trait for parameter containers
template<typename T>
struct is_parameter_container : std::disjunction<
  std::is_same<T, Parameter>,
  std::is_same<T, ParametersView>,
  std::is_same<T, std::vector<Parameter>>,
  std::is_same<T, Map>,
  std::is_same<T, NestedArray>,
  std::is_same<T, MapArray>
> {};

// Helper variable template for cleaner syntax
template<typename T>
inline constexpr bool is_parameter_container_v = is_parameter_container<T>::value;

// ================================ FIND PARAMETERS ================================ //
void find_all_recursive_helper(
  const Parameter& parameter,
  ParameterPtrs& result,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const Parameter& parameter,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const ParametersView& roots,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const std::vector<Parameter>& roots,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const Map& map,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const NestedArray& nested_array,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(
  const MapArray& map_array,
  const SearchParamFilters& filters
);

template<typename containerT>
typename std::enable_if<is_parameter_container<containerT>::value, ParameterPtr>::type
find_one(
  const containerT& container,
  const SearchParamFilters& filters,
  bool allow_throw = true
) {
  ParameterPtrs result = find_all(container, filters);
  if (result.size() > 1) {
    if (allow_throw) {
      throw TooManyResults("Multiple parameters found for filters: " + to_string(filters));
    } else {
      return nullptr;
    }
  }
  return result.empty() ? nullptr : result[0];
}

// =================================== CONTAINS ==================================== //
template<typename containerT>
typename std::enable_if<is_parameter_container<containerT>::value, bool>::type
has_param(
  const containerT& container,
  const SearchParamFilters& filters
) {
  return !find_all(container, filters).empty();
}

// ================================= GET PARAMS ==================================== //
template<typename containerT>
typename std::enable_if<is_parameter_container_v<containerT>, std::vector<Parameter>>::type
get_params(
  const containerT& container,
  const SearchParamFilters& filters
) {
  std::vector<Parameter> result;
  for (const auto& param : find_all(container, filters)) {
    result.push_back(*param);
  }
  return result;
};

// ================================== GET PARAM ==================================== //
template<typename containerT>
typename std::enable_if<is_parameter_container_v<containerT>, Parameter>::type
get_param(
  const containerT& container,
  const SearchParamFilters& filters
) {
  const Parameter* result = find_one(
    container,
    filters,
   true 
  );
  if (result == nullptr) {
    throw ParameterNotFound("Parameter not found");
  }
  return *result;
}

template<typename containerT>
typename std::enable_if<is_parameter_container_v<containerT>, bool>::type
try_get_param(
  const containerT& container,
  const SearchParamFilters& filters,
  Parameter& param_result
) {
  ParameterPtr ptr_result = find_one(
    container,
    filters,
    false
  );
  if (ptr_result) {
    param_result = *ptr_result;
  }
  return ptr_result != nullptr;
}

template<typename containerT, typename ValueT>
typename std::enable_if<is_parameter_container_v<containerT>, bool>::type
try_get_param(
  const containerT& container,
  const SearchParamFilters& filters,
  ValueT & value_result 
) {
  ParameterPtr ptr_result = find_one(
    container,
    filters,
    false
  );
  if (ptr_result) {
    value_result = ptr_result->as<ValueT>();
  }
  return ptr_result != nullptr;
}

template<typename containerT, typename ValueT>
typename std::enable_if<is_parameter_container_v<containerT>, ValueT>::type
get_param_or(
  const containerT& container,
  const SearchParamFilters& filters,
  const ValueT & default_value
) {
  ParameterPtr ptr_result = find_one(
    container,
    filters,
    false
  );
  if (ptr_result) {
    return ptr_result->as<ValueT>();
  } else {
    return default_value;
  }
}

template<typename containerT, typename ValueT>
typename std::enable_if<is_parameter_container_v<containerT>, bool>::type
try_get_param_or(
  const containerT& container,
  const SearchParamFilters& filters,
  ValueT & result,
  const ValueT & default_value
) {
  ParameterPtr ptr_result = find_one(
    container,
    filters,
    false
  );
  if (ptr_result) {
    result = ptr_result->as<ValueT>();
  } else {
    result = default_value;
  }
  return ptr_result != nullptr;
}

}  // namespace miru::params
