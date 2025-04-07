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
        case ParameterType::PARAMETER_OBJECT:
            return "object";
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            return "object array";
    }
    return "unknown type"; 
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

std::string to_string(const Scalar & scalar) {
    return scalar.as_string();
}

std::ostream & operator<<(std::ostream & os, const Scalar & scalar) {
    os << to_string(scalar);
    return os;
}

std::vector<bool> to_boolean_array(const std::vector<Scalar> & scalars) {
    std::vector<bool> dest;
    dest.reserve(scalars.size());
    std::transform(
        scalars.begin(), 
        scalars.end(),
        std::back_inserter(dest),
        [](const Scalar& s) { return s.as_bool(); }
    );
    return dest;
}

std::vector<int64_t> to_integer_array(const std::vector<Scalar> & scalars) {
    std::vector<int64_t> dest;
    dest.reserve(scalars.size());
    std::transform(
        scalars.begin(), 
        scalars.end(),
        std::back_inserter(dest),
        [](const Scalar& s) { return s.as_int(); }
    );
    return dest;
}

std::vector<double> to_double_array(const std::vector<Scalar> & scalars) {
    std::vector<double> dest;
    dest.reserve(scalars.size());
    std::transform(
        scalars.begin(), 
        scalars.end(),
        std::back_inserter(dest),
        [](const Scalar& s) { return s.as_double(); }
    );
    return dest;
}

std::vector<std::string> to_string_array(const std::vector<Scalar> & scalars) {
    std::vector<std::string> dest;
    dest.reserve(scalars.size());
    std::transform(
        scalars.begin(), 
        scalars.end(),
        std::back_inserter(dest),
        [](const Scalar& s) { return s.as_string(); }
    );
    return dest;
}


// Interprets the scalar as a boolean using YAML boolean rules
// 
// https://yaml.org/type/bool.html
bool Scalar::as_bool() const {
    std::string lower = value_;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "y" || lower == "yes" || lower == "true" || lower == "on") {
        return true;
    }
    if (lower == "n" || lower == "no" || lower == "false" || lower == "off") {
        return false;
    }
    throw InvalidScalarConversion(
        "scalar",
        value_,
        to_string(ParameterType::PARAMETER_BOOL),
        "cannot interpret value as a boolean"
    );
}

int64_t Scalar::as_int() const {
    // use long long to ensure we maintain precision (long is not enough on all systems)
    return std::stoll(value_);
}

double Scalar::as_double() const {
    return std::stod(value_);
}

const std::vector<uint8_t> & Scalar::as_byte_array() const {
    // lazily cache the byte array to return
    if (!byte_array_is_set_) {
        byte_array_ = std::vector<uint8_t>(value_.begin(), value_.end());
        byte_array_is_set_ = true;
    }
    return byte_array_;
}


std::string to_string(const ParameterValue & value)
{
    switch (value.get_type()) {
        // ros 2 parameter types
        case ParameterType::PARAMETER_NOT_SET:
            return "\"not set\"";
        case ParameterType::PARAMETER_BOOL:
            return value.get<bool>() ? "true" : "false";
        case ParameterType::PARAMETER_INTEGER:
            return std::to_string(value.get<int>());
        case ParameterType::PARAMETER_DOUBLE:
            return std::to_string(value.get<double>());
        case ParameterType::PARAMETER_STRING:
            return "\"" + value.get<std::string>() + "\"";
        case ParameterType::PARAMETER_BOOL_ARRAY:
            return array_to_string(value.get<std::vector<bool>>(), std::ios::boolalpha);
        case ParameterType::PARAMETER_INTEGER_ARRAY:
            return array_to_string(value.get<std::vector<int64_t>>());
        case ParameterType::PARAMETER_DOUBLE_ARRAY:
            return array_to_string(value.get<std::vector<double>>());
        case ParameterType::PARAMETER_STRING_ARRAY:
            return array_to_string(value.get<std::vector<std::string>>());

        // miru parameter types
        case ParameterType::PARAMETER_NULL:
            return "null";
        case ParameterType::PARAMETER_SCALAR:
            return "\"" + value.get<Scalar>().as_string() + "\"";
        case ParameterType::PARAMETER_SCALAR_ARRAY:
            return array_to_string(value.get<std::vector<Scalar>>());
        case ParameterType::PARAMETER_NESTED_ARRAY:
            return array_to_string(value.get<NestedArray>());
        case ParameterType::PARAMETER_OBJECT_ARRAY:
            return array_to_string(value.get<ObjectArray>());
        case ParameterType::PARAMETER_OBJECT:
            return to_string(value.get<Object>());
    }
    return "unknown type";
}

ParameterValue::ParameterValue() {
    value_ = nullptr;
    type_ = ParameterType::PARAMETER_NOT_SET;
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

ParameterValue::ParameterValue(const Scalar & scalar_value) {
    value_ = scalar_value;
    type_ = ParameterType::PARAMETER_SCALAR;
}

ParameterValue::ParameterValue(const std::vector<Scalar> & scalar_array_value) {
    value_ = scalar_array_value;
    type_ = ParameterType::PARAMETER_SCALAR_ARRAY;
}

ParameterValue::ParameterValue(const NestedArray & nested_array_value) {
    value_ = nested_array_value;
    type_ = ParameterType::PARAMETER_NESTED_ARRAY;
}

ParameterValue::ParameterValue(const Object & object_value) {
    value_ = object_value;
    type_ = ParameterType::PARAMETER_OBJECT;
}

ParameterValue::ParameterValue(const ObjectArray & object_array_value)  {
    value_ = object_array_value;
    type_ = ParameterType::PARAMETER_OBJECT_ARRAY;
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

bool ParameterValue::is_object_array() const {
    return type_ == ParameterType::PARAMETER_OBJECT_ARRAY;
}

bool ParameterValue::is_array() const {
    return is_scalar_array() || is_nested_array() || is_object_array();
}

bool ParameterValue::is_leaf() const {
    return !(is_object() || is_object_array());
}

} // namespace miru::params