#pragma once

// std
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

// internal
#include <miru/params/composite.hpp>
#include <miru/params/errors.hpp>
#include <miru/params/scalar.hpp>
#include <miru/params/type.hpp>
#include <miru/utils.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

/// Store the type and value of a parameter.
class ParameterValue {
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
  explicit ParameterValue(const std::string &string_value);
  /// Construct a parameter value with type PARAMETER_STRING.
  explicit ParameterValue(const char *string_value);

  // byte arrays are not supported
  // /// Construct a parameter value with type PARAMETER_BYTE_ARRAY.
  // explicit ParameterValue(const std::vector<uint8_t> & byte_array_value);

  /// Construct a parameter value with type PARAMETER_BOOL_ARRAY.
  explicit ParameterValue(const std::vector<bool> &bool_array_value);
  /// Construct a parameter value with type PARAMETER_INTEGER_ARRAY.
  explicit ParameterValue(const std::vector<int> &int_array_value);
  /// Construct a parameter value with type PARAMETER_INTEGER_ARRAY.
  explicit ParameterValue(const std::vector<int64_t> &int_array_value);
  /// Construct a parameter value with type PARAMETER_DOUBLE_ARRAY.
  explicit ParameterValue(const std::vector<float> &double_array_value);
  /// Construct a parameter value with type PARAMETER_DOUBLE_ARRAY.
  explicit ParameterValue(const std::vector<double> &double_array_value);
  /// Construct a parameter value with type PARAMETER_STRING_ARRAY.
  explicit ParameterValue(const std::vector<std::string> &string_array_value);

  // we are not going to support type information in the public interface right now
  // since yaml does not support strongly typed information ("4" vs 4) and we have no
  // quick way of doing so ourself. ROS2 rolled their own parser to grab type
  // information. We will be storing a SCALAR type and I don't want to expose this
  // to users since it will likely confuse them when an integer is parsed as a scalar
  // and not an integer when using yaml.

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L124

  /// Return an enum indicating the type of the set value.
  ParameterType get_type() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L129

  // /// Return a message populated with the parameter value
  // rcl_interfaces::msg::ParameterValue
  // to_value_msg() const;

  /// Equal operator.
  bool operator==(const ParameterValue &rhs) const;

  /// Not equal operator.
  bool operator!=(const ParameterValue &rhs) const;

  // ROS2 "GET" methods using the ParameterType enum
  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L146

  template <ParameterType type>
  constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_BOOL, const bool>::type
    get() const {
    switch (type_) {
      case ParameterType::PARAMETER_BOOL:
        return std::get<bool>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_bool();
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_BOOL, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_INTEGER, const int64_t>::type
    get() const {
    switch (type_) {
      case ParameterType::PARAMETER_INTEGER:
        return std::get<int64_t>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_int();
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_INTEGER, type_);
    }
  }

  template <ParameterType type>
  constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE, const double>::type
    get() const {
    switch (type_) {
      case ParameterType::PARAMETER_DOUBLE:
        return std::get<double>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_double();
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_DOUBLE, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_STRING, const std::string &>::type
    get() const {
    switch (type_) {
      case ParameterType::PARAMETER_STRING:
        return std::get<std::string>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_string();
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_STRING, type_);
    }
  }

  // byte arrays are not supported

  // template<ParameterType type>
  // constexpr
  // typename std::enable_if<
  //     type == ParameterType::PARAMETER_BYTE_ARRAY, const std::vector<uint8_t>
  //     &>::type
  // get() const
  // {
  //     switch (type_) {
  //         case ParameterType::PARAMETER_BYTE_ARRAY:
  //             return std::get<std::vector<uint8_t>>(value_);
  //         case ParameterType::PARAMETER_SCALAR:
  //             return std::get<Scalar>(value_).as_byte_array();
  //         default:
  //             throw ParameterTypeException(ParameterType::PARAMETER_BYTE_ARRAY,
  //             type_);
  //     }
  // }

  template <ParameterType type>
  constexpr typename std::enable_if<
    type == ParameterType::PARAMETER_BOOL_ARRAY,
    const std::vector<bool> &>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_BOOL_ARRAY:
        return std::get<std::vector<bool>>(value_);
      case ParameterType::PARAMETER_SCALAR_ARRAY:
        if (!(cache_flags_ & BOOL_ARRAY_CACHED)) {
          scalar_bool_array_ =
            scalar_array_as<bool>(std::get<std::vector<Scalar>>(value_));
          cache_flags_ |= BOOL_ARRAY_CACHED;
        }
        return scalar_bool_array_;
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_BOOL_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<
    type == ParameterType::PARAMETER_INTEGER_ARRAY,
    const std::vector<int64_t> &>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_INTEGER_ARRAY:
        return std::get<std::vector<int64_t>>(value_);
      case ParameterType::PARAMETER_SCALAR_ARRAY:
        if (!(cache_flags_ & INT_ARRAY_CACHED)) {
          scalar_int_array_ =
            scalar_array_as<int64_t>(std::get<std::vector<Scalar>>(value_));
          cache_flags_ |= INT_ARRAY_CACHED;
        }
        return scalar_int_array_;
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_INTEGER_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<
    type == ParameterType::PARAMETER_DOUBLE_ARRAY,
    const std::vector<double> &>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_DOUBLE_ARRAY:
        return std::get<std::vector<double>>(value_);
      case ParameterType::PARAMETER_SCALAR_ARRAY:
        if (!(cache_flags_ & DOUBLE_ARRAY_CACHED)) {
          scalar_double_array_ =
            scalar_array_as<double>(std::get<std::vector<Scalar>>(value_));
          cache_flags_ |= DOUBLE_ARRAY_CACHED;
        }
        return scalar_double_array_;
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_DOUBLE_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<
    type == ParameterType::PARAMETER_STRING_ARRAY,
    const std::vector<std::string> &>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_STRING_ARRAY:
        return std::get<std::vector<std::string>>(value_);
      case ParameterType::PARAMETER_SCALAR_ARRAY:
        if (!(cache_flags_ & STRING_ARRAY_CACHED)) {
          scalar_string_array_ =
            scalar_array_as<std::string>(std::get<std::vector<Scalar>>(value_));
          cache_flags_ |= STRING_ARRAY_CACHED;
        }
        return scalar_string_array_;
      default:
        THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_STRING_ARRAY, type_);
    }
  }

  // ROS2 "GET" methods using primitive types
  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter_value.hpp#L252

  template <typename type>
  constexpr typename std::enable_if<std::is_same<type, bool>::value, const bool>::type
  get() const {
    return get<ParameterType::PARAMETER_BOOL>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_integral<type>::value && !std::is_same<type, bool>::value,
    const type>::type
  get() const {
    // use the integer conversion function which conducts additional checks
    // for converting int64 to the target type
    return miru::utils::int64_as<type>(get<ParameterType::PARAMETER_INTEGER>());
  }

  template <typename type>
  constexpr
    typename std::enable_if<std::is_floating_point<type>::value, const type>::type
    get() const {
    // use the double conversion function which conducts additional checks
    // for converting double to the target type
    return miru::utils::double_as<type>(get<ParameterType::PARAMETER_DOUBLE>());
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, std::string>::value &&
      !std::is_null_pointer_v<type> &&  // don't allow null
                                        // types for strings
      !std::is_same_v<type, std::nullptr_t>,
    const std::string &>::type
  get() const {
    return get<ParameterType::PARAMETER_STRING>();
  }

  // byte arrays are not supported

  // template<typename type>
  // constexpr
  // typename std::enable_if<
  //     std::is_convertible<
  //     type, const std::vector<uint8_t> &>::value, const std::vector<uint8_t> &>::type
  // get() const
  // {
  //     return get<ParameterType::PARAMETER_BYTE_ARRAY>();
  // }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<bool> &>::value,
    const std::vector<bool> &>::type
  get() const {
    return get<ParameterType::PARAMETER_BOOL_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<int> &>::value,
    const std::vector<int64_t> &>::type
  get() const {
    return get<ParameterType::PARAMETER_INTEGER_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<int64_t> &>::value,
    const std::vector<int64_t> &>::type
  get() const {
    return get<ParameterType::PARAMETER_INTEGER_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<float> &>::value,
    const std::vector<double> &>::type
  get() const {
    return get<ParameterType::PARAMETER_DOUBLE_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<double> &>::value,
    const std::vector<double> &>::type
  get() const {
    return get<ParameterType::PARAMETER_DOUBLE_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const std::vector<std::string> &>::value,
    const std::vector<std::string> &>::type
  get() const {
    return get<ParameterType::PARAMETER_STRING_ARRAY>();
  }

  // ============================== MIRU INTERFACES ============================== //

  /// Construct a parameter value with type PARAMETER_INTEGER.
  explicit ParameterValue(const unsigned int uint_value);
  /// Construct a parameter value with type PARAMETER_NULL.
  explicit ParameterValue(const std::nullptr_t null_value);
  /// Construct a parameter value with type PARAMETER_SCALAR.
  explicit ParameterValue(const Scalar &scalar_value);
  /// Construct a parameter value with type PARAMETER_SCALAR_ARRAY.
  explicit ParameterValue(const std::vector<Scalar> &scalar_array_value);
  /// Construct a parameter value with type PARAMETER_NESTED_ARRAY.
  explicit ParameterValue(const NestedArray &nested_array_value);
  /// Construct a parameter value with type PARAMETER_MAP.
  explicit ParameterValue(const Map &map_value);
  /// Construct a parameter value with type PARAMETER_MAP_ARRAY.
  explicit ParameterValue(const MapArray &map_array_value);

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_NULL, const std::nullptr_t>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_NULL) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_NULL, type_);
    }
    return nullptr;
  }

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_SCALAR, const Scalar &>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_SCALAR) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_SCALAR, type_);
    }
    return std::get<Scalar>(value_);
  }

  template <ParameterType type>
  constexpr typename std::enable_if<
    type == ParameterType::PARAMETER_SCALAR_ARRAY,
    const std::vector<Scalar> &>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_SCALAR_ARRAY) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_SCALAR_ARRAY, type_);
    }
    return std::get<std::vector<Scalar>>(value_);
  }

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_NESTED_ARRAY, const NestedArray &>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_NESTED_ARRAY) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_NESTED_ARRAY, type_);
    }
    return std::get<NestedArray>(value_);
  }

  template <ParameterType type>
  constexpr
    typename std::enable_if<type == ParameterType::PARAMETER_MAP, const Map &>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_MAP) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_MAP, type_);
    }
    return std::get<Map>(value_);
  }

  template <ParameterType type>
  constexpr typename std::
    enable_if<type == ParameterType::PARAMETER_MAP_ARRAY, const MapArray &>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_MAP_ARRAY) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_MAP_ARRAY, type_);
    }
    return std::get<MapArray>(value_);
  }

  template <typename type>
  constexpr typename std::
    enable_if<std::is_same<type, std::nullptr_t>::value, const std::nullptr_t>::type
    get() const {
    if (type_ != ParameterType::PARAMETER_NULL) {
      THROW_INVALID_PARAMETER_VALUE_TYPE(ParameterType::PARAMETER_NULL, type_);
    }
    return nullptr;
  }

  template <typename type>
  constexpr
    typename std::enable_if<std::is_same<type, Scalar>::value, const Scalar &>::type
    get() const {
    return get<ParameterType::PARAMETER_SCALAR>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_same<type, std::vector<Scalar>>::value,
    const std::vector<Scalar> &>::type
  get() const {
    return get<ParameterType::PARAMETER_SCALAR_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const NestedArray &>::value,
    const NestedArray &>::type
  get() const {
    return get<ParameterType::PARAMETER_NESTED_ARRAY>();
  }

  template <typename type>
  constexpr typename std::
    enable_if<std::is_convertible<type, const Map &>::value, const Map &>::type
    get() const {
    return get<ParameterType::PARAMETER_MAP>();
  }

  template <typename type>
  constexpr typename std::enable_if<
    std::is_convertible<type, const MapArray &>::value,
    const MapArray &>::type
  get() const {
    return get<ParameterType::PARAMETER_MAP_ARRAY>();
  }

  /// Get the value of parameter as using the given ParameterType as a template argument
  template <ParameterType ParamT>
  constexpr decltype(auto) as() const {
    return get<ParamT>();
  }

  /// Get the value of a parameter using the given c++ type as a template argument
  template <typename T>
  constexpr decltype(auto) as() const {
    return get<T>();
  }

  bool is_null() const;
  bool is_scalar() const;
  bool is_map() const;
  bool is_scalar_array() const;
  bool is_nested_array() const;
  bool is_map_array() const;
  bool is_array() const;

 private:
  explicit ParameterValue(ParameterType type);

  ParameterType type_;
  std::variant<
    std::nullptr_t,
    bool,
    int64_t,
    double,
    std::string,
    Scalar,
    std::vector<uint8_t>,
    std::vector<bool>,
    std::vector<int64_t>,
    std::vector<double>,
    std::vector<std::string>,
    std::vector<Scalar>,
    NestedArray,
    Map,
    MapArray>
    value_;

  // cached array conversions for scalar arrays
  mutable std::vector<bool> scalar_bool_array_;
  mutable std::vector<int64_t> scalar_int_array_;
  mutable std::vector<double> scalar_double_array_;
  mutable std::vector<std::string> scalar_string_array_;

  mutable uint8_t cache_flags_ = 0;
  static constexpr uint8_t BOOL_ARRAY_CACHED = 1 << 0;    // 0b0001
  static constexpr uint8_t INT_ARRAY_CACHED = 1 << 1;     // 0b0010
  static constexpr uint8_t DOUBLE_ARRAY_CACHED = 1 << 2;  // 0b0100
  static constexpr uint8_t STRING_ARRAY_CACHED = 1 << 3;  // 0b1000
};

/// Return the value of a parameter as a string
std::string to_string(const ParameterValue &value);

std::string to_string(const ParameterValue &value, const int indent);

std::string param_value_array_to_string(
  const std::vector<ParameterValue> &array,
  const int indent,
  const bool with_newlines
);

std::string param_map_to_string(const miru::params::Map &map, const int indent);

std::ostream &operator<<(std::ostream &os, const ParameterValue &value);

}  // namespace miru::params
