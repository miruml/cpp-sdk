#pragma once

// internal
#include <miru/config/config.hpp>
#include <miru/params/composite.hpp>
#include <miru/params/details/iterator.hpp>
#include <miru/params/parameter.hpp>
#include <miru/query/errors.hpp>
#include <miru/query/filter.hpp>

namespace miru::query::details {

using Parameter = miru::params::Parameter;
using ParameterType = miru::params::ParameterType;
using Map = miru::params::Map;
using NestedArray = miru::params::NestedArray;
using MapArray = miru::params::MapArray;
using ParametersView = miru::params::details::ParametersView;

using ParameterPtr = const Parameter*;
using ParameterPtrs = std::vector<ParameterPtr>;

// Define type trait for parameter roots
template <typename T>
struct is_parameter_root : std::disjunction<
                             std::is_same<T, Parameter>,
                             std::is_same<T, ParametersView>,
                             std::is_same<T, std::vector<Parameter>>,
                             std::is_same<T, Map>,
                             std::is_same<T, NestedArray>,
                             std::is_same<T, MapArray>,
                             std::is_same<T, miru::config::Config>> {};

// Helper variable template for cleaner syntax
template <typename T>
inline constexpr bool is_parameter_root_v = is_parameter_root<T>::value;

// ================================ FIND PARAMETERS ================================ //
void find_all_recursive_helper(
  const Parameter& parameter,
  ParameterPtrs& result,
  const SearchParamFilters& filters
);

ParameterPtrs find_all(const Parameter& parameter, const SearchParamFilters& filters);

ParameterPtrs find_all(const ParametersView& roots, const SearchParamFilters& filters);

ParameterPtrs
find_all(const std::vector<Parameter>& roots, const SearchParamFilters& filters);

ParameterPtrs find_all(const Map& map, const SearchParamFilters& filters);

ParameterPtrs
find_all(const NestedArray& nested_array, const SearchParamFilters& filters);

ParameterPtrs find_all(const MapArray& map_array, const SearchParamFilters& filters);

ParameterPtrs
find_all(const miru::config::Config& config, const SearchParamFilters& filters);

template <typename rootT>
typename std::enable_if<is_parameter_root<rootT>::value, ParameterPtr>::type find_one(
  const rootT& root,
  const SearchParamFilters& filters,
  bool allow_throw = true
) {
  ParameterPtrs result = find_all(root, filters);
  if (result.size() > 1) {
    if (allow_throw) {
      THROW_TOO_MANY_RESULTS(filters, "Multiple parameters found");
    } else {
      return nullptr;
    }
  }
  return result.empty() ? nullptr : result[0];
}

}  // namespace miru::query::details
