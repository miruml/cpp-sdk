#pragma once

// internal
#include <miru/params/parameter.hpp>

namespace miru::query {

using Parameter = miru::params::Parameter;
using ParameterType = miru::params::ParameterType;
using Map = miru::params::Map;
using NestedArray = miru::params::NestedArray;
using MapArray = miru::params::MapArray;

// ================================ SEARCH FILTERS ================================ //
class SearchParamFilters {
public:
  SearchParamFilters() : param_names(), prefixes(), leaves_only(true) {}

  std::vector<std::string> param_names;
  std::vector<std::string> prefixes;
  bool leaves_only;

  bool has_param_name_filter() const { return !param_names.empty(); }
  bool has_prefix_filter() const { return !prefixes.empty(); }

  bool matches(const Parameter& parameter) const;
  bool continue_search(const Parameter& parameter) const;

  // matching operations
  bool matches_param_name(const std::string_view& param_name) const;
  bool matches_prefix(const std::string_view& param_name) const;
  bool matches_leaves_only(const Parameter& parameter) const;

  // continue searching operations
  bool child_might_match_param_name(const std::string_view& param_name) const;
  bool child_might_match_prefix(const std::string_view& param_name) const;
};

std::string to_string(const SearchParamFilters& filters);

class SearchParamFiltersBuilder {
public:
  SearchParamFiltersBuilder() : filters() {}

  SearchParamFiltersBuilder& with_param_name(const std::string& param_name);
  SearchParamFiltersBuilder& with_param_names(
    const std::vector<std::string>& param_names
  );
  SearchParamFiltersBuilder& with_prefix(const std::string& prefix);
  SearchParamFiltersBuilder& with_prefixes(
    const std::vector<std::string>& prefixes
  );
  SearchParamFiltersBuilder& with_leaves_only(bool leaves_only);

  SearchParamFilters build() const { return filters; }

private:
  SearchParamFilters filters;
};

}  // namespace miru::query