// internal
#include <miru/params/errors.hpp>
#include <miru/params/parameter.hpp>
#include <miru/params/utils.hpp>
#include <miru/params/details/utils.hpp>
#include <test/utils_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

class ParamUtils : public ::testing::Test {
 protected:
};

TEST_F(ParamUtils, not_set_variant) {
  miru::params::Parameter not_set = miru::params::Parameter();
  EXPECT_TRUE(not_set.get_type() == miru::params::ParameterType::PARAMETER_NOT_SET);

  EXPECT_TRUE(miru::params::is_leaf(not_set));
  EXPECT_FALSE(miru::params::has_children(not_set));
  EXPECT_TRUE(miru::params::get_children_view(not_set).empty());
}

TEST_F(ParamUtils, bool_variant) {
  miru::params::Parameter bool_param = miru::params::Parameter("bool_param", true);
  EXPECT_TRUE(bool_param.get_type() == miru::params::ParameterType::PARAMETER_BOOL);

  EXPECT_TRUE(miru::params::is_leaf(bool_param));
  EXPECT_FALSE(miru::params::has_children(bool_param));
  EXPECT_TRUE(miru::params::get_children_view(bool_param).empty());
}

TEST_F(ParamUtils, int_variant) {
  miru::params::Parameter int_param = miru::params::Parameter("int_param", 4);
  EXPECT_TRUE(int_param.get_type() == miru::params::ParameterType::PARAMETER_INTEGER);

  EXPECT_TRUE(miru::params::is_leaf(int_param));
  EXPECT_FALSE(miru::params::has_children(int_param));
  EXPECT_TRUE(miru::params::get_children_view(int_param).empty());
}

TEST_F(ParamUtils, double_variant) {
  miru::params::Parameter double_param = miru::params::Parameter("double_param", 4.0);
  EXPECT_TRUE(double_param.get_type() == miru::params::ParameterType::PARAMETER_DOUBLE);

  EXPECT_TRUE(miru::params::is_leaf(double_param));
  EXPECT_FALSE(miru::params::has_children(double_param));
  EXPECT_TRUE(miru::params::get_children_view(double_param).empty());
}

TEST_F(ParamUtils, string_variant) {
  miru::params::Parameter string_param =
    miru::params::Parameter("string_param", "test");
  EXPECT_TRUE(string_param.get_type() == miru::params::ParameterType::PARAMETER_STRING);

  EXPECT_TRUE(miru::params::is_leaf(string_param));
  EXPECT_FALSE(miru::params::has_children(string_param));
  EXPECT_TRUE(miru::params::get_children_view(string_param).empty());
}

TEST_F(ParamUtils, bool_array_variant) {
  miru::params::Parameter bool_array_param =
    miru::params::Parameter("bool_array_param", std::vector<bool>{true, false, true});
  EXPECT_TRUE(miru::params::is_leaf(bool_array_param));

  EXPECT_FALSE(miru::params::has_children(bool_array_param));
  EXPECT_TRUE(miru::params::get_children_view(bool_array_param).empty());
}

TEST_F(ParamUtils, int_array_variant) {
  miru::params::Parameter int_array_param =
    miru::params::Parameter("int_array_param", std::vector<int>{1, 2, 3});
  EXPECT_TRUE(
    int_array_param.get_type() == miru::params::ParameterType::PARAMETER_INTEGER_ARRAY
  );

  EXPECT_TRUE(miru::params::is_leaf(int_array_param));
  EXPECT_FALSE(miru::params::has_children(int_array_param));
  EXPECT_TRUE(miru::params::get_children_view(int_array_param).empty());
}

TEST_F(ParamUtils, double_array_variant) {
  miru::params::Parameter double_array_param =
    miru::params::Parameter("double_array_param", std::vector<double>{1.0, 2.2, 3.3});
  EXPECT_TRUE(
    double_array_param.get_type() == miru::params::ParameterType::PARAMETER_DOUBLE_ARRAY
  );

  EXPECT_TRUE(miru::params::is_leaf(double_array_param));
  EXPECT_FALSE(miru::params::has_children(double_array_param));
  EXPECT_TRUE(miru::params::get_children_view(double_array_param).empty());
}

TEST_F(ParamUtils, string_array_variant) {
  miru::params::Parameter string_array_param = miru::params::Parameter(
    "string_array_param", std::vector<std::string>{"test1", "test2", "test3"}
  );
  EXPECT_TRUE(
    string_array_param.get_type() == miru::params::ParameterType::PARAMETER_STRING_ARRAY
  );

  EXPECT_TRUE(miru::params::is_leaf(string_array_param));
  EXPECT_FALSE(miru::params::has_children(string_array_param));
  EXPECT_TRUE(miru::params::get_children_view(string_array_param).empty());
}

TEST_F(ParamUtils, null_variant) {
  miru::params::Parameter null_param = miru::params::Parameter("null_param", nullptr);
  EXPECT_TRUE(null_param.get_type() == miru::params::ParameterType::PARAMETER_NULL);

  EXPECT_TRUE(miru::params::is_leaf(null_param));
  EXPECT_FALSE(miru::params::has_children(null_param));
  EXPECT_TRUE(miru::params::get_children_view(null_param).empty());
}

TEST_F(ParamUtils, scalar_variant) {
  miru::params::Parameter scalar_param =
    miru::params::Parameter("scalar_param", miru::params::Scalar("test"));
  EXPECT_TRUE(scalar_param.get_type() == miru::params::ParameterType::PARAMETER_SCALAR);

  EXPECT_TRUE(miru::params::is_leaf(scalar_param));
  EXPECT_FALSE(miru::params::has_children(scalar_param));
  EXPECT_TRUE(miru::params::get_children_view(scalar_param).empty());
}

TEST_F(ParamUtils, scalar_array_variant) {
  miru::params::Parameter scalar_array_param = miru::params::Parameter(
    "scalar_array_param",
    std::vector<miru::params::Scalar>{
      miru::params::Scalar("true"),
      miru::params::Scalar("false"),
      miru::params::Scalar("true")
    }
  );
  EXPECT_TRUE(
    scalar_array_param.get_type() == miru::params::ParameterType::PARAMETER_SCALAR_ARRAY
  );

  EXPECT_TRUE(miru::params::is_leaf(scalar_array_param));
  EXPECT_FALSE(miru::params::has_children(scalar_array_param));
  EXPECT_TRUE(miru::params::get_children_view(scalar_array_param).empty());
}

TEST_F(ParamUtils, nested_array_variant) {
  miru::params::Parameter nested_array_param = miru::params::Parameter(
    "nested_array",
    miru::params::NestedArray{std::vector<miru::params::Parameter>{
      miru::params::Parameter("nested_array.0", std::vector<int>{1, 2, 3}),
      miru::params::Parameter("nested_array.1", std::vector<int>{4, 5, 6})
    }}
  );
  EXPECT_TRUE(
    nested_array_param.get_type() == miru::params::ParameterType::PARAMETER_NESTED_ARRAY
  );

  EXPECT_TRUE(miru::params::is_leaf(nested_array_param));
  EXPECT_TRUE(miru::params::has_children(nested_array_param));
  int i = 0;
  for (const auto& child : miru::params::get_children_view(nested_array_param)) {
    if (i == 0) {
      EXPECT_EQ(child.get_name(), "nested_array.0");
    } else if (i == 1) {
      EXPECT_EQ(child.get_name(), "nested_array.1");
    }
    EXPECT_TRUE(miru::params::is_leaf(child));
    EXPECT_FALSE(miru::params::has_children(child));
    EXPECT_TRUE(miru::params::get_children_view(child).empty());
    i++;
  }
  EXPECT_EQ(i, 2);
}

TEST_F(ParamUtils, map_variant) {
  miru::params::Parameter map_param = miru::params::Parameter(
    "map",
    miru::params::Map{std::vector<miru::params::Parameter>{
      miru::params::Parameter("map.param1", miru::params::Scalar("test1")),
      miru::params::Parameter("map.param2", miru::params::Scalar("test3"))
    }}
  );
  EXPECT_TRUE(map_param.get_type() == miru::params::ParameterType::PARAMETER_MAP);

  EXPECT_FALSE(miru::params::is_leaf(map_param));
  EXPECT_TRUE(miru::params::has_children(map_param));
  int i = 0;
  for (const auto& child : miru::params::get_children_view(map_param)) {
    if (i == 0) {
      EXPECT_EQ(child.get_name(), "map.param1");
    } else if (i == 1) {
      EXPECT_EQ(child.get_name(), "map.param2");
    }
    EXPECT_TRUE(miru::params::is_leaf(child));
    EXPECT_FALSE(miru::params::has_children(child));
    EXPECT_TRUE(miru::params::get_children_view(child).empty());
    i++;
  }
  EXPECT_EQ(i, 2);
}

TEST_F(ParamUtils, map_array_variant) {
  miru::params::Parameter map_array = miru::params::Parameter(
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
  EXPECT_TRUE(map_array.get_type() == miru::params::ParameterType::PARAMETER_MAP_ARRAY);

  EXPECT_FALSE(miru::params::is_leaf(map_array));
  EXPECT_TRUE(miru::params::has_children(map_array));
  int i = 0;
  for (const auto& child : miru::params::get_children_view(map_array)) {
    if (i == 0) {
      EXPECT_EQ(child.get_name(), "maps.0");
    } else if (i == 1) {
      EXPECT_EQ(child.get_name(), "maps.1");
    }
    EXPECT_FALSE(miru::params::is_leaf(child));
    EXPECT_TRUE(miru::params::has_children(child));
    EXPECT_FALSE(miru::params::get_children_view(child).empty());
    i++;
  }
  EXPECT_EQ(i, 2);
}

}  // namespace test::params
