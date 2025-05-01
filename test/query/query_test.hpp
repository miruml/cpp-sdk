#pragma once

// internal
#include <test/test_utils/query.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::query {

using SearchParamFilters = miru::query::SearchParamFilters;
using SearchParamFiltersBuilder = miru::query::SearchParamFiltersBuilder;

struct SingleQueryTest {
  std::string description;
  std::string config_slug;
  miru::params::Parameter data;
  test::test_utils::Filter filter;
  std::vector<test::test_utils::Result> results;
};

class QueryTest : public ::testing::Test {
 public:
  static std::vector<SingleQueryTest> get_tests() {
    miru::filesys::Dir query_dir = miru::test_utils::query_testdata_dir();
    nlohmann::json json = query_dir.file("query.json").read_json();
    std::vector<SingleQueryTest> tests;
    for (const auto& test_json : json) {
      auto test_set = test::test_utils::QueryTestSet::from_json(test_json);
      int i = 0;
      for (const auto& query : test_set.queries) {
        tests.push_back(
          {test_set.description + "." + std::to_string(i),
           test_set.config_slug,
           test_set.data,
           query.filter,
           query.results}
        );
        i++;
      }
    }
    return tests;
  }
};

}  // namespace test::query
