#pragma once

// std
#include <string>
#include <optional>
#include <variant>

// internal
#include <miru/filesys/file.hpp>
#include <miru/config/utils.hpp>
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

    // dump the config to a stream
    void dump(std::ostream& out, int indent = 2) const;
    void operator<<(std::ostream& out) const;

    // dump a parameter to a stream
    void dump_param(std::ostream& out, const std::string& key, int indent = 2) const;
    // dump a parameter to a stream
    void get_param(const std::string& key) const;
    // check if a parameter exists
    bool has_param(const std::string& key) const;
    // check if a parameter is null
    bool is_param_null(const std::string& key) const;
    std::vector<std::string> list_param_keys() const;

    // get a parameter from the config
    template <typename T>
    T get_param(const std::string &key);

private:
    friend class ConfigBuilder;
    Config(
        const miru::filesys::File& schema_file,
        const std::string& config_slug,
        ConfigSource source,
        const std::variant<nlohmann::json, YAML::Node>& data,
        const std::optional<std::string>& schema_digest,
        const std::optional<miru::filesys::File>& config_file
    ) 
        : schema_file_(schema_file)
        , config_slug_(config_slug)
        , source_(source)  
        , data_(data)
        , schema_digest_(schema_digest)
        , config_file_(config_file)
    {}

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

// get a parameter from the config
template <typename T>
T get_param(const std::string &key) {
    std::vector<std::string> keys = miru::utils::split(key, '/');
    if (auto json_data_ptr = std::get_if<nlohmann::json>(&data_)) {
        return miru::config::get_json_param<T>(
            *json_data_ptr, keys
        );
    } else if (auto yaml_data_ptr = std::get_if<YAML::Node>(&data_)) {
        return miru::config::get_yaml_param<T>(
            *yaml_data_ptr, keys
        );
    } else {
        throw std::runtime_error("Unable to find config data");
    }
}

class ConfigBuilder {
public:
    ConfigBuilder() {}
    ConfigBuilder& with_schema_file(const miru::filesys::File& schema_file);
    ConfigBuilder& with_config_slug(const std::string& config_slug);
    ConfigBuilder& with_source(ConfigSource source);
    ConfigBuilder& with_data(const std::variant<nlohmann::json, YAML::Node>& data);
    ConfigBuilder& with_schema_digest(const std::string& schema_digest);
    ConfigBuilder& with_config_file(const miru::filesys::File& config_file);
    Config build();

private:
    // required
    std::optional<miru::filesys::File> schema_file_;
    std::optional<std::string> config_slug_;
    std::optional<ConfigSource> source_;
    std::optional<std::variant<nlohmann::json, YAML::Node>> data_;

    // only needed if sourcing from the agent
    std::optional<std::string> schema_digest_; 

    // only needed if sourcing from the file system
    std::optional<miru::filesys::File> config_file_;
};

class ConfigSlugNotFound : public std::runtime_error {
public:
    explicit ConfigSlugNotFound(const miru::filesys::File& schema_file) 
        : std::runtime_error("Unable to find config slug in schema file '" + schema_file.abs_path().string() + "'")  {}
};

} // namespace miru::config