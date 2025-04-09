#pragma once

// std
#include <stdexcept>
#include <string>

namespace miru::params {

// ================================ ROS2 INTERFACES ================================ //

// The following exceptions are taken from ros2 rclcpp:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/exceptions/exceptions.hpp

/// Thrown if passed parameter value is invalid.
class InvalidParameterValue : public std::runtime_error {
  // Inherit constructors from runtime_error.
  using std::runtime_error::runtime_error;
};

/// Thrown if requested parameter type is invalid.
/**
 * Essentially the same as rclcpp::ParameterTypeException, but with parameter
 * name in the error message.
 */
class InvalidParameterType : public std::runtime_error {
 public:
  /// Construct an instance.
  /**
   * \param[in] name the name of the parameter.
   * \param[in] message custom exception message.
   */
  InvalidParameterType(const std::string& name, const std::string message)
      : std::runtime_error("parameter '" + name + "' has invalid type: " + message) {}
};

// ================================ MIRU INTERFACES ================================= //
class InvalidScalarConversion : public std::runtime_error {
 public:
  InvalidScalarConversion(const std::string& value, const std::string& dest_type,
                          const std::string& message)
      : std::runtime_error("unable to convert scalar '" + value + "' to " + dest_type +
                           ": " + message) {}
};

// ================================ EXCEPTIONS ===================================== //
class EmptyInitialization : public std::invalid_argument {
  public:
    EmptyInitialization(
      const std::string& object_to_initialize
    ) : std::invalid_argument(object_to_initialize + " must be initialized with at least one item") {}
};

class DuplicateFieldNames : public std::invalid_argument {
  public:
    DuplicateFieldNames(
      const std::string& object_to_initialize,
      const std::string& duplicate_field_name
    ) : std::invalid_argument("unable to initialize " + object_to_initialize + " with duplicate field name: " + duplicate_field_name) {}
};

class MismatchingParentNames : public std::invalid_argument {
  public:
    MismatchingParentNames(
      const std::string& object_to_initialize,
      const std::string& param1_name,
      const std::string& param1_parent_name,
      const std::string& param2_name,
      const std::string& param2_parent_name
    ) : std::invalid_argument("unable to initialize " + object_to_initialize + " with mismatching parent names. Every field must have the same parent name but field '" + param1_name + "' has parent name '" + param1_parent_name + "' while field '" + param2_name + "' has parent name '" + param2_parent_name + "'") {}
};

class InvalidArrayKeys : public std::invalid_argument {
  public:
    InvalidArrayKeys(
      const std::string& object_to_initialize,
      int index,
      const std::string& param_name
    ) : std::invalid_argument("Items in " + object_to_initialize + " must have keys that are integers in ascending order. Unable to find index '" + std::to_string(index) + "' in item '" + param_name + "'") {}
};

}  // namespace miru::params