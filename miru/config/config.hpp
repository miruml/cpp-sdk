#pragma once

// std
#include <string>
#include <optional>

// internal
#include <miru/filesys/file.hpp>
#include <miru/params/parameter.hpp>
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

    // ============================== ROS2 INTERFACES ============================== //

    // The following ROS2 interfaces are pulled from the rclcpp NodeParametersInterface.
    // ROS2 doesn't have a notion of a CompositeObjectParameter. The ParameterClient is
    // actually our version of 'ConfigClient' but these methods are similar in form to
    // the ParameterClient methods in that a ComposeObjectParameter acts as a container
    // for parameters. Although a ComposeObjectParameter has less functionality since it
    // isn't a client but a simple data structure for parameters

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L116

    bool has_parameter(
        const std::string & parameter_name,
        bool primitives_only = true
    ) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L144

    std::vector<miru::params::Parameter>
    get_parameters(
        const std::vector<std::string> & parameter_names,
        bool primitives_only = true
    ) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L155

    miru::params::Parameter get_parameter(
        const std::string & name,
        bool primitives_only = true
    ) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L194
    // std::vector<rcl_interfaces::msg::ParameterDescriptor>
    // describe_parameters(const std::vector<std::string> & names) const; is not supported
    // since it returns a Descriptor, of which miru has no equivalent

    // we are not going to support type information in the public interface right now
    // since yaml does not support strongly typed information ("4" vs 4) and we have no
    // quick way of doing so ourself. ROS2 rolled their own parser to grab type
    // information. We will be storing a SCALAR type and I don't want to expose this
    // to users since it will likely confuse them when an integer is parsed as a scalar
    // and not an integer when using yaml.

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L199

    // std::vector<ParameterType>
    // get_parameter_types(const std::vector<std::string> & parameter_names);

    // for the list_parameter_names method, we're altering the data structure a bit.
    // ROS2 calls it list parameters but it's actually just a list of parameter names. Our list_parameters methods will return the actual parameter data structures

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L204

    std::vector<std::string>
    list_parameter_names(
        const std::vector<std::string> & parameter_prefixes,
        bool primitives_only = true
    ) const;

    // ============================== MIRU INTERFACES ============================== //

    std::vector<miru::params::Parameter>
    list_parameters(bool primitives_only = true) const;


private:
    friend class ConfigBuilder;
    Config(
        const miru::filesys::File& schema_file,
        const std::string& config_slug,
        ConfigSource source,
        const miru::params::Parameter& data,
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
    miru::params::Parameter data_;

    // only needed if sourcing from the agent
    std::optional<std::string> schema_digest_; 

    // only needed if sourcing from the file system
    std::optional<miru::filesys::File> config_file_;
};

class ConfigBuilder {
public:
    ConfigBuilder() {}
    ConfigBuilder& with_schema_file(const miru::filesys::File& schema_file);
    ConfigBuilder& with_config_slug(const std::string& config_slug);
    ConfigBuilder& with_source(ConfigSource source);
    ConfigBuilder& with_data(const miru::params::Parameter& data);
    ConfigBuilder& with_schema_digest(const std::string& schema_digest);
    ConfigBuilder& with_config_file(const miru::filesys::File& config_file);
    Config build();

private:
    // required
    std::optional<miru::filesys::File> schema_file_;
    std::optional<std::string> config_slug_;
    std::optional<ConfigSource> source_;
    std::optional<miru::params::Parameter> data_;

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