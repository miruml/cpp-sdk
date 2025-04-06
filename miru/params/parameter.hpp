#pragma once

// std
#include <string>
#include <variant>
#include <optional>

// internal
#include <miru/utils.hpp>
#include <miru/params/exceptions.hpp>

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
    PARAMETER_OBJECT_ARRAY = 130,
    PARAMETER_OBJECT = 131,
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

// ================================ MIRU INTERFACES ================================ //

class Parameter;

class ScalarValue {

public:
    ScalarValue(
        std::string scalar,
        std::optional<bool> bool_value,
        std::optional<int64_t> int_value,
        std::optional<double> double_value
    );

    bool operator==(const ScalarValue & other) const;
    bool operator!=(const ScalarValue & other) const;

    bool has_bool_value() const { return flags_ & HAS_BOOL_VALUE; }
    bool has_int_value() const { return flags_ & HAS_INT_VALUE; }
    bool has_double_value() const { return flags_ & HAS_DOUBLE_VALUE; }

    const std::string& get_scalar() const { return scalar; }
    const bool& get_bool_value() const;
    const int64_t& get_int_value() const;
    const double& get_double_value() const;

private:
    std::string scalar;
    bool bool_value_;
    int64_t int_value_;
    double double_value_;

    uint8_t flags_ = 0;
    static constexpr uint8_t HAS_BOOL_VALUE = 1 << 0;
    static constexpr uint8_t HAS_INT_VALUE = 1 << 1;
    static constexpr uint8_t HAS_DOUBLE_VALUE = 1 << 2;
};

/// Indicate the parameter type does not match the expected type.
class ScalarConversionException: public std::runtime_error
{
public:
  /// Construct an instance.
  /**
   * \param[in] scalar the scalar value.
   * \param[in] target_type the target type attempted to convert to.
   */
  ScalarConversionException(const ScalarValue& scalar, ParameterType target_type)
  : std::runtime_error("scalar [" + scalar.get_scalar() + "] cannot be converted to [" + to_string(target_type) + "]")
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

    /// Return an enum indicating the type of the set value.
    ParameterType
    get_type() const;

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
        switch (type_) {
            case ParameterType::PARAMETER_BOOL:
                return std::get<bool>(value_);
            case ParameterType::PARAMETER_SCALAR:
                if (!std::get<ScalarValue>(value_).has_bool_value()) {
                    throw ScalarConversionException(
                        std::get<ScalarValue>(value_),
                        ParameterType::PARAMETER_BOOL
                    );
                }
                return std::get<ScalarValue>(value_).get_bool_value();
            default:
                throw ParameterTypeException(ParameterType::PARAMETER_BOOL, type_);
        }
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_INTEGER, const int64_t &>::type
    get() const
    {
        switch (type_) {
            case ParameterType::PARAMETER_INTEGER:
                return std::get<int64_t>(value_);
            case ParameterType::PARAMETER_SCALAR:
                if (!std::get<ScalarValue>(value_).has_int_value()) {
                    throw ScalarConversionException(
                        std::get<ScalarValue>(value_),
                        ParameterType::PARAMETER_INTEGER
                    );
                }
                return std::get<ScalarValue>(value_).get_int_value();
            default:
                throw ParameterTypeException(ParameterType::PARAMETER_INTEGER, type_);
        }
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE, const double &>::type
    get() const
    {
        switch (type_) {
            case ParameterType::PARAMETER_DOUBLE:
                return std::get<double>(value_);
            case ParameterType::PARAMETER_SCALAR:
                if (!std::get<ScalarValue>(value_).has_double_value()) {
                    throw ScalarConversionException(
                        std::get<ScalarValue>(value_),
                        ParameterType::PARAMETER_DOUBLE
                    );
                }
                return std::get<ScalarValue>(value_).get_double_value();
            default:
                throw ParameterTypeException(ParameterType::PARAMETER_DOUBLE, type_);
        }
    }

    template<ParameterType type>
    constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_STRING, const std::string &>::type
    get() const
    {
        switch (type_) {
            case ParameterType::PARAMETER_STRING:
                return std::get<std::string>(value_);
            case ParameterType::PARAMETER_SCALAR:
                return std::get<ScalarValue>(value_).get_scalar();
            default:
                throw ParameterTypeException(ParameterType::PARAMETER_STRING, type_);
        }
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
        return std::get<std::vector<uint8_t>>(value_);
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
        return std::get<std::vector<bool>>(value_);
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
        return std::get<std::vector<int64_t>>(value_);
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
        return std::get<std::vector<double>>(value_);
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
        return std::get<std::vector<std::string>>(value_);
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

    /// Construct a parameter value with type PARAMETER_OBJECT_ARRAY.
    explicit ParameterValue(
        const ParameterType& type,
        const std::vector<Parameter> & object_array_value
    );

    static ParameterValue make_null();
    static ParameterValue make_object(const std::vector<Parameter> & object_entries);
    static ParameterValue make_object_array(const std::vector<Parameter> & object_entries);

    template<ParameterType type>
    constexpr
    typename std::enable_if<
        type == ParameterType::PARAMETER_OBJECT || 
        type == ParameterType::PARAMETER_OBJECT_ARRAY,
        const std::vector<Parameter> &>::type
    get() const
    {
        if (type_ != ParameterType::PARAMETER_OBJECT) {
        throw ParameterTypeException(ParameterType::PARAMETER_OBJECT, type_);
        }
        return std::get<std::vector<Parameter>>(value_);
    }

    template<typename type>
    constexpr
    typename std::enable_if<
        std::is_convertible<
        type, const std::vector<Parameter> &>::value, const std::vector<Parameter> &>::type
    get() const
    {
        return get<ParameterType::PARAMETER_OBJECT>();
    }

    bool is_null() const;
    bool is_scalar() const;
    bool is_array() const;
    bool is_object() const;
    bool is_object_array() const;

private:
    explicit ParameterValue(ParameterType type);

    ParameterType type_;
    std::variant<
        bool,
        int64_t,
        double,
        std::string,
        ScalarValue,
        std::vector<uint8_t>,
        std::vector<bool>,
        std::vector<int64_t>,
        std::vector<double>,
        std::vector<std::string>,
        std::vector<Parameter>
    > value_;

};

/// Return the value of a parameter as a string
std::string
to_string(const ParameterValue & value);

// primitive parameter class
class Parameter {
public:
    // ============================== ROS2 INTERFACES ============================== // 

    // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
    // exposing these to users initially

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L55

    /// Construct with an empty key and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    Parameter();

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L59

    /// Construct with given name and a parameter value of type ParameterType::PARAMETER_NOT_SET.
    explicit Parameter(const std::string & name);

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L63

    /// Construct with given name and given parameter value.
    Parameter(const std::string & name, const ParameterValue & value);

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

    /// Get the type of the parameter
    ParameterType get_type() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L92

    /// Get the name of the type of the parameter
    std::string get_type_name() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L97

    /// Get the full name of the parameter from the root of the config tree
    const std::string& get_name() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L101
    // get_value_message() is not supported since it returns an
    // rcl_interfaces::msg::ParameterValue type

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L106
    const ParameterValue& get_parameter_value() const;

    // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L115

    /// Get the value of parameter as using the given ParameterType as a template argument
    template<ParameterType ParamT>
    decltype(auto)
    get_value() const 
    {
        return value_.get<ParamT>();
    }

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

    /// Get the value of parameter as an object array (vector<Parameter>)
    const std::vector<Parameter>& as_object_array() const;

    /// Get the value of parameter as an object (Parameter)
    const std::vector<Parameter>& as_object() const;

    /// Get the key of the parameter (the final part of the name)
    std::string get_key() const;

    /// Check if the parameter is null
    bool is_null() const;
    bool is_scalar() const;
    bool is_array() const;

private:
    ParameterType type_;
    std::string name_;
    ParameterValue value_; 
};

namespace detail
{

template<typename T>
auto
get_value_helper(const Parameter * parameter)
{
    return parameter->get_parameter_value().get<T>();
}

// Specialization allowing Parameter::get() to return a const ref to the parameter value object.
template<>
inline
auto
get_value_helper<ParameterValue>(const Parameter * parameter)
{
    return parameter->get_parameter_value();
}

// Specialization allowing Parameter::get() to return a const ref to the parameter itself.
template<>
inline
auto
get_value_helper<Parameter>(const Parameter * parameter)
{
    // Use this lambda to ensure it's a const reference being returned (and not a copy).
    auto type_enforcing_lambda =
        [&parameter]() -> const Parameter & {
        return *parameter;
        };
    return type_enforcing_lambda();
}

}  // namespace detail

template<typename T> decltype(auto)
Parameter::get_value() const
{
    try {
        // use the helper to specialize for the ParameterValue and Parameter cases.
        return detail::get_value_helper<T>(this);
    } catch (const ParameterTypeException & ex) {
        throw miru::params::InvalidParameterTypeException(this->name_, ex.what());
    }
}

// ================================ ROS2 INTERFACES ================================ //
// We are not supporting the to_json_dict_entry function. This is a cascade effect of
// not being able to support support type information in the public interface right now
// since yaml does not support strongly typed information ("4" vs 4) and we have no
// quick way of doing so ourself. ROS2 rolled their own parser to grab type information.
// We will be storing a SCALAR type and I don't want to expose this to users since it
// will likely confuse them when an integer is parsed as a scalar and not an integer
// when using yaml.

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L219

std::string _to_json_dict_entry(const Parameter & param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L224

std::ostream& operator<<(std::ostream & os, const Parameter& param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L228

std::ostream& operator<<(std::ostream & os, const std::vector<Parameter>& parameters);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L287
std::string to_string(const Parameter & param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L291
std::string to_string(const std::vector<Parameter> & parameters);

} // namespace miru::params