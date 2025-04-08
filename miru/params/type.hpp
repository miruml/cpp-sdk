#pragma once

// std
#include <cstdint>
#include <string>
#include <stdexcept>
#include <iostream>
#include <type_traits>
#include <limits>

// miru
#include <miru/params/exceptions.hpp>

namespace miru::params {

enum ParameterType : uint8_t {

    // ============================== ROS2 INTERFACES ============================== // 
    // The following types are ALL the ros2 interface types for ros params. We will add
    // some additions to this list but our parameter interface will be a superset of
    // ros params
    // rclcpp: https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L33
    // rcl_interfaces: https://github.com/ros2/rcl_interfaces/blob/rolling/rcl_interfaces/msg/ParameterType.msg

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
    PARAMETER_OBJECT = 132,
    PARAMETER_OBJECT_ARRAY = 133,
};

// ================================ ROS2 INTERFACES ================================ //

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L48

/// Return the name of a parameter type
std::string to_string(ParameterType type);

std::ostream &operator<<(std::ostream & os, const ParameterType & type);

/// Indicate the parameter type does not match the expected type.
class InvalidParameterValueType : public std::runtime_error {
public:
    /// Construct an instance.
    /**
    * \param[in] expected the expected parameter type.
    * \param[in] actual the actual parameter type.
    */
    InvalidParameterValueType(ParameterType expected, ParameterType actual)
    : std::runtime_error("expected [" + to_string(expected) + "] got [" + to_string(actual) + "]")
    {}
};

template<typename type>  constexpr
typename std::enable_if<
    std::is_integral<type>::value && !std::is_same<type, bool>::value, 
    type>::type
cast_int64_to(const int64_t & value) {
    // need to handle unsigned types separately
    if (std::is_unsigned<type>::value && value < 0) {
        throw InvalidTypeCast(
            std::to_string(value),
            "int64_t",
            "integer (type '" + std::string(typeid(type).name()) + "')",
            "value is negative for unsigned type"
        );
    }

    // check for overflow with the target integer type
    if (value > std::numeric_limits<type>::max() || 
        value < std::numeric_limits<type>::lowest()
    ) {
        throw InvalidTypeCast(
            std::to_string(value),
            "int64_t",
            "integer (type '" + std::string(typeid(type).name()) + "')",
            "value outside target integer range [" + std::to_string(std::numeric_limits<type>::lowest()) + ", " + std::to_string(std::numeric_limits<type>::max()) + "]"
        );
    }
    return static_cast<type>(value);
}

template<typename type>  constexpr
typename std::enable_if<
    std::is_floating_point<type>::value,
    type>::type
cast_double_to(const double & value) {
    // check for overflow with the target floating point type
    if (value > std::numeric_limits<type>::max() || 
        value < std::numeric_limits<type>::lowest()
    ) {
        throw InvalidTypeCast(
            std::to_string(value),
            "double",
            "floating point (type '" + std::string(typeid(type).name()) + "')",
            "value outside target floating point range [" + std::to_string(std::numeric_limits<type>::lowest()) + ", " + std::to_string(std::numeric_limits<type>::max()) + "]"
        );
    }
    return static_cast<type>(value);
}


} // namespace miru::params