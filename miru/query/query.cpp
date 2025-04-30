// std
#include <vector>

// internal
#include <miru/params/details/iterator.hpp>
#include <miru/params/details/utils.hpp>
#include <miru/query/query.hpp>

namespace miru::query {

void find_all_recursive_helper(
  const Parameter& parameter,
  std::vector<const Parameter*>& result,
  const SearchParamFilters& filters
) {
  // check for a match
  if (filters.matches(parameter)) {
    result.push_back(&parameter);
  }

  // check if we should continue searching
  if (!filters.continue_search(parameter)) {
    return;
  }

  // find recursively
  for (const auto& item : miru::params::details::get_children_view(parameter)) {
    find_all_recursive_helper(item, result, filters);
  }
}

std::vector<const Parameter*>
find_all(const Parameter& root, const SearchParamFilters& filters) {
  std::vector<const Parameter*> result;
  find_all_recursive_helper(root, result, filters);
  return result;
}

std::vector<const Parameter*>
find_all(const ParametersView& roots, const SearchParamFilters& filters) {
  std::vector<const Parameter*> result;
  for (const auto& root : roots) {
    result.insert(
      result.end(), find_all(root, filters).begin(), find_all(root, filters).end()
    );
  }
  return result;
}

std::vector<const Parameter*>
find_all(const std::vector<Parameter>& roots, const SearchParamFilters& filters) {
  return find_all(ParametersView(roots.begin(), roots.end()), filters);
}

std::vector<const Parameter*>
find_all(const Map& map, const SearchParamFilters& filters) {
  return find_all(ParametersView(map.begin(), map.end()), filters);
}

std::vector<const Parameter*>
find_all(const NestedArray& nested_array, const SearchParamFilters& filters) {
  return find_all(ParametersView(nested_array.begin(), nested_array.end()), filters);
}

std::vector<const Parameter*>
find_all(const MapArray& map_array, const SearchParamFilters& filters) {
  return find_all(ParametersView(map_array.begin(), map_array.end()), filters);
}

std::vector<const Parameter*>
find_all(const miru::config::Config& config, const SearchParamFilters& filters) {
  return find_all(config.root_parameter(), filters);
}
}  // namespace miru::query
