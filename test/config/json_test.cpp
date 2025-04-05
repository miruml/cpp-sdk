// std
#include <filesystem>
#include <iostream>

// internal
#include <miru/filesys/file.hpp>
#include <miru/config/json.hpp>
#include <test/test_utils/utils.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

// =================================== get_json_param() ============================ //
class GetJsonParam : public testing::Test {
protected:
    miru::filesys::File json_file = miru::test_utils::filesys_testdata_dir().file("valid.json");
    nlohmann::json json_data = json_file.read_json();
};

TEST_F(GetJsonParam, KeyNotFound) {
    EXPECT_THROW(miru::config::get_json_node_as<int>(json_data, {"nonexistent"}), miru::config::ParamNotFound);
}

TEST_F(GetJsonParam, TypeMismatch) {
    std::cout << sizeof(json_data) << std::endl;
    EXPECT_ANY_THROW(miru::config::get_json_node_as<int>(json_data, {"name"})); 
}

TEST_F(GetJsonParam, NotAPrimitiveValue) {
    std::cout << sizeof(json_data) << std::endl;
    EXPECT_ANY_THROW(miru::config::get_json_node_as<int>(json_data, {"metadata"}));
}

TEST_F(GetJsonParam, Integer) {
    EXPECT_EQ(miru::config::get_json_node_as<int>(json_data, {"settings", "timeout"}), 30);
}

TEST_F(GetJsonParam, Float) {
    EXPECT_EQ(miru::config::get_json_node_as<float>(json_data, {"version"}), 1.0f);
}

TEST_F(GetJsonParam, Boolean) {
    EXPECT_EQ(miru::config::get_json_node_as<bool>(json_data, {"active"}), true);
}

TEST_F(GetJsonParam, String) {
    EXPECT_EQ(
        miru::config::get_json_node_as<std::string>(json_data, {"name"}), 
        "Example Test Data"
    );
}

TEST_F(GetJsonParam, Array) {
    EXPECT_EQ(
        miru::config::get_json_node_as<std::vector<std::string>>(json_data, {"tags"}), 
        std::vector<std::string>({"test", "example", "demo"})
    );
}