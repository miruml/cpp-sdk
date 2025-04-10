// internal
#include "miru/params/composite.hpp"
#include <miru/params/exceptions.hpp>
#include <miru/params/parameter.hpp>
#include <miru/params/scalar.hpp>
#include <miru/utils.hpp>
#include <test/utils_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

// ================================ MAP CONSTRUCTOR ================================ //
class MapConstructor : public ::testing::Test {
 protected:
};

TEST_F(MapConstructor, empty_map) {
  EXPECT_THROW(miru::params::Map(std::vector<miru::params::Parameter>()),
               miru::params::EmptyInitialization);
}

TEST_F(MapConstructor, duplicate_field_names) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field1", miru::params::Scalar("value2"));
  EXPECT_THROW(
      miru::params::Map(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::DuplicateFieldNames);
}

TEST_F(MapConstructor, mismatching_parent_names) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field2.field2", miru::params::Scalar("value2"));
  EXPECT_THROW(
      miru::params::Map(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::MismatchingParentNames);
}

TEST_F(MapConstructor, mismatching_parent_names_nested) {
  miru::params::Parameter param1("l1.l2.l3.field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("l1.l2.l4.field2", miru::params::Scalar("value2"));
  EXPECT_THROW(
      miru::params::Map(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::MismatchingParentNames);
}

TEST_F(MapConstructor, success) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field2", miru::params::Scalar("value2"));
  miru::params::Map map({param1, param2});
  EXPECT_EQ(map.size(), 2);
  EXPECT_EQ(map["field1"].as_string(), "value1");
  EXPECT_EQ(map["field2"].as_string(), "value2");
}

// ================================ MAP ACCESSOR ================================ //
class MapAccessor : public ::testing::Test {
 protected:
};

TEST_F(MapAccessor, success) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field2", miru::params::Scalar("value2"));
  miru::params::Map map({param1, param2});
  EXPECT_EQ(map["field1"].as_string(), "value1");
  EXPECT_EQ(map["field2"].as_string(), "value2");
}

TEST_F(MapAccessor, nested_field) {
  miru::params::Parameter param1("deeply.nested.field1",
                                 miru::params::Scalar("value1"));
  miru::params::Parameter param2("deeply.nested.field2",
                                 miru::params::Scalar("value2"));
  miru::params::Map map({param1, param2});
  EXPECT_EQ(map["field1"].as_string(), "value1");
  EXPECT_EQ(map["field2"].as_string(), "value2");
}

TEST_F(MapAccessor, many_fields) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field2", miru::params::Scalar("value2"));
  miru::params::Parameter param3("field3", miru::params::Scalar("value3"));
  miru::params::Parameter param4("field4", miru::params::Scalar("value4"));
  miru::params::Parameter param5("field5", miru::params::Scalar("value5"));
  miru::params::Parameter param6("field6", miru::params::Scalar("value6"));
  miru::params::Parameter param7("field7", miru::params::Scalar("value7"));
  miru::params::Parameter param8("field8", miru::params::Scalar("value8"));
  miru::params::Parameter param9("field9", miru::params::Scalar("value9"));
  miru::params::Parameter param10("field10", miru::params::Scalar("value10"));
  miru::params::Map map({param1, param2, param3, param4, param5, param6, param7, param8,
                         param9, param10});
  EXPECT_EQ(map["field1"].as_string(), "value1");
  EXPECT_EQ(map["field2"].as_string(), "value2");
  EXPECT_EQ(map["field3"].as_string(), "value3");
  EXPECT_EQ(map["field4"].as_string(), "value4");
  EXPECT_EQ(map["field5"].as_string(), "value5");
  EXPECT_EQ(map["field6"].as_string(), "value6");
  EXPECT_EQ(map["field7"].as_string(), "value7");
  EXPECT_EQ(map["field8"].as_string(), "value8");
  EXPECT_EQ(map["field9"].as_string(), "value9");
  EXPECT_EQ(map["field10"].as_string(), "value10");
}

// ========================== MAP ARRAY CONSTRUCTOR ================================ //
class MapArrayConstructor : public ::testing::Test {
 protected:
};

TEST_F(MapArrayConstructor, empty_map_array) {
  // parameter constructor
  EXPECT_THROW(miru::params::MapArray(std::vector<miru::params::Parameter>()),
               miru::params::EmptyInitialization);

  // map constructor
  EXPECT_THROW(miru::params::MapArray(std::vector<miru::params::Map>()),
               miru::params::EmptyInitialization);
}

TEST_F(MapArrayConstructor, non_map_parameters) {
  miru::params::Parameter param1("field1", miru::params::Scalar("value1"));
  miru::params::Parameter param2("field1", miru::params::Scalar("value2"));
  EXPECT_THROW(
      miru::params::MapArray(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::InvalidParameterType);
}

TEST_F(MapArrayConstructor, duplicate_field_names) {
  miru::params::Map map1(
      {miru::params::Parameter("0.scalar1", miru::params::Scalar("value1")),
       miru::params::Parameter("0.int", 2)});
  miru::params::Map map2(
      {miru::params::Parameter("0.string3", "value3"),
       miru::params::Parameter("0.float4", 4.0)});

  // parameter constructor
  miru::params::Parameter param1("0", map1);
  miru::params::Parameter param2("0", map2);
  EXPECT_THROW(
      miru::params::MapArray(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::DuplicateFieldNames);

  // map constructor
  EXPECT_THROW(
      miru::params::MapArray map_array({map1, map2}),
      miru::params::DuplicateFieldNames);
}

TEST_F(MapArrayConstructor, mismatching_parent_names) {
  miru::params::Map map1(
      {miru::params::Parameter("parent1.0.scalar1", miru::params::Scalar("value1")),
       miru::params::Parameter("parent1.0.int", 2)});
  miru::params::Map map2(
      {miru::params::Parameter("parent2.1.string3", "value3"),
       miru::params::Parameter("parent2.1.float4", 4.0)});

  // parameter constructor
  miru::params::Parameter param1("parent1.0", map1);
  miru::params::Parameter param2("parent2.1", map2);
  EXPECT_THROW(
      miru::params::MapArray(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::MismatchingParentNames);

  // map constructor
  EXPECT_THROW(
      miru::params::MapArray map_array({map1, map2}),
      miru::params::MismatchingParentNames);
}

TEST_F(MapArrayConstructor, invalid_array_indices) {
  miru::params::Map map1(
      {miru::params::Parameter("parent.0.scalar1", miru::params::Scalar("value1")),
       miru::params::Parameter("parent.0.int", 2)});
  miru::params::Map map2(
      {miru::params::Parameter("parent.2.string3", "value3"),
       miru::params::Parameter("parent.2.float4", 4.0)});

  // parameter constructor
  miru::params::Parameter param1("parent.0", map1);
  miru::params::Parameter param2("parent.2", map2);
  EXPECT_THROW(
      miru::params::MapArray(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::InvalidArrayKeys);

  // map constructor
  EXPECT_THROW(
      miru::params::MapArray map_array({map1, map2}),
      miru::params::InvalidArrayKeys);
}

TEST_F(MapArrayConstructor, missing_parent_name) {
  miru::params::Map map1(
      {miru::params::Parameter("scalar1", miru::params::Scalar("value1")),
       miru::params::Parameter("int", 2)});
  miru::params::Map map2(
      {miru::params::Parameter("string3", "value3"),
       miru::params::Parameter("float4", 4.0)});

  EXPECT_THROW(
    miru::params::MapArray(std::vector<miru::params::Map>({map1, map2})),
    miru::params::DuplicateFieldNames
  );
}

TEST_F(MapArrayConstructor, success) {
  miru::params::Map map1(
      {miru::params::Parameter("parent.0.scalar1", miru::params::Scalar("value1")),
       miru::params::Parameter("parent.0.int", 2)});
  miru::params::Map map2(
      {miru::params::Parameter("parent.1.string3", "value3"),
       miru::params::Parameter("parent.1.float4", 4.0)});

  // parameter constructor
  miru::params::Parameter param1("parent.0", map1);
  miru::params::Parameter param2("parent.1", map2);
  miru::params::MapArray map_array({param1, param2});
  EXPECT_EQ(map_array.size(), 2);
  EXPECT_EQ(map_array[0].as_map()["scalar1"].as_string(), "value1");
  EXPECT_EQ(map_array[0].as_map()["int"].as_int(), 2);
  EXPECT_EQ(map_array[1].as_map()["string3"].as_string(), "value3");
  EXPECT_EQ(map_array[1].as_map()["float4"].as_double(), 4.0);

  // map constructor
  miru::params::MapArray map_array2({map1, map2});
  EXPECT_EQ(map_array2.size(), 2);
  EXPECT_EQ(map_array2[0].as_map()["scalar1"].as_string(), "value1");
  EXPECT_EQ(map_array2[0].as_map()["int"].as_int(), 2);
  EXPECT_EQ(map_array2[1].as_map()["string3"].as_string(), "value3");
  EXPECT_EQ(map_array2[1].as_map()["float4"].as_double(), 4.0);
}

// ========================== NESTED ARRAY CONSTRUCTOR ================================
class NestedArrayConstructor : public ::testing::Test {
 protected:
};

TEST_F(NestedArrayConstructor, empty_nested_array) {
  EXPECT_THROW(miru::params::NestedArray(std::vector<miru::params::Parameter>()),
               miru::params::EmptyInitialization);
  EXPECT_THROW(miru::params::NestedArray(std::vector<miru::params::NestedArray>()),
              miru::params::EmptyInitialization);
}

TEST_F(NestedArrayConstructor, non_array_parameters) {
  miru::params::Parameter param1(
      "parent.0",
      miru::params::Map(
          {miru::params::Parameter("parent.0.scalar1", miru::params::Scalar("value1")),
           miru::params::Parameter("parent.0.int", 2)}));
  miru::params::Parameter param2(
      "parent.1",
      miru::params::Map({miru::params::Parameter("parent.1.string3", "value3"),
                         miru::params::Parameter("parent.1.float4", 4.0)}));
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::Parameter>({param1, param2})),
      miru::params::InvalidParameterType);
}

TEST_F(NestedArrayConstructor, parameter_constructor_duplicate_field_names) {
  miru::params::Parameter array1("0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("0", std::vector<int>({4, 5, 6}));

  // parameter constructor
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::Parameter>({array1, array2})),
      miru::params::DuplicateFieldNames);
}

TEST_F(NestedArrayConstructor, nested_array_constructor_duplicate_field_names) {
  miru::params::Parameter array1("0.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("0.1", std::vector<int>({4, 5, 6}));
  miru::params::NestedArray nested_array1({array1, array2});

  miru::params::Parameter array3("0.0", std::vector<int>({7, 8, 9}));
  miru::params::Parameter array4("0.1", std::vector<int>({10, 11, 12}));
  miru::params::NestedArray nested_array2({array3, array4});

  // parameter constructor
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::NestedArray>({nested_array1, nested_array2})),
      miru::params::DuplicateFieldNames);
}

TEST_F(NestedArrayConstructor, mismatching_parent_names) {
  miru::params::Parameter array1("parent1.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("parent2.0", std::vector<int>({4, 5, 6}));
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::Parameter>({array1, array2})),
      miru::params::MismatchingParentNames);
}

TEST_F(NestedArrayConstructor, nested_array_constructor_mismatching_parent_names) {
  miru::params::Parameter array1("parent1.0.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("parent1.0.1", std::vector<int>({4, 5, 6}));
  miru::params::NestedArray nested_array1({array1, array2});

  miru::params::Parameter array3("parent2.1.0", std::vector<int>({7, 8, 9}));
  miru::params::Parameter array4("parent2.1.1", std::vector<int>({10, 11, 12}));
  miru::params::NestedArray nested_array2({array3, array4});

  // parameter constructor
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::NestedArray>({nested_array1, nested_array2})),
      miru::params::MismatchingParentNames);
}

TEST_F(NestedArrayConstructor, invalid_array_indices) {
  miru::params::Parameter array1("parent.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("parent.3", std::vector<int>({4, 5, 6}));
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::Parameter>({array1, array2})),
      miru::params::InvalidArrayKeys);
}

TEST_F(NestedArrayConstructor, nested_array_constructor_invalid_array_indices) {
  miru::params::Parameter array1("0.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("0.1", std::vector<int>({4, 5, 6}));
  miru::params::NestedArray nested_array1({array1, array2});

  miru::params::Parameter array3("3.0", std::vector<int>({7, 8, 9}));
  miru::params::Parameter array4("3.1", std::vector<int>({10, 11, 12}));
  miru::params::NestedArray nested_array2({array3, array4});

  // parameter constructor
  EXPECT_THROW(
      miru::params::NestedArray(std::vector<miru::params::NestedArray>({nested_array1, nested_array2})),
      miru::params::InvalidArrayKeys);
}

TEST_F(NestedArrayConstructor, success) {
  miru::params::Parameter array1("parent.0", std::vector<int>({1, 2, 3}));
  miru::params::Parameter array2("parent.1", std::vector<int>({4, 5, 6}));
  miru::params::NestedArray nested_array({array1, array2});
  EXPECT_EQ(nested_array.size(), 2);
  EXPECT_EQ(nested_array[0].get_value<std::vector<int>>()[0], 1);
  EXPECT_EQ(nested_array[0].get_value<std::vector<int>>()[1], 2);
  EXPECT_EQ(nested_array[0].get_value<std::vector<int>>()[2], 3);
  EXPECT_EQ(nested_array[1].get_value<std::vector<int>>()[0], 4);
  EXPECT_EQ(nested_array[1].get_value<std::vector<int>>()[1], 5);
  EXPECT_EQ(nested_array[1].get_value<std::vector<int>>()[2], 6);
}

}  // namespace test::params
