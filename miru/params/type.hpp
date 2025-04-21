#pragma once

// std
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

// miru
#include <miru/errors.hpp>

namespace miru::params {

enum ParameterType : uint8_t {

  // ============================== ROS2 INTERFACES ============================== //
  // The following types are ALL the ros2 interface types for ros params. We will add
  // some additions to this list but our parameter interface will be a superset of
  // ros params
  // rclcpp:
  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L33
  // rcl_interfaces:
  // https://github.com/ros2/rcl_interfaces/blob/rolling/rcl_interfaces/msg/ParameterType.msg

  PARAMETER_NOT_SET = 0,
  PARAMETER_BOOL = 1,
  PARAMETER_INTEGER = 2,
  PARAMETER_DOUBLE = 3,
  PARAMETER_STRING = 4,

  // json schema does not support binary types directly (just a string). It is possible
  // to support it by leveraging the contentEncoding field for strings but the json
  // schema libraries in c++ aren't great so we'd probably be forced to write our own
  // parser or fork a new one from existing libraries. Needless to say, byte arrays
  // simply won't be supported for now.
  // PARAMETER_BYTE_ARRAY = 5,

  PARAMETER_BOOL_ARRAY = 6,
  PARAMETER_INTEGER_ARRAY = 7,
  PARAMETER_DOUBLE_ARRAY = 8,
  PARAMETER_STRING_ARRAY = 9,

  // ============================== MIRU INTERFACES ============================== //
  // we'll start our own types here starting at 128 (max is (2^8)-1 = 255 since
  // uint8_t is 8 bits) so that we can add new ros param types without breaking
  // existing code in the future

  PARAMETER_NULL = 128,
  // yaml scalar since yaml-cpp does not support strongly typed information ("4" vs 4)
  PARAMETER_SCALAR = 129,
  PARAMETER_SCALAR_ARRAY = 130,
  PARAMETER_NESTED_ARRAY = 131,
  PARAMETER_MAP = 132,
  PARAMETER_MAP_ARRAY = 133,
};

// ================================ ROS2 INTERFACES ================================ //

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L48

/// Return the name of a parameter type
std::string to_string(ParameterType type);

std::ostream &operator<<(std::ostream &os, const ParameterType &type);

/// Indicate the parameter type does not match the expected type.
class InvalidParameterValueTypeError : public std::runtime_error {
 public:
  /// Construct an instance.
  /**
   * \param[in] expected the expected parameter type.
   * \param[in] actual the actual parameter type.
   */
  InvalidParameterValueTypeError(ParameterType expected, ParameterType actual)
    : std::runtime_error(
        "expected [" + to_string(expected) + "] got [" + to_string(actual) + "]"
      ) {}
};

}  // namespace miru::params