// std
#include <execinfo.h>

// internal
#include <miru/params/load.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>
#include <boost/stacktrace.hpp>

namespace test::params {

enum class LoaderExceptionType {
  None,
  UnsupportedNode,
  InvalidScalarConversion,
};

// ================================== load_json ==================================== //
class LoadJson : public ::testing::Test {
 protected:
  miru::filesys::File json_file =
      miru::test_utils::filesys_testdata_dir().file("load-params.json");
};

TEST_F(LoadJson, ValidBool) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json bool_json = json["boolean"];
  auto param = miru::params::load_json("test", bool_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_BOOL);
}

TEST_F(LoadJson, ValidInt) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json int_json = json["integer"];
  auto param = miru::params::load_json("test", int_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_INTEGER);
}

TEST_F(LoadJson, ValidDouble) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json double_json = json["double"];
  auto param = miru::params::load_json("test", double_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_DOUBLE);
}

TEST_F(LoadJson, ValidString) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json string_json = json["string"];
  auto param = miru::params::load_json("test", string_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_STRING);
}

TEST_F(LoadJson, ValidStringArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json string_array_json = json["string_array"];
  auto param = miru::params::load_json("test", string_array_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_STRING_ARRAY);
}

TEST_F(LoadJson, ValidNull) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json null_json = json["null"];
  auto param = miru::params::load_json("test", null_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NULL);
}

TEST_F(LoadJson, ValidNestedArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json nested_array_json = json["nested_array"];
  auto param = miru::params::load_json("test", nested_array_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
}

TEST_F(LoadJson, ValidDeepNestedArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json deep_nested_array_json = json["deep_nested_array"];
  auto param = miru::params::load_json("test", deep_nested_array_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
}

TEST_F(LoadJson, ValidMap) {
  nlohmann::json json = json_file.read_json();
  auto param = miru::params::load_json("test", json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_MAP);
}

TEST_F(LoadJson, ValidMapArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json map_array_json = json["map_array"];
  auto param = miru::params::load_json("test", map_array_json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_MAP_ARRAY);
}

// ================================== load_yaml ==================================== //
class LoadYaml : public ::testing::Test {
 protected:
  miru::filesys::File yaml_file =
      miru::test_utils::filesys_testdata_dir().file("load-params.yaml");
};

TEST_F(LoadYaml, ValidBool) { EXPECT_EQ(true, false); }

}  // namespace test::params