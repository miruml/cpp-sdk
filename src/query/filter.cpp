// internal
#include <miru/params/parameter.hpp>
#include <miru/query/filter.hpp>
#include <params/utils.hpp>

namespace miru::query {

// ================================ SEARCH FILTERS ================================ //
bool SearchParamFilters::matches(const Parameter& parameter) const {
  return (
    matches_param_name(parameter.get_name()) && matches_prefix(parameter.get_name()) &&
    matches_leaves_only(parameter)
  );
}

bool SearchParamFilters::continue_search(const Parameter& parameter) const {
  return (
    !miru::params::is_leaf(parameter) &&
    child_might_match_param_name(parameter.get_name()) &&
    child_might_match_prefix(parameter.get_name())
  );
}

// matching operations
bool SearchParamFilters::matches_param_name(const std::string_view& param_name) const {
  if (!has_param_name_filter()) {
    return true;
  }
  return std::find(param_names.begin(), param_names.end(), param_name) !=
         param_names.end();
}

bool SearchParamFilters::matches_prefix(const std::string_view& param_name) const {
  if (!has_prefix_filter()) {
    return true;
  }
  for (const auto& prefix : prefixes) {
    if (miru::utils::has_prefix(param_name, prefix)) {
      return true;
    }
  }
  return false;
}

bool SearchParamFilters::matches_leaves_only(const Parameter& parameter) const {
  return !leaves_only || miru::params::is_leaf(parameter);
}

// continue searching operations
bool SearchParamFilters::child_might_match_param_name(const std::string_view& param_name
) const {
  if (!has_param_name_filter()) {
    return true;
  }
  for (const auto& name : param_names) {
    if (miru::utils::has_prefix(name, param_name)) {
      return true;
    }
  }
  return false;
}

bool SearchParamFilters::child_might_match_prefix(const std::string_view& param_name
) const {
  if (!has_prefix_filter()) {
    return true;
  }
  for (const auto& prefix : prefixes) {
    if (miru::utils::has_prefix(param_name, prefix)) {
      return true;
    }
    if (miru::utils::has_prefix(prefix, param_name)) {
      return true;
    }
  }
  return false;
}

std::string to_string(const SearchParamFilters& filters) {
  std::stringstream ss;
  ss << "SearchParamFilters(";
  if (filters.has_param_name_filter()) {
    ss << "param_names: " << miru::utils::to_string(filters.param_names) << ", ";
  }
  if (filters.has_prefix_filter()) {
    ss << "prefixes: " << miru::utils::to_string(filters.prefixes) << ", ";
  }
  return ss.str();
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_param_name(
  const std::string& param_name
) {
  filters.param_names.push_back(param_name);
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_param_names(
  const std::vector<std::string>& param_names
) {
  filters.param_names.insert(
    filters.param_names.end(), param_names.begin(), param_names.end()
  );
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_prefix(
  const std::string& prefix
) {
  filters.prefixes.push_back(prefix);
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_prefixes(
  const std::vector<std::string>& prefixes
) {
  filters.prefixes.insert(filters.prefixes.end(), prefixes.begin(), prefixes.end());
  return *this;
}

SearchParamFiltersBuilder& SearchParamFiltersBuilder::with_leaves_only(bool leaves_only
) {
  filters.leaves_only = leaves_only;
  return *this;
}

}  // namespace miru::query
