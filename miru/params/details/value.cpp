// internal
#include <miru/params/details/utils.hpp>
#include <miru/params/details/value.hpp>
#include <miru/params/parameter.hpp>
#include <miru/params/type.hpp>

namespace miru::params::details {

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
      return details::to_string(bool_array, 0, false);
    }
    case ParameterType::PARAMETER_INTEGER_ARRAY: {
      std::vector<ParameterValue> int_array;
      for (const int64_t& int_value : value.get<std::vector<int64_t>>()) {
        int_array.push_back(ParameterValue(int_value));
      }
      return details::to_string(int_array, 0, false);
    }
    case ParameterType::PARAMETER_DOUBLE_ARRAY: {
      std::vector<ParameterValue> double_array;
      for (const double& double_value : value.get<std::vector<double>>()) {
        double_array.push_back(ParameterValue(double_value));
      }
      return details::to_string(double_array, 0, false);
    }
    case ParameterType::PARAMETER_STRING_ARRAY: {
      std::vector<ParameterValue> string_array;
      for (const std::string& string_value : value.get<std::vector<std::string>>()) {
        string_array.push_back(ParameterValue(string_value));
      }
      return details::to_string(string_array, 0, false);
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
      return details::to_string(scalar_array, 0, false);
    }
    case ParameterType::PARAMETER_NESTED_ARRAY: {
      std::vector<ParameterValue> nested_array;
      for (const Parameter& param : value.get<NestedArray>()) {
        nested_array.push_back(param.get_parameter_value());
      }
      return details::to_string(nested_array, 0, true);
    }
    case ParameterType::PARAMETER_MAP: {
      return details::to_string(value.get<Map>(), indent);
    }
    case ParameterType::PARAMETER_MAP_ARRAY: {
      std::vector<ParameterValue> map_array;
      for (const Parameter& param : value.get<MapArray>()) {
        map_array.push_back(param.get_parameter_value());
      }
      return details::to_string(map_array, 0, true);
    }
  }
  return "unknown type";
}

std::string to_string(const miru::params::Map& map, const int indent) {
  std::stringstream type_array;
  bool first_item = true;
  type_array << std::string(indent, ' ') << "{";
  for (const auto& param : map) {
    if (!first_item) {
      type_array << ",";
    } else {
      first_item = false;
    }
    type_array << "\n";
    type_array << std::string(indent + 2, ' ');
    type_array << "\"" << param.get_key() << "\": ";
    type_array << to_string(param.get_parameter_value(), indent + 2);
  }
  type_array << "\n";
  type_array << std::string(indent, ' ') << "}";
  return type_array.str();
}

std::string to_string(
  const std::vector<ParameterValue>& array,
  const int indent,
  const bool with_newlines
) {
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

}  // namespace miru::params::details