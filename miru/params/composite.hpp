#pragma once 

// std
#include <string>
#include <vector>
#include <variant>

// internal
#include <miru/params/parameter.hpp>

namespace miru::params {

class ArrayObject;

class Object {
public:
    // CompositeParameter(const std::string& key, const CompositeParameterType& type);

    // // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
    // // exposing these to users initially

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L55

    // /// Construct with an empty key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    // CompositeParameter();

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L59

    // /// Construct with given key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    // explicit CompositeParameter(const std::string & key);

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L63

    // /// Construct with given key and given parameter value.
    // CompositeParameter(const std::string & key, const ParameterValue & value);

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L67

    // /// Construct with given name and given parameter value.
    // template<typename ValueTypeT>
    // CompositeParameter(const std::string & name, ValueTypeT value)
    // : CompositeParameter(name, ParameterValue(value))
    // {}

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L73
    // explicit Parameter(const rclcpp::node_interfaces::ParameterInfo &
    // parameter_info); is not supported since it uses ROS2 specific interface which has
    // no miru equivalent

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
        bool with_composite_params = false
    ) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L144

    std::vector<Parameter>
    get_parameters(const std::vector<std::string> & parameter_names) const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/node_interfaces/node_parameters_interface.hpp#L155

    Parameter get_parameter(const std::string & name) const;

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
    list_parameter_names(const std::vector<std::string> & parameter_prefixes) const;

    // ============================== MIRU INTERFACES ============================== //

    std::vector<std::variant<Object, ArrayObject>> get_composites() const;

    std::vector<Object> get_objects() const;

    std::vector<ArrayObject> get_object_arrays() const;

    std::vector<Parameter>
    list_parameters(bool with_composite_params = false) const;

    template<typename T>
    decltype(auto)
    get_value() const;

    // Get the key of the parameter
    std::string get_name() const;
    std::string get_key() const;

private:
    std::string name_;
    std::vector<Parameter> primitives_;
    std::vector<Object> composites_;
};

class ArrayObjectParameter {
public:
    // CompositeParameter(const std::string& key, const CompositeParameterType& type);

    // // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
    // // exposing these to users initially

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L55

    // /// Construct with an empty key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    // CompositeParameter();

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L59

    // /// Construct with given key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    // explicit CompositeParameter(const std::string & key);

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L63

    // /// Construct with given key and given parameter value.
    // CompositeParameter(const std::string & key, const ParameterValue & value);

    // // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L67

    // /// Construct with given name and given parameter value.
    // template<typename ValueTypeT>
    // CompositeParameter(const std::string & name, ValueTypeT value)
    // : CompositeParameter(name, ParameterValue(value))
    // {}

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L73
    // explicit Parameter(const rclcpp::node_interfaces::ParameterInfo &
    // parameter_info); is not supported since it uses ROS2 specific interface which has
    // no miru equivalent

    // ============================== MIRU INTERFACES ============================== //

    std::string get_name() const;
    std::string get_key() const;

    const std::vector<Object>& get_value() const;

    template<typename T>
    decltype(auto)
    get_value() const;

private:
    std::string key_;
    std::vector<Object> value_;
};

} // namespace miru::params