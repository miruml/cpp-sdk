#pragma once

// std
#include <stdexcept>
#include <string>

namespace miru::params {

// ================================ ROS2 INTERFACES ================================ //

/// Thrown if passed parameter value is invalid.
class InvalidParameterValueException : public std::runtime_error
{
    // Inherit constructors from runtime_error.
    using std::runtime_error::runtime_error;
};

/// Thrown if requested parameter type is invalid.
/**
 * Essentially the same as rclcpp::ParameterTypeException, but with parameter
 * name in the error message.
 */
class InvalidParameterTypeException : public std::runtime_error
{
public:
    /// Construct an instance.
    /**
    * \param[in] name the name of the parameter.
    * \param[in] message custom exception message.
    */
    InvalidParameterTypeException(const std::string & name, const std::string message)
    : std::runtime_error("parameter '" + name + "' has invalid type: " + message)
    {}
};

// ================================ MIRU INTERFACES ================================ //
class ParamNotFound : public std::runtime_error {
    public:
        ParamNotFound(
            const std::string& param_path
        ) : runtime_error("Parameter '" + param_path + "' not found") {}
};

class NotAPrimitiveValue : public std::runtime_error {
    public:
        NotAPrimitiveValue(
            const std::string& param_path
        ) : runtime_error("Found object for parameter '" + param_path + "' when a primitive value (4, true, 'hello', etc.) was expected") {}
};

} // namespace miru::params