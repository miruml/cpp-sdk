#pragma once

// std
#include <string>
#include <optional>
#include <variant>

// internal
#include <miru/filesys/file.hpp>
#include <miru/filesys/json.hpp>
#include <miru/filesys/yaml.hpp>
#include <miru/utils.hpp>

// external
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

namespace miru::config {

// used to determine where to source the config from
enum class ConfigSource {
    Agent,
    FileSystem,
};

// Config class
class Config {

public:
    // get a parameter from the config
    template <typename T>
    T get_param(const std::string &key) {
        std::vector<std::string> keys = miru::utils::split(key, '/');
        if (auto json_data_ptr = std::get_if<nlohmann::json>(&data_)) {
            return miru::filesys::get_json_param<T>(
                *json_data_ptr, keys
            );
        } else if (auto yaml_data_ptr = std::get_if<YAML::Node>(&data_)) {
            return miru::filesys::get_yaml_param<T>(
                *yaml_data_ptr, keys
            );
        } else {
            throw std::runtime_error("Config data is not of a supported type");
        }
    }

protected:
    Config() 
        : schema_file_("notset")
        , config_slug_("")
        , source_(ConfigSource::Agent)  
    {}

private:
    // Initialize the config from a file system source. The config will read its
    // configuration and schema file from the file system.
    static Config from_file(
        const std::string& schema_file_path,
        const std::string& config_file_path
    );

    // Initialize the config from an agent source. The config will read its
    // configuration and schema file from the on-device agent.
    static Config from_agent(
        const std::string& schema_file_path
    );

    // required
    miru::filesys::File schema_file_;
    std::string config_slug_;
    ConfigSource source_;
    std::variant<nlohmann::json, YAML::Node> data_;

    // only needed if sourcing from the agent
    std::optional<std::string> schema_digest_; 

    // only needed if sourcing from the file system
    std::optional<miru::filesys::File> config_file_;
};

} // namespace miru::config