// std
#include <string>

// internal
#include <miru/params/errors.hpp>
#include <miru/params/parameter.hpp>
#include <test/utils_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

// ==================== CONSTRUCTORS + EQUALITY + INFO GETTERS ===================== //
class ParameterConstructors : public ::testing::Test {
 protected:
};

TEST_F(ParameterConstructors, trailing_delimiter_in_constructor) {
  miru::params::ParameterValue value(2);

  // one level
  miru::params::Parameter param1_constructor1 = miru::params::Parameter("test.");
  miru::params::Parameter param1_constructor2 = miru::params::Parameter("test.", value);
  miru::params::Parameter param1_constructor3 = miru::params::Parameter("test.", 2);
  EXPECT_EQ(param1_constructor1.get_name(), "test");
  EXPECT_EQ(param1_constructor2.get_name(), "test");
  EXPECT_EQ(param1_constructor3.get_name(), "test");

  // two levels
  miru::params::Parameter param2_constructor1 = miru::params::Parameter("l1.l2.");
  miru::params::Parameter param2_constructor2 =
    miru::params::Parameter("l1.l2.", value);
  miru::params::Parameter param2_constructor3 = miru::params::Parameter("l1.l2.", 2);
  EXPECT_EQ(param2_constructor1.get_name(), "l1.l2");
  EXPECT_EQ(param2_constructor2.get_name(), "l1.l2");
  EXPECT_EQ(param2_constructor3.get_name(), "l1.l2");

  // three levels
  miru::params::Parameter param3_constructor1 = miru::params::Parameter("l1.l2.l3.");
  miru::params::Parameter param3_constructor2 =
    miru::params::Parameter("l1.l2.l3.", value);
  miru::params::Parameter param3_constructor3 = miru::params::Parameter("l1.l2.l3.", 2);
  EXPECT_EQ(param3_constructor1.get_name(), "l1.l2.l3");
  EXPECT_EQ(param3_constructor2.get_name(), "l1.l2.l3");
  EXPECT_EQ(param3_constructor3.get_name(), "l1.l2.l3");

  // many trailing slashes
  miru::params::Parameter param4_constructor1 = miru::params::Parameter("l1.l2.l3...");
  miru::params::Parameter param4_constructor2 =
    miru::params::Parameter("l1.l2.l3...", value);
  miru::params::Parameter param4_constructor3 =
    miru::params::Parameter("l1.l2.l3...", 2);
  EXPECT_EQ(param4_constructor1.get_name(), "l1.l2.l3");
  EXPECT_EQ(param4_constructor2.get_name(), "l1.l2.l3");
  EXPECT_EQ(param4_constructor3.get_name(), "l1.l2.l3");

  // no trailing slashes
  miru::params::Parameter param5_constructor1 = miru::params::Parameter("test");
  miru::params::Parameter param5_constructor2 = miru::params::Parameter("test", value);
  miru::params::Parameter param5_constructor3 = miru::params::Parameter("test", 2);
  EXPECT_EQ(param5_constructor1.get_name(), "test");
  EXPECT_EQ(param5_constructor2.get_name(), "test");
  EXPECT_EQ(param5_constructor3.get_name(), "test");
}

TEST_F(ParameterConstructors, not_set_variant) {
  // equal
  miru::params::Parameter not_set_variant1 = miru::params::Parameter();
  miru::params::Parameter not_set_variant2 = miru::params::Parameter("");
  EXPECT_EQ(not_set_variant1, not_set_variant2);
  miru::params::Parameter not_set_variant3 =
    miru::params::Parameter("", miru::params::ParameterValue());

  // not equal
  miru::params::Parameter diff_name = miru::params::Parameter("arglebargle");
  EXPECT_NE(not_set_variant1, diff_name);

  // get type
  EXPECT_EQ(
    not_set_variant1.get_type(), miru::params::ParameterType::PARAMETER_NOT_SET
  );

  // get type name
  EXPECT_EQ(not_set_variant1.get_type_name(), "not set");

  // get parameter value
  EXPECT_EQ(not_set_variant1.get_parameter_value(), miru::params::ParameterValue());

  // value to string
  EXPECT_EQ(not_set_variant1.value_to_string(), "\"not set\"");

  EXPECT_FALSE(not_set_variant1.is_null());
  EXPECT_FALSE(not_set_variant1.is_scalar());
  EXPECT_FALSE(not_set_variant1.is_scalar_array());
  EXPECT_FALSE(not_set_variant1.is_nested_array());
  EXPECT_FALSE(not_set_variant1.is_map());
  EXPECT_FALSE(not_set_variant1.is_map_array());
  EXPECT_FALSE(not_set_variant1.is_array());
}

TEST_F(ParameterConstructors, bool_variant) {
  // equal
  miru::params::ParameterValue bool_value(true);
  miru::params::Parameter bool_variant1 =
    miru::params::Parameter("bool_param", bool_value);
  miru::params::Parameter bool_variant2 = miru::params::Parameter("bool_param", true);
  EXPECT_EQ(bool_variant1, bool_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter("bool_param", false);
  EXPECT_NE(bool_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", bool_value);
  EXPECT_NE(bool_variant1, diff_name);

  // get type
  EXPECT_EQ(bool_variant1.get_type(), miru::params::ParameterType::PARAMETER_BOOL);

  // get type name
  EXPECT_EQ(bool_variant1.get_type_name(), "bool");

  // get parameter value
  EXPECT_EQ(bool_variant1.get_parameter_value(), bool_value);

  // value to string
  EXPECT_EQ(bool_variant1.value_to_string(), "true");

  EXPECT_FALSE(bool_variant1.is_null());
  EXPECT_TRUE(bool_variant1.is_scalar());
  EXPECT_FALSE(bool_variant1.is_scalar_array());
  EXPECT_FALSE(bool_variant1.is_nested_array());
  EXPECT_FALSE(bool_variant1.is_map());
  EXPECT_FALSE(bool_variant1.is_map_array());
  EXPECT_FALSE(bool_variant1.is_array());
}

TEST_F(ParameterConstructors, int_variant) {
  // equal
  miru::params::ParameterValue int_value(4);
  miru::params::Parameter int_variant1 =
    miru::params::Parameter("int_param", int_value);
  miru::params::Parameter int_variant2 = miru::params::Parameter("int_param", 4);
  EXPECT_EQ(int_variant1, int_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter("int_param", 5);
  EXPECT_NE(int_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name = miru::params::Parameter("arglebargle", int_value);
  EXPECT_NE(int_variant1, diff_name);

  // get type
  EXPECT_EQ(int_variant1.get_type(), miru::params::ParameterType::PARAMETER_INTEGER);

  // get type name
  EXPECT_EQ(int_variant1.get_type_name(), "int");

  // get parameter value
  EXPECT_EQ(int_variant1.get_parameter_value(), int_value);

  // value to string
  EXPECT_EQ(int_variant1.value_to_string(), "4");

  EXPECT_FALSE(int_variant1.is_null());
  EXPECT_TRUE(int_variant1.is_scalar());
  EXPECT_FALSE(int_variant1.is_scalar_array());
  EXPECT_FALSE(int_variant1.is_nested_array());
  EXPECT_FALSE(int_variant1.is_map());
  EXPECT_FALSE(int_variant1.is_map_array());
  EXPECT_FALSE(int_variant1.is_array());
}

TEST_F(ParameterConstructors, double_variant) {
  miru::params::ParameterValue double_value(4.0);
  miru::params::Parameter double_variant1 =
    miru::params::Parameter("double_param", double_value);
  miru::params::Parameter double_variant2 =
    miru::params::Parameter("double_param", 4.0);
  EXPECT_EQ(double_variant1, double_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter("double_param", 5.0);
  EXPECT_NE(double_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", double_value);
  EXPECT_NE(double_variant1, diff_name);

  // get type
  EXPECT_EQ(double_variant1.get_type(), miru::params::ParameterType::PARAMETER_DOUBLE);

  // get type name
  EXPECT_EQ(double_variant1.get_type_name(), "double");

  // get parameter value
  EXPECT_EQ(double_variant1.get_parameter_value(), double_value);

  // value to string
  EXPECT_EQ(double_variant1.value_to_string(), "4.000000");

  EXPECT_FALSE(double_variant1.is_null());
  EXPECT_TRUE(double_variant1.is_scalar());
  EXPECT_FALSE(double_variant1.is_scalar_array());
  EXPECT_FALSE(double_variant1.is_nested_array());
  EXPECT_FALSE(double_variant1.is_map());
  EXPECT_FALSE(double_variant1.is_map_array());
}

TEST_F(ParameterConstructors, string_variant) {
  miru::params::ParameterValue string_value("test");
  miru::params::Parameter string_variant1 =
    miru::params::Parameter("string_param", string_value);
  miru::params::Parameter string_variant2 =
    miru::params::Parameter("string_param", "test");
  EXPECT_EQ(string_variant1, string_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter("string_param", "test2");
  EXPECT_NE(string_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", string_value);
  EXPECT_NE(string_variant1, diff_name);

  // get type
  EXPECT_EQ(string_variant1.get_type(), miru::params::ParameterType::PARAMETER_STRING);

  // get type name
  EXPECT_EQ(string_variant1.get_type_name(), "string");

  // get parameter value
  EXPECT_EQ(string_variant1.get_parameter_value(), string_value);

  // value to string
  EXPECT_EQ(string_variant1.value_to_string(), "\"test\"");

  EXPECT_FALSE(string_variant1.is_null());
  EXPECT_TRUE(string_variant1.is_scalar());
  EXPECT_FALSE(string_variant1.is_scalar_array());
  EXPECT_FALSE(string_variant1.is_nested_array());
  EXPECT_FALSE(string_variant1.is_map());
  EXPECT_FALSE(string_variant1.is_map_array());
  EXPECT_FALSE(string_variant1.is_array());
}

TEST_F(ParameterConstructors, bool_array_variant) {
  miru::params::ParameterValue bool_array_value(std::vector<bool>{true, false, true});
  miru::params::Parameter bool_array_variant1 =
    miru::params::Parameter("bool_array_param", bool_array_value);
  miru::params::Parameter bool_array_variant2 =
    miru::params::Parameter("bool_array_param", std::vector<bool>{true, false, true});
  EXPECT_EQ(bool_array_variant1, bool_array_variant2);

  // value not equal
  miru::params::Parameter diff_value =
    miru::params::Parameter("bool_array_param", std::vector<bool>{true, true, true});
  EXPECT_NE(bool_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", bool_array_value);
  EXPECT_NE(bool_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    bool_array_variant1.get_type(), miru::params::ParameterType::PARAMETER_BOOL_ARRAY
  );

  // get type name
  EXPECT_EQ(bool_array_variant1.get_type_name(), "bool array");

  // get parameter value
  EXPECT_EQ(bool_array_variant1.get_parameter_value(), bool_array_value);

  // value to string
  EXPECT_EQ(bool_array_variant1.value_to_string(), "[true, false, true]");

  EXPECT_FALSE(bool_array_variant1.is_null());
  EXPECT_FALSE(bool_array_variant1.is_scalar());
  EXPECT_TRUE(bool_array_variant1.is_scalar_array());
  EXPECT_FALSE(bool_array_variant1.is_nested_array());
  EXPECT_FALSE(bool_array_variant1.is_map());
  EXPECT_FALSE(bool_array_variant1.is_map_array());
  EXPECT_TRUE(bool_array_variant1.is_array());
}

TEST_F(ParameterConstructors, int_array_variant) {
  miru::params::ParameterValue int_array_value(std::vector<int>{1, 2, 3});
  miru::params::Parameter int_array_variant1 =
    miru::params::Parameter("int_array_param", int_array_value);
  miru::params::Parameter int_array_variant2 =
    miru::params::Parameter("int_array_param", std::vector<int>{1, 2, 3});
  EXPECT_EQ(int_array_variant1, int_array_variant2);

  // value not equal
  miru::params::Parameter diff_value =
    miru::params::Parameter("int_array_param", std::vector<int>{1, 2, 4});
  EXPECT_NE(int_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", int_array_value);
  EXPECT_NE(int_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    int_array_variant1.get_type(), miru::params::ParameterType::PARAMETER_INTEGER_ARRAY
  );

  // get type name
  EXPECT_EQ(int_array_variant1.get_type_name(), "int array");

  // get parameter value
  EXPECT_EQ(int_array_variant1.get_parameter_value(), int_array_value);

  // value to string
  EXPECT_EQ(int_array_variant1.value_to_string(), "[1, 2, 3]");

  EXPECT_FALSE(int_array_variant1.is_null());
  EXPECT_FALSE(int_array_variant1.is_scalar());
  EXPECT_TRUE(int_array_variant1.is_scalar_array());
  EXPECT_FALSE(int_array_variant1.is_nested_array());
  EXPECT_FALSE(int_array_variant1.is_map());
  EXPECT_FALSE(int_array_variant1.is_map_array());
  EXPECT_TRUE(int_array_variant1.is_array());
}

TEST_F(ParameterConstructors, double_array_variant) {
  miru::params::ParameterValue double_array_value(std::vector<double>{1.0, 2.2, 3.3});
  miru::params::Parameter double_array_variant1 =
    miru::params::Parameter("double_array_param", double_array_value);
  miru::params::Parameter double_array_variant2 =
    miru::params::Parameter("double_array_param", std::vector<double>{1.0, 2.2, 3.3});
  EXPECT_EQ(double_array_variant1, double_array_variant2);

  // value not equal
  miru::params::Parameter diff_value =
    miru::params::Parameter("double_array_param", std::vector<double>{1.0, 2.0, 4.0});
  EXPECT_NE(double_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", double_array_value);
  EXPECT_NE(double_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    double_array_variant1.get_type(),
    miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY
  );

  // get type name
  EXPECT_EQ(double_array_variant1.get_type_name(), "double array");

  // get parameter value
  EXPECT_EQ(double_array_variant1.get_parameter_value(), double_array_value);

  // value to string
  EXPECT_EQ(double_array_variant1.value_to_string(), "[1.000000, 2.200000, 3.300000]");

  EXPECT_FALSE(double_array_variant1.is_null());
  EXPECT_FALSE(double_array_variant1.is_scalar());
  EXPECT_TRUE(double_array_variant1.is_scalar_array());
  EXPECT_FALSE(double_array_variant1.is_nested_array());
  EXPECT_FALSE(double_array_variant1.is_map());
  EXPECT_FALSE(double_array_variant1.is_map_array());
  EXPECT_TRUE(double_array_variant1.is_array());
}

TEST_F(ParameterConstructors, string_array_variant) {
  miru::params::ParameterValue string_array_value(
    std::vector<std::string>{"test1", "test2", "test3"}
  );
  miru::params::Parameter string_array_variant1 =
    miru::params::Parameter("string_array_param", string_array_value);
  miru::params::Parameter string_array_variant2 = miru::params::Parameter(
    "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
  );
  EXPECT_EQ(string_array_variant1, string_array_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter(
    "string_array_param", std::vector<std::string>{"test1", "test2", "test4"}
  );
  EXPECT_NE(string_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", string_array_value);
  EXPECT_NE(string_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    string_array_variant1.get_type(),
    miru::params::ParameterType::PARAMETER_STRING_ARRAY
  );

  // get type name
  EXPECT_EQ(string_array_variant1.get_type_name(), "string array");

  // get parameter value
  EXPECT_EQ(string_array_variant1.get_parameter_value(), string_array_value);

  // value to string
  EXPECT_EQ(
    string_array_variant1.value_to_string(), "[\"test1\", \"test2\", \"test3\"]"
  );

  EXPECT_FALSE(string_array_variant1.is_null());
  EXPECT_FALSE(string_array_variant1.is_scalar());
  EXPECT_TRUE(string_array_variant1.is_scalar_array());
  EXPECT_FALSE(string_array_variant1.is_nested_array());
  EXPECT_FALSE(string_array_variant1.is_map());
  EXPECT_FALSE(string_array_variant1.is_map_array());
  EXPECT_TRUE(string_array_variant1.is_array());
}

TEST_F(ParameterConstructors, null_variant) {
  miru::params::Parameter null_variant1 =
    miru::params::Parameter("null_param", nullptr);
  miru::params::Parameter null_variant2 =
    miru::params::Parameter("null_param", miru::params::ParameterValue(nullptr));
  EXPECT_EQ(null_variant1, null_variant2);

  // unequal value is not possible

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", miru::params::ParameterValue(nullptr));
  EXPECT_NE(null_variant1, diff_name);

  // get type
  EXPECT_EQ(null_variant1.get_type(), miru::params::ParameterType::PARAMETER_NULL);

  // get type name
  EXPECT_EQ(null_variant1.get_type_name(), "null");

  // get parameter value
  EXPECT_EQ(null_variant1.get_parameter_value(), miru::params::ParameterValue(nullptr));

  // value to string
  EXPECT_EQ(null_variant1.value_to_string(), "null");

  EXPECT_TRUE(null_variant1.is_null());
  EXPECT_FALSE(null_variant1.is_scalar());
  EXPECT_FALSE(null_variant1.is_scalar_array());
  EXPECT_FALSE(null_variant1.is_nested_array());
  EXPECT_FALSE(null_variant1.is_map());
  EXPECT_FALSE(null_variant1.is_map_array());
  EXPECT_FALSE(null_variant1.is_array());
}

TEST_F(ParameterConstructors, scalar_variant) {
  miru::params::ParameterValue scalar_value(miru::params::Scalar("test"));
  miru::params::Parameter scalar_variant1 =
    miru::params::Parameter("scalar_param", scalar_value);
  miru::params::Parameter scalar_variant2 =
    miru::params::Parameter("scalar_param", miru::params::Scalar("test"));
  EXPECT_EQ(scalar_variant1, scalar_variant2);

  // value not equal
  miru::params::Parameter diff_value =
    miru::params::Parameter("scalar_param", miru::params::Scalar("test2"));
  EXPECT_NE(scalar_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", scalar_value);
  EXPECT_NE(scalar_variant1, diff_name);

  // get type
  EXPECT_EQ(scalar_variant1.get_type(), miru::params::ParameterType::PARAMETER_SCALAR);

  // get type name
  EXPECT_EQ(scalar_variant1.get_type_name(), "scalar");

  // get parameter value
  EXPECT_EQ(scalar_variant1.get_parameter_value(), scalar_value);

  // value to string
  EXPECT_EQ(scalar_variant1.value_to_string(), "\"test\"");

  EXPECT_FALSE(scalar_variant1.is_null());
  EXPECT_TRUE(scalar_variant1.is_scalar());
  EXPECT_FALSE(scalar_variant1.is_scalar_array());
  EXPECT_FALSE(scalar_variant1.is_nested_array());
  EXPECT_FALSE(scalar_variant1.is_map());
  EXPECT_FALSE(scalar_variant1.is_map_array());
  EXPECT_FALSE(scalar_variant1.is_array());
}

TEST_F(ParameterConstructors, scalar_array_variant) {
  miru::params::ParameterValue scalar_array_value(std::vector<miru::params::Scalar>{
    miru::params::Scalar("true"), miru::params::Scalar("false"),
    miru::params::Scalar("true")
  });
  miru::params::Parameter scalar_array_variant1 =
    miru::params::Parameter("scalar_array_param", scalar_array_value);
  miru::params::Parameter scalar_array_variant2 = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("true"), miru::params::Scalar("false"),
      miru::params::Scalar("true")
    }
  );
  EXPECT_EQ(scalar_array_variant1, scalar_array_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("true"), miru::params::Scalar("false"),
      miru::params::Scalar("false")
    }
  );
  EXPECT_NE(scalar_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name =
    miru::params::Parameter("arglebargle", scalar_array_value);
  EXPECT_NE(scalar_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    scalar_array_variant1.get_type(),
    miru::params::ParameterType::PARAMETER_SCALAR_ARRAY
  );

  // get type name
  EXPECT_EQ(scalar_array_variant1.get_type_name(), "scalar array");

  // get parameter value
  EXPECT_EQ(scalar_array_variant1.get_parameter_value(), scalar_array_value);

  // value to string
  EXPECT_EQ(scalar_array_variant1.value_to_string(), "[\"true\", \"false\", \"true\"]");

  EXPECT_FALSE(scalar_array_variant1.is_null());
  EXPECT_FALSE(scalar_array_variant1.is_scalar());
  EXPECT_TRUE(scalar_array_variant1.is_scalar_array());
  EXPECT_FALSE(scalar_array_variant1.is_nested_array());
  EXPECT_FALSE(scalar_array_variant1.is_map());
  EXPECT_FALSE(scalar_array_variant1.is_map_array());
  EXPECT_TRUE(scalar_array_variant1.is_array());
}

TEST_F(ParameterConstructors, nested_array_variant) {
  miru::params::ParameterValue nested_array_value(
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 3}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );
  miru::params::Parameter nested_array_variant1 =
    miru::params::Parameter("nested_array", nested_array_value);
  miru::params::Parameter nested_array_variant2 = miru::params::Parameter(
    "nested_array",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 3}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );
  EXPECT_EQ(nested_array_variant1, nested_array_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter(
    "nested_array",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 9}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );
  EXPECT_NE(nested_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name = miru::params::Parameter(
    "argle_bargle",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("argle_bargle.0", std::vector<int>{1, 2, 9}),
      miru::params::Parameter("argle_bargle.1", std::vector<int>{4, 5, 6})
    }}
  );
  EXPECT_NE(nested_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    nested_array_variant1.get_type(),
    miru::params::ParameterType::PARAMETER_NESTED_ARRAY
  );

  // get type name
  EXPECT_EQ(nested_array_variant1.get_type_name(), "nested array");

  // get parameter value
  EXPECT_EQ(nested_array_variant1.get_parameter_value(), nested_array_value);

  // value to string
  EXPECT_EQ(nested_array_variant1.value_to_string(), "[\n  [1, 2, 3],\n  [4, 5, 6]\n]");

  EXPECT_FALSE(nested_array_variant1.is_null());
  EXPECT_FALSE(nested_array_variant1.is_scalar());
  EXPECT_FALSE(nested_array_variant1.is_scalar_array());
  EXPECT_TRUE(nested_array_variant1.is_nested_array());
  EXPECT_FALSE(nested_array_variant1.is_map());
  EXPECT_FALSE(nested_array_variant1.is_map_array());
  EXPECT_TRUE(nested_array_variant1.is_array());

  // nested arrays of maps are not leaves
  miru::params::ParameterValue map_array_item1 = miru::params::ParameterValue(
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "nested_array.0.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "nested_array.0.0.scalar1", miru::params::Scalar("test1")
          ),
          miru::params::Parameter(
            "nested_array.0.0.scalar2", miru::params::Scalar("test2")
          )
        }}
      ),
      miru::params::Parameter(
        "nested_array.0.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "nested_array.0.1.scalar1", miru::params::Scalar("test3")
          ),
          miru::params::Parameter(
            "nested_array.0.1.scalar2", miru::params::Scalar("test4")
          )
        }}
      )
    }}
  );
  miru::params::ParameterValue map_array_item2 = miru::params::ParameterValue(
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "nested_array.1.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "nested_array.1.0.scalar1", miru::params::Scalar("test5")
          ),
          miru::params::Parameter(
            "nested_array.1.0.scalar2", miru::params::Scalar("test6")
          )
        }}
      ),
      miru::params::Parameter(
        "nested_array.1.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "nested_array.1.1.scalar1", miru::params::Scalar("test7")
          ),
          miru::params::Parameter(
            "nested_array.1.1.scalar2", miru::params::Scalar("test8")
          )
        }}
      )
    }}
  );

  miru::params::Parameter nested_array_not_a_leaf = miru::params::Parameter(
    "nested_array", miru::params::NestedArray{std::vector<miru::params::Parameter>{
                      miru::params::Parameter("nested_array.0", map_array_item1),
                      miru::params::Parameter("nested_array.1", map_array_item2)
                    }}
  );

  EXPECT_FALSE(nested_array_not_a_leaf.is_null());
  EXPECT_FALSE(nested_array_not_a_leaf.is_scalar());
  EXPECT_FALSE(nested_array_not_a_leaf.is_scalar_array());
  EXPECT_TRUE(nested_array_not_a_leaf.is_nested_array());
  EXPECT_FALSE(nested_array_not_a_leaf.is_map());
  EXPECT_FALSE(nested_array_not_a_leaf.is_map_array());
  EXPECT_TRUE(nested_array_not_a_leaf.is_array());
}

TEST_F(ParameterConstructors, nested_array_variant_parent_name_mismatch) {
  EXPECT_THROW(
    miru::params::Parameter(
      "arglebargle",
      miru::params::NestedArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 9}),
        miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "too/deep/nested_array",
      miru::params::NestedArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 9}),
        miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "nested_array",
      miru::params::NestedArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter("too/deep/nested_array.0", std::vector<int>{1, 2, 9}),
        miru::params::Parameter("too/deep/nested_array.1", std::vector<int>{4, 5, 6})
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
}

TEST_F(ParameterConstructors, map_variant) {
  miru::params::ParameterValue map_value(
    miru::params::Map{std::vector<miru::params::Parameter>{
      miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
      miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
    }}
  );
  miru::params::Parameter map_variant1 = miru::params::Parameter("map", map_value);
  miru::params::Parameter map_variant2 = miru::params::Parameter(
    "map", miru::params::Map{std::vector<miru::params::Parameter>{
             miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
             miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
           }}
  );
  EXPECT_EQ(map_variant1, map_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter(
    "map", miru::params::Map{std::vector<miru::params::Parameter>{
             miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
             miru::params::Parameter("map.param2", miru::params::Scalar("test3"))
           }}
  );
  EXPECT_NE(map_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name = miru::params::Parameter(
    "arglebargle",
    miru::params::Map{std::vector<miru::params::Parameter>{
      miru::params::Parameter("arglebargle.param1", miru::params::Scalar("test1")),
      miru::params::Parameter("arglebargle.param2", miru::params::Scalar("test3"))
    }}
  );
  EXPECT_NE(map_variant1, diff_name);

  // get type
  EXPECT_EQ(map_variant1.get_type(), miru::params::ParameterType::PARAMETER_MAP);

  // get type name
  EXPECT_EQ(map_variant1.get_type_name(), "map");

  // get parameter value
  EXPECT_EQ(map_variant1.get_parameter_value(), map_value);

  // value to string
  EXPECT_EQ(
    map_variant1.value_to_string(),
    "{\n  \"param1\": \"test1\",\n  \"param2\": \"test2\"\n}"
  );

  EXPECT_FALSE(map_variant1.is_null());
  EXPECT_FALSE(map_variant1.is_scalar());
  EXPECT_FALSE(map_variant1.is_scalar_array());
  EXPECT_FALSE(map_variant1.is_nested_array());
  EXPECT_TRUE(map_variant1.is_map());
  EXPECT_FALSE(map_variant1.is_map_array());
  EXPECT_FALSE(map_variant1.is_array());
}

TEST_F(ParameterConstructors, map_variant_parent_name_mismatch) {
  EXPECT_THROW(
    miru::params::Parameter(
      "pam", miru::params::Map{std::vector<miru::params::Parameter>{
               miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
               miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
             }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "map",
      miru::params::Map{std::vector<miru::params::Parameter>{
        miru::params::Parameter("too.deep.map.param1", miru::params::Scalar("test1")),
        miru::params::Parameter("too.deep.map.param2", miru::params::Scalar("test2"))
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "too.deep.map",
      miru::params::Map{std::vector<miru::params::Parameter>{
        miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
        miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
}

TEST_F(ParameterConstructors, map_array_variant) {
  miru::params::ParameterValue map_array_value(
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "maps.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.0.param1", miru::params::Scalar("test1")),
          miru::params::Parameter("maps.0.param2", miru::params::Scalar("test2"))
        }}
      ),
      miru::params::Parameter(
        "maps.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.1.param1", miru::params::Scalar("test3")),
          miru::params::Parameter("maps.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );
  miru::params::Parameter map_array_variant1 =
    miru::params::Parameter("maps", map_array_value);
  miru::params::Parameter map_array_variant2 = miru::params::Parameter(
    "maps",
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "maps.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.0.param1", miru::params::Scalar("test1")),
          miru::params::Parameter("maps.0.param2", miru::params::Scalar("test2"))
        }}
      ),
      miru::params::Parameter(
        "maps.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.1.param1", miru::params::Scalar("test3")),
          miru::params::Parameter("maps.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );
  EXPECT_EQ(map_array_variant1, map_array_variant2);

  // value not equal
  miru::params::Parameter diff_value = miru::params::Parameter(
    "maps",
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "maps.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.0.param1", miru::params::Scalar("test1")),
          miru::params::Parameter("maps.0.param2", miru::params::Scalar("test3"))
        }}
      ),
      miru::params::Parameter(
        "maps.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.1.param1", miru::params::Scalar("test3")),
          miru::params::Parameter("maps.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );
  EXPECT_NE(map_array_variant1, diff_value);

  // name not equal
  miru::params::Parameter diff_name = miru::params::Parameter(
    "arglebargle",
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "arglebargle.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "arglebargle.0.param1", miru::params::Scalar("test1")
          ),
          miru::params::Parameter("arglebargle.0.param2", miru::params::Scalar("test3"))
        }}
      ),
      miru::params::Parameter(
        "arglebargle.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter(
            "arglebargle.1.param1", miru::params::Scalar("test3")
          ),
          miru::params::Parameter("arglebargle.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );
  EXPECT_NE(map_array_variant1, diff_name);

  // get type
  EXPECT_EQ(
    map_array_variant1.get_type(), miru::params::ParameterType::PARAMETER_MAP_ARRAY
  );

  // get type name
  EXPECT_EQ(map_array_variant1.get_type_name(), "map array");

  // get parameter value
  EXPECT_EQ(map_array_variant1.get_parameter_value(), map_array_value);

  // value to string
  EXPECT_EQ(
    map_array_variant1.value_to_string(),
    "[\n  {\n    \"param1\": \"test1\",\n    \"param2\": \"test2\"\n  },\n  "
    "{\n    \"param1\": \"test3\",\n    \"param2\": \"test4\"\n  }\n]"
  );

  EXPECT_FALSE(map_array_variant1.is_null());
  EXPECT_FALSE(map_array_variant1.is_scalar());
  EXPECT_FALSE(map_array_variant1.is_scalar_array());
  EXPECT_FALSE(map_array_variant1.is_nested_array());
  EXPECT_FALSE(map_array_variant1.is_map());
  EXPECT_TRUE(map_array_variant1.is_map_array());
  EXPECT_TRUE(map_array_variant1.is_array());
}

TEST_F(ParameterConstructors, map_array_variant_parent_name_mismatch) {
  EXPECT_THROW(
    miru::params::Parameter(
      "map",
      miru::params::MapArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter(
          "arglebargle.0", miru::params::Map{std::vector<miru::params::Parameter>{
                             miru::params::Parameter(
                               "arglebargle.0.param1", miru::params::Scalar("test1")
                             ),
                             miru::params::Parameter(
                               "arglebargle.0.param2", miru::params::Scalar("test3")
                             )
                           }}
        ),
        miru::params::Parameter(
          "arglebargle.1", miru::params::Map{std::vector<miru::params::Parameter>{
                             miru::params::Parameter(
                               "arglebargle.1.param1", miru::params::Scalar("test3")
                             ),
                             miru::params::Parameter(
                               "arglebargle.1.param2", miru::params::Scalar("test4")
                             )
                           }}
        )
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "map",
      miru::params::MapArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter(
          "too.deep.map.0", miru::params::Map{std::vector<miru::params::Parameter>{
                              miru::params::Parameter(
                                "too/deep/map.0.param1", miru::params::Scalar("test1")
                              ),
                              miru::params::Parameter(
                                "too/deep/map.0.param2", miru::params::Scalar("test3")
                              )
                            }}
        ),
        miru::params::Parameter(
          "too.deep.map.1", miru::params::Map{std::vector<miru::params::Parameter>{
                              miru::params::Parameter(
                                "too.deep.map.1.param1", miru::params::Scalar("test3")
                              ),
                              miru::params::Parameter(
                                "too.deep.map.1.param2", miru::params::Scalar("test4")
                              )
                            }}
        )
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
  EXPECT_THROW(
    miru::params::Parameter(
      "too.deep.map",
      miru::params::MapArray{std::vector<miru::params::Parameter>{
        miru::params::Parameter(
          "map.0",
          miru::params::Map{std::vector<miru::params::Parameter>{
            miru::params::Parameter("map.0.param1", miru::params::Scalar("test1")),
            miru::params::Parameter("map.0.param2", miru::params::Scalar("test3"))
          }}
        ),
        miru::params::Parameter(
          "too.deep.map.1",
          miru::params::Map{std::vector<miru::params::Parameter>{
            miru::params::Parameter("map.1.param1", miru::params::Scalar("test3")),
            miru::params::Parameter("map.1.param2", miru::params::Scalar("test4"))
          }}
        )
      }}
    ),
    miru::params::ChildParentNameMismatchError
  );
}

// ================================= NAME + KEY ==================================== //
struct KeyTestCase {
  std::string name;
  std::string key;
};

class ParameterKey : public ::testing::Test {
 protected:
  std::vector<KeyTestCase> test_cases = {
    {"", ""},           {"l1", "l1"},       {"l1.l2", "l2"},
    {"l1/l2", "l1/l2"}, {"l1/l2.l3", "l3"}, {"l1/l2.l3/l4", "l3/l4"},
  };
};

TEST_F(ParameterKey, names) {
  for (const auto &test_case : test_cases) {
    miru::params::Parameter param(test_case.name, miru::params::ParameterValue());
    EXPECT_EQ(param.get_name(), test_case.name);
  }
}

TEST_F(ParameterKey, keys) {
  for (const auto &test_case : test_cases) {
    miru::params::Parameter param(test_case.name, miru::params::ParameterValue());
    EXPECT_EQ(param.get_key(), test_case.key);
  }
}

// ================================= CONVERSIONS =================================== //
void test_conversion_funcs_throw_type_exceptions(
  const miru::params::Parameter &param,
  std::vector<std::function<void()>> conversion_funcs,
  const miru::params::ParameterType target_type
) {
  for (const auto &conversion_func : conversion_funcs) {
    EXPECT_THROW(conversion_func(), miru::params::InvalidParameterTypeError)
      << "Failed to throw InvalidParameterTypeError for target parameter type '"
      << target_type << "'";
  }
}

void test_conversion_type_exceptions(
  const miru::params::Parameter &param,
  const std::vector<miru::params::ParameterType> &exclude
) {
  // boolean
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_BOOL
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {
        [&]() { param.as_bool(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_BOOL>(); },
        [&]() { param.get_value<bool>(); },
        [&]() { param.as<miru::params::ParameterType::PARAMETER_BOOL>(); },
        [&]() { param.as<bool>(); },
      },
      miru::params::ParameterType::PARAMETER_BOOL
    );
  }

  // integer
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_INTEGER
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {
        [&]() { param.as_int(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_INTEGER>(); },
        [&]() { param.get_value<int>(); },
        [&]() { param.as<miru::params::ParameterType::PARAMETER_INTEGER>(); },
        [&]() { param.as<int>(); },
      },
      miru::params::ParameterType::PARAMETER_INTEGER
    );
  }

  // double
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_DOUBLE
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_double(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE>(); },
       [&]() { param.get_value<double>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_DOUBLE>(); },
       [&]() { param.as<double>(); }},
      miru::params::ParameterType::PARAMETER_DOUBLE
    );
  }

  // string
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_STRING
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_string(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_STRING>(); },
       [&]() { param.get_value<std::string>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_STRING>(); },
       [&]() { param.as<std::string>(); }},
      miru::params::ParameterType::PARAMETER_STRING
    );
  }

  // bool array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_BOOL_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_bool_array(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>(); },
       [&]() { param.get_value<std::vector<bool>>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>(); },
       [&]() { param.as<std::vector<bool>>(); }},
      miru::params::ParameterType::PARAMETER_BOOL_ARRAY
    );
  }

  // integer array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_INTEGER_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_integer_array(); },
       [&]() {
         param.get_value<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>();
       },
       [&]() { param.get_value<std::vector<int64_t>>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>(); },
       [&]() { param.as<std::vector<int64_t>>(); }},
      miru::params::ParameterType::PARAMETER_INTEGER_ARRAY
    );
  }

  // double array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_double_array(); },
       [&]() {
         param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>();
       },
       [&]() { param.get_value<std::vector<double>>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>(); },
       [&]() { param.as<std::vector<double>>(); }},
      miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY
    );
  }

  // string array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_STRING_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_string_array(); },
       [&]() {
         param.get_value<miru::params::ParameterType::PARAMETER_STRING_ARRAY>();
       },
       [&]() { param.get_value<std::vector<std::string>>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_STRING_ARRAY>(); },
       [&]() { param.as<std::vector<std::string>>(); }},
      miru::params::ParameterType::PARAMETER_STRING_ARRAY
    );
  }

  // null
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_NULL
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_null(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_NULL>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_NULL>(); }},
      miru::params::ParameterType::PARAMETER_NULL
    );
  }

  // scalar
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_SCALAR
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_scalar(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_SCALAR>(); },
       [&]() { param.get_value<miru::params::Scalar>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_SCALAR>(); },
       [&]() { param.as<miru::params::Scalar>(); }},
      miru::params::ParameterType::PARAMETER_SCALAR
    );
  }

  // scalar array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_SCALAR_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_scalar_array(); },
       [&]() {
         param.get_value<miru::params::ParameterType::PARAMETER_SCALAR_ARRAY>();
       },
       [&]() { param.get_value<std::vector<miru::params::Scalar>>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_SCALAR_ARRAY>(); },
       [&]() { param.as<std::vector<miru::params::Scalar>>(); }},
      miru::params::ParameterType::PARAMETER_SCALAR_ARRAY
    );
  }

  // nested array
  if (std::find(
        exclude.begin(), exclude.end(),
        miru::params::ParameterType::PARAMETER_NESTED_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_nested_array(); },
       [&]() {
         param.get_value<miru::params::ParameterType::PARAMETER_NESTED_ARRAY>();
       },
       [&]() { param.get_value<miru::params::NestedArray>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_NESTED_ARRAY>(); },
       [&]() { param.as<miru::params::NestedArray>(); }},
      miru::params::ParameterType::PARAMETER_NESTED_ARRAY
    );
  }

  // map
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_MAP
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_map(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_MAP>(); },
       [&]() { param.get_value<miru::params::Map>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_MAP>(); },
       [&]() { param.as<miru::params::Map>(); }},
      miru::params::ParameterType::PARAMETER_MAP
    );
  }

  // map array
  if (std::find(
        exclude.begin(), exclude.end(), miru::params::ParameterType::PARAMETER_MAP_ARRAY
      ) == exclude.end()) {
    test_conversion_funcs_throw_type_exceptions(
      param,
      {[&]() { param.as_map_array(); },
       [&]() { param.get_value<miru::params::ParameterType::PARAMETER_MAP_ARRAY>(); },
       [&]() { param.get_value<miru::params::MapArray>(); },
       [&]() { param.as<miru::params::ParameterType::PARAMETER_MAP_ARRAY>(); },
       [&]() { param.as<miru::params::MapArray>(); }},
      miru::params::ParameterType::PARAMETER_MAP_ARRAY
    );
  }
}

class ParameterInvalidTypeConversions : public ::testing::Test {
 protected:
};

TEST_F(ParameterInvalidTypeConversions, not_set_variant) {
  miru::params::Parameter not_set_variant = miru::params::Parameter();
  test_conversion_type_exceptions(not_set_variant, {});
}

TEST_F(ParameterInvalidTypeConversions, bool_variant) {
  miru::params::Parameter bool_variant = miru::params::Parameter("bool_param", true);
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_BOOL,
  };
  test_conversion_type_exceptions(bool_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, int_variant) {
  miru::params::Parameter int_variant = miru::params::Parameter("int_param", 4);
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_INTEGER,
  };
  test_conversion_type_exceptions(int_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, double_variant) {
  miru::params::Parameter double_variant = miru::params::Parameter("double_param", 4.0);
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_DOUBLE,
  };
  test_conversion_type_exceptions(double_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, string_variant) {
  miru::params::Parameter string_variant =
    miru::params::Parameter("string_param", "test");
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_STRING,
  };
  test_conversion_type_exceptions(string_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, bool_array_variant) {
  miru::params::Parameter bool_array_variant =
    miru::params::Parameter("bool_array_param", std::vector<bool>{true, false, true});
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_BOOL_ARRAY,
  };
  test_conversion_type_exceptions(bool_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, int_array_variant) {
  miru::params::Parameter int_array_variant =
    miru::params::Parameter("int_array_param", std::vector<int>{1, 2, 3});
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_INTEGER_ARRAY,
  };
  test_conversion_type_exceptions(int_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, double_array_variant) {
  miru::params::Parameter double_array_variant =
    miru::params::Parameter("double_array_param", std::vector<double>{1.0, 2.0, 3.0});
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
  };
  test_conversion_type_exceptions(double_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, string_array_variant) {
  miru::params::Parameter string_array_variant = miru::params::Parameter(
    "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_STRING_ARRAY,
  };
  test_conversion_type_exceptions(string_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, null_variant) {
  miru::params::Parameter null_variant = miru::params::Parameter("null_param", nullptr);
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_NULL,
  };
  test_conversion_type_exceptions(null_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_bool_variant) {
  miru::params::Parameter scalar_variant =
    miru::params::Parameter("scalar_param", miru::params::Scalar("true"));
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR,
    miru::params::ParameterType::PARAMETER_STRING,
    miru::params::ParameterType::PARAMETER_BOOL,
  };
  test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_int_variant) {
  miru::params::Parameter scalar_variant =
    miru::params::Parameter("scalar_param", miru::params::Scalar("4"));
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR,
    miru::params::ParameterType::PARAMETER_STRING,
    miru::params::ParameterType::PARAMETER_INTEGER,
    miru::params::ParameterType::PARAMETER_DOUBLE,
  };
  test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_double_variant) {
  miru::params::Parameter scalar_variant =
    miru::params::Parameter("scalar_param", miru::params::Scalar("1.1"));
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR,
    miru::params::ParameterType::PARAMETER_STRING,
    miru::params::ParameterType::PARAMETER_DOUBLE,
  };
  test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_string_variant) {
  miru::params::Parameter scalar_variant =
    miru::params::Parameter("scalar_param", miru::params::Scalar("test"));
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR,
    miru::params::ParameterType::PARAMETER_STRING,
  };
  test_conversion_type_exceptions(scalar_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_boolean_variant) {
  miru::params::Parameter scalar_array_variant = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("true"), miru::params::Scalar("false"),
      miru::params::Scalar("true")
    }
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
    miru::params::ParameterType::PARAMETER_BOOL_ARRAY,
    miru::params::ParameterType::PARAMETER_STRING_ARRAY,
  };
  test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_integer_variant) {
  miru::params::Parameter scalar_array_variant = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("1"), miru::params::Scalar("2"), miru::params::Scalar("3")
    }
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
    miru::params::ParameterType::PARAMETER_INTEGER_ARRAY,
    miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
    miru::params::ParameterType::PARAMETER_STRING_ARRAY,
  };
  test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_double_variant) {
  miru::params::Parameter scalar_array_variant = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("1.1"), miru::params::Scalar("2.2"),
      miru::params::Scalar("3.3")
    }
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
    miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY,
    miru::params::ParameterType::PARAMETER_STRING_ARRAY,
  };
  test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, scalar_array_string_variant) {
  miru::params::Parameter scalar_array_variant = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("test1"), miru::params::Scalar("test2"),
      miru::params::Scalar("test3")
    }
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_SCALAR_ARRAY,
    miru::params::ParameterType::PARAMETER_STRING_ARRAY,
  };
  test_conversion_type_exceptions(scalar_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, nested_array_variant) {
  miru::params::Parameter nested_array_variant = miru::params::Parameter(
    "nested_array",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 3}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_NESTED_ARRAY,
  };
  test_conversion_type_exceptions(nested_array_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, map_variant) {
  miru::params::Parameter map_variant = miru::params::Parameter(
    "map", miru::params::Map{std::vector<miru::params::Parameter>{
             miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
             miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
           }}
  );

  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_MAP,
  };
  test_conversion_type_exceptions(map_variant, exclude);
}

TEST_F(ParameterInvalidTypeConversions, map_array_variant) {
  miru::params::Parameter map_array_variant = miru::params::Parameter(
    "maps",
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "maps.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.0.param1", miru::params::Scalar("test1")),
          miru::params::Parameter("maps.0.param2", miru::params::Scalar("test2"))
        }}
      ),
      miru::params::Parameter(
        "maps.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.1.param1", miru::params::Scalar("test3")),
          miru::params::Parameter("maps.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );
  std::vector<miru::params::ParameterType> exclude = {
    miru::params::ParameterType::PARAMETER_MAP_ARRAY,
  };
  test_conversion_type_exceptions(map_array_variant, exclude);
}

// ================================= TYPE CASTING ================================== //
class IntegerCasting : public test::utils::IntegerCasting {};

TEST_F(IntegerCasting, integer_type_casting_success) {
  for (const auto &test_case : all_test_cases()) {
    if (test_case.expect_failure) {
      continue;
    }
    miru::params::Parameter integer_variant =
      miru::params::Parameter("integer_param", test_case.input);
    std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_EQ(integer_variant.get_value<T>(), std::get<T>(test_case.expected));
      },
      test_case.expected
    );
  }
}

TEST_F(IntegerCasting, integer_type_casting_failure) {
  for (const auto &test_case : all_test_cases()) {
    if (!test_case.expect_failure) {
      continue;
    }
    miru::params::Parameter integer_variant =
      miru::params::Parameter("integer_param", test_case.input);
    std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_THROW(
          integer_variant.get_value<T>(), miru::params::InvalidParameterTypeError
        );
      },
      test_case.expected
    );
  }
}

// casting integer arrays to different types is not currently supported (only
// std::vector<int64_t> is supported)

class DoubleCasting : public test::utils::DoubleCasting {};

TEST_F(DoubleCasting, double_type_casting_success) {
  for (const auto &test_case : double_test_cases) {
    if (test_case.expect_failure) {
      continue;
    }
    miru::params::Parameter double_variant =
      miru::params::Parameter("double_param", test_case.input);
    std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_EQ(double_variant.get_value<T>(), std::get<T>(test_case.expected));
      },
      test_case.expected
    );
  }
}

TEST_F(DoubleCasting, double_type_casting_failure) {
  for (const auto &test_case : double_test_cases) {
    if (!test_case.expect_failure) {
      continue;
    }
    miru::params::Parameter double_variant =
      miru::params::Parameter("double_param", test_case.input);
    std::visit(
      [&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_THROW(
          double_variant.get_value<T>(), miru::params::InvalidParameterTypeError
        );
      },
      test_case.expected
    );
  }
}

// casting double arrays to different types is not currently supported (only
// std::vector<double> is supported)

class ParameterValidTypeConversions : public ::testing::Test {
 protected:
};

TEST_F(ParameterValidTypeConversions, bool_type_coversion_success) {
  std::function<std::vector<std::function<bool()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<bool()>>{
        [&]() { return param.as_bool(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_BOOL>();
        },
        [&]() { return param.get_value<bool>(); },
      };
    };

  miru::params::Parameter true_variant = miru::params::Parameter("bool_param", true);
  miru::params::Parameter false_variant = miru::params::Parameter("bool_param", false);

  for (const auto &variant : {true_variant, false_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<bool>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_bool_type_coversion_success) {
  std::function<std::vector<std::function<bool()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<bool()>>{
        [&]() { return param.as_bool(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_BOOL>();
        },
        [&]() { return param.get_value<bool>(); },
      };
    };

  miru::params::Parameter true_variant =
    miru::params::Parameter("bool_param", miru::params::Scalar("true"));
  miru::params::Parameter false_variant =
    miru::params::Parameter("bool_param", miru::params::Scalar("false"));

  for (const auto &variant : {true_variant, false_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<bool>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, int_type_conversion_success) {
  std::function<std::vector<std::function<int64_t()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<int64_t()>>{
        [&]() { return param.as_int(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_INTEGER>();
        },
        [&]() { return param.get_value<int64_t>(); },
      };
    };

  miru::params::Parameter positive_int = miru::params::Parameter("int_param", 1);
  miru::params::Parameter negative_int = miru::params::Parameter("int_param", -1);

  for (const auto &variant : {positive_int, negative_int}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<int64_t>());
    }
  }

  // int8
  miru::params::Parameter int8 = miru::params::Parameter("int_param", 127);
  EXPECT_EQ(int8.get_value<int8_t>(), int8_t(127));
  miru::params::Parameter int8_negative = miru::params::Parameter("int_param", -128);
  EXPECT_EQ(int8_negative.get_value<int8_t>(), int8_t(-128));

  // int16
  miru::params::Parameter int16 = miru::params::Parameter("int_param", 32767);
  EXPECT_EQ(int16.get_value<int16_t>(), int16_t(32767));
  miru::params::Parameter int16_negative = miru::params::Parameter("int_param", -32768);
  EXPECT_EQ(int16_negative.get_value<int16_t>(), int16_t(-32768));

  // int32
  miru::params::Parameter int32 = miru::params::Parameter("int_param", 2147483647);
  EXPECT_EQ(int32.get_value<int32_t>(), 2147483647);
  miru::params::Parameter int32_negative =
    miru::params::Parameter("int_param", -2147483648);
  EXPECT_EQ(int32_negative.get_value<int32_t>(), int32_t(-2147483648));

  // int64
  miru::params::Parameter int64 =
    miru::params::Parameter("int_param", 9223372036854775807);
  EXPECT_EQ(int64.get_value<int64_t>(), int64_t(9223372036854775807));
  miru::params::Parameter int64_negative =
    miru::params::Parameter("int_param", -9223372036854775807);
  EXPECT_EQ(int64_negative.get_value<int64_t>(), int64_t(-9223372036854775807));

  // uint8
  miru::params::Parameter uint8 = miru::params::Parameter("int_param", 255);
  EXPECT_EQ(uint8.get_value<uint8_t>(), uint8_t(255));
  miru::params::Parameter uint8_negative = miru::params::Parameter("int_param", 0);
  EXPECT_EQ(uint8_negative.get_value<uint8_t>(), uint8_t(0));

  // uint16
  miru::params::Parameter uint16 = miru::params::Parameter("int_param", 65535);
  EXPECT_EQ(uint16.get_value<uint16_t>(), uint16_t(65535));
  miru::params::Parameter uint16_negative = miru::params::Parameter("int_param", 0);
  EXPECT_EQ(uint16_negative.get_value<uint16_t>(), uint16_t(0));

  // uint32
  miru::params::Parameter uint32 = miru::params::Parameter("int_param", 4294967295);
  EXPECT_EQ(uint32.get_value<uint32_t>(), uint32_t(4294967295));
  miru::params::Parameter uint32_negative = miru::params::Parameter("int_param", 0);
  EXPECT_EQ(uint32_negative.get_value<uint32_t>(), uint32_t(0));

  // uint64
  miru::params::Parameter uint64 =
    miru::params::Parameter("int_param", 9223372036854775807);
  EXPECT_EQ(uint64.get_value<uint64_t>(), uint64_t(9223372036854775807));
  miru::params::Parameter uint64_negative = miru::params::Parameter("int_param", 0);
  EXPECT_EQ(uint64_negative.get_value<uint64_t>(), uint64_t(0));
};

TEST_F(ParameterValidTypeConversions, scalar_int_type_conversion_success) {
  std::function<std::vector<std::function<int64_t()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<int64_t()>>{
        [&]() { return param.as_int(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_INTEGER>();
        },
        [&]() { return param.get_value<int64_t>(); },
      };
    };

  miru::params::Parameter positive_int =
    miru::params::Parameter("int_param", miru::params::Scalar("1"));
  miru::params::Parameter negative_int =
    miru::params::Parameter("int_param", miru::params::Scalar("-1"));

  for (const auto &variant : {positive_int, negative_int}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<int64_t>());
    }
  }

  // int8
  miru::params::Parameter int8 =
    miru::params::Parameter("int_param", miru::params::Scalar("127"));
  EXPECT_EQ(int8.get_value<int8_t>(), int8_t(127));
  miru::params::Parameter int8_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("-128"));
  EXPECT_EQ(int8_negative.get_value<int8_t>(), int8_t(-128));

  // int16
  miru::params::Parameter int16 =
    miru::params::Parameter("int_param", miru::params::Scalar("32767"));
  EXPECT_EQ(int16.get_value<int16_t>(), int16_t(32767));
  miru::params::Parameter int16_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("-32768"));
  EXPECT_EQ(int16_negative.get_value<int16_t>(), int16_t(-32768));

  // int32
  miru::params::Parameter int32 =
    miru::params::Parameter("int_param", miru::params::Scalar("2147483647"));
  EXPECT_EQ(int32.get_value<int32_t>(), 2147483647);
  miru::params::Parameter int32_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("-2147483648"));
  EXPECT_EQ(int32_negative.get_value<int32_t>(), int32_t(-2147483648));

  // int64
  miru::params::Parameter int64 =
    miru::params::Parameter("int_param", miru::params::Scalar("9223372036854775807"));
  EXPECT_EQ(int64.get_value<int64_t>(), int64_t(9223372036854775807));
  miru::params::Parameter int64_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("-9223372036854775807"));
  EXPECT_EQ(int64_negative.get_value<int64_t>(), int64_t(-9223372036854775807));

  // uint8
  miru::params::Parameter uint8 =
    miru::params::Parameter("int_param", miru::params::Scalar("255"));
  EXPECT_EQ(uint8.get_value<uint8_t>(), uint8_t(255));
  miru::params::Parameter uint8_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("0"));
  EXPECT_EQ(uint8_negative.get_value<uint8_t>(), uint8_t(0));

  // uint16
  miru::params::Parameter uint16 =
    miru::params::Parameter("int_param", miru::params::Scalar("65535"));
  EXPECT_EQ(uint16.get_value<uint16_t>(), uint16_t(65535));
  miru::params::Parameter uint16_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("0"));
  EXPECT_EQ(uint16_negative.get_value<uint16_t>(), uint16_t(0));

  // uint32
  miru::params::Parameter uint32 =
    miru::params::Parameter("int_param", miru::params::Scalar("4294967295"));
  EXPECT_EQ(uint32.get_value<uint32_t>(), uint32_t(4294967295));
  miru::params::Parameter uint32_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("0"));
  EXPECT_EQ(uint32_negative.get_value<uint32_t>(), uint32_t(0));

  // uint64
  miru::params::Parameter uint64 =
    miru::params::Parameter("int_param", miru::params::Scalar("9223372036854775807"));
  EXPECT_EQ(uint64.get_value<uint64_t>(), uint64_t(9223372036854775807));
  miru::params::Parameter uint64_negative =
    miru::params::Parameter("int_param", miru::params::Scalar("0"));
  EXPECT_EQ(uint64_negative.get_value<uint64_t>(), uint64_t(0));
};

TEST_F(ParameterValidTypeConversions, double_type_conversion_success) {
  std::function<std::vector<std::function<double()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<double()>>{
        [&]() { return param.as_double(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE>();
        },
        [&]() { return param.get_value<double>(); },
      };
    };

  miru::params::Parameter positive_double =
    miru::params::Parameter("double_param", 1.1);
  miru::params::Parameter negative_double =
    miru::params::Parameter("double_param", -1.1);

  for (const auto &variant : {positive_double, negative_double}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<double>());
    }
  }

  // float
  miru::params::Parameter float_variant = miru::params::Parameter("double_param", 1.1f);
  EXPECT_EQ(float_variant.get_value<float>(), 1.1f);
  miru::params::Parameter float_negative_variant =
    miru::params::Parameter("double_param", -1.1f);
  EXPECT_EQ(float_negative_variant.get_value<float>(), -1.1f);

  // double
  miru::params::Parameter double_variant = miru::params::Parameter("double_param", 1.1);
  EXPECT_EQ(double_variant.get_value<double>(), 1.1);
  miru::params::Parameter double_negative_variant =
    miru::params::Parameter("double_param", -1.1);
  EXPECT_EQ(double_negative_variant.get_value<double>(), -1.1);
}

TEST_F(ParameterValidTypeConversions, scalar_double_type_conversion_success) {
  std::function<std::vector<std::function<double()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<double()>>{
        [&]() { return param.as_double(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE>();
        },
        [&]() { return param.get_value<double>(); },
      };
    };

  miru::params::Parameter positive_double =
    miru::params::Parameter("double_param", miru::params::Scalar("1.1"));
  miru::params::Parameter negative_double =
    miru::params::Parameter("double_param", miru::params::Scalar("-1.1"));

  for (const auto &variant : {positive_double, negative_double}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<double>());
    }
  }

  // float
  miru::params::Parameter float_variant =
    miru::params::Parameter("double_param", miru::params::Scalar("1.1"));
  EXPECT_EQ(float_variant.get_value<float>(), 1.1f);
  miru::params::Parameter float_negative_variant =
    miru::params::Parameter("double_param", miru::params::Scalar("-1.1"));
  EXPECT_EQ(float_negative_variant.get_value<float>(), -1.1f);

  // double
  miru::params::Parameter double_variant =
    miru::params::Parameter("double_param", miru::params::Scalar("1.1"));
  EXPECT_EQ(double_variant.get_value<double>(), 1.1);
  miru::params::Parameter double_negative_variant =
    miru::params::Parameter("double_param", miru::params::Scalar("-1.1"));
  EXPECT_EQ(double_negative_variant.get_value<double>(), -1.1);
}

TEST_F(ParameterValidTypeConversions, string_type_conversion_success) {
  std::function<
    std::vector<std::function<std::string()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::string()>>{
        [&]() { return param.as_string(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_STRING>();
        },
        [&]() { return param.get_value<std::string>(); },
      };
    };

  miru::params::Parameter string_variant =
    miru::params::Parameter("string_param", "test");

  for (const auto &variant : {string_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::string>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_string_type_conversion_success) {
  std::function<
    std::vector<std::function<std::string()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::string()>>{
        [&]() { return param.as_string(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_STRING>();
        },
        [&]() { return param.get_value<std::string>(); },
      };
    };

  miru::params::Parameter string_variant =
    miru::params::Parameter("string_param", miru::params::Scalar("test"));

  for (const auto &variant : {string_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::string>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, bool_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<bool>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<bool>()>>{
        [&]() { return param.as_bool_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<bool>>(); },
      };
    };

  miru::params::Parameter bool_array_variant =
    miru::params::Parameter("bool_array_param", std::vector<bool>{true, false, true});

  for (const auto &variant : {bool_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<bool>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_bool_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<bool>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<bool>()>>{
        [&]() { return param.as_bool_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<bool>>(); },
      };
    };

  miru::params::Parameter bool_array_variant = miru::params::Parameter(
    "bool_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("true"), miru::params::Scalar("false"),
      miru::params::Scalar("true")
    }
  );

  for (const auto &variant : {bool_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<bool>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, int_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<int64_t>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param
                             ) -> std::vector<std::function<std::vector<int64_t>()>> {
    return std::vector<std::function<std::vector<int64_t>()>>{
      [&]() -> std::vector<int64_t> { return param.as_integer_array(); },
      [&]() -> std::vector<int64_t> {
        return param.get_value<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>();
      },
      [&]() -> std::vector<int64_t> { return param.get_value<std::vector<int64_t>>(); },
    };
  };

  miru::params::Parameter int_array_variant =
    miru::params::Parameter("int_array_param", std::vector<int64_t>{1, 2, 3});

  for (const auto &variant : {int_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<int64_t>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_int_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<int64_t>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param
                             ) -> std::vector<std::function<std::vector<int64_t>()>> {
    return std::vector<std::function<std::vector<int64_t>()>>{
      [&]() -> std::vector<int64_t> { return param.as_integer_array(); },
      [&]() -> std::vector<int64_t> {
        return param.get_value<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>();
      },
      [&]() -> std::vector<int64_t> { return param.get_value<std::vector<int64_t>>(); },
    };
  };

  miru::params::Parameter int_array_variant = miru::params::Parameter(
    "int_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("1"), miru::params::Scalar("2"), miru::params::Scalar("3")
    }
  );

  for (const auto &variant : {int_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<int64_t>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, double_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<double>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<double>()>>{
        [&]() { return param.as_double_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<double>>(); },
      };
    };

  miru::params::Parameter double_array_variant =
    miru::params::Parameter("double_array_param", std::vector<double>{1.0, 2.0, 3.0});

  for (const auto &variant : {double_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<double>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_double_array_type_conversion_success) {
  std::function<
    std::vector<std::function<std::vector<double>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<double>()>>{
        [&]() { return param.as_double_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<double>>(); },
      };
    };

  miru::params::Parameter double_array_variant = miru::params::Parameter(
    "double_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("1.0"), miru::params::Scalar("2.0"),
      miru::params::Scalar("3.0")
    }
  );

  for (const auto &variant : {double_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<double>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, string_array_type_conversion_success) {
  std::function<std::vector<
    std::function<std::vector<std::string>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<std::string>()>>{
        [&]() { return param.as_string_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_STRING_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<std::string>>(); },
      };
    };

  miru::params::Parameter string_array_variant = miru::params::Parameter(
    "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
  );

  for (const auto &variant : {string_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<std::string>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, scalar_string_array_type_conversion_success) {
  std::function<std::vector<
    std::function<std::vector<std::string>()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<std::vector<std::string>()>>{
        [&]() { return param.as_string_array(); },
        [&]() {
          return param.get_value<miru::params::ParameterType::PARAMETER_STRING_ARRAY>();
        },
        [&]() { return param.get_value<std::vector<std::string>>(); },
      };
    };

  miru::params::Parameter string_array_variant = miru::params::Parameter(
    "string_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("test1"), miru::params::Scalar("test2"),
      miru::params::Scalar("test3")
    }
  );

  for (const auto &variant : {string_array_variant}) {
    for (const auto &conversion_func : build_conversion_funcs(variant)) {
      EXPECT_EQ(conversion_func(), variant.get_value<std::vector<std::string>>());
    }
  }
}

TEST_F(ParameterValidTypeConversions, null_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_null(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_NULL>(); },
      };
    };

  miru::params::Parameter null_variant = miru::params::Parameter("null_param", nullptr);

  for (const auto &conversion_func : build_conversion_funcs(null_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

TEST_F(ParameterValidTypeConversions, scalar_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_scalar(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_SCALAR>(); },
        [&]() { param.get_value<miru::params::Scalar>(); },
      };
    };

  miru::params::Parameter scalar_variant =
    miru::params::Parameter("scalar_param", miru::params::Scalar("test"));

  for (const auto &conversion_func : build_conversion_funcs(scalar_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

TEST_F(ParameterValidTypeConversions, scalar_array_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_scalar_array(); },
        [&]() {
          param.get_value<miru::params::ParameterType::PARAMETER_SCALAR_ARRAY>();
        },
        [&]() { param.get_value<std::vector<miru::params::Scalar>>(); },
      };
    };

  miru::params::Parameter scalar_array_variant = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("test1"), miru::params::Scalar("test2"),
      miru::params::Scalar("test3")
    }
  );

  for (const auto &conversion_func : build_conversion_funcs(scalar_array_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

TEST_F(ParameterValidTypeConversions, nested_array_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_nested_array(); },
        [&]() {
          param.get_value<miru::params::ParameterType::PARAMETER_NESTED_ARRAY>();
        },
        [&]() { param.get_value<miru::params::NestedArray>(); },
      };
    };

  miru::params::Parameter nested_array_variant = miru::params::Parameter(
    "nested_array",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 3}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );

  for (const auto &conversion_func : build_conversion_funcs(nested_array_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

TEST_F(ParameterValidTypeConversions, map_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_map(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_MAP>(); },
        [&]() { param.get_value<miru::params::Map>(); },
      };
    };

  miru::params::Parameter map_variant = miru::params::Parameter(
    "map", miru::params::Map{std::vector<miru::params::Parameter>{
             miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
             miru::params::Parameter("map.param2", miru::params::Scalar("test2"))
           }}
  );

  for (const auto &conversion_func : build_conversion_funcs(map_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

TEST_F(ParameterValidTypeConversions, map_array_type_conversion_success) {
  std::function<std::vector<std::function<void()>>(const miru::params::Parameter &)>
    build_conversion_funcs = [](const miru::params::Parameter &param) {
      return std::vector<std::function<void()>>{
        [&]() { param.as_map_array(); },
        [&]() { param.get_value<miru::params::ParameterType::PARAMETER_MAP_ARRAY>(); },
        [&]() { param.get_value<miru::params::MapArray>(); },
      };
    };

  miru::params::Parameter map_array_variant = miru::params::Parameter(
    "maps",
    miru::params::MapArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter(
        "maps.0",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.0.param1", miru::params::Scalar("test1")),
          miru::params::Parameter("maps.0.param2", miru::params::Scalar("test2"))
        }}
      ),
      miru::params::Parameter(
        "maps.1",
        miru::params::Map{std::vector<miru::params::Parameter>{
          miru::params::Parameter("maps.1.param1", miru::params::Scalar("test3")),
          miru::params::Parameter("maps.1.param2", miru::params::Scalar("test4"))
        }}
      )
    }}
  );

  for (const auto &conversion_func : build_conversion_funcs(map_array_variant)) {
    EXPECT_NO_THROW(conversion_func());
  }
}

}  // namespace test::params