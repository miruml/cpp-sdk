// internal
#include <miru/params/parameter.hpp>
#include <miru/params/type.hpp>
#include <miru/params/utils.hpp>

// external
#include <fmt/ranges.h>

namespace miru::params {

// ================================ ROS2 INTERFACES ================================ //
// The majority of the following code is take from ros2 rclcpp:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/src/rclcpp/parameter_value.cpp

std::string to_string(const ParameterType type) {
  switch (type) {
    // ros 2 parameter types
    case ParameterType::PARAMETER_NOT_SET:
      return "not set";
    case ParameterType::PARAMETER_BOOL:
      return "bool";
    case ParameterType::PARAMETER_INTEGER:
      return "int";
    case ParameterType::PARAMETER_DOUBLE:
      return "double";
    case ParameterType::PARAMETER_STRING:
      return "string";
    case ParameterType::PARAMETER_BOOL_ARRAY:
      return "bool array";
    case ParameterType::PARAMETER_INTEGER_ARRAY:
      return "int array";
    case ParameterType::PARAMETER_DOUBLE_ARRAY:
      return "double array";
    case ParameterType::PARAMETER_STRING_ARRAY:
      return "string array";

    // miru parameter types
    case ParameterType::PARAMETER_NULL:
      return "null";
    case ParameterType::PARAMETER_SCALAR:
      return "scalar";
    case ParameterType::PARAMETER_SCALAR_ARRAY:
      return "scalar array";
    case ParameterType::PARAMETER_NESTED_ARRAY:
      return "nested array";
    case ParameterType::PARAMETER_MAP:
      return "map";
    case ParameterType::PARAMETER_MAP_ARRAY:
      return "map array";
  }
  return "unknown type";
}

std::ostream& operator<<(std::ostream& os, const ParameterType& type) {
  os << to_string(type);
  return os;
}

std::string to_string(const ParameterValue& value) { return to_string(value, 0); }

std::string to_string(const ParameterValue& value, const int indent) {
  switch (value.get_type()) {
    // ros 2 parameter types
    case ParameterType::PARAMETER_NOT_SET:
      return "\"not set\"";
    case ParameterType::PARAMETER_BOOL:
      return (value.get<bool>() ? "true" : "false");
    case ParameterType::PARAMETER_INTEGER:
      return std::to_string(value.get<int>());
    case ParameterType::PARAMETER_DOUBLE:
      return std::to_string(value.get<double>());
    case ParameterType::PARAMETER_STRING:
      return "\"" + value.get<std::string>() + "\"";
    case ParameterType::PARAMETER_BOOL_ARRAY: {
      std::vector<ParameterValue> bool_array;
      for (const bool& bool_value : value.get<std::vector<bool>>()) {
        bool_array.push_back(ParameterValue(bool_value));
      }
      return miru::params::param_value_array_to_string(bool_array, 0, false);
    }
    case ParameterType::PARAMETER_INTEGER_ARRAY: {
      std::vector<ParameterValue> int_array;
      for (const int64_t& int_value : value.get<std::vector<int64_t>>()) {
        int_array.push_back(ParameterValue(int_value));
      }
      return miru::params::param_value_array_to_string(int_array, 0, false);
    }
    case ParameterType::PARAMETER_DOUBLE_ARRAY: {
      std::vector<ParameterValue> double_array;
      for (const double& double_value : value.get<std::vector<double>>()) {
        double_array.push_back(ParameterValue(double_value));
      }
      return miru::params::param_value_array_to_string(double_array, 0, false);
    }
    case ParameterType::PARAMETER_STRING_ARRAY: {
      std::vector<ParameterValue> string_array;
      for (const std::string& string_value : value.get<std::vector<std::string>>()) {
        string_array.push_back(ParameterValue(string_value));
      }
      return miru::params::param_value_array_to_string(string_array, 0, false);
    }

    // miru parameter types
    case ParameterType::PARAMETER_NULL:
      return "null";
    case ParameterType::PARAMETER_SCALAR:
      return "\"" + value.get<Scalar>().as_string() + "\"";
    case ParameterType::PARAMETER_SCALAR_ARRAY: {
      std::vector<ParameterValue> scalar_array;
      for (const Scalar& scalar_value : value.get<std::vector<Scalar>>()) {
        scalar_array.push_back(ParameterValue(scalar_value));
      }
      return miru::params::param_value_array_to_string(scalar_array, 0, false);
    }
    case ParameterType::PARAMETER_NESTED_ARRAY: {
      std::vector<ParameterValue> nested_array;
      for (const Parameter& param : value.get<NestedArray>()) {
        nested_array.push_back(param.get_parameter_value());
      }
      return miru::params::param_value_array_to_string(nested_array, 0, true);
    }
    case ParameterType::PARAMETER_MAP: {
      return miru::params::param_map_to_string(value.get<Map>(), indent);
    }
    case ParameterType::PARAMETER_MAP_ARRAY: {
      std::vector<ParameterValue> map_array;
      for (const Parameter& param : value.get<MapArray>()) {
        map_array.push_back(param.get_parameter_value());
      }
      return miru::params::param_value_array_to_string(map_array, 0, true);
    }
  }
  return "unknown type";
}

std::string param_map_to_string(const miru::params::Map& map, const int indent) {
  std::stringstream type_array;
  bool first_item = true;
  type_array << std::string(indent, ' ') << "{";
  for (const Parameter& param : map) {
    if (!first_item) {
      type_array << ",";
    } else {
      first_item = false;
    }
    type_array << "\n";
    type_array << std::string(indent + 2, ' ');
    type_array << "\"" << param.get_key();
    type_array << "\": ";
    type_array << to_string(param.get_parameter_value(), indent + 2);
  }
  type_array << "\n";
  type_array << std::string(indent, ' ') << "}";
  return type_array.str();
}

std::string param_value_array_to_string(const std::vector<ParameterValue>& array,
                                        const int indent, const bool with_newlines) {
  std::stringstream type_array;
  bool first_item = true;
  with_newlines ? type_array << std::string(indent, ' ') << "[" : type_array << "[";
  for (const ParameterValue& value : array) {
    if (!first_item) {
      with_newlines ? type_array << "," : type_array << ", ";
    } else {
      first_item = false;
    }
    with_newlines ? type_array << "\n" : type_array << "";
    with_newlines&& is_leaf(value) ? type_array << std::string(indent + 2, ' ')
                                   : type_array << "";
    type_array << to_string(value, with_newlines ? indent + 2 : indent);
  }
  with_newlines ? type_array << "\n"
                             << std::string(indent, ' ') << "]"
                : type_array << "]";
  return type_array.str();
}

std::ostream& operator<<(std::ostream& os, const ParameterValue& value) {
  os << to_string(value);
  return os;
}

ParameterValue::ParameterValue() {
  value_ = nullptr;
  type_ = ParameterType::PARAMETER_NOT_SET;
}

ParameterValue::ParameterValue(const bool bool_value) {
  value_ = bool_value;
  type_ = ParameterType::PARAMETER_BOOL;
}

ParameterValue::ParameterValue(const int int_value) {
  value_ = int_value;
  type_ = ParameterType::PARAMETER_INTEGER;
}

ParameterValue::ParameterValue(const int64_t int_value) {
  value_ = int_value;
  type_ = ParameterType::PARAMETER_INTEGER;
}

ParameterValue::ParameterValue(const float double_value) {
  value_ = static_cast<double>(double_value);
  type_ = ParameterType::PARAMETER_DOUBLE;
}

ParameterValue::ParameterValue(const double double_value) {
  value_ = double_value;
  type_ = ParameterType::PARAMETER_DOUBLE;
}

ParameterValue::ParameterValue(const std::string& string_value) {
  value_ = string_value;
  type_ = ParameterType::PARAMETER_STRING;
}

ParameterValue::ParameterValue(const char* string_value)
    : ParameterValue(std::string(string_value)) {}

ParameterValue::ParameterValue(const std::vector<bool>& bool_array_value) {
  value_ = bool_array_value;
  type_ = ParameterType::PARAMETER_BOOL_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<int>& int_array_value) {
  std::vector<int64_t> int64_array_value;
  int64_array_value.assign(int_array_value.cbegin(), int_array_value.cend());
  value_ = int64_array_value;
  type_ = ParameterType::PARAMETER_INTEGER_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<int64_t>& int_array_value) {
  value_ = int_array_value;
  type_ = ParameterType::PARAMETER_INTEGER_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<float>& float_array_value) {
  std::vector<double> double_array_value;
  double_array_value.assign(float_array_value.cbegin(), float_array_value.cend());
  value_ = double_array_value;
  type_ = ParameterType::PARAMETER_DOUBLE_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<double>& double_array_value) {
  value_ = double_array_value;
  type_ = ParameterType::PARAMETER_DOUBLE_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<std::string>& string_array_value) {
  value_ = string_array_value;
  type_ = ParameterType::PARAMETER_STRING_ARRAY;
}

ParameterType ParameterValue::get_type() const { return type_; }

bool ParameterValue::operator==(const ParameterValue& other) const {
  return type_ == other.type_ && value_ == other.value_;
}

bool ParameterValue::operator!=(const ParameterValue& other) const {
  return !(*this == other);
}

// ================================ MIRU INTERFACES ================================ //

ParameterValue::ParameterValue(const unsigned int uint_value) {
  if (uint_value > static_cast<unsigned int>(std::numeric_limits<int64_t>::max())) {
    throw std::overflow_error("Unsigned integer value too large for Parameter");
  }
  value_ = static_cast<int64_t>(uint_value);
  type_ = ParameterType::PARAMETER_INTEGER;
}

ParameterValue::ParameterValue(const std::nullptr_t null_value) {
  value_ = null_value;
  type_ = ParameterType::PARAMETER_NULL;
}

ParameterValue::ParameterValue(const Scalar& scalar_value) {
  value_ = scalar_value;
  type_ = ParameterType::PARAMETER_SCALAR;
}

ParameterValue::ParameterValue(const std::vector<Scalar>& scalar_array_value) {
  value_ = scalar_array_value;
  type_ = ParameterType::PARAMETER_SCALAR_ARRAY;
}

ParameterValue::ParameterValue(const NestedArray& nested_array_value) {
  value_ = nested_array_value;
  type_ = ParameterType::PARAMETER_NESTED_ARRAY;
}

ParameterValue::ParameterValue(const Map& map_value) {
  value_ = map_value;
  type_ = ParameterType::PARAMETER_MAP;
}

ParameterValue::ParameterValue(const MapArray& map_array_value) {
  value_ = map_array_value;
  type_ = ParameterType::PARAMETER_MAP_ARRAY;
}

bool ParameterValue::is_null() const { return type_ == ParameterType::PARAMETER_NULL; }

bool ParameterValue::is_scalar() const {
  switch (type_) {
    case ParameterType::PARAMETER_BOOL:
    case ParameterType::PARAMETER_INTEGER:
    case ParameterType::PARAMETER_DOUBLE:
    case ParameterType::PARAMETER_STRING:
    case ParameterType::PARAMETER_SCALAR:
      return true;
    default:
      return false;
  }
}

bool ParameterValue::is_map() const { return type_ == ParameterType::PARAMETER_MAP; }

bool ParameterValue::is_scalar_array() const {
  return type_ == ParameterType::PARAMETER_BOOL_ARRAY ||
         type_ == ParameterType::PARAMETER_INTEGER_ARRAY ||
         type_ == ParameterType::PARAMETER_DOUBLE_ARRAY ||
         type_ == ParameterType::PARAMETER_STRING_ARRAY ||
         type_ == ParameterType::PARAMETER_SCALAR_ARRAY;
}

bool ParameterValue::is_nested_array() const {
  return type_ == ParameterType::PARAMETER_NESTED_ARRAY;
}

bool ParameterValue::is_map_array() const {
  return type_ == ParameterType::PARAMETER_MAP_ARRAY;
}

bool ParameterValue::is_array() const {
  return is_scalar_array() || is_nested_array() || is_map_array();
}

}  // namespace miru::params