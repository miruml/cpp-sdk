// internal
#include <miru/params/parameter.hpp>

// external
#include <fmt/ranges.h>

namespace miru::params {

// ================================ ROS2 INTERFACES ================================ //
// The majority of the following code is take from ros2 rclcpp:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/src/rclcpp/parameter_value.cpp

std::string to_string(const ParameterType type) {
    switch (type) {
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
        case ParameterType::PARAMETER_BYTE_ARRAY:
            return "byte array";
        case ParameterType::PARAMETER_BOOL_ARRAY:
            return "bool array";
        case ParameterType::PARAMETER_INTEGER_ARRAY:
            return "int array";
        case ParameterType::PARAMETER_DOUBLE_ARRAY:
            return "double array";
        case ParameterType::PARAMETER_STRING_ARRAY:
            return "string array";
        case ParameterType::PARAMETER_NULL:
            return "null";
        case ParameterType::PARAMETER_SCALAR:
            return "scalar";
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            return "object array";
        case ParameterType::PARAMETER_OBJECT:
            return "object";
        default:
            return "unknown type";
    }
}

std::ostream& operator<<(std::ostream& os, const ParameterType& type) {
    os << to_string(type);
    return os;
}   

template<typename ValType, typename PrintType = ValType>
std::string
array_to_string(
  const std::vector<ValType> & array,
  const std::ios::fmtflags format_flags = std::ios::dec)
{
    std::stringstream type_array;
    bool first_item = true;
    type_array << "[";
    type_array.setf(format_flags, std::ios_base::basefield | std::ios::boolalpha);
    type_array << std::showbase;
    for (const ValType & value : array) {
        if (!first_item) {
            type_array << ", ";
        } else {
            first_item = false;
        }
        type_array << static_cast<PrintType>(value);
    }
    type_array << "]";
    return type_array.str();
}

ScalarValue::ScalarValue(
    std::string scalar,
    std::optional<bool> bool_value,
    std::optional<int64_t> int_value,
    std::optional<double> double_value
)
: scalar(scalar)
{
    if (bool_value.has_value()) {
        bool_value_ = bool_value.value();
        flags_ |= HAS_BOOL_VALUE;
    }
    if (int_value.has_value()) {
        int_value_ = int_value.value();
        flags_ |= HAS_INT_VALUE;
    }
    if (double_value.has_value()) {
        double_value_ = int_value.value();
        flags_ |= HAS_DOUBLE_VALUE;
    }
}

bool ScalarValue::operator==(const ScalarValue & other) const {
    return flags_ == other.flags_ &&
        scalar == other.scalar &&
        bool_value_ == other.bool_value_ &&
        int_value_ == other.int_value_ &&
        double_value_ == other.double_value_;
}

bool ScalarValue::operator!=(const ScalarValue & other) const {
    return !(*this == other);
}

const bool& ScalarValue::get_bool_value() const {
    if (!has_bool_value()) {
        throw ParameterTypeException(ParameterType::PARAMETER_BOOL, ParameterType::PARAMETER_SCALAR);
    }
    return bool_value_;
}

const int64_t& ScalarValue::get_int_value() const {
    if (!has_int_value()) {
        throw ParameterTypeException(ParameterType::PARAMETER_INTEGER, ParameterType::PARAMETER_SCALAR);
    }
    return int_value_;
}

const double& ScalarValue::get_double_value() const {
    if (!has_double_value()) {
        throw ParameterTypeException(ParameterType::PARAMETER_DOUBLE, ParameterType::PARAMETER_SCALAR);
    }
    return double_value_;
}

std::string to_string(const ParameterValue & value)
{
    switch (value.get_type()) {
        case ParameterType::PARAMETER_NOT_SET:
            return "not set";
        case ParameterType::PARAMETER_BOOL:
            return value.get<bool>() ? "true" : "false";
        case ParameterType::PARAMETER_INTEGER:
            return std::to_string(value.get<int>());
        case ParameterType::PARAMETER_DOUBLE:
            return std::to_string(value.get<double>());
        case ParameterType::PARAMETER_STRING:
            return value.get<std::string>();
        case ParameterType::PARAMETER_BYTE_ARRAY:
            return array_to_string<uint8_t, int>(value.get<std::vector<uint8_t>>(), std::ios::hex);
        case ParameterType::PARAMETER_BOOL_ARRAY:
            return array_to_string(value.get<std::vector<bool>>(), std::ios::boolalpha);
        case ParameterType::PARAMETER_INTEGER_ARRAY:
            return array_to_string(value.get<std::vector<int64_t>>());
        case ParameterType::PARAMETER_DOUBLE_ARRAY:
            return array_to_string(value.get<std::vector<double>>());
        case ParameterType::PARAMETER_STRING_ARRAY:
            return array_to_string(value.get<std::vector<std::string>>());
        case ParameterType::PARAMETER_NULL:
            return "null";
        case ParameterType::PARAMETER_SCALAR:
            return fmt::format("\"{}\"", value.get<std::string>());
        // FIXME @ben: this needs to be implemented
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            return "object array string representation not implemented";
        case ParameterType::PARAMETER_OBJECT:
            return "object string representation not implemented";
        default:
            return "unknown type";
    }
}

ParameterValue::ParameterValue(const bool bool_value)
{
    value_ = bool_value;
    type_ = ParameterType::PARAMETER_BOOL;
}

ParameterValue::ParameterValue(const int int_value)
{
    value_ = int_value;
    type_ = ParameterType::PARAMETER_INTEGER;
}

ParameterValue::ParameterValue(const int64_t int_value)
{
    value_ = int_value;
    type_ = ParameterType::PARAMETER_INTEGER;
}

ParameterValue::ParameterValue(const float double_value)
{
    value_ = static_cast<double>(double_value);
    type_ = ParameterType::PARAMETER_DOUBLE;
}

ParameterValue::ParameterValue(const double double_value)
{
    value_ = double_value;
    type_ = ParameterType::PARAMETER_DOUBLE;
}

ParameterValue::ParameterValue(const std::string & string_value)
{
    value_ = string_value;
    type_ = ParameterType::PARAMETER_STRING;
}

ParameterValue::ParameterValue(const char * string_value)
: ParameterValue(std::string(string_value))
{}

ParameterValue::ParameterValue(const std::vector<uint8_t> & byte_array_value)
{
    value_ = byte_array_value;
    type_ = ParameterType::PARAMETER_BYTE_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<bool> & bool_array_value)
{
    value_ = bool_array_value;
    type_ = ParameterType::PARAMETER_BOOL_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<int> & int_array_value)
{
    std::vector<int64_t> int64_array_value;
    int64_array_value.assign(int_array_value.cbegin(), int_array_value.cend());
    value_ = int64_array_value;
    type_ = ParameterType::PARAMETER_INTEGER_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<int64_t> & int_array_value)
{
    value_ = int_array_value;
    type_ = ParameterType::PARAMETER_INTEGER_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<float> & float_array_value)
{
    std::vector<double> double_array_value;
    double_array_value.assign(float_array_value.cbegin(), float_array_value.cend());
    value_ = double_array_value;
    type_ = ParameterType::PARAMETER_DOUBLE_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<double> & double_array_value)
{
    value_ = double_array_value;
    type_ = ParameterType::PARAMETER_DOUBLE_ARRAY;
}

ParameterValue::ParameterValue(const std::vector<std::string> & string_array_value)
{
    value_ = string_array_value;
    type_ = ParameterType::PARAMETER_STRING_ARRAY;
}

ParameterType ParameterValue::get_type() const {
    return type_;
}

bool ParameterValue::operator==(const ParameterValue& other) const {
    return type_ == other.type_ && value_ == other.value_;
}

bool ParameterValue::operator!=(const ParameterValue& other) const {
    return !(*this == other);
}

// ================================ MIRU INTERFACES ================================ //
ParameterValue::ParameterValue(
    const ParameterType& type,
    const std::vector<Parameter> & object_array_value) 
{
    if (type != ParameterType::PARAMETER_OBJECT_ARRAY &&
        type != ParameterType::PARAMETER_OBJECT) {
        throw ParameterTypeException(ParameterType::PARAMETER_OBJECT_ARRAY, type);
    }
    value_ = object_array_value;
    type_ = type;
}

ParameterValue ParameterValue::make_null() {
    return ParameterValue(ParameterType::PARAMETER_NULL);
}

ParameterValue ParameterValue::make_object(const std::vector<Parameter> & object_entries) {
    return ParameterValue(ParameterType::PARAMETER_OBJECT, object_entries);
}

ParameterValue ParameterValue::make_object_array(const std::vector<Parameter> & object_entries) {
    return ParameterValue(ParameterType::PARAMETER_OBJECT_ARRAY, object_entries);
}

bool ParameterValue::is_null() const {
    return type_ == ParameterType::PARAMETER_NULL;
}

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

bool ParameterValue::is_object() const {
    return type_ == ParameterType::PARAMETER_OBJECT;
}

bool ParameterValue::is_object_array() const {
    return type_ == ParameterType::PARAMETER_OBJECT_ARRAY;
}

// ================================ ROS2 INTERFACES ================================ //
// The majority of the following code is take from ros2 rclcpp:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/src/rclcpp/parameter.cpp

Parameter::Parameter() : name_("") {}

Parameter::Parameter(const std::string & name) : name_(name) {}

Parameter::Parameter(const std::string & name, const ParameterValue & value)
: name_(name), value_(value) {}


bool Parameter::operator==(const Parameter & other) const {
    return name_ == other.name_ && value_ == other.value_;
}

bool Parameter::operator!=(const Parameter & other) const {
    return !(*this == other);
}

const std::string& Parameter::get_name() const {
    return name_;
}

const ParameterValue& Parameter::get_parameter_value() const {
    return value_;
}

bool
Parameter::as_bool() const
{
    return get_value<ParameterType::PARAMETER_BOOL>();
}

int64_t
Parameter::as_int() const
{
    return get_value<ParameterType::PARAMETER_INTEGER>();
}

double
Parameter::as_double() const
{
    return get_value<ParameterType::PARAMETER_DOUBLE>();
}

const std::string &
Parameter::as_string() const
{
    return get_value<ParameterType::PARAMETER_STRING>();
}

const std::vector<uint8_t> &
Parameter::as_byte_array() const
{
    return get_value<ParameterType::PARAMETER_BYTE_ARRAY>();
}

const std::vector<bool> &
Parameter::as_bool_array() const
{
    return get_value<ParameterType::PARAMETER_BOOL_ARRAY>();
}

const std::vector<int64_t> &
Parameter::as_integer_array() const
{
    return get_value<ParameterType::PARAMETER_INTEGER_ARRAY>();
}

const std::vector<double> &
Parameter::as_double_array() const
{
    return get_value<ParameterType::PARAMETER_DOUBLE_ARRAY>();
}

const std::vector<std::string> &
Parameter::as_string_array() const
{
    return get_value<ParameterType::PARAMETER_STRING_ARRAY>();
}

const std::vector<Parameter> &
Parameter::as_object_array() const
{
    return get_value<ParameterType::PARAMETER_OBJECT_ARRAY>();
}

const std::vector<Parameter> &
Parameter::as_object() const
{
    return get_value<ParameterType::PARAMETER_OBJECT>();
}

std::string Parameter::value_to_string() const {
    return to_string(value_);
}

std::string _to_json_dict_entry(const Parameter & param)
{
  std::stringstream ss;
  ss << "\"" << param.get_name() << "\": ";
  ss << "{\"type\": \"" << param.get_type_name() << "\", ";
  ss << "\"value\": \"" << param.value_to_string() << "\"}";
  return ss.str();
}

std::ostream & operator<<(std::ostream & os, const Parameter & pv)
{
  os << to_string(pv);
  return os;
}

std::ostream & operator<<(std::ostream & os, const std::vector<Parameter> & parameters)
{
  os << to_string(parameters);
  return os;
}

std::string to_string(const Parameter & param)
{
  std::stringstream ss;
  ss << "{\"name\": \"" << param.get_name() << "\", ";
  ss << "\"type\": \"" << param.get_type_name() << "\", ";
  ss << "\"value\": \"" << param.value_to_string() << "\"}";
  return ss.str();
}

std::string to_string(const std::vector<Parameter> & parameters)
{
  std::stringstream ss;
  ss << "{";
  bool first = true;
  for (const auto & pv : parameters) {
    if (first == false) {
      ss << ", ";
    } else {
      first = false;
    }
    ss << _to_json_dict_entry(pv);
  }
  ss << "}";
  return ss.str();
}


} // namespace miru::params