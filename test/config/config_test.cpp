// internal
#include <miru/http/details/models/RefreshLatestConcreteConfigRequest.h>

#include <miru/config/config.hpp>
#include <miru/config/errors.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/http/mock.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

namespace openapi = org::openapitools::server::model;

// ============================== READ CONFIG SLUG ================================= //
TEST(Config, ConfigSlugFoundJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  EXPECT_EQ(miru::config::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, ConfigSlugFoundYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  EXPECT_EQ(miru::config::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, EmptyConfigSlugJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.json")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug
  );
}

TEST(Config, EmptyConfigSlugYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.yaml")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug
  );
}

TEST(Config, ConfigSlugNotFoundJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.json")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::ConfigSlugNotFound
  );
}

TEST(Config, ConfigSlugNotFoundYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.yaml")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_slug(schema_file), miru::config::ConfigSlugNotFound
  );
}

// ================================ FROM FILE SYSTEM =============================== //
TEST(Config, FromFileSystemJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::details::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::details::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemJsonRos2) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::details::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.json")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto ros2 = config.ros2();
  auto speed = ros2.get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(Config, FromFileSystemYamlRos2) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::details::File config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::Config config = miru::config::Config::from_file(
    schema_file.abs_path().string(), config_file.abs_path().string()
  );

  auto speed = config.ros2().get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

// =================================== FROM AGENT ================================== //
TEST(Config, FromAgentRefreshSuccess) {
  // set the response from the mock client
  miru::http::details::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  mock_client.refresh_latest_concrete_config_func = []() {
    nlohmann::json concrete_config = {
      {"speed", 89},
      {"features", {{"spin", true}, {"jump", false}, {"backflip", false}}},
      {"accelerometer",
       {{"id", "123"},
        {"offsets", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"scaling_factor", {{"x", 1}, {"y", 1}, {"z", 1}}}}}
    };
    return openapi::BaseConcreteConfig{
      "concrete_config",
      "cncr_cfg_123",
      "2021-01-01T00:00:00Z",
      "cli_123",
      "cfg_sch_123",
      concrete_config,
    };
  };

  // set the schema file and default config file
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::details::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::Config config = miru::config::Config::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::Agent);

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 89);
}

TEST(Config, FromAgentRefreshFailureGetSuccess) {
  // set the response from the mock client
  miru::http::details::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // refresh latest concrete config will fail
  mock_client.refresh_latest_concrete_config_func =
    []() -> openapi::BaseConcreteConfig {
    throw std::runtime_error("refresh latest concrete config failed");
  };

  // read the latest concrete config will succeed
  mock_client.get_latest_concrete_config_func = []() {
    nlohmann::json concrete_config = {
      {"speed", 74},
      {"features", {{"spin", true}, {"jump", false}, {"backflip", false}}},
      {"accelerometer",
       {{"id", "123"},
        {"offsets", {{"x", 0}, {"y", 0}, {"z", 0}}},
        {"scaling_factor", {{"x", 1}, {"y", 1}, {"z", 1}}}}}
    };
    return openapi::BaseConcreteConfig{
      "concrete_config",
      "cncr_cfg_123",
      "2021-01-01T00:00:00Z",
      "cli_123",
      "cfg_sch_123",
      concrete_config,
    };
  };

  // set the schema file and default config file
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::details::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::Config config = miru::config::Config::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::Agent);

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 74);
}

TEST(Config, FromAgentDefaultFile) {
  // set the response from the mock client
  miru::http::details::MockBackendClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // refresh latest concrete config will fail
  mock_client.refresh_latest_concrete_config_func =
    []() -> openapi::BaseConcreteConfig {
    throw std::runtime_error("refresh latest concrete config failed");
  };

  // read the latest concrete config will also fail (for whatever reason)
  mock_client.get_latest_concrete_config_func = []() -> openapi::BaseConcreteConfig {
    throw std::runtime_error("get latest concrete config failed");
  };

  // set the schema file and default config file
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::details::File default_config_file(
    miru::test_utils::config_data_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_config_path = default_config_file.abs_path().string();
  miru::config::Config config = miru::config::Config::from_agent(
    mock_client, schema_file.abs_path().string(), options
  );
  EXPECT_EQ(config.get_source(), miru::config::ConfigSource::FileSystem);

  auto speed = miru::query::get_param(config, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

}  // namespace test::config