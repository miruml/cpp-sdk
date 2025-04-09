// internal
#include <miru/params/parameter.hpp>
#include <miru/utils.hpp>

// external
#include <fmt/ranges.h>

namespace miru::params {

// ================================ ROS2 INTERFACES ================================ //
// The majority of the following code is take from ros2 rclcpp:
// https://github.com/ros2/rclcpp/blob/a0a2a067d84fd6a38ab4f71b691d51ca5aa97ba5/rclcpp/src/rclcpp/parameter.cpp

Parameter::Parameter() : name_("") {}

Parameter::Parameter(const std::string& name) : name_(name) {}

void validate_child_parent_name_consistency(const std::string& parent_name,
                                   const Parameter& child) {
  // the child's name must follow its parent's name
  if (child.get_parent_name() != parent_name) {
    throw ChildParentNameMismatch(
      "Parameter",
      child.get_name(),
      child.get_parent_name(),
      parent_name
    );
  }
}

Parameter::Parameter(const std::string& name, const ParameterValue& value)
    : name_(name), value_(value) {
  // remove any trailing slashes from the name
  name_ = utils::remove_trailing(name_, "/");

  // check that the key doesn't have any slashes
  std::vector<std::string> child_names;
  switch (value_.get_type()) {
    case ParameterType::PARAMETER_MAP:
      for (const auto& field : value_.get<ParameterType::PARAMETER_MAP>()) {
        validate_child_parent_name_consistency(name_, field);
      }
      break;
    case ParameterType::PARAMETER_MAP_ARRAY:
      for (const auto& item : value_.get<ParameterType::PARAMETER_MAP_ARRAY>()) {
        validate_child_parent_name_consistency(name_, item);
      }
      break;
    case ParameterType::PARAMETER_NESTED_ARRAY:
      for (const auto& item : value_.get<ParameterType::PARAMETER_NESTED_ARRAY>()) {
        validate_child_parent_name_consistency(name_, item);
      }
      break;
    default:
      // has no child parameters with names to validate -> return
      return;
  }
}

bool Parameter::operator==(const Parameter& other) const {
  return name_ == other.name_ && value_ == other.value_;
}

bool Parameter::operator!=(const Parameter& other) const { return !(*this == other); }

ParameterType Parameter::get_type() const { return value_.get_type(); }

std::string Parameter::get_type_name() const { return to_string(get_type()); }

const std::string& Parameter::get_name() const { return name_; }

const ParameterValue& Parameter::get_parameter_value() const { return value_; }

bool Parameter::as_bool() const { return get_value<ParameterType::PARAMETER_BOOL>(); }

int64_t Parameter::as_int() const {
  return get_value<ParameterType::PARAMETER_INTEGER>();
}

double Parameter::as_double() const {
  return get_value<ParameterType::PARAMETER_DOUBLE>();
}

const std::string& Parameter::as_string() const {
  return get_value<ParameterType::PARAMETER_STRING>();
}

const std::vector<bool>& Parameter::as_bool_array() const {
  return get_value<ParameterType::PARAMETER_BOOL_ARRAY>();
}

const std::vector<int64_t>& Parameter::as_integer_array() const {
  return get_value<ParameterType::PARAMETER_INTEGER_ARRAY>();
}

const std::vector<double>& Parameter::as_double_array() const {
  return get_value<ParameterType::PARAMETER_DOUBLE_ARRAY>();
}

const std::vector<std::string>& Parameter::as_string_array() const {
  return get_value<ParameterType::PARAMETER_STRING_ARRAY>();
}

std::string Parameter::value_to_string() const { return to_string(value_); }

std::ostream& operator<<(std::ostream& os, const Parameter& pv) {
  os << to_string(pv);
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<Parameter>& parameters) {
  os << to_string(parameters);
  return os;
}

std::string to_string(const Parameter& param) {
  std::stringstream ss;
  ss << "{\"name\": \"" << param.get_name() << "\", ";
  ss << "\"key\": \"" << param.get_key() << "\", ";
  ss << "\"type\": \"" << param.get_type_name() << "\", ";
  ss << "\"value\": " << param.value_to_string() << "}";
  return ss.str();
}

std::string to_string(const std::vector<Parameter>& parameters) {
  auto add_param_entry = [](const Parameter& param) {
    std::stringstream ss;
    ss << "\"" << param.get_key() << "\": ";
    ss << "{\"type\": \"" << param.get_type_name() << "\", ";
    ss << "\"value\": \"" << param.value_to_string() << "\"}";
    return ss.str();
  };

  std::stringstream ss;
  ss << "{";
  bool first = true;
  for (const auto& pv : parameters) {
    if (first == false) {
      ss << ", ";
    } else {
      first = false;
    }
    ss << add_param_entry(pv);
  }
  ss << "}";
  return ss.str();
}

// ================================ MIRU INTERFACES ================================ //

std::string Parameter::get_key() const {
  return name_.substr(name_.find_last_of("/") + 1, name_.length());
}

std::string Parameter::get_parent_name() const {
  return utils::remove_trailing(
    name_.substr(0, name_.length() - get_key().length()),
    "/"
  );
}

const std::nullptr_t Parameter::as_null() const {
  return get_value<ParameterType::PARAMETER_NULL>();
}

const Scalar& Parameter::as_scalar() const {
  return get_value<ParameterType::PARAMETER_SCALAR>();
}

const std::vector<Scalar>& Parameter::as_scalar_array() const {
  return get_value<ParameterType::PARAMETER_SCALAR_ARRAY>();
}

const NestedArray& Parameter::as_nested_array() const {
  return get_value<ParameterType::PARAMETER_NESTED_ARRAY>();
}

const Map& Parameter::as_map() const {
  return get_value<ParameterType::PARAMETER_MAP>();
}

const MapArray& Parameter::as_map_array() const {
  return get_value<ParameterType::PARAMETER_MAP_ARRAY>();
}

bool Parameter::is_null() const { return value_.is_null(); }

bool Parameter::is_scalar() const { return value_.is_scalar(); }

bool Parameter::is_scalar_array() const { return value_.is_scalar_array(); }

bool Parameter::is_nested_array() const { return value_.is_nested_array(); }

bool Parameter::is_map() const { return value_.is_map(); }

bool Parameter::is_map_array() const { return value_.is_map_array(); }

bool Parameter::is_array() const { return value_.is_array(); }

}  // namespace miru::params