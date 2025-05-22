// internal
#include <configs/instance_impl.hpp>
#include <miru/configs/instance.hpp>

namespace miru::config {

ConfigInstance::~ConfigInstance() = default;

ConfigInstance::ConfigInstance(std::unique_ptr<ConfigInstanceImpl> impl)
  : impl_(std::move(impl)) {}

ConfigInstance ConfigInstance::from_file(
  const std::filesystem::path& schema_file_path,
  const std::filesystem::path& instance_file_path
) {
  ConfigInstanceImpl impl =
    ConfigInstanceImpl::from_file(schema_file_path, instance_file_path);
  return ConfigInstance(std::make_unique<ConfigInstanceImpl>(std::move(impl)));
}

ConfigInstance ConfigInstance::from_agent(
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  ConfigInstanceImpl impl = ConfigInstanceImpl::from_agent(schema_file_path, options);
  return ConfigInstance(std::make_unique<ConfigInstanceImpl>(std::move(impl)));
}

const ConfigInstanceSource ConfigInstance::get_source() const {
  return impl_->get_source();
}

const miru::params::Parameter& ConfigInstance::root_parameter() const {
  return impl_->root_parameter();
}

}  // namespace miru::config
