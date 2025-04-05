#pragma once

// std
#include <string>

// internal
#include <miru/utils.hpp>
#include <miru/params/value.hpp>

// external
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

namespace miru::params {

// primitive parameter class
class Parameter {

    // ============================== ROS2 INTERFACES ============================== // 

    // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
    // exposing these to users initially

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L55

    /// Construct with an empty key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    Parameter();

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L59

    /// Construct with given key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    explicit Parameter(const std::string & key);

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L63

    /// Construct with given key and given parameter value.
    Parameter(const std::string & key, const ParameterValue & value);

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L67

    /// Construct with given name and given parameter value.
    template<typename ValueTypeT>
    Parameter(const std::string & name, ValueTypeT value)
    : Parameter(name, ParameterValue(value))
    {}

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L73
    // explicit Parameter(const rclcpp::node_interfaces::ParameterInfo &
    // parameter_info); is not supported since it uses ROS2 specific interface which has
    // no miru equivalent


    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L76

    /// Equal operator
    bool operator==(const Parameter& other) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L81

    /// Not equal operator
    bool operator!=(const Parameter& other) const;

    // we are not going to support type information in the public interface right now
    // since yaml does not support strongly typed information ("4" vs 4) and we have no
    // quick way of doing so ourself. ROS2 rolled their own parser to grab type
    // information. We will be storing a SCALAR type and I don't want to expose this
    // to users since it will likely confuse them when an integer is parsed as a scalar
    // and not an integer when using yaml.

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L86

    // /// Get the type of the parameter
    // ParameterType get_type() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L92

    // /// Get the name of the type of the parameter
    // std::string get_type_name() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L97

    /// Get the full name of the parameter from the root of the config tree
    std::string get_name() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L101
    // get_value_message() is not supported since it returns an
    // rcl_interfaces::msg::ParameterValue type

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L106
    // get_parameter_value() is not supported since it returns a const rclcpp::ParameterValue &.
    // we *could* support this in the future but I'm not inclined to do so right now.
    // we'll do it in the future if needed

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L115

    /// Get the value of parameter as using the given ParameterType as a template argument
    template<ParameterType ParamT>
    decltype(auto)
    get_value() const; 

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L122

    /// Get the value of parameter as using the given c++ type as a template argument 
    template<typename T>
    decltype(auto)
    get_value() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L127

    /// Get the value of parameter as a bool
    bool as_bool() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L135

    /// Get the value of parameter as an integer
    int64_t as_int() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L143

    /// Get the value of parameter as a double
    double as_double() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L151

    /// Get the value of parameter as a string
    const std::string& as_string() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L159

    /// Get the value of parameter as a byte array (vector<uint8_t>)
    const std::vector<uint8_t>& as_byte_array() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L167

    /// Get the value of parameter as a bool array (vector<bool>)
    const std::vector<bool>& as_bool_array() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L175

    /// Get the value of parameter as an integer array (vector<int64_t>)
    const std::vector<int64_t>& as_integer_array() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L183

    /// Get the value of parameter as a double array (vector<double>)
    const std::vector<double>& as_double_array() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L191

    /// Get the value of parameter as a string array (vector<std::string>)
    const std::vector<std::string>& as_string_array() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L199
    // from_parameter_msg() is not supported since it takes a
    // rcl_interfaces::msg::Parameter type as an argument

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L204
    // to_parameter_msg() is not supported since it returns a rcl_interfaces::msg::Parameter type

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L209

    /// Get the value of parameter as a string
    std::string value_to_string() const;

    // ============================== MIRU INTERFACES ============================== // 

    /// Get the key of the parameter (the final part of the name)
    std::string get_key() const;

    /// Check if the parameter is null
    bool is_null() const;
    bool is_scalar() const;
    bool is_array() const;

    private:
        ParameterType type_;
        std::string key_;
        ParameterValue value_; 
};

// ================================ ROS2 INTERFACES ================================ //
// We are not supporting the to_json_dict_entry function. This is a cascade effect of
// not being able to support support type information in the public interface right now
// since yaml does not support strongly typed information ("4" vs 4) and we have no
// quick way of doing so ourself. ROS2 rolled their own parser to grab type information.
// We will be storing a SCALAR type and I don't want to expose this to users since it
// will likely confuse them when an integer is parsed as a scalar and not an integer
// when using yaml.

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L219

// std::string _to_json_dict_entry(const Parameter & param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L224

std::ostream& operator<<(std::ostream & os, const Parameter& param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L228

std::ostream& operator<<(std::ostream & os, const std::vector<Parameter>& parameters);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L287
// to_string(const rclcpp::Parameter & param) is not supported since it returns a
// std::string which is not recommended syntax in c++ (should not define std interface
// manually)

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L291
// to_string(const std::vector<rclcpp::Parameter> & parameters) is not supported since
// it returns a std::string which is not recommended syntax in c++ (should not define
// std interface manually)

} // namespace miru::params