// internal
#include <http/models/RefreshLatestConfigInstanceRequest.h>

#include <config/config_impl.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/http/mock.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

namespace openapi = org::openapitools::server::model;

// ================================ FROM FILE SYSTEM =============================== //
TEST(Config, FromFileSystemJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemJsonRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto ros2 = miru::query::ROS2(config);
  auto speed = ros2.get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYamlRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto ros2 = miru::query::ROS2(config);
  auto speed = ros2.get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

// =================================== FROM AGENT ================================== //
TEST(Config, FromAgentRefreshSuccess) {
  // set the response from the mock client
  test::http::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  mock_client.refresh_latest_config_instance_func = []() {
    nlohmann::json config_instance = {
      {"speed", 89},
      {"features", {{"spin", true}, {"jump", false}, {"backflip", false}}},
      {"accelerometer",
       {{"id", "123"},
        {"offsets", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"scaling_factor", {{"x", 1}, {"y", 1}, {"z", 1}}}}}
    };
    return openapi::BaseConfigInstance{
      "config_instance",
      "cfg_inst_123",
      "2021-01-01T00:00:00Z",
      "cli_123",
      "cfg_sch_123",
      config_instance,
    };
  };

  // set the schema file and default config file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::ConfigImpl config_impl = miru::config::ConfigImpl::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  miru::config::Config config =
    miru::config::Config(std::make_unique<miru::config::ConfigImpl>(config_impl));

  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::Agent);
  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 89);
}

TEST(Config, FromAgentRefreshFailureGetSuccess) {
  // set the response from the mock client
  test::http::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // refresh latest config instance will fail
  mock_client.refresh_latest_config_instance_func =
    []() -> openapi::BaseConfigInstance {
    throw std::runtime_error("refresh latest config instance failed");
  };

  // read the latest config instance will succeed
  mock_client.get_latest_config_instance_func = []() {
    nlohmann::json config_instance = {
      {"speed", 74},
      {"features", {{"spin", true}, {"jump", false}, {"backflip", false}}},
      {"accelerometer",
       {{"id", "123"},
        {"offsets", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"scaling_factor", {{"x", 1}, {"y", 1}, {"z", 1}}}}}
    };
    return openapi::BaseConfigInstance{
      "config_instance",
      "cfg_inst_123",
      "2021-01-01T00:00:00Z",
      "cli_123",
      "cfg_sch_123",
      config_instance,
    };
  };

  // set the schema file and default config file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::ConfigImpl config_impl = miru::config::ConfigImpl::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  miru::config::Config config =
    miru::config::Config(std::make_unique<miru::config::ConfigImpl>(config_impl));
  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::Agent);

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 74);
}

TEST(Config, FromAgentDefaultFile) {
  // set the response from the mock client
  test::http::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // refresh latest config instance will fail
  mock_client.refresh_latest_config_instance_func =
    []() -> openapi::BaseConfigInstance {
    throw std::runtime_error("refresh latest config instance failed");
  };

  // read the latest config instance will also fail (for whatever reason)
  mock_client.get_latest_config_instance_func = []() -> openapi::BaseConfigInstance {
    throw std::runtime_error("get latest config instance failed");
  };

  // set the schema file and default config file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::ConfigImpl config_impl = miru::config::ConfigImpl::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  miru::config::Config config =
    miru::config::Config(std::make_unique<miru::config::ConfigImpl>(config_impl));
  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::FileSystem);

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

}  // namespace test::config