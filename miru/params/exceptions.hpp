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

}  // namespace miru::params