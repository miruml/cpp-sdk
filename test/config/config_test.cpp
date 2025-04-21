// internal
#include <miru/config/config.hpp>
#include <miru/config/errors.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

// ============================== READ CONFIG SLUG ================================= //
TEST(Config, ConfigSlugFoundJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json"));
  EXPECT_EQ(miru::config::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, ConfigSlugFoundYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml"));
  EXPECT_EQ(miru::config::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, EmptyConfigSlugJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.json"));
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug);
}

TEST(Config, EmptyConfigSlugYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.yaml"));
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug);
}

TEST(Config, ConfigSlugNotFoundJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.json"));
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file),
    miru::config::ConfigSlugNotFound);
}

TEST(Config, ConfigSlugNotFoundYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.yaml"));
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file),
    miru::config::ConfigSlugNotFound);
}

// ================================ FROM FILE SYSTEM =============================== //

TEST(Config, FromFileSystemJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json"));
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json"));
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string());

  auto speed = miru::query::get_param(config, "motion-control.speed");

  std::cout << "speed from json: " << speed.as<int>() << std::endl;
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml"));
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml"));
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string());

  auto speed = miru::query::get_param(config, "motion-control.speed");

  std::cout << "speed from yaml: " << speed.as<int>() << std::endl;
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemJsonRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json"));
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json"));
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string());

  auto ros2 = config.ros2();
  auto speed = ros2.get_parameter("motion-control.speed");
  std::cout << "speed from json ros2: " << speed.as<int>() << std::endl;
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYamlRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml"));
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml"));
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string());

  auto speed = config.ros2().get_parameter("motion-control.speed");
  std::cout << "speed from yaml ros2: " << speed.get_value<int>() << std::endl;
  EXPECT_EQ(speed.as<int>(), 15);
}

// =================================== FROM AGENT ================================== //

}  // namespace test::config