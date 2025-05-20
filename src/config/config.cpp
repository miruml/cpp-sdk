// internal
#include <config/config_impl.hpp>
#include <miru/config/config.hpp>

namespace miru::config {

Config::~Config() = default;

Config::Config(std::unique_ptr<ConfigImpl> impl) : impl_(std::move(impl)) {}

Config Config::from_file(
  const std::filesystem::path& schema_file_path,
  const std::filesystem::path& config_instance_file_path
) {
  ConfigImpl impl = ConfigImpl::from_file(schema_file_path, config_instance_file_path);
  return Config(std::make_unique<ConfigImpl>(std::move(impl)));
}

Config Config::from_agent(
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  ConfigImpl impl = ConfigImpl::from_agent(schema_file_path, options);
  return Config(std::make_unique<ConfigImpl>(std::move(impl)));
}

const ConfigSource Config::get_source() const { return impl_->get_source(); }

const miru::params::Parameter& Config::root_parameter() const {
  return impl_->root_parameter();
}

}  // namespace miru::config
