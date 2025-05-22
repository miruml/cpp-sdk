// internal
#include <configs/instance_builder.hpp>
#include <configs/instance_impl.hpp>

namespace miru::config {

ConfigInstanceBuilder& ConfigInstanceBuilder::with_schema_file(const miru::filesys::File& schema_file) {
  if (schema_file_.has_value()) {
    throw std::runtime_error("Schema file already set");
  }
  schema_file_ = schema_file;
  return *this;
}

ConfigInstanceBuilder& ConfigInstanceBuilder::with_config_type_slug(const std::string& config_type_slug) {
  if (config_type_slug_.has_value()) {
    throw std::runtime_error("Config type slug already set");
  }
  config_type_slug_ = config_type_slug;
  return *this;
}

ConfigInstanceBuilder& ConfigInstanceBuilder::with_source(miru::config::ConfigInstanceSource source) {
  if (source_.has_value()) {
    throw std::runtime_error("Source already set");
  }
  source_ = source;
  return *this;
}

ConfigInstanceBuilder& ConfigInstanceBuilder::with_data(const miru::params::Parameter& data) {
  if (data_.has_value()) {
    throw std::runtime_error("Data already set");
  }
  data_ = data;
  return *this;
}

ConfigInstanceBuilder& ConfigInstanceBuilder::with_schema_digest(const std::string& schema_digest) {
  if (schema_digest_.has_value()) {
    throw std::runtime_error("Schema digest already set");
  }
  schema_digest_ = schema_digest;
  return *this;
}

ConfigInstanceBuilder& ConfigInstanceBuilder::with_config_instance_file(const miru::filesys::File& config_instance_file) {
  if (config_instance_file_.has_value()) {
    throw std::runtime_error("Config instance file already set");
  }
  config_instance_file_ = config_instance_file;
  return *this;
}

ConfigInstanceImpl ConfigInstanceBuilder::build() {
  if (!schema_file_.has_value()) {
    throw std::runtime_error("Schema file not set");
  }
  if (!config_type_slug_.has_value()) {
    throw std::runtime_error("Config type slug not set");
  }
  if (!source_.has_value()) {
    throw std::runtime_error("Source not set");
  }
  if (!data_.has_value()) {
    throw std::runtime_error("Data not set");
  }
  if (source_ == miru::config::ConfigInstanceSource::Agent && !schema_digest_.has_value()) {
    throw std::runtime_error(
      "Schema digest not set (must be set when retrieving the config instance from the agent)"
    );
  }
  if (source_ == miru::config::ConfigInstanceSource::FileSystem && !config_instance_file_.has_value()) {
    throw std::runtime_error(
      "Config instance file not set (must be set when retrieving the config instance from the file "
      "system)"
    );
  }
  return ConfigInstanceImpl(
    *schema_file_, *config_type_slug_, *source_, *data_, schema_digest_, *config_instance_file_
  );
}

}  // namespace miru::config