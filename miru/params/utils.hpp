#pragma once

// internal
#include <miru/params/parameter.hpp>

namespace miru::params {

// ============================== MIRU INTERFACES ============================== //

bool is_leaf(const ParameterValue& parameter);
bool is_leaf(const Parameter& parameter);

std::vector<miru::params::Parameter> list_parameters(bool leaves_only = true);

// ============================== ROS2 INTERFACES ============================== //

// The following ROS2 interfaces are pulled from the rclcpp NodeParametersInterface.

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L116

bool has_parameter(const std::string& parameter_name, bool leaves_only = true);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L144

std::vector<miru::params::Parameter> get_parameters(
    const std::vector<std::string>& parameter_names, bool leaves_only = true);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L155

miru::params::Parameter get_parameter(const std::string& name, bool leaves_only = true);

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
// ROS2 calls it list parameters but it's actually just a list of parameter names. Our
// list_parameters methods will return the actual parameter data structures

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L204

std::vector<std::string> list_parameter_names(
    const std::vector<std::string>& parameter_prefixes,
    uint64_t depth,
    bool leaves_only = true
);

}  // namespace miru::params
