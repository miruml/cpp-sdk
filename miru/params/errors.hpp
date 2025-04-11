#pragma once

// std
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// internal
#include <miru/errors.hpp>

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
  InvalidParameterType(
    const std::string& name,
    const std::string& message,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(name, message, trace)) {}

  static std::string format_message(
    const std::string& name,
    const std::string& message,
    const errors::ErrorTrace& trace
  ) {
    return "parameter '" + name + "' has invalid type: " + message + errors::format_source_location(trace);
  }
};

#define THROW_INVALID_PARAMETER_TYPE(name, message) \
    throw InvalidParameterType(name, message, ERROR_TRACE)

// ================================ MIRU INTERFACES ================================= //
class InvalidScalarConversion : public std::runtime_error {
 public:
  InvalidScalarConversion(
    const std::string& value,
    const std::string& dest_type,
    const std::string& message,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(value, dest_type, message, trace)) {}

  static std::string format_message(
    const std::string& value,
    const std::string& dest_type,
    const std::string& message,
    const errors::ErrorTrace& trace
  ) {
    return "unable to convert scalar '" + value + "' to " + dest_type + ": " + message + errors::format_source_location(trace);
  }
};

#define THROW_INVALID_SCALAR_CONVERSION(value, dest_type, message) \
    throw InvalidScalarConversion(value, dest_type, message, ERROR_TRACE)

// ================================ EXCEPTIONS ===================================== //
class EmptyInitialization : public std::invalid_argument {
 public:
  EmptyInitialization(
    const std::string& object_to_initialize,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const errors::ErrorTrace& trace
  ) {
    return object_to_initialize + " must be initialized with at least one item" + errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_INITIALIZATION(object_to_initialize) \
    throw EmptyInitialization(object_to_initialize, ERROR_TRACE)

class DuplicateFieldNames : public std::invalid_argument {
 public:
  DuplicateFieldNames(
    const std::string& object_to_initialize,
    const std::string& duplicate_field_name,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(
      format_message(
        object_to_initialize,
        duplicate_field_name,
        trace
      )
    ) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& duplicate_field_name,
    const errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with duplicate field name: " + duplicate_field_name + errors::format_source_location(trace);
  }
};

#define THROW_DUPLICATE_FIELD_NAMES(object_to_initialize, duplicate_field_name) \
    throw DuplicateFieldNames(object_to_initialize, duplicate_field_name, ERROR_TRACE)

class MismatchingParentNames : public std::invalid_argument {
 public:
  MismatchingParentNames(
    const std::string& object_to_initialize,
    const std::string& param1_name,
    const std::string& param1_parent_name,
    const std::string& param2_name,
    const std::string& param2_parent_name,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, param1_name, param1_parent_name, param2_name, param2_parent_name, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& param1_name,
    const std::string& param1_parent_name,
    const std::string& param2_name,
    const std::string& param2_parent_name,
    const errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with mismatching parent names. Every field must have "
           "the same parent name but field '" +
           param1_name + "' has parent name '" + param1_parent_name +
           "' while field '" + param2_name + "' has parent name '" +
           param2_parent_name + "'" + errors::format_source_location(trace);
  }
};

#define THROW_MISMATCHING_PARENT_NAMES(object_to_initialize, param1_name, param1_parent_name, param2_name, param2_parent_name) \
    throw MismatchingParentNames(object_to_initialize, param1_name, param1_parent_name, param2_name, param2_parent_name, ERROR_TRACE)

class EmptyParentName : public std::invalid_argument {
 public:
  EmptyParentName(
    const std::string& object_to_initialize,
    const std::string& param_name,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, param_name, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& param_name,
    const errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with empty parent name. Every field must have a parent name but field '" +
           param_name + "' has an empty parent name" + errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_PARENT_NAME(object_to_initialize, param_name) \
    throw EmptyParentName(object_to_initialize, param_name, ERROR_TRACE)

class InvalidArrayKeys : public std::invalid_argument {
 public:
  InvalidArrayKeys(
    const std::string& object_to_initialize,
    int index,
    const std::vector<std::string>& item_names,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, index, item_names, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    int index,
    const std::vector<std::string>& item_names,
    const errors::ErrorTrace& trace
  ) {
    std::stringstream ss;
    ss << "[";
    bool is_first = true;
    for (const auto& item : item_names) {
    if (!is_first) {
        ss << ", ";
      }
      ss << item;
      is_first = false;
    }
    ss << "]";
    return "items in " + object_to_initialize +
           " must have keys that are integers in ascending order. "
           "Unable to find index '" + std::to_string(index) + "' in items '" + ss.str() + "'" + errors::format_source_location(trace);
  }
};

#define THROW_INVALID_ARRAY_KEYS(object_to_initialize, index, item_names) \
    throw InvalidArrayKeys(object_to_initialize, index, item_names, ERROR_TRACE)

class ChildParentNameMismatch : public std::invalid_argument {
 public:
  ChildParentNameMismatch(
    const std::string& object_to_initialize,
    const std::string& child_name,
    const std::string& child_parent_name,
    const std::string& parent_name,
    const errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, child_name, child_parent_name, parent_name, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& child_name,
    const std::string& child_parent_name,
    const std::string& parent_name,
    const errors::ErrorTrace& trace
  ) {
    return "child parameter with name '" + child_name +
           "' must have its parent parameter name '" + parent_name +
           "' as a prefix but has parent name '" +
           child_parent_name + "'" + errors::format_source_location(trace);
  }
};

#define THROW_CHILD_PARENT_NAME_MISMATCH(object_to_initialize, child_name, child_parent_name, parent_name) \
    throw ChildParentNameMismatch(object_to_initialize, child_name, child_parent_name, parent_name, ERROR_TRACE)

}  // namespace miru::params