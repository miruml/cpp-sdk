// internal
#include <configs/errors.hpp>
#include <configs/instance_impl.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

// ============================== READ CONFIG SLUG ================================= //
TEST(ConfigInstance, ConfigTypeSlugFoundJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  EXPECT_EQ(miru::config::read_schema_config_type_slug(schema_file), "motion-control");
}

TEST(ConfigInstance, ConfigTypeSlugFoundYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  EXPECT_EQ(miru::config::read_schema_config_type_slug(schema_file), "motion-control");
}

TEST(ConfigInstance, EmptyConfigTypeSlugJson) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-type-slug.json")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_type_slug(schema_file),
    miru::config::EmptyConfigTypeSlug
  );
}

TEST(ConfigInstance, EmptyConfigTypeSlugYaml) {
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-type-slug.yaml")
  );
  EXPECT_THROW(
    miru::config::read_schema_config_type_slug(schema_file),
    miru::config::EmptyConfigTypeSlug
  );
}

TEST(ConfigInstance, ConfigTypeSlugNotFoundJson) {
  miru::filesys::File schema_file(miru::test_utils::config_schemas_testdata_dir().file(
    "missing-config-type-slug.json"
  ));
  EXPECT_THROW(
    miru::config::read_schema_config_type_slug(schema_file),
    miru::config::ConfigTypeSlugNotFound
  );
}

TEST(ConfigInstance, ConfigTypeSlugNotFoundYaml) {
  miru::filesys::File schema_file(miru::test_utils::config_schemas_testdata_dir().file(
    "missing-config-type-slug.yaml"
  ));
  EXPECT_THROW(
    miru::config::read_schema_config_type_slug(schema_file),
    miru::config::ConfigTypeSlugNotFound
  );
}

}  // namespace test::config