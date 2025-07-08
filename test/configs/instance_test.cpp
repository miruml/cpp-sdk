// internal
#include <http/models/RefreshLatestConfigInstanceRequest.h>

#include <configs/errors.hpp>
#include <configs/instance_impl.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/http/mock.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

namespace openapi = org::openapitools::server::model;

// ================================ FROM FILE SYSTEM =============================== //
TEST(ConfigInstance, FromFileSystemJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::File instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.json")
  );
  miru::config::ConfigInstance config_instance =
    miru::config::ConfigInstance::from_file(
      schema_file.abs_path().string(), instance_file.abs_path().string()
    );

  auto speed = miru::query::get_param(config_instance, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(ConfigInstance, FromFileSystemYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.yaml")
  );
  miru::config::ConfigInstance config_instance =
    miru::config::ConfigInstance::from_file(
      schema_file.abs_path().string(), instance_file.abs_path().string()
    );

  auto speed = miru::query::get_param(config_instance, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(ConfigInstance, FromFileSystemJsonRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  miru::filesys::File instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.json")
  );
  miru::config::ConfigInstance config_instance =
    miru::config::ConfigInstance::from_file(
      schema_file.abs_path().string(), instance_file.abs_path().string()
    );

  auto ROS2NodeI = miru::query::ROS2NodeI(config_instance);
  auto speed = ROS2NodeI.get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(ConfigInstance, FromFileSystemYamlRos2) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.yaml")
  );
  miru::config::ConfigInstance config_instance =
    miru::config::ConfigInstance::from_file(
      schema_file.abs_path().string(), instance_file.abs_path().string()
    );

  auto ROS2NodeI = miru::query::ROS2NodeI(config_instance);
  auto speed = ROS2NodeI.get_parameter("motion-control.speed");
  EXPECT_EQ(speed.as<int>(), 15);
}

// =================================== FROM AGENT ================================== //
TEST(ConfigInstance, FromAgentSuccess_NoDefaultInstanceFile) {
  // set the response from the mock client
  test::http::MockAgentClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  mock_client.get_deployed_config_instance_func = []() {
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

  // set the schema file and default config instance file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::config::ConfigInstanceImpl config_impl =
    miru::config::ConfigInstanceImpl::from_agent(
      mock_client, schema_file.abs_path().string()
    );
  miru::config::ConfigInstance config_instance = miru::config::ConfigInstance(
    std::make_unique<miru::config::ConfigInstanceImpl>(config_impl)
  );

  EXPECT_EQ(config_instance.get_source(), miru::config::ConfigInstanceSource::Agent);
  auto speed = miru::query::get_param(config_instance, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 89);
}

TEST(ConfigInstance, FromAgentSuccess_DefaultInstanceFile) {
  // set the response from the mock client
  test::http::MockAgentClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  mock_client.get_deployed_config_instance_func = []() {
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

  // set the schema file and default config instance file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_instance_file_path = default_instance_file.abs_path();
  miru::config::ConfigInstanceImpl config_impl =
    miru::config::ConfigInstanceImpl::from_agent(
      mock_client, schema_file.abs_path().string(), options
    );
  miru::config::ConfigInstance config_instance = miru::config::ConfigInstance(
    std::make_unique<miru::config::ConfigInstanceImpl>(config_impl)
  );

  EXPECT_EQ(config_instance.get_source(), miru::config::ConfigInstanceSource::Agent);
  auto speed = miru::query::get_param(config_instance, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 89);
}

TEST(ConfigInstance, FromAgentFailure_DefaultFileSuccess) {
  // set the response from the mock client
  test::http::MockAgentClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // read the deployed config instance will fail (for whatever reason)
  mock_client.get_deployed_config_instance_func = []() -> openapi::BaseConfigInstance {
    throw std::runtime_error("get deployed config instance failed");
  };

  // set the schema file and default config instance file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_instance_file(
    miru::test_utils::config_instances_testdata_dir().file("motion-control.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_instance_file_path = default_instance_file.abs_path();
  miru::config::ConfigInstanceImpl config_impl =
    miru::config::ConfigInstanceImpl::from_agent(
      mock_client, schema_file.abs_path().string(), options
    );
  miru::config::ConfigInstance config_instance = miru::config::ConfigInstance(
    std::make_unique<miru::config::ConfigInstanceImpl>(config_impl)
  );
  EXPECT_EQ(
    config_instance.get_source(), miru::config::ConfigInstanceSource::FileSystem
  );

  auto speed = miru::query::get_param(config_instance, "motion-control.speed");

  EXPECT_EQ(speed.as<int>(), 15);
}

TEST(ConfigInstance, FromAgentFailure_DefaultFileFailure) {
  // set the response from the mock client
  test::http::MockAgentClient mock_client;
  mock_client.hash_schema_func = []() { return "sha256:a1b2c3d4e5f6g7h8i9j0k1l2"; };

  // read the deployed config instance will fail (for whatever reason)
  mock_client.get_deployed_config_instance_func = []() -> openapi::BaseConfigInstance {
    throw std::runtime_error("get deployed config instance failed");
  };

  // set the schema file and default config instance file
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  miru::filesys::File default_instance_file(
    miru::test_utils::config_instances_testdata_dir().file("doesnt-exist.yaml")
  );
  miru::config::FromAgentOptions options;
  options.default_instance_file_path = default_instance_file.abs_path();
  EXPECT_THROW(
    miru::config::ConfigInstanceImpl::from_agent(
      mock_client, schema_file.abs_path().string(), options
    ),
    miru::config::GetDeployedConfigInstanceError
  );

  EXPECT_THROW(
    miru::config::ConfigInstanceImpl::from_agent(
      mock_client, schema_file.abs_path().string(), options
    ),
    miru::config::GetDeployedConfigInstanceError
  );
}

}  // namespace test::config