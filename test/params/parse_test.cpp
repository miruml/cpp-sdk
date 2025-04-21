// std
#include <execinfo.h>

// internal
#include <miru/params/composite.hpp>
#include <miru/params/parse.hpp>
#include <miru/query/query.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

enum class LoaderExceptionType {
  None,
  UnsupportedNode,
  InvalidScalarConversion,
};

using Scalar = miru::params::Scalar;

// ================================ parse_json_node ================================ //
class ParseJsonNode : public ::testing::Test {
 protected:
  miru::filesys::File json_file =
    miru::test_utils::params_testdata_dir().file("load.json");
};

TEST_F(ParseJsonNode, DoesntExist) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json doesnt_exist_json = json["doesnt_exist"];
  auto param = miru::params::parse_json_node("doesnt_exist", doesnt_exist_json);

  // json parses nonexistent nodes as null, be careful with inputting these into the
  // constructor
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NULL);
  EXPECT_TRUE(param.is_null());
}

TEST_F(ParseJsonNode, ValidBool) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json bool_json = json["boolean"];
  auto param = miru::params::parse_json_node("test", bool_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_BOOL);
  EXPECT_EQ(param.as_bool(), true);
}

TEST_F(ParseJsonNode, ValidInt) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json int_json = json["integer"];
  auto param = miru::params::parse_json_node("test", int_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_INTEGER);
  EXPECT_EQ(param.as_int(), 42);
}

TEST_F(ParseJsonNode, ValidDouble) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json double_json = json["double"];
  auto param = miru::params::parse_json_node("test", double_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_DOUBLE);
  EXPECT_EQ(param.as_double(), 3.14);
}

TEST_F(ParseJsonNode, ValidString) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json string_json = json["string"];
  auto param = miru::params::parse_json_node("test", string_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_STRING);
  EXPECT_EQ(param.as_string(), "Hello, world!");
}

TEST_F(ParseJsonNode, ValidBoolArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json bool_array_json = json["bool_array"];
  auto param = miru::params::parse_json_node("test", bool_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_BOOL_ARRAY);
  EXPECT_EQ(param.as_bool_array(), std::vector<bool>({true, false, true}));
}

TEST_F(ParseJsonNode, ValidIntArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json int_array_json = json["int_array"];
  auto param = miru::params::parse_json_node("test", int_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_INTEGER_ARRAY);
  EXPECT_EQ(param.as_integer_array(), std::vector<int64_t>({1, 2, 3}));
}

TEST_F(ParseJsonNode, ValidDoubleArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json double_array_json = json["double_array"];
  auto param = miru::params::parse_json_node("test", double_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY);
  EXPECT_EQ(param.as_double_array(), std::vector<double>({1.0, 2.0, 3.0}));
}

TEST_F(ParseJsonNode, ValidStringArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json string_array_json = json["string_array"];
  auto param = miru::params::parse_json_node("test", string_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_STRING_ARRAY);
  EXPECT_EQ(param.as_string_array(), std::vector<std::string>({"foo", "bar", "baz"}));
}

TEST_F(ParseJsonNode, ValidNull) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json null_json = json["null"];
  auto param = miru::params::parse_json_node("test", null_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NULL);
  EXPECT_TRUE(param.is_null());
}

TEST_F(ParseJsonNode, ValidNestedArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json nested_array_json = json["nested_array"];
  auto param = miru::params::parse_json_node("config-slug", nested_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
  EXPECT_EQ(
    param.as_nested_array(),
    miru::params::NestedArray(
      {miru::params::Parameter("config-slug.0", std::vector<int64_t>({1, 2, 3})),
       miru::params::Parameter("config-slug.1", std::vector<int64_t>({4, 5, 6})),
       miru::params::Parameter("config-slug.2", std::vector<int64_t>({7, 8, 9}))}));
}

TEST_F(ParseJsonNode, ValidDeepNestedArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json deep_nested_array_json = json["deep_nested_array"];
  auto param = miru::params::parse_json_node("config-slug", deep_nested_array_json);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
  EXPECT_EQ(
    param.as_nested_array(),
    miru::params::NestedArray(
      {miru::params::NestedArray(
         {miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.0.0.0", std::vector<int64_t>({1, 2, 3})),
             miru::params::Parameter(
               "config-slug.0.0.1", std::vector<int64_t>({4, 5, 6})),
             miru::params::Parameter(
               "config-slug.0.0.2", std::vector<int64_t>({7, 8, 9}))}),
          miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.0.1.0", std::vector<int64_t>({1, 2, 3})),
             miru::params::Parameter(
               "config-slug.0.1.1", std::vector<int64_t>({4, 5, 6})),
             miru::params::Parameter(
               "config-slug.0.1.2", std::vector<int64_t>({7, 8, 9}))})}),
       miru::params::NestedArray(
         {miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.1.0.0", std::vector<int64_t>({1, 2, 3})),
             miru::params::Parameter(
               "config-slug.1.0.1", std::vector<int64_t>({4, 5, 6})),
             miru::params::Parameter(
               "config-slug.1.0.2", std::vector<int64_t>({7, 8, 9}))}),
          miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.1.1.0", std::vector<int64_t>({1, 2, 3})),
             miru::params::Parameter(
               "config-slug.1.1.1", std::vector<int64_t>({4, 5, 6})),
             miru::params::Parameter(
               "config-slug.1.1.2", std::vector<int64_t>({7, 8, 9}))})})}));
}

TEST_F(ParseJsonNode, ValidMap) {
  nlohmann::json json = json_file.read_json();
  auto param = miru::params::parse_json_node("test", json);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_MAP);
  // all of these individual values are tested above so we'll just let this rest
}

TEST_F(ParseJsonNode, ValidMapArray) {
  nlohmann::json json = json_file.read_json();
  nlohmann::json map_array_json = json["map_array"];
  auto param = miru::params::parse_json_node("config-slug", map_array_json);

  EXPECT_EQ(
    param.as_map_array(),
    miru::params::MapArray(
      {miru::params::Map(
         {miru::params::Parameter("config-slug.0.key1", "value1"),
          miru::params::Parameter("config-slug.0.key2", "value2")}),
       miru::params::Map(
         {miru::params::Parameter("config-slug.1.key3", "value3"),
          miru::params::Parameter("config-slug.1.key4", "value4")}),
       miru::params::Map(
         {miru::params::Parameter("config-slug.2.key5", "value5"),
          miru::params::Parameter("config-slug.2.key6", "value6")})}));
}

// ================================== load_yaml ==================================== //
class ParseYamlNode : public ::testing::Test {
 protected:
  miru::filesys::File yaml_file =
    miru::test_utils::params_testdata_dir().file("load.yaml");
};

TEST_F(ParseYamlNode, DoesntExist) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node doesnt_exist_yaml = yaml["doesnt_exist"];
  EXPECT_THROW(
    miru::params::parse_yaml_node("doesnt_exist", doesnt_exist_yaml),
    std::runtime_error);
}

TEST_F(ParseYamlNode, ValidBool) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node bool_yaml = yaml["boolean"];
  auto param = miru::params::parse_yaml_node("test", bool_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR);
  EXPECT_EQ(param.as_bool(), true);
}

TEST_F(ParseYamlNode, ValidInt) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node int_yaml = yaml["integer"];
  auto param = miru::params::parse_yaml_node("test", int_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR);
  EXPECT_EQ(param.as_int(), 42);
}

TEST_F(ParseYamlNode, ValidDouble) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node double_yaml = yaml["double"];
  auto param = miru::params::parse_yaml_node("test", double_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR);
  EXPECT_EQ(param.as_double(), 3.14);
}

TEST_F(ParseYamlNode, ValidString) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node string_yaml = yaml["string"];
  auto param = miru::params::parse_yaml_node("test", string_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR);
  EXPECT_EQ(param.as_string(), "Hello, world!");
}

TEST_F(ParseYamlNode, ValidBoolArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node bool_array_yaml = yaml["bool_array"];
  auto param = miru::params::parse_yaml_node("test", bool_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR_ARRAY);
  EXPECT_EQ(param.as_bool_array(), std::vector<bool>({true, false, true}));
}

TEST_F(ParseYamlNode, ValidIntArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node int_array_yaml = yaml["int_array"];
  auto param = miru::params::parse_yaml_node("test", int_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR_ARRAY);
  EXPECT_EQ(param.as_integer_array(), std::vector<int64_t>({1, 2, 3}));
}

TEST_F(ParseYamlNode, ValidDoubleArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node double_array_yaml = yaml["double_array"];
  auto param = miru::params::parse_yaml_node("test", double_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR_ARRAY);
  EXPECT_EQ(param.as_double_array(), std::vector<double>({1.0, 2.0, 3.0}));
}

TEST_F(ParseYamlNode, ValidStringArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node string_array_yaml = yaml["string_array"];
  auto param = miru::params::parse_yaml_node("test", string_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_SCALAR_ARRAY);
  EXPECT_EQ(param.as_string_array(), std::vector<std::string>({"foo", "bar", "baz"}));
}

TEST_F(ParseYamlNode, ValidNull) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node null_yaml = yaml["null"];
  auto param = miru::params::parse_yaml_node("test", null_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NULL);
  EXPECT_TRUE(param.is_null());
}

TEST_F(ParseYamlNode, ValidNestedArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node nested_array_yaml = yaml["nested_array"];
  auto param = miru::params::parse_yaml_node("config-slug", nested_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
  EXPECT_EQ(
    param.as_nested_array(),
    miru::params::NestedArray(
      {miru::params::Parameter(
         "config-slug.0", std::vector<Scalar>({Scalar("1"), Scalar("2"), Scalar("3")})),
       miru::params::Parameter(
         "config-slug.1", std::vector<Scalar>({Scalar("4"), Scalar("5"), Scalar("6")})),
       miru::params::Parameter(
         "config-slug.2",
         std::vector<Scalar>({Scalar("7"), Scalar("8"), Scalar("9")}))}));
}

TEST_F(ParseYamlNode, ValidDeepNestedArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node deep_nested_array_yaml = yaml["deep_nested_array"];
  auto param = miru::params::parse_yaml_node("config-slug", deep_nested_array_yaml);

  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_NESTED_ARRAY);
  EXPECT_EQ(
    param.as_nested_array(),
    miru::params::NestedArray(
      {miru::params::NestedArray(
         {miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.0.0.0",
               std::vector<Scalar>({Scalar("1"), Scalar("2"), Scalar("3")})),
             miru::params::Parameter(
               "config-slug.0.0.1",
               std::vector<Scalar>({Scalar("4"), Scalar("5"), Scalar("6")})),
             miru::params::Parameter(
               "config-slug.0.0.2",
               std::vector<Scalar>({Scalar("7"), Scalar("8"), Scalar("9")}))}),
          miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.0.1.0",
               std::vector<Scalar>({Scalar("1"), Scalar("2"), Scalar("3")})),
             miru::params::Parameter(
               "config-slug.0.1.1",
               std::vector<Scalar>({Scalar("4"), Scalar("5"), Scalar("6")})),
             miru::params::Parameter(
               "config-slug.0.1.2",
               std::vector<Scalar>({Scalar("7"), Scalar("8"), Scalar("9")}))})}),
       miru::params::NestedArray(
         {miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.1.0.0",
               std::vector<Scalar>({Scalar("1"), Scalar("2"), Scalar("3")})),
             miru::params::Parameter(
               "config-slug.1.0.1",
               std::vector<Scalar>({Scalar("4"), Scalar("5"), Scalar("6")})),
             miru::params::Parameter(
               "config-slug.1.0.2",
               std::vector<Scalar>({Scalar("7"), Scalar("8"), Scalar("9")}))}),
          miru::params::NestedArray(
            {miru::params::Parameter(
               "config-slug.1.1.0",
               std::vector<Scalar>({Scalar("1"), Scalar("2"), Scalar("3")})),
             miru::params::Parameter(
               "config-slug.1.1.1",
               std::vector<Scalar>({Scalar("4"), Scalar("5"), Scalar("6")})),
             miru::params::Parameter(
               "config-slug.1.1.2",
               std::vector<Scalar>({Scalar("7"), Scalar("8"), Scalar("9")}))})})}));
}

TEST_F(ParseYamlNode, ValidMap) {
  YAML::Node yaml = yaml_file.read_yaml();
  auto param = miru::params::parse_yaml_node("test", yaml);
  EXPECT_EQ(param.get_type(), miru::params::ParameterType::PARAMETER_MAP);
  // all of these individual values are tested above so we'll just let this rest
}

TEST_F(ParseYamlNode, ValidMapArray) {
  YAML::Node yaml = yaml_file.read_yaml();
  YAML::Node map_array_yaml = yaml["map_array"];
  auto param = miru::params::parse_yaml_node("config-slug", map_array_yaml);

  EXPECT_EQ(
    param.as_map_array(),
    miru::params::MapArray(
      {miru::params::Map(
         {miru::params::Parameter("config-slug.0.key1", Scalar("value1")),
          miru::params::Parameter("config-slug.0.key2", Scalar("value2"))}),
       miru::params::Map(
         {miru::params::Parameter("config-slug.1.key3", Scalar("value3")),
          miru::params::Parameter("config-slug.1.key4", Scalar("value4"))}),
       miru::params::Map(
         {miru::params::Parameter("config-slug.2.key5", Scalar("value5")),
          miru::params::Parameter("config-slug.2.key6", Scalar("value6"))})}));
}

}  // namespace test::params