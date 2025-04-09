#pragma once

// std
#include <cstddef>
#include <cstdint>
#include <string>
#include <variant>

// internal
#include <miru/params/exceptions.hpp>
#include <miru/params/scalar.hpp>
#include <miru/params/type.hpp>
#include <miru/utils.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

// ================================================================================= //
// ================================================================================= //
// =============================== PARAMETER VALUE ================================= //
// ================================================================================= //
// ================================================================================= //

class Parameter;

class Map {
 public:
  Map(const std::vector<Parameter> &fields);

  bool operator==(const Map &other) const;
  bool operator!=(const Map &other) const;

  // Iterator type aliases
  using iterator = std::vector<Parameter>::iterator;
  using const_iterator = std::vector<Parameter>::const_iterator;

  // Iterator access methods
  const_iterator begin() const { return sorted_fields_.begin(); }
  const_iterator end() const { return sorted_fields_.end(); }
  size_t size() const { return sorted_fields_.size(); }

  // Access a parameter by key
  const Parameter &operator[](const std::string &key) const;

 private:
  std::vector<Parameter> sorted_fields_;
};

class MapArray {
 public:
  MapArray(const std::vector<Parameter> &maps);

  // Iterator type aliases
  using iterator = std::vector<Parameter>::iterator;
  using const_iterator = std::vector<Parameter>::const_iterator;

  // Iterator access methods
  const_iterator begin() const { return items_.begin(); }
  const_iterator end() const { return items_.end(); }
  size_t size() const { return items_.size(); }

  bool operator==(const MapArray &other) const;
  bool operator!=(const MapArray &other) const;

  // Access a parameter by index
  const Parameter &operator[](const size_t index) const;

 private:
  std::vector<Parameter> items_;
};

class NestedArray {
 public:
  NestedArray(const std::vector<Parameter> &items);

  // Iterator type aliases
  using iterator = std::vector<Parameter>::iterator;
  using const_iterator = std::vector<Parameter>::const_iterator;

  // Iterator access methods
  const_iterator begin() const { return items_.begin(); }
  const_iterator end() const { return items_.end(); }
  size_t size() const { return items_.size(); }

  bool operator==(const NestedArray &other) const;
  bool operator!=(const NestedArray &other) const;

  // Access a parameter by index
  const Parameter &operator[](const size_t index) const;

 private:
  std::vector<Parameter> items_;
};

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
        throw InvalidParameterValueType(ParameterType::PARAMETER_BOOL, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_INTEGER,
                                    const int64_t>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_INTEGER:
        return std::get<int64_t>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_int();
      default:
        throw InvalidParameterValueType(ParameterType::PARAMETER_INTEGER, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE,
                                    const double>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_DOUBLE:
        return std::get<double>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_double();
      default:
        throw InvalidParameterValueType(ParameterType::PARAMETER_DOUBLE, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_STRING,
                                    const std::string &>::type
  get() const {
    switch (type_) {
      case ParameterType::PARAMETER_STRING:
        return std::get<std::string>(value_);
      case ParameterType::PARAMETER_SCALAR:
        return std::get<Scalar>(value_).as_string();
      default:
        throw InvalidParameterValueType(ParameterType::PARAMETER_STRING, type_);
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
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_BOOL_ARRAY,
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
        throw InvalidParameterValueType(ParameterType::PARAMETER_BOOL_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_INTEGER_ARRAY,
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
        throw InvalidParameterValueType(ParameterType::PARAMETER_INTEGER_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE_ARRAY,
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
        throw InvalidParameterValueType(ParameterType::PARAMETER_DOUBLE_ARRAY, type_);
    }
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_STRING_ARRAY,
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
        throw InvalidParameterValueType(ParameterType::PARAMETER_STRING_ARRAY, type_);
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
  constexpr typename std::enable_if<std::is_integral<type>::value &&
                                        !std::is_same<type, bool>::value,
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
  constexpr typename std::enable_if<std::is_convertible<type, std::string>::value,
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
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_NULL,
                                    const std::nullptr_t>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_NULL) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_NULL, type_);
    }
    return nullptr;
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_SCALAR,
                                    const Scalar &>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_SCALAR) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_SCALAR, type_);
    }
    return std::get<Scalar>(value_);
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_SCALAR_ARRAY,
                                    const std::vector<Scalar> &>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_SCALAR_ARRAY) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_SCALAR_ARRAY, type_);
    }
    return std::get<std::vector<Scalar>>(value_);
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_NESTED_ARRAY,
                                    const NestedArray &>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_NESTED_ARRAY) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_NESTED_ARRAY, type_);
    }
    return std::get<NestedArray>(value_);
  }

  template <ParameterType type>
  constexpr
      typename std::enable_if<type == ParameterType::PARAMETER_MAP, const Map &>::type
      get() const {
    if (type_ != ParameterType::PARAMETER_MAP) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_MAP, type_);
    }
    return std::get<Map>(value_);
  }

  template <ParameterType type>
  constexpr typename std::enable_if<type == ParameterType::PARAMETER_MAP_ARRAY,
                                    const MapArray &>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_MAP_ARRAY) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_MAP_ARRAY, type_);
    }
    return std::get<MapArray>(value_);
  }

  template <typename type>
  constexpr typename std::enable_if<std::is_same<type, std::nullptr_t>::value,
                                    const std::nullptr_t>::type
  get() const {
    if (type_ != ParameterType::PARAMETER_NULL) {
      throw InvalidParameterValueType(ParameterType::PARAMETER_NULL, type_);
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
  constexpr typename std::enable_if<std::is_same<type, std::vector<Scalar>>::value,
                                    const std::vector<Scalar> &>::type
  get() const {
    return get<ParameterType::PARAMETER_SCALAR_ARRAY>();
  }

  template <typename type>
  constexpr
      typename std::enable_if<std::is_convertible<type, const NestedArray &>::value,
                              const NestedArray &>::type
      get() const {
    return get<ParameterType::PARAMETER_NESTED_ARRAY>();
  }

  template <typename type>
  constexpr typename std::enable_if<std::is_convertible<type, const Map &>::value,
                                    const Map &>::type
  get() const {
    return get<ParameterType::PARAMETER_MAP>();
  }

  template <typename type>
  constexpr typename std::enable_if<std::is_convertible<type, const MapArray &>::value,
                                    const MapArray &>::type
  get() const {
    return get<ParameterType::PARAMETER_MAP_ARRAY>();
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
  std::variant<std::nullptr_t, bool, int64_t, double, std::string, Scalar,
               std::vector<uint8_t>, std::vector<bool>, std::vector<int64_t>,
               std::vector<double>, std::vector<std::string>, std::vector<Scalar>,
               NestedArray, Map, MapArray>
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

std::string param_value_array_to_string(const std::vector<ParameterValue> &array,
                                        const int indent, const bool with_newlines);

std::string param_map_to_string(const miru::params::Map &map, const int indent);

std::ostream &operator<<(std::ostream &os, const ParameterValue &value);

// ================================================================================= //
// ================================================================================= //
// ================================== PARAMETERS =================================== //
// ================================================================================= //
// ================================================================================= //

// primitive parameter class
class Parameter {
 public:
  // ============================== ROS2 INTERFACES ============================== //

  // Although we'll conform to the ROS2 interfaces for constructors, we'll not be
  // exposing these to users initially

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L55

  /// Construct with an empty key and a parameter value of type
  /// ParameterType::PARAMETER_NOT_SET.
  Parameter();

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L59

  /// Construct with given name and a parameter value of type
  /// ParameterType::PARAMETER_NOT_SET.
  explicit Parameter(const std::string &key);

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L63

  /// Construct with given name and given parameter value.
  Parameter(const std::string &name, const ParameterValue &parameter_value);

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L67

  /// Construct with given name and given parameter value.
  template <typename ValueTypeT>
  Parameter(const std::string &name, ValueTypeT value)
      : Parameter(name, ParameterValue(value)) {}

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L73
  // explicit Parameter(const rclcpp::node_interfaces::ParameterInfo &
  // parameter_info); is not supported since it uses ROS2 specific interface which has
  // no miru equivalent

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L76

  /// Equal operator
  bool operator==(const Parameter &other) const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L81

  /// Not equal operator
  bool operator!=(const Parameter &other) const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L86

  /// Get the type of the parameter
  ParameterType get_type() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L92

  /// Get the name of the type of the parameter
  std::string get_type_name() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L97

  /// Get the full name of the parameter from the root of the config tree
  const std::string &get_name() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L101
  // get_value_message() is not supported since it returns an
  // rcl_interfaces::msg::ParameterValue type

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L106
  const ParameterValue &get_parameter_value() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L115

  /// Get the value of parameter as using the given ParameterType as a template argument
  template <ParameterType ParamT>
  decltype(auto) get_value() const {
    try {
      return value_.get<ParamT>();
    } catch (const InvalidParameterValueType &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    } catch (const InvalidScalarConversion &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    } catch (const utils::InvalidTypeConversion &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    }
  }

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L122

  /// Get the value of parameter as using the given c++ type as a template argument
  template <typename T>
  decltype(auto) get_value() const {
    try {
      return value_.get<T>();
    } catch (const InvalidParameterValueType &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    } catch (const InvalidScalarConversion &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    } catch (const utils::InvalidTypeConversion &ex) {
      throw InvalidParameterType(this->name_, ex.what());
    }
  }

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
  const std::string &as_string() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L159

  // byte arrays are not supported

  // /// Get the value of parameter as a byte array (vector<uint8_t>)
  // const std::vector<uint8_t>& as_byte_array() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L167

  /// Get the value of parameter as a bool array (vector<bool>)
  const std::vector<bool> &as_bool_array() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L175

  /// Get the value of parameter as an integer array (vector<int64_t>)
  const std::vector<int64_t> &as_integer_array() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L183

  /// Get the value of parameter as a double array (vector<double>)
  const std::vector<double> &as_double_array() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L191

  /// Get the value of parameter as a string array (vector<std::string>)
  const std::vector<std::string> &as_string_array() const;

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L199
  // from_parameter_msg() is not supported since it takes a
  // rcl_interfaces::msg::Parameter type as an argument

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L204
  // to_parameter_msg() is not supported since it returns a
  // rcl_interfaces::msg::Parameter type

  // https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L209

  /// Get the value of parameter as a string
  std::string value_to_string() const;

  // ============================== MIRU INTERFACES ============================== //

  /// Get the key of the parameter
  std::string get_key() const;

  /// Get the name of the parent of the parameter
  std::string get_parent_name() const;

  /// Get the value of parameter as a null type
  const std::nullptr_t as_null() const;
  /// Get the value of parameter as a scalar type
  const Scalar &as_scalar() const;
  /// Get the value of parameter as a scalar array type
  const std::vector<Scalar> &as_scalar_array() const;
  /// Get the value of parameter as a nested array type
  const NestedArray &as_nested_array() const;
  /// Get the value of parameter as a map (Parameter)
  const Map &as_map() const;
  /// Get the value of parameter as a map array (vector<Parameter>)
  const MapArray &as_map_array() const;

  bool is_null() const;
  bool is_scalar() const;
  bool is_scalar_array() const;
  bool is_nested_array() const;
  bool is_map() const;
  bool is_map_array() const;
  bool is_array() const;

 private:
  ParameterType type_;
  std::string name_;
  ParameterValue value_;
};

// ================================ ROS2 INTERFACES ================================ //
// We are not supporting the to_json_dict_entry function. This is a cascade effect of
// not being able to support support type information in the public interface right now
// since yaml does not support strongly typed information ("4" vs 4) and we have no
// quick way of doing so ourself. ROS2 rolled their own parser to grab type information.
// We will be storing a SCALAR type and I don't want to expose this to users since it
// will likely confuse them when an integer is parsed as a scalar and not an integer
// when using yaml.

// this is a helper function that we don't use
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L219
// std::string _to_json_dict_entry(const Parameter & param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L224

std::ostream &operator<<(std::ostream &os, const Parameter &param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L228

std::ostream &operator<<(std::ostream &os, const std::vector<Parameter> &parameters);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L287
std::string to_string(const Parameter &param);

// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/include/rclcpp/parameter.hpp#L291
std::string to_string(const std::vector<Parameter> &parameters);

}  // namespace miru::params