// internal
#include <miru/config/errors.hpp>
#include <miru/config/details/utils.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

namespace test::config {

// ============================== READ CONFIG SLUG ================================= //
TEST(Config, ConfigSlugFoundJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );
  EXPECT_EQ(miru::config::details::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, ConfigSlugFoundYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.yaml")
  );
  EXPECT_EQ(miru::config::details::read_schema_config_slug(schema_file), "motion-control");
}

TEST(Config, EmptyConfigSlugJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.json")
  );
  EXPECT_THROW(
    miru::config::details::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug
  );
}

TEST(Config, EmptyConfigSlugYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("empty-config-slug.yaml")
  );
  EXPECT_THROW(
    miru::config::details::read_schema_config_slug(schema_file), miru::config::EmptyConfigSlug
  );
}

TEST(Config, ConfigSlugNotFoundJson) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.json")
  );
  EXPECT_THROW(
    miru::config::details::read_schema_config_slug(schema_file), miru::config::ConfigSlugNotFound
  );
}

TEST(Config, ConfigSlugNotFoundYaml) {
  miru::filesys::details::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("missing-config-slug.yaml")
  );
  EXPECT_THROW(
    miru::config::details::read_schema_config_slug(schema_file), miru::config::ConfigSlugNotFound
  );
}

}  // namespace test::config