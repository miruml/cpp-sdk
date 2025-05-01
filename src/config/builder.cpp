// internal
#include <config/builder.hpp>
#include <config/config_impl.hpp>

namespace miru::config {

ConfigBuilder& ConfigBuilder::with_schema_file(const miru::filesys::File& schema_file) {
  if (schema_file_.has_value()) {
    throw std::runtime_error("Schema file already set");
  }
  schema_file_ = schema_file;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_config_slug(const std::string& config_slug) {
  if (config_slug_.has_value()) {
    throw std::runtime_error("Config slug already set");
  }
  config_slug_ = config_slug;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_source(miru::config::ConfigSource source) {
  if (source_.has_value()) {
    throw std::runtime_error("Source already set");
  }
  source_ = source;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_data(const miru::params::Parameter& data) {
  if (data_.has_value()) {
    throw std::runtime_error("Data already set");
  }
  data_ = data;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_schema_digest(const std::string& schema_digest) {
  if (schema_digest_.has_value()) {
    throw std::runtime_error("Schema digest already set");
  }
  schema_digest_ = schema_digest;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_config_file(const miru::filesys::File& config_file) {
  if (config_file_.has_value()) {
    throw std::runtime_error("Config file already set");
  }
  config_file_ = config_file;
  return *this;
}

ConfigImpl ConfigBuilder::build() {
  if (!schema_file_.has_value()) {
    throw std::runtime_error("Schema file not set");
  }
  if (!config_slug_.has_value()) {
    throw std::runtime_error("Config slug not set");
  }
  if (!source_.has_value()) {
    throw std::runtime_error("Source not set");
  }
  if (!data_.has_value()) {
    throw std::runtime_error("Data not set");
  }
  if (source_ == miru::config::ConfigSource::Agent && !schema_digest_.has_value()) {
    throw std::runtime_error(
      "Schema digest not set (must be set when retrieving the config from the agent)"
    );
  }
  if (source_ == miru::config::ConfigSource::FileSystem && !config_file_.has_value()) {
    throw std::runtime_error(
      "Config file not set (must be set when retrieving the config from the file "
      "system)"
    );
  }
  return ConfigImpl(
    *schema_file_, *config_slug_, *source_, *data_, schema_digest_, config_file_
  );
}

}  // namespace miru::config