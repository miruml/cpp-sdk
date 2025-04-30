// std
#include <execinfo.h>

// internal
#include <miru/query/ros2.hpp>
#include <test/query/query_test.hpp>
#include <test/test_utils/query.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

namespace test::query {

// use simple tests for these since they are thin wrappers around the query library

// =============================== HAS PARAMETER =================================== //
TEST(ROS2HasParamTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  miru::query::ROS2 ros2(parameter);
  EXPECT_EQ(ros2.has_parameter("exists"), true);
}

TEST(ROS2HasParamTests, DoesntExist) {
  miru::params::Parameter parameter;
  miru::query::ROS2 ros2(parameter);
  EXPECT_EQ(ros2.has_parameter("doesnt_exist"), false);
}

// =============================== GET PARAMETER =================================== //
TEST(ROS2GetParamTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  miru::query::ROS2 ros2(parameter);
  EXPECT_EQ(ros2.get_parameter("exists").as<double>(), 42.3);
}

TEST(ROS2GetParamTests, DoesntExist) {
  miru::params::Parameter parameter;
  miru::query::ROS2 ros2(parameter);
  EXPECT_THROW(ros2.get_parameter("doesnt_exist"), miru::query::ParameterNotFoundError);
}

// ====================== GET PARAMETER WITH DEFAULT PARAMETER ===================== //
TEST(ROS2GetParamWithDefaultParameterTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  miru::params::Parameter default_parameter("default", 100.0);
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter("exists", default_parameter), true);
  EXPECT_EQ(default_parameter.as<double>(), 42.3);
}

TEST(ROS2GetParamWithDefaultParameterTests, DoesntExist) {
  miru::params::Parameter parameter("exists", 42.3);
  miru::params::Parameter default_parameter("default", 100.0);
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter("doesnt_exist", default_parameter), false);
  EXPECT_EQ(default_parameter.as<double>(), 100.0);
}

// ======================== GET PARAMETER WITH DEFAULT VALUE ======================= //
TEST(ROS2GetParamWithDefaultValueTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter("exists", default_value), true);
  EXPECT_EQ(default_value, 42.3);
}

TEST(ROS2GetParamWithDefaultValueTests, InvalidType) {
  miru::params::Parameter parameter("exists", 42.3);
  int default_value = 100;
  miru::query::ROS2 ros2(parameter);

  EXPECT_THROW(
    ros2.get_parameter("exists", default_value), miru::params::InvalidParameterTypeError
  );
}

TEST(ROS2GetParamWithDefaultValueTests, DoesntExist) {
  miru::params::Parameter parameter("exists", 42.3);
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter("doesnt_exist", default_value), false);
  EXPECT_EQ(default_value, 100.0);
}

// ============================== TRY GET PARAMETER ================================ //
TEST(ROS2TryGetParamTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter_or("exists", default_value), 42.3);
}

TEST(ROS2TryGetParamTests, InvalidType) {
  miru::params::Parameter parameter("exists", 42.3);
  int default_value = 100;
  miru::query::ROS2 ros2(parameter);

  EXPECT_THROW(
    ros2.get_parameter_or("exists", default_value),
    miru::params::InvalidParameterTypeError
  );
}

TEST(ROS2TryGetParamTests, DoesntExist) {
  miru::params::Parameter parameter("exists", 42.3);
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter_or("doesnt_exist", default_value), 100.0);
}

// ====================== TRY GET PARAMETER WITH DEFAULT VALUE ===================== //
TEST(ROS2TryGetParamWithDefaultValueTests, Exists) {
  miru::params::Parameter parameter("exists", 42.3);
  double result_value;
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter_or("exists", result_value, default_value), true);
  EXPECT_EQ(result_value, 42.3);
}

TEST(ROS2TryGetParamWithDefaultValueTests, InvalidType) {
  miru::params::Parameter parameter("exists", 42.3);
  int result_value;
  int default_value = 100;
  miru::query::ROS2 ros2(parameter);

  EXPECT_THROW(
    ros2.get_parameter_or("exists", result_value, default_value),
    miru::params::InvalidParameterTypeError
  );
}

TEST(ROS2TryGetParamWithDefaultValueTests, DoesntExist) {
  miru::params::Parameter parameter("exists", 42.3);
  double result_value = 0;
  double default_value = 100.0;
  miru::query::ROS2 ros2(parameter);

  EXPECT_EQ(ros2.get_parameter_or("doesnt_exist", result_value, default_value), false);
  EXPECT_EQ(default_value, 100.0);
  EXPECT_EQ(result_value, 100.0);
}

// =============================== GET PARAMETERS ================================== //
TEST(ROS2GetParamsTests, Simple) {
  miru::params::Parameter parameter("root", 42.3);
  miru::query::ROS2 ros2(parameter);

  auto params = ros2.get_parameters({"root"});
  EXPECT_EQ(params.size(), 1);
  EXPECT_EQ(params[0].get_name(), "root");
  EXPECT_EQ(params[0].as<double>(), 42.3);
}

TEST(ROS2GetParamsTests, DoesntExist) {
  miru::params::Parameter parameter("root", 42.3);
  miru::query::ROS2 ros2(parameter);
  EXPECT_THROW(
    ros2.get_parameters({"root", "doesnt_exist"}), miru::query::ParameterNotFoundError
  );
}

}  // namespace test::query