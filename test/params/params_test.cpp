// These tests were originally taken from the rclcpp parameter tests:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/test/rclcpp/test_parameter.cpp 
// and modified to work with miru::params::Parameter

// std
#include <string>

// internal
#include "miru/params/parameter.hpp"

// external
#include <gtest/gtest.h>

// =================================== UTILS ======================================= //
void test_conversion_exceptions(const miru::params::Parameter & param) {
    // boolean
    EXPECT_THROW(param.as_bool(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_BOOL>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<bool>(),
        miru::params::InvalidParameterTypeException);

    // integer
    EXPECT_THROW(param.as_int(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_INTEGER>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<int>(),
        miru::params::InvalidParameterTypeException);

    // double
    EXPECT_THROW(param.as_double(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<double>(),
        miru::params::InvalidParameterTypeException);

    // string
    EXPECT_THROW(param.as_string(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_STRING>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::string>(),
        miru::params::InvalidParameterTypeException);

    // bool array
    EXPECT_THROW(param.as_bool_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_BOOL_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::vector<bool>>(),
        miru::params::InvalidParameterTypeException);

    // integer array
    EXPECT_THROW(param.as_integer_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_INTEGER_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::vector<int>>(),
        miru::params::InvalidParameterTypeException);

    // double array
    EXPECT_THROW(param.as_double_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::vector<double>>(),
        miru::params::InvalidParameterTypeException);

    // string array
    EXPECT_THROW(param.as_string_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_STRING_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::vector<std::string>>(),
        miru::params::InvalidParameterTypeException);

    // null
    EXPECT_THROW(param.as_null(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_NULL>(),
        miru::params::ParameterTypeException);

    // scalar
    EXPECT_THROW(param.as_scalar(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_SCALAR>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::Scalar>(),
        miru::params::InvalidParameterTypeException);

    // scalar array
    EXPECT_THROW(param.as_scalar_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_SCALAR_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<std::vector<miru::params::Scalar>>(),
        miru::params::InvalidParameterTypeException);

    // nested array
    EXPECT_THROW(param.as_nested_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_NESTED_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::NestedArray>(),
        miru::params::InvalidParameterTypeException);

    // object
    EXPECT_THROW(param.as_object(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_OBJECT>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::Object>(),
        miru::params::InvalidParameterTypeException);

    // object array
    EXPECT_THROW(param.as_object_array(), miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ParameterType::PARAMETER_OBJECT_ARRAY>(),
        miru::params::ParameterTypeException);
    EXPECT_THROW(
        param.get_value<miru::params::ObjectArray>(),
        miru::params::InvalidParameterTypeException);

}


// ================================= CONVERSIONS =================================== //
class ParameterConversions : public ::testing::Test {
protected:
};

TEST_F(ParameterConversions, not_set_variant) {
    miru::params::Parameter not_set_variant1 = miru::params::Parameter();
    miru::params::Parameter not_set_variant2 = miru::params::Parameter(
        "not_set_param"
    );
    test_conversion_exceptions(not_set_variant1);
    test_conversion_exceptions(not_set_variant2);
}

TEST_F(ParameterConversions, bool_variant) {
    miru::params::ParameterValue bool_value(true);
    miru::params::Parameter bool_variant2 = miru::params::Parameter(
        "bool_param", bool_value
    );
    miru::params::Parameter bool_variant1 = miru::params::Parameter(
        "bool_param", true
    );
    test_conversion_exceptions(bool_variant1);
    test_conversion_exceptions(bool_variant2);
}

// TEST_F(TestParameter, bool_variant) {
//   // Direct instantiation
//   rclcpp::Parameter bool_variant_true("bool_param", true);
//   EXPECT_EQ("bool_param", bool_variant_true.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BOOL, bool_variant_true.get_type());
//   EXPECT_EQ("bool", bool_variant_true.get_type_name());
//   EXPECT_TRUE(bool_variant_true.get_value<rclcpp::ParameterType::PARAMETER_BOOL>());
//   EXPECT_TRUE(bool_variant_true.get_value_message().bool_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL,
//     bool_variant_true.get_value_message().type);
//   EXPECT_TRUE(bool_variant_true.as_bool());

//   EXPECT_THROW(bool_variant_true.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_variant_true.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("true", bool_variant_true.value_to_string());
//   std::stringstream ss;
//   ss << bool_variant_true;
//   EXPECT_EQ("{\"name\": \"bool_param\", \"type\": \"bool\", \"value\": \"true\"}", ss.str());


//   rclcpp::Parameter bool_variant_false("bool_param", false);
//   EXPECT_FALSE(bool_variant_false.get_value<rclcpp::ParameterType::PARAMETER_BOOL>());
//   EXPECT_FALSE(bool_variant_false.get_value_message().bool_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL,
//     bool_variant_false.get_value_message().type);

//   rcl_interfaces::msg::Parameter bool_param = bool_variant_true.to_parameter_msg();
//   EXPECT_EQ("bool_param", bool_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_BOOL, bool_param.value.type);
//   EXPECT_TRUE(bool_param.value.bool_value);

//   // From parameter message
//   rclcpp::Parameter from_msg_true =
//     rclcpp::Parameter::from_parameter_msg(bool_param);
//   EXPECT_EQ("bool_param", from_msg_true.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BOOL, from_msg_true.get_type());
//   EXPECT_EQ("bool", from_msg_true.get_type_name());
//   EXPECT_TRUE(from_msg_true.get_value<rclcpp::ParameterType::PARAMETER_BOOL>());
//   EXPECT_TRUE(from_msg_true.get_value_message().bool_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL,
//     bool_variant_false.get_value_message().type);

//   bool_param.value.bool_value = false;
//   rclcpp::Parameter from_msg_false =
//     rclcpp::Parameter::from_parameter_msg(bool_param);
//   EXPECT_FALSE(from_msg_false.get_value<rclcpp::ParameterType::PARAMETER_BOOL>());
//   EXPECT_FALSE(from_msg_false.get_value_message().bool_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL,
//     bool_variant_false.get_value_message().type);

//   // Check == and != operators work as expected
//   EXPECT_EQ(bool_variant_true, bool_variant_true);
//   EXPECT_NE(bool_variant_false, bool_variant_true);
//   EXPECT_EQ(bool_variant_true, from_msg_true);
// }

// TEST_F(TestParameter, integer_variant) {
//   const int TEST_VALUE {42};

//   // Direct instantiation
//   rclcpp::Parameter integer_variant("integer_param", TEST_VALUE);
//   EXPECT_EQ("integer_param", integer_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER, integer_variant.get_type());
//   EXPECT_EQ("integer", integer_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     integer_variant.get_value<rclcpp::ParameterType::PARAMETER_INTEGER>());
//   EXPECT_EQ(TEST_VALUE, integer_variant.get_value_message().integer_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER,
//     integer_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, integer_variant.as_int());

//   EXPECT_THROW(integer_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("42", integer_variant.value_to_string());
//   std::stringstream ss;
//   ss << integer_variant;
//   EXPECT_EQ("{\"name\": \"integer_param\", \"type\": \"integer\", \"value\": \"42\"}", ss.str());

//   rcl_interfaces::msg::Parameter integer_param = integer_variant.to_parameter_msg();
//   EXPECT_EQ("integer_param", integer_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER, integer_param.value.type);
//   EXPECT_EQ(TEST_VALUE, integer_param.value.integer_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(integer_param);
//   EXPECT_EQ("integer_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER, from_msg.get_type());
//   EXPECT_EQ("integer", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_INTEGER>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().integer_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   EXPECT_EQ(integer_variant, integer_variant);
//   EXPECT_NE(integer_variant, rclcpp::Parameter("integer_param", 41));
//   EXPECT_EQ(integer_variant, from_msg);
// }

// TEST_F(TestParameter, long_integer_variant) {
//   const int64_t TEST_VALUE {std::numeric_limits<int64_t>::max()};

//   // Direct instantiation
//   rclcpp::Parameter long_variant("long_integer_param", TEST_VALUE);
//   EXPECT_EQ("long_integer_param", long_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER, long_variant.get_type());
//   EXPECT_EQ("integer", long_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     long_variant.get_value<rclcpp::ParameterType::PARAMETER_INTEGER>());
//   EXPECT_EQ(TEST_VALUE, long_variant.get_value_message().integer_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER,
//     long_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, long_variant.as_int());

//   EXPECT_THROW(long_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("9223372036854775807", long_variant.value_to_string());
//   std::stringstream ss;
//   ss << long_variant;
//   EXPECT_EQ(
//     "{\"name\": \"long_integer_param\", \"type\": \"integer\", \"value\": "
//     "\"9223372036854775807\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter integer_param = long_variant.to_parameter_msg();
//   EXPECT_EQ("long_integer_param", integer_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER, integer_param.value.type);
//   EXPECT_EQ(TEST_VALUE, integer_param.value.integer_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(integer_param);
//   EXPECT_EQ("long_integer_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER, from_msg.get_type());
//   EXPECT_EQ("integer", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_INTEGER>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().integer_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER,
//     from_msg.get_value_message().type);


//   // Check == and != operators work as expected
//   EXPECT_EQ(long_variant, long_variant);
//   EXPECT_NE(long_variant, rclcpp::Parameter("long_integer_param", 41));
//   EXPECT_EQ(long_variant, from_msg);
// }

// TEST_F(TestParameter, float_variant) {
//   const float TEST_VALUE {42.0f};

//   // Direct instantiation
//   rclcpp::Parameter float_variant("float_param", TEST_VALUE);
//   EXPECT_EQ("float_param", float_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE, float_variant.get_type());
//   EXPECT_EQ("double", float_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     float_variant.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE>());
//   EXPECT_EQ(TEST_VALUE, float_variant.get_value_message().double_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE,
//     float_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, float_variant.as_double());

//   EXPECT_THROW(float_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("42.000000", float_variant.value_to_string());
//   std::stringstream ss;
//   ss << float_variant;
//   EXPECT_EQ(
//     "{\"name\": \"float_param\", \"type\": \"double\", \"value\": \"42.000000\"}", ss.str());

//   rcl_interfaces::msg::Parameter float_param = float_variant.to_parameter_msg();
//   EXPECT_EQ("float_param", float_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE, float_param.value.type);
//   EXPECT_EQ(TEST_VALUE, float_param.value.double_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(float_param);
//   EXPECT_EQ("float_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE, from_msg.get_type());
//   EXPECT_EQ("double", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().double_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   EXPECT_EQ(float_variant, float_variant);
//   EXPECT_NE(float_variant, rclcpp::Parameter("float_param", 41.0));
//   EXPECT_EQ(float_variant, from_msg);
// }

// TEST_F(TestParameter, double_variant) {
//   const double TEST_VALUE {-42.1};

//   // Direct instantiation
//   rclcpp::Parameter double_variant("double_param", TEST_VALUE);
//   EXPECT_EQ("double_param", double_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE, double_variant.get_type());
//   EXPECT_EQ("double", double_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     double_variant.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE>());
//   EXPECT_EQ(TEST_VALUE, double_variant.get_value_message().double_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE,
//     double_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, double_variant.as_double());

//   EXPECT_THROW(double_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("-42.100000", double_variant.value_to_string());
//   std::stringstream ss;
//   ss << double_variant;
//   EXPECT_EQ(
//     "{\"name\": \"double_param\", \"type\": \"double\", \"value\": \"-42.100000\"}", ss.str());

//   rcl_interfaces::msg::Parameter double_param = double_variant.to_parameter_msg();
//   EXPECT_EQ("double_param", double_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE, double_param.value.type);
//   EXPECT_EQ(TEST_VALUE, double_param.value.double_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(double_param);
//   EXPECT_EQ("double_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE, from_msg.get_type());
//   EXPECT_EQ("double", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().double_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   EXPECT_EQ(double_variant, double_variant);
//   EXPECT_NE(double_variant, rclcpp::Parameter("double_param", -41.2));
//   EXPECT_EQ(double_variant, from_msg);
// }

// TEST_F(TestParameter, string_variant) {
//   const std::string TEST_VALUE {"ROS2"};

//   // Direct instantiation
//   rclcpp::Parameter string_variant("string_param", TEST_VALUE);
//   EXPECT_EQ("string_param", string_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_STRING, string_variant.get_type());
//   EXPECT_EQ("string", string_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     string_variant.get_value<rclcpp::ParameterType::PARAMETER_STRING>());
//   EXPECT_EQ(TEST_VALUE, string_variant.get_value_message().string_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_STRING,
//     string_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, string_variant.as_string());

//   EXPECT_THROW(string_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ(TEST_VALUE, string_variant.value_to_string());
//   std::stringstream ss;
//   ss << string_variant;
//   EXPECT_EQ("{\"name\": \"string_param\", \"type\": \"string\", \"value\": \"ROS2\"}", ss.str());

//   rcl_interfaces::msg::Parameter string_param = string_variant.to_parameter_msg();
//   EXPECT_EQ("string_param", string_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_STRING, string_param.value.type);
//   EXPECT_EQ(TEST_VALUE, string_param.value.string_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(string_param);
//   EXPECT_EQ("string_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_STRING, from_msg.get_type());
//   EXPECT_EQ("string", from_msg.get_type_name());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value<rclcpp::ParameterType::PARAMETER_STRING>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().string_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_STRING,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   EXPECT_EQ(string_variant, string_variant);
//   EXPECT_NE(string_variant, rclcpp::Parameter("string_param", "ROS1"));
//   EXPECT_EQ(string_variant, from_msg);
// }

// TEST_F(TestParameter, byte_array_variant) {
//   const std::vector<uint8_t> TEST_VALUE {0x52, 0x4f, 0x53, 0x32};

//   // Direct instantiation
//   rclcpp::Parameter byte_array_variant("byte_array_param", TEST_VALUE);
//   EXPECT_EQ("byte_array_param", byte_array_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BYTE_ARRAY, byte_array_variant.get_type());
//   EXPECT_EQ("byte_array", byte_array_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     byte_array_variant.get_value<rclcpp::ParameterType::PARAMETER_BYTE_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, byte_array_variant.get_value_message().byte_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BYTE_ARRAY,
//     byte_array_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, byte_array_variant.as_byte_array());

//   EXPECT_THROW(byte_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(byte_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("[0x52, 0x4f, 0x53, 0x32]", byte_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << byte_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"byte_array_param\", \"type\": \"byte_array\", \"value\": "
//     "\"[0x52, 0x4f, 0x53, 0x32]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter byte_array_param = byte_array_variant.to_parameter_msg();
//   EXPECT_EQ("byte_array_param", byte_array_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_BYTE_ARRAY, byte_array_param.value.type);
//   EXPECT_EQ(TEST_VALUE, byte_array_param.value.byte_array_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(byte_array_param);
//   EXPECT_EQ("byte_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BYTE_ARRAY, from_msg.get_type());
//   EXPECT_EQ("byte_array", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_BYTE_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().byte_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BYTE_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<uint8_t> TEST_VALUE2 {0x1, 0x2, 0x3, 0x4};
//   EXPECT_EQ(byte_array_variant, byte_array_variant);
//   EXPECT_NE(byte_array_variant, rclcpp::Parameter("byte_array_param", TEST_VALUE2));
//   EXPECT_EQ(byte_array_variant, from_msg);
// }

// TEST_F(TestParameter, bool_array_variant) {
//   const std::vector<bool> TEST_VALUE {false, true, true, false, false, true};

//   // Direct instantiation
//   rclcpp::Parameter bool_array_variant("bool_array_param", TEST_VALUE);
//   EXPECT_EQ("bool_array_param", bool_array_variant.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BOOL_ARRAY, bool_array_variant.get_type());
//   EXPECT_EQ("bool_array", bool_array_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     bool_array_variant.get_value<rclcpp::ParameterType::PARAMETER_BOOL_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, bool_array_variant.get_value_message().bool_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL_ARRAY,
//     bool_array_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, bool_array_variant.as_bool_array());

//   EXPECT_THROW(bool_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(bool_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("[false, true, true, false, false, true]", bool_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << bool_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"bool_array_param\", \"type\": \"bool_array\", \"value\": "
//     "\"[false, true, true, false, false, true]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter bool_array_param = bool_array_variant.to_parameter_msg();
//   EXPECT_EQ("bool_array_param", bool_array_param.name);
//   EXPECT_EQ(rcl_interfaces::msg::ParameterType::PARAMETER_BOOL_ARRAY, bool_array_param.value.type);
//   EXPECT_EQ(TEST_VALUE, bool_array_param.value.bool_array_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(bool_array_param);
//   EXPECT_EQ("bool_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_BOOL_ARRAY, from_msg.get_type());
//   EXPECT_EQ("bool_array", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_BOOL_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().bool_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_BOOL_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<bool> TEST_VALUE2 {true, true, true};
//   EXPECT_EQ(bool_array_variant, bool_array_variant);
//   EXPECT_NE(bool_array_variant, rclcpp::Parameter("bool_array_param", TEST_VALUE2));
//   EXPECT_EQ(bool_array_variant, from_msg);
// }

// TEST_F(TestParameter, integer_array_variant) {
//   const std::vector<int> TEST_VALUE
//   {42, -99, std::numeric_limits<int>::max(), std::numeric_limits<int>::lowest(), 0};

//   // Direct instantiation
//   rclcpp::Parameter integer_array_variant("integer_array_param", TEST_VALUE);

//   EXPECT_EQ("integer_array_param", integer_array_variant.get_name());
//   EXPECT_EQ(
//     rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY,
//     integer_array_variant.get_type());
//   EXPECT_EQ("integer_array", integer_array_variant.get_type_name());
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY,
//     integer_array_variant.get_value_message().type);

//   // No direct comparison of vectors of ints and long ints
//   const auto & param_value_ref =
//     integer_array_variant.get_value<rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>();
//   auto mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value_ref.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value_ref.end(), mismatches.second);

//   auto param_value = integer_array_variant.get_value_message().integer_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   param_value = integer_array_variant.as_integer_array();
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   EXPECT_THROW(integer_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(integer_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ(
//     "[42, -99, 2147483647, -2147483648, 0]",
//     integer_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << integer_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"integer_array_param\", \"type\": \"integer_array\", \"value\": "
//     "\"[42, -99, 2147483647, -2147483648, 0]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter integer_array_param = integer_array_variant.to_parameter_msg();
//   EXPECT_EQ("integer_array_param", integer_array_param.name);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY,
//     integer_array_param.value.type);

//   param_value = integer_array_param.value.integer_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(integer_array_param);
//   EXPECT_EQ("integer_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY, from_msg.get_type());
//   EXPECT_EQ("integer_array", from_msg.get_type_name());

//   param_value = from_msg.get_value<rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>();
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   param_value = from_msg.get_value_message().integer_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   EXPECT_EQ(
//     from_msg.get_value_message().type,
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY);

//   // Check == and != operators work as expected
//   const std::vector<int> TEST_VALUE2{1, 2, 3, 4, 5};
//   EXPECT_EQ(integer_array_variant, integer_array_variant);
//   EXPECT_NE(integer_array_variant, rclcpp::Parameter("integer_array_param", TEST_VALUE2));
//   EXPECT_EQ(integer_array_variant, from_msg);
// }

// TEST_F(TestParameter, long_integer_array_variant) {
//   const std::vector<int64_t> TEST_VALUE
//   {42, -99, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::lowest(), 0};

//   rclcpp::Parameter long_array_variant("long_integer_array_param", TEST_VALUE);
//   EXPECT_EQ("long_integer_array_param", long_array_variant.get_name());
//   EXPECT_EQ(
//     rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY,
//     long_array_variant.get_type());
//   EXPECT_EQ("integer_array", long_array_variant.get_type_name());
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY,
//     long_array_variant.get_value_message().type);
//   EXPECT_EQ(
//     TEST_VALUE,
//     long_array_variant.get_value<rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, long_array_variant.get_value_message().integer_array_value);
//   EXPECT_EQ(TEST_VALUE, long_array_variant.as_integer_array());

//   EXPECT_THROW(long_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_double_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(long_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ(
//     "[42, -99, 9223372036854775807, -9223372036854775808, 0]",
//     long_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << long_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"long_integer_array_param\", \"type\": \"integer_array\", \"value\": "
//     "\"[42, -99, 9223372036854775807, -9223372036854775808, 0]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter integer_array_param = long_array_variant.to_parameter_msg();
//   EXPECT_EQ("long_integer_array_param", integer_array_param.name);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY,
//     integer_array_param.value.type);
//   EXPECT_EQ(TEST_VALUE, integer_array_param.value.integer_array_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(integer_array_param);
//   EXPECT_EQ("long_integer_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY, from_msg.get_type());
//   EXPECT_EQ("integer_array", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_INTEGER_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().integer_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_INTEGER_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<int64_t> TEST_VALUE2{1, 2, 3, 4, 5};
//   EXPECT_EQ(long_array_variant, long_array_variant);
//   EXPECT_NE(long_array_variant, rclcpp::Parameter("long_integer_array_param", TEST_VALUE2));
//   EXPECT_EQ(long_array_variant, from_msg);
// }

// TEST_F(TestParameter, float_array_variant) {
//   const std::vector<float> TEST_VALUE
//   {42.1f, -99.1f, std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest(), 0.1f};

//   // Direct instantiation
//   rclcpp::Parameter float_array_variant("float_array_param", TEST_VALUE);

//   EXPECT_EQ("float_array_param", float_array_variant.get_name());
//   EXPECT_EQ(
//     rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     float_array_variant.get_type());
//   EXPECT_EQ("double_array", float_array_variant.get_type_name());
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     float_array_variant.get_value_message().type);

//   // No direct comparison of vectors of floats and doubles
//   const auto & param_value_ref =
//     float_array_variant.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>();
//   auto mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value_ref.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value_ref.end(), mismatches.second);

//   auto param_value = float_array_variant.get_value_message().double_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   param_value = float_array_variant.as_double_array();
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   EXPECT_THROW(float_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(float_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ(
//     "[42.1, -99.1, 3.40282e+38, -3.40282e+38, 0.1]",
//     float_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << float_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"float_array_param\", \"type\": \"double_array\", \"value\": "
//     "\"[42.1, -99.1, 3.40282e+38, -3.40282e+38, 0.1]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter float_array_param = float_array_variant.to_parameter_msg();
//   EXPECT_EQ("float_array_param", float_array_param.name);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     float_array_param.value.type);

//   param_value = float_array_param.value.double_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(float_array_param);
//   EXPECT_EQ("float_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY, from_msg.get_type());
//   EXPECT_EQ("double_array", from_msg.get_type_name());

//   param_value = from_msg.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>();
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   param_value = from_msg.get_value_message().double_array_value;
//   mismatches = std::mismatch(TEST_VALUE.begin(), TEST_VALUE.end(), param_value.begin());
//   EXPECT_EQ(TEST_VALUE.end(), mismatches.first);
//   EXPECT_EQ(param_value.end(), mismatches.second);

//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<float> TEST_VALUE2{1.0, 2.0, 3.0, 4.0};
//   EXPECT_EQ(float_array_variant, float_array_variant);
//   EXPECT_NE(float_array_variant, rclcpp::Parameter("float_array_param", TEST_VALUE2));
//   EXPECT_EQ(float_array_variant, from_msg);
// }

// TEST_F(TestParameter, double_array_variant) {
//   const std::vector<double> TEST_VALUE
//   {42.1, -99.1, std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(), 0.1};

//   rclcpp::Parameter double_array_variant("double_array_param", TEST_VALUE);
//   EXPECT_EQ("double_array_param", double_array_variant.get_name());
//   EXPECT_EQ(
//     rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     double_array_variant.get_type());
//   EXPECT_EQ("double_array", double_array_variant.get_type_name());
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     double_array_variant.get_value_message().type);
//   EXPECT_EQ(
//     TEST_VALUE,
//     double_array_variant.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, double_array_variant.get_value_message().double_array_value);
//   EXPECT_EQ(TEST_VALUE, double_array_variant.as_double_array());

//   EXPECT_THROW(double_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(double_array_variant.as_string_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ(
//     "[42.1, -99.1, 1.79769e+308, -1.79769e+308, 0.1]",
//     double_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << double_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"double_array_param\", \"type\": \"double_array\", \"value\": "
//     "\"[42.1, -99.1, 1.79769e+308, -1.79769e+308, 0.1]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter double_array_param = double_array_variant.to_parameter_msg();
//   EXPECT_EQ("double_array_param", double_array_param.name);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     double_array_param.value.type);
//   EXPECT_EQ(TEST_VALUE, double_array_param.value.double_array_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(double_array_param);
//   EXPECT_EQ("double_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY, from_msg.get_type());
//   EXPECT_EQ("double_array", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_DOUBLE_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().double_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<double> TEST_VALUE2{1.0, 2.0, 3.0, 4.0};
//   EXPECT_EQ(double_array_variant, double_array_variant);
//   EXPECT_NE(double_array_variant, rclcpp::Parameter("double_array_param", TEST_VALUE2));
//   EXPECT_EQ(double_array_variant, from_msg);
// }

// TEST_F(TestParameter, string_array_variant) {
//   const std::vector<std::string> TEST_VALUE {"R", "O", "S2"};

//   // Direct instantiation
//   rclcpp::Parameter string_array_variant("string_array_param", TEST_VALUE);
//   EXPECT_EQ("string_array_param", string_array_variant.get_name());
//   EXPECT_EQ(
//     rclcpp::ParameterType::PARAMETER_STRING_ARRAY,
//     string_array_variant.get_type());
//   EXPECT_EQ("string_array", string_array_variant.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     string_array_variant.get_value<rclcpp::ParameterType::PARAMETER_STRING_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, string_array_variant.get_value_message().string_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_STRING_ARRAY,
//     string_array_variant.get_value_message().type);
//   EXPECT_EQ(TEST_VALUE, string_array_variant.as_string_array());

//   EXPECT_THROW(string_array_variant.as_bool(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_int(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_double(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_string(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_byte_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_bool_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_integer_array(), rclcpp::ParameterTypeException);
//   EXPECT_THROW(string_array_variant.as_double_array(), rclcpp::ParameterTypeException);

//   EXPECT_EQ("[R, O, S2]", string_array_variant.value_to_string());
//   std::stringstream ss;
//   ss << string_array_variant;
//   EXPECT_EQ(
//     "{\"name\": \"string_array_param\", \"type\": \"string_array\", \"value\": \"[R, O, S2]\"}",
//     ss.str());

//   rcl_interfaces::msg::Parameter string_array_param = string_array_variant.to_parameter_msg();
//   EXPECT_EQ("string_array_param", string_array_param.name);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_STRING_ARRAY,
//     string_array_param.value.type);
//   EXPECT_EQ(TEST_VALUE, string_array_param.value.string_array_value);

//   // From parameter message
//   rclcpp::Parameter from_msg =
//     rclcpp::Parameter::from_parameter_msg(string_array_param);
//   EXPECT_EQ("string_array_param", from_msg.get_name());
//   EXPECT_EQ(rclcpp::ParameterType::PARAMETER_STRING_ARRAY, from_msg.get_type());
//   EXPECT_EQ("string_array", from_msg.get_type_name());
//   EXPECT_EQ(
//     TEST_VALUE,
//     from_msg.get_value<rclcpp::ParameterType::PARAMETER_STRING_ARRAY>());
//   EXPECT_EQ(TEST_VALUE, from_msg.get_value_message().string_array_value);
//   EXPECT_EQ(
//     rcl_interfaces::msg::ParameterType::PARAMETER_STRING_ARRAY,
//     from_msg.get_value_message().type);

//   // Check == and != operators work as expected
//   const std::vector<std::string> TEST_VALUE2{"R", "O", "S1"};
//   EXPECT_EQ(string_array_variant, string_array_variant);
//   EXPECT_NE(string_array_variant, rclcpp::Parameter("string_array_param", TEST_VALUE2));
//   EXPECT_EQ(string_array_variant, from_msg);
// }

// TEST_F(TestParameter, parameter_vector_stringification) {
//   const std::vector<rclcpp::Parameter> parameters = {
//     rclcpp::Parameter(),
//     rclcpp::Parameter("bool_param", true),
//     rclcpp::Parameter("integer_param", 42),
//     rclcpp::Parameter("double_param", 3.14159),
//     rclcpp::Parameter("string_param", "I'm a string"),
//   };

//   std::stringstream ss;
//   ss << parameters;
//   EXPECT_EQ(
//     "{"
//     "\"\": {\"type\": \"not set\", \"value\": \"not set\"}, "
//     "\"bool_param\": {\"type\": \"bool\", \"value\": \"true\"}, "
//     "\"integer_param\": {\"type\": \"integer\", \"value\": \"42\"}, "
//     "\"double_param\": {\"type\": \"double\", \"value\": \"3.141590\"}, "
//     "\"string_param\": {\"type\": \"string\", \"value\": \"I'm a string\"}}",
//     ss.str());
// }
