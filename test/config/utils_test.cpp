// std
#include <filesystem>
#include <fstream>

// internal
#include <miru/filesys/file.hpp>
#include <miru/config/utils.hpp>
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
    EXPECT_THROW(miru::config::get_json_param<int>(json_data, {"nonexistent"}), miru::config::ParamNotFound);
}

TEST_F(GetJsonParam, TypeMismatch) {
    EXPECT_ANY_THROW(miru::config::get_json_param<int>(json_data, {"name"})); 
}

TEST_F(GetJsonParam, NotAPrimitiveValue) {
    EXPECT_ANY_THROW(miru::config::get_json_param<int>(json_data, {"metadata"}));
}

TEST_F(GetJsonParam, Integer) {
    EXPECT_EQ(miru::config::get_json_param<int>(json_data, {"settings", "timeout"}), 30);
}

TEST_F(GetJsonParam, Float) {
    EXPECT_EQ(miru::config::get_json_param<float>(json_data, {"version"}), 1.0f);
}

TEST_F(GetJsonParam, Boolean) {
    EXPECT_EQ(miru::config::get_json_param<bool>(json_data, {"active"}), true);
}

TEST_F(GetJsonParam, String) {
    EXPECT_EQ(
        miru::config::get_json_param<std::string>(json_data, {"name"}), 
        "Example Test Data"
    );
}

TEST_F(GetJsonParam, Array) {
    EXPECT_EQ(
        miru::config::get_json_param<std::vector<std::string>>(json_data, {"tags"}), 
        std::vector<std::string>({"test", "example", "demo"})
    );
}

TEST_F(GetJsonParam, Null) {
    EXPECT_EQ(
        miru::config::get_json_param<std::optional<int>>(json_data, {"doesnt_exist"}), 
        std::nullopt
    );
}

// =================================== get_yaml_param() ============================ //
class GetYamlParam : public testing::Test {
protected:
    miru::filesys::File yaml_file = miru::test_utils::filesys_testdata_dir().file("valid.yaml");
    YAML::Node yaml_data = YAML::LoadFile(yaml_file.abs_path().string());
};

TEST_F(GetYamlParam, KeyNotFound) {
    EXPECT_THROW(miru::config::get_yaml_param<int>(yaml_data, {"nonexistent"}), miru::config::ParamNotFound);
}

TEST_F(GetYamlParam, TypeMismatch) {
    EXPECT_ANY_THROW(miru::config::get_yaml_param<int>(yaml_data, {"name"}));
}

TEST_F(GetYamlParam, NotAPrimitiveValue) {
    EXPECT_ANY_THROW(miru::config::get_yaml_param<int>(yaml_data, {"metadata"}));
}

TEST_F(GetYamlParam, Integer) {
    EXPECT_EQ(miru::config::get_yaml_param<int>(yaml_data, {"settings", "timeout"}), 30);
}

TEST_F(GetYamlParam, Float) {
    EXPECT_EQ(miru::config::get_yaml_param<float>(yaml_data, {"version"}), 1.0f);
}

TEST_F(GetYamlParam, Boolean) {
    EXPECT_EQ(miru::config::get_yaml_param<bool>(yaml_data, {"active"}), true);
}

TEST_F(GetYamlParam, String) {
    EXPECT_EQ(miru::config::get_yaml_param<std::string>(yaml_data, {"name"}), "Example Test Data");
}

TEST_F(GetYamlParam, Array) {
    EXPECT_EQ(
        miru::config::get_yaml_param<std::vector<std::string>>(yaml_data, {"tags"}), 
        std::vector<std::string>({"test", "example", "demo"})
    );
}