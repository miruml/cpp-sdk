// std
#include <execinfo.h>

// internal
#include <miru/params/composite.hpp>
#include <miru/params/errors.hpp>
#include <miru/params/parse.hpp>
#include <miru/params/type.hpp>
#include <miru/query/errors.hpp>
#include <miru/query/query.hpp>
#include <test/test_utils/query.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

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
           query.results});
        i++;
      }
    }
    return tests;
  }
};

// ================================= HAS PARAM ==================================== //
class HasParamTests : public testing::TestWithParam<SingleQueryTest> {};

void test_has_param(
  const miru::params::Parameter& data,
  const SingleQueryTest& test,
  const std::string& test_name) {
  // build the query
  SearchParamFiltersBuilder builder;
  builder.with_param_names(test.filter.param_names);
  builder.with_prefixes(test.filter.prefixes);
  builder.with_leaves_only(test.filter.leaves_only);
  SearchParamFilters filter = builder.build();

  // if the expected result is larger than 1, expect an exception
  EXPECT_EQ(miru::query::has_param(data, filter), !test.results.empty());

  // test reading each parameter by their name
  for (const auto& result : test.results) {
    builder = SearchParamFiltersBuilder();
    builder.with_param_name(result.param_name);
    builder.with_leaves_only(false);
    SearchParamFilters filter = builder.build();
    EXPECT_EQ(miru::query::has_param(data, filter), true);
  }
}

std::string HasParamTestNameGenerator(
  const testing::TestParamInfo<SingleQueryTest>& info) {
  return miru::test_utils::sanitize_test_name(info.param.description);
}

TEST_P(HasParamTests, Run) {
  const auto& test = GetParam();
  test_has_param(test.data, test, test.description);
}

INSTANTIATE_TEST_SUITE_P(
  Queries,
  HasParamTests,
  testing::ValuesIn(QueryTest::get_tests()),
  HasParamTestNameGenerator);

TEST(HasParamTests, DoesntExist) {
  miru::params::Parameter parameter;

  SearchParamFilters filter =
    SearchParamFiltersBuilder().with_param_name("doesnt_exist").build();
  EXPECT_EQ(miru::query::has_param(parameter, filter), false);
}

// =================================== GET PARAMS ====================================
// //
class GetParamsTests : public testing::TestWithParam<SingleQueryTest> {};

void test_get_params(
  const miru::params::Parameter& data,
  const SingleQueryTest& test,
  const std::string& test_name) {
  // build the query
  SearchParamFiltersBuilder builder;
  builder.with_param_names(test.filter.param_names);
  builder.with_prefixes(test.filter.prefixes);
  builder.with_leaves_only(test.filter.leaves_only);
  SearchParamFilters filter = builder.build();

  // execute the query
  std::vector<miru::params::Parameter> results = miru::query::get_params(data, filter);

  // verify the result
  EXPECT_EQ(results.size(), test.results.size());

  std::vector<std::string> expected_names;
  for (const auto& result : test.results) {
    expected_names.push_back(result.param_name);
  }
  for (const auto& param : results) {
    EXPECT_TRUE(
      std::find(expected_names.begin(), expected_names.end(), param.get_name()) !=
      expected_names.end())
      << "Parameter " << param.get_name() << " not found in " << test_name;
  }
}

std::string GetParamsTestNameGenerator(
  const testing::TestParamInfo<SingleQueryTest>& info) {
  return miru::test_utils::sanitize_test_name(info.param.description);
}

TEST_P(GetParamsTests, Run) {
  const auto& test = GetParam();
  test_get_params(test.data, test, test.description);
}

INSTANTIATE_TEST_SUITE_P(
  Queries,
  GetParamsTests,
  testing::ValuesIn(QueryTest::get_tests()),
  GetParamsTestNameGenerator);

// =================================== GET PARAM ==================================== //
enum class QueryException { None, ParameterNotFound, TooManyResults };

template <typename ExceptionT>
void test_must_get_param_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  EXPECT_THROW(miru::query::get_param(root, filter), ExceptionT)
    << "Expected exception not thrown";
}

void test_try_get_param_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  miru::params::Parameter param_result;
  miru::params::Parameter unchanged;
  EXPECT_EQ(miru::query::try_get_param(root, filter, param_result), false)
    << "try_get_param should have failed";
  EXPECT_EQ(param_result, unchanged) << "param_result should be unchanged";
}

void test_try_get_param_by_value_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  int int_vehicle = 42;
  EXPECT_EQ(miru::query::try_get_param(root, filter, int_vehicle), false)
    << "try_get_param should have failed";
  EXPECT_EQ(int_vehicle, 42) << "int_vehicle should be unchanged";
}

void test_get_param_or_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  EXPECT_EQ(miru::query::get_param_or(root, filter, 103), 103)
    << "get_param_or should return the default value (103)";
}

void test_try_get_param_or_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  int int_vehicle = 42;
  int default_value = 44;
  EXPECT_EQ(
    miru::query::try_get_param_or(root, filter, int_vehicle, default_value), false)
    << "try_get_param_or should have failed";
  EXPECT_EQ(int_vehicle, default_value) << "int_vehicle should be default_value";
}

template <typename ExceptionT>
void test_get_param_exception(
  const miru::params::Parameter& root,
  const SearchParamFilters& filter) {
  test_must_get_param_exception<ExceptionT>(root, filter);
  test_try_get_param_exception(root, filter);
  test_try_get_param_by_value_exception(root, filter);
  test_get_param_or_exception(root, filter);
  test_try_get_param_or_exception(root, filter);
}

template <typename ValueT>
void test_must_get_param_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected) {
  EXPECT_EQ(miru::query::get_param(data, filter).as<ValueT>(), expected);
}

template <typename ValueT>
void test_try_get_param_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected) {
  miru::params::Parameter param_result;
  EXPECT_EQ(miru::query::try_get_param(data, filter, param_result), true);
  EXPECT_EQ(param_result.as<ValueT>(), expected);
}

template <typename ValueT>
void test_try_get_param_by_value_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected,
  ValueT result_vehicle) {
  EXPECT_NE(result_vehicle, expected);
  EXPECT_EQ(miru::query::try_get_param(data, filter, result_vehicle), true);
  EXPECT_EQ(result_vehicle, expected);
}

template <typename ValueT>
void test_get_param_or_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected,
  ValueT result_vehicle) {
  EXPECT_NE(result_vehicle, expected);
  EXPECT_EQ(miru::query::get_param_or(data, filter, result_vehicle), expected);
}

template <typename ValueT>
void test_try_get_param_or_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected,
  ValueT result_vehicle) {
  EXPECT_NE(result_vehicle, expected);
  EXPECT_EQ(
    miru::query::try_get_param_or(data, filter, result_vehicle, result_vehicle), true);
  EXPECT_EQ(result_vehicle, expected);
}

template <typename ValueT>
void test_get_param_success_helper(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const ValueT& expected,
  ValueT result_vehicle) {
  test_must_get_param_success<ValueT>(data, filter, expected);
  test_try_get_param_success<ValueT>(data, filter, expected);
  test_try_get_param_by_value_success<ValueT>(data, filter, expected, result_vehicle);
  test_get_param_or_success<ValueT>(data, filter, expected, result_vehicle);
  test_try_get_param_or_success<ValueT>(data, filter, expected, result_vehicle);
}

void test_get_param_success(
  const miru::params::Parameter& data,
  const SearchParamFilters& filter,
  const miru::params::ParameterValue& expected) {
  miru::params::ParameterType expected_type = expected.get_type();
  switch (expected_type) {
    case miru::params::ParameterType::PARAMETER_NOT_SET:
      throw std::invalid_argument("Parameter not set");
      break;
    case miru::params::ParameterType::PARAMETER_BOOL:
      test_get_param_success_helper<bool>(data, filter, expected.as<bool>(), false);
      break;
    case miru::params::ParameterType::PARAMETER_INTEGER:
      test_get_param_success_helper<int64_t>(data, filter, expected.as<int64_t>(), 67);
      break;
    case miru::params::ParameterType::PARAMETER_DOUBLE:
      test_get_param_success_helper<double>(data, filter, expected.as<double>(), 67.0);
      break;
    case miru::params::ParameterType::PARAMETER_STRING:
      test_get_param_success_helper<std::string>(
        data, filter, expected.as<std::string>(), "hello");
      break;
    case miru::params::ParameterType::PARAMETER_BOOL_ARRAY:
      test_get_param_success_helper<std::vector<bool>>(
        data,
        filter,
        expected.as<std::vector<bool>>(),
        std::vector<bool>({true, false, true, true, true, false, false, true}));
      break;
    case miru::params::ParameterType::PARAMETER_INTEGER_ARRAY:
      test_get_param_success_helper<std::vector<int64_t>>(
        data,
        filter,
        expected.as<std::vector<int64_t>>(),
        std::vector<int64_t>({104, 42, 97}));
      break;
    case miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY:
      test_get_param_success_helper<std::vector<double>>(
        data,
        filter,
        expected.as<std::vector<double>>(),
        std::vector<double>({104.0, 42.0, 97.0}));
      break;
    case miru::params::ParameterType::PARAMETER_STRING_ARRAY:
      test_get_param_success_helper<std::vector<std::string>>(
        data,
        filter,
        expected.as<std::vector<std::string>>(),
        std::vector<std::string>({"hello", "world"}));
      break;
    case miru::params::ParameterType::PARAMETER_NULL:
      // test_get_param_success_helper<std::nullptr_t>(
      //     data,
      //     filter,
      //     nullptr,
      //     nullptr
      // );
      break;
    case miru::params::ParameterType::PARAMETER_SCALAR:
      test_get_param_success_helper<miru::params::Scalar>(
        data,
        filter,
        expected.as<miru::params::PARAMETER_SCALAR>(),
        miru::params::Scalar("67"));
      break;
    case miru::params::ParameterType::PARAMETER_SCALAR_ARRAY:
      test_get_param_success_helper<std::vector<miru::params::Scalar>>(
        data,
        filter,
        expected.as<miru::params::PARAMETER_SCALAR_ARRAY>(),
        std::vector<miru::params::Scalar>(
          {miru::params::Scalar("1.0"),
           miru::params::Scalar("2.0"),
           miru::params::Scalar("3.0")}));
      break;
    case miru::params::ParameterType::PARAMETER_MAP:
      test_get_param_success_helper<miru::params::Map>(
        data,
        filter,
        expected.as<miru::params::PARAMETER_MAP>(),
        miru::params::Map(
          {{"key1", miru::params::Scalar("value1")},
           {"key2", miru::params::Scalar("value2")}}));
      break;
    case miru::params::ParameterType::PARAMETER_MAP_ARRAY:
      test_get_param_success_helper<miru::params::MapArray>(
        data,
        filter,
        expected.as<miru::params::PARAMETER_MAP_ARRAY>(),
        miru::params::MapArray({miru::params::Map(
          {{"0.key1", miru::params::Scalar("value1")},
           {"0.key2", miru::params::Scalar("value2")}})}));
      break;
    case miru::params::ParameterType::PARAMETER_NESTED_ARRAY:
      test_get_param_success_helper<miru::params::NestedArray>(
        data,
        filter,
        expected.as<miru::params::PARAMETER_NESTED_ARRAY>(),
        miru::params::NestedArray(
          {miru::params::Parameter("0", std::vector<int64_t>({1, 2, 3})),
           miru::params::Parameter("1", std::vector<int64_t>({4, 5, 6})),
           miru::params::Parameter("2", std::vector<int64_t>({7, 8, 9}))}));
      break;
  }
}

void test_get_param(
  const miru::params::Parameter& data,
  const SingleQueryTest& test,
  const std::string& test_name) {
  // build the query
  SearchParamFiltersBuilder builder;
  builder.with_param_names(test.filter.param_names);
  builder.with_prefixes(test.filter.prefixes);
  builder.with_leaves_only(test.filter.leaves_only);
  SearchParamFilters filter = builder.build();

  // if the expected result is larger than 1, expect an exception
  if (test.results.size() > 1) {
    test_get_param_exception<miru::query::TooManyResultsError>(data, filter);
  }

  // test reading each parameter by their name
  int i = 0;
  for (const auto& result : test.results) {
    builder = SearchParamFiltersBuilder();
    builder.with_param_name(result.param_name);
    builder.with_leaves_only(false);
    SearchParamFilters filter = builder.build();
    test_get_param_success(data, filter, result.value);
    i++;
  }
}

class GetParamTests : public testing::TestWithParam<SingleQueryTest> {};

std::string GetParamTestNameGenerator(
  const testing::TestParamInfo<SingleQueryTest>& info) {
  return miru::test_utils::sanitize_test_name(info.param.description);
}

TEST_P(GetParamTests, Run) {
  const auto& test = GetParam();
  test_get_param(test.data, test, test.description);
}

INSTANTIATE_TEST_SUITE_P(
  Queries,
  GetParamTests,
  testing::ValuesIn(QueryTest::get_tests()),
  GetParamTestNameGenerator);

TEST(GetParamTests, NotFound) {
  SearchParamFiltersBuilder builder;
  builder.with_param_name("not_found");
  SearchParamFilters filter = builder.build();
  miru::params::Parameter root = miru::params::Parameter("root");
  test_get_param_exception<miru::query::ParameterNotFoundError>(root, filter);
}

TEST(GetParamTests, InvalidType) {
  SearchParamFiltersBuilder builder;
  builder.with_param_name("invalid_type");
  SearchParamFilters filter = builder.build();
  miru::params::Parameter root = miru::params::Parameter("root", 42);
  test_get_param_exception<miru::params::InvalidParameterTypeError>(root, filter);
}

}  // namespace test::query