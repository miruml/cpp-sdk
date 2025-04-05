#pragma once

// std
#include <string>

// internal
#include <miru/utils.hpp>

// external
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

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
    PARAMETER_BYTE_ARRAY = 5,
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
};

// ================================ ROS2 INTERFACES ================================ //
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L48

/// Return the name of a parameter type
std::string
to_string(ParameterType type);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L48

std::ostream &
operator<<(std::ostream & os, ParameterType type);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L57

/// Indicate the parameter type does not match the expected type.
class ParameterTypeException : public std::runtime_error
{
public:
  /// Construct an instance.
  /**
   * \param[in] expected the expected parameter type.
   * \param[in] actual the actual parameter type.
   */
  ParameterTypeException(ParameterType expected, ParameterType actual)
  : std::runtime_error("expected [" + to_string(expected) + "] got [" + to_string(actual) + "]")
  {}
};

/// Store the type and value of a parameter.
class ParameterValue
{
public:

    // ============================== ROS2 INTERFACES ============================== //

    // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
    // exposing these to users initially
    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L75

    // This constructor is not supported since it uses a ROS2 specific interface of
    // which miru has no equivalent
    // /// Construct a parameter value from a message.
    // explicit ParameterValue(const rcl_interfaces::msg::ParameterValue & value);

    /// Construct a parameter value with type PARAMETER_NOT_SET.
    ParameterValue();
    /// Construct a parameter value with type PARAMETER_BOOL.
    explicit ParameterValue(const bool bool_value);
    /// Construct a parameter value with type PARAMETER_INTEGER.
    explicit ParameterValue(const int int_value);
    /// Construct a parameter value with type PARAMETER_INTEGER.
    explicit ParameterValue(const int64_t int_value);
    /// Construct a parameter value with type PARAMETER_DOUBLE.
    explicit ParameterValue(const float double_value);
    /// Construct a parameter value with type PARAMETER_DOUBLE.
    explicit ParameterValue(const double double_value);
    /// Construct a parameter value with type PARAMETER_STRING.
    explicit ParameterValue(const std::string & string_value);
    /// Construct a parameter value with type PARAMETER_STRING.
    explicit ParameterValue(const char * string_value);
    /// Construct a parameter value with type PARAMETER_BYTE_ARRAY.
    explicit ParameterValue(const std::vector<uint8_t> & byte_array_value);
    /// Construct a parameter value with type PARAMETER_BOOL_ARRAY.
    explicit ParameterValue(const std::vector<bool> & bool_array_value);
    /// Construct a parameter value with type PARAMETER_INTEGER_ARRAY.
    explicit ParameterValue(const std::vector<int> & int_array_value);
    /// Construct a parameter value with type PARAMETER_INTEGER_ARRAY.
    explicit ParameterValue(const std::vector<int64_t> & int_array_value);
    /// Construct a parameter value with type PARAMETER_DOUBLE_ARRAY.
    explicit ParameterValue(const std::vector<float> & double_array_value);
    /// Construct a parameter value with type PARAMETER_DOUBLE_ARRAY.
    explicit ParameterValue(const std::vector<double> & double_array_value);
    /// Construct a parameter value with type PARAMETER_STRING_ARRAY.
    explicit ParameterValue(const std::vector<std::string> & string_array_value);

    // we are not going to support type information in the public interface right now
    // since yaml does not support strongly typed information ("4" vs 4) and we have no
    // quick way of doing so ourself. ROS2 rolled their own parser to grab type
    // information. We will be storing a SCALAR type and I don't want to expose this
    // to users since it will likely confuse them when an integer is parsed as a scalar
    // and not an integer when using yaml.

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L124

    // /// Return an enum indicating the type of the set value.
    // ParameterType
    // get_type() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L129

    // /// Return a message populated with the parameter value
    // rcl_interfaces::msg::ParameterValue
    // to_value_msg() const;

    /// Equal operator.
    bool
    operator==(const ParameterValue & rhs) const;

    /// Not equal operator.
    bool
    operator!=(const ParameterValue & rhs) const;

    // ROS2 "GET" methods using the ParameterType enum
    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L146

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_BOOL, const bool &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_BOOL) {
        throw ParameterTypeException(ParameterType::PARAMETER_BOOL, type_);
        }
        return bool_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_INTEGER, const int64_t &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_INTEGER) {
        throw ParameterTypeException(ParameterType::PARAMETER_INTEGER, type_);
        }
        return int_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE, const double &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_DOUBLE) {
        throw ParameterTypeException(ParameterType::PARAMETER_DOUBLE, type_);
        }
        return double_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_STRING, const std::string &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_STRING) {
        throw ParameterTypeException(ParameterType::PARAMETER_STRING, type_);
        }
        return string_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_BYTE_ARRAY, const std::vector<uint8_t> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_BYTE_ARRAY) {
        throw ParameterTypeException(ParameterType::PARAMETER_BYTE_ARRAY, type_);
        }
        return byte_array_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_BOOL_ARRAY, const std::vector<bool> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_BOOL_ARRAY) {
        throw ParameterTypeException(ParameterType::PARAMETER_BOOL_ARRAY, type_);
        }
        return bool_array_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_INTEGER_ARRAY, const std::vector<int64_t> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_INTEGER_ARRAY) {
        throw ParameterTypeException(ParameterType::PARAMETER_INTEGER_ARRAY, type_);
        }
        return int_array_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_DOUBLE_ARRAY, const std::vector<double> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_DOUBLE_ARRAY) {
        throw ParameterTypeException(ParameterType::PARAMETER_DOUBLE_ARRAY, type_);
        }
        return double_array_value_;
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_STRING_ARRAY, const std::vector<std::string> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_STRING_ARRAY) {
        throw ParameterTypeException(ParameterType::PARAMETER_STRING_ARRAY, type_);
        }
        return string_array_value_;
    }

    // ROS2 "GET" methods using primitive types
    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L149

    template<typename type>
    constexpr
    typename std::enable_if<std::is_same<type, bool>::value, const bool &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_BOOL>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_integral<type>::value && !std::is_same<type, bool>::value, const int64_t &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_INTEGER>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<std::is_floating_point<type>::value, const double &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_DOUBLE>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<std::is_convertible<type, std::string>::value, const std::string &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_STRING>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<uint8_t> &>::value, const std::vector<uint8_t> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_BYTE_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<bool> &>::value, const std::vector<bool> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_BOOL_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<int> &>::value, const std::vector<int64_t> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_INTEGER_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<int64_t> &>::value, const std::vector<int64_t> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_INTEGER_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<float> &>::value, const std::vector<double> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_DOUBLE_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<double> &>::value, const std::vector<double> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_DOUBLE_ARRAY>();
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<std::string> &>::value, const std::vector<std::string> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_STRING_ARRAY>();
    }

    // ============================== MIRU INTERFACES ============================== //

    bool is_null() const;
    bool is_scalar() const;
    bool is_array() const;

private:
    ParameterType type_;
    bool bool_value_;
    int64_t int_value_;
    double double_value_;
    std::string string_value_;
    std::vector<uint8_t> byte_array_value_;
    std::vector<bool> bool_array_value_;
    std::vector<int64_t> int_array_value_;
    std::vector<double> double_array_value_;
    std::vector<std::string> string_array_value_;
};

/// Return the value of a parameter as a string
std::string
to_string(const ParameterValue & value);


} // namespace miru::params