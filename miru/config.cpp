// internal
#include <miru/config.hpp>

// external
#include <nlohmann/json.hpp>

namespace miru::config {

Config Config::from_file(
    const std::string& schema_file_path,
    const std::string& config_file_path
) {
    Config config;

    // read the schema file and extract the config slug
    config.schema_file_ = miru::filesys::File(schema_file_path);
    nlohmann::json schema_content = config.schema_file_.read_json();

    // extract the config slug
    config.config_slug_ = schema_content["config_slug"];
    if (config.config_slug_.empty()) {
        throw std::runtime_error("config_slug not found in schema file");
    }

    // read the config file
    config.config_file_ = miru::filesys::File(config_file_path);
    config.config_file_->assert_exists();
    return config;
}

} // namespace miru::config