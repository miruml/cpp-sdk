// internal
#include <miru/filesys/file.hpp>
#include <miru/params/parameter.hpp>
#include <miru/params/parse.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

miru::params::Parameter parse_json_node(
  const std::string& name, const nlohmann::json& node
) {
  switch (node.type()) {
    case nlohmann::json::value_t::discarded:
      throw std::runtime_error("Discarded node");
    case nlohmann::json::value_t::null:
      return miru::params::Parameter(name, nullptr);
    case nlohmann::json::value_t::boolean:
      return miru::params::Parameter(name, node.get<bool>());
    case nlohmann::json::value_t::number_integer:
      return miru::params::Parameter(name, node.get<int>());
    case nlohmann::json::value_t::number_unsigned:
      return miru::params::Parameter(name, node.get<unsigned int>());
    case nlohmann::json::value_t::number_float:
      return miru::params::Parameter(name, node.get<double>());
    case nlohmann::json::value_t::binary:
      throw std::runtime_error(
        "Binary values are not supported. Please contact Ben at ben@miruml.com if "
        "you need this feature."
      );
    case nlohmann::json::value_t::string:
      return miru::params::Parameter(name, node.get<std::string>());
    case nlohmann::json::value_t::array:
      return parse_json_array(name, node);
    case nlohmann::json::value_t::object: {
      std::vector<miru::params::Parameter> entries;
      for (const auto& entry : node.items()) {
        std::string entry_name = name + DELIMITER + entry.key();
        entries.push_back(parse_json_node(entry_name, entry.value()));
      }
      return miru::params::Parameter(name, miru::params::Map(entries));
    }
  }
  throw std::runtime_error("Unsupported node type");
}

miru::params::Parameter parse_json_array(
  const std::string& name, const nlohmann::json& node
) {
  // double check the node is an array
  if (!node.is_array()) {
    throw std::runtime_error("Node is not an array");
  }

  // if it's empty then just return an empty scalar array
  if (node.empty()) {
    return miru::params::Parameter(
      name, miru::params::ParameterValue(std::vector<Scalar>())
    );
  }

  // create an array of the correct type
  nlohmann::json first_entry = node[0];
  switch (first_entry.type()) {
    case nlohmann::json::value_t::discarded: {
      throw std::runtime_error("Discarded node");
    }
    case nlohmann::json::value_t::null: {
      throw std::runtime_error(
        "Null values are not supported in arrays. Please contact Ben at "
        "ben@miruml.com if you need this feature."
      );
    }
    case nlohmann::json::value_t::boolean: {
      std::vector<bool> array = node.get<std::vector<bool>>();
      return miru::params::Parameter(name, miru::params::ParameterValue(array));
    }
    case nlohmann::json::value_t::number_integer: {
      std::vector<int64_t> array = node.get<std::vector<int64_t>>();
      return miru::params::Parameter(name, miru::params::ParameterValue(array));
    }
    case nlohmann::json::value_t::number_unsigned: {
      // this is lossy ??
      std::vector<int64_t> array = node.get<std::vector<int64_t>>();
      return miru::params::Parameter(name, miru::params::ParameterValue(array));
    }
    case nlohmann::json::value_t::number_float: {
      std::vector<double> array = node.get<std::vector<double>>();
      return miru::params::Parameter(name, miru::params::ParameterValue(array));
    }
    case nlohmann::json::value_t::binary: {
      throw std::runtime_error(
        "Binary values are not supported. Please contact Ben at ben@miruml.com if "
        "you need this feature."
      );
    }
    case nlohmann::json::value_t::string: {
      std::vector<std::string> array = node.get<std::vector<std::string>>();
      return miru::params::Parameter(name, miru::params::ParameterValue(array));
    }
    case nlohmann::json::value_t::array: {
      std::vector<miru::params::Parameter> entries;
      int i = 0;
      for (const auto& entry : node.items()) {
        if (entry.value().type() != nlohmann::json::value_t::array) {
          throw std::runtime_error(
            "Heterogeneous array types are not supported. Please contact Ben at "
            "ben@miruml.com if you need this feature."
          );
        }
        std::string entry_name = name + DELIMITER + std::to_string(i);
        entries.push_back(parse_json_array(entry_name, entry.value()));
        i++;
      }
      return miru::params::Parameter(name, miru::params::NestedArray(entries));
    }
    case nlohmann::json::value_t::object: {
      std::vector<miru::params::Parameter> entries;
      int i = 0;
      for (const auto& entry : node.items()) {
        if (entry.value().type() != nlohmann::json::value_t::object) {
          throw std::runtime_error(
            "Heterogeneous array types are not supported. Please contact Ben at "
            "ben@miruml.com if you need this feature."
          );
        }
        std::string entry_name = name + DELIMITER + std::to_string(i);
        entries.push_back(parse_json_node(entry_name, entry.value()));
        i++;
      }
      return miru::params::Parameter(name, miru::params::MapArray(entries));
    }
  }
  throw std::runtime_error("Unsupported node type");
}

miru::params::Parameter parse_yaml_array(
  const std::string& name, const YAML::Node& node
) {
  // double check the node is an array
  if (!node.IsSequence()) {
    throw std::runtime_error("Node is not an array");
  }

  // if it's empty than just return an empty scalar array
  if (node.size() == 0) {
    return miru::params::Parameter(
      name, miru::params::ParameterValue(std::vector<Scalar>())
    );
  }

  // create an array of the correct type
  YAML::Node first_entry = node[0];
  switch (first_entry.Type()) {
    case YAML::NodeType::Undefined: {
      throw std::runtime_error("Undefined node");
    }
    case YAML::NodeType::Null: {
      throw std::runtime_error(
        "Null values are not supported in arrays. Please contact Ben at "
        "ben@miruml.com if you need this feature."
      );
    }
    case YAML::NodeType::Scalar: {
      std::vector<std::string> array = node.as<std::vector<std::string>>();
      std::vector<Scalar> scalar_array;
      for (const auto& scalar : array) {
        scalar_array.push_back(Scalar(scalar));
      }
      return miru::params::Parameter(name, miru::params::ParameterValue(scalar_array));
    }
    case YAML::NodeType::Sequence: {
      std::vector<miru::params::Parameter> entries;
      int i = 0;
      for (const auto& entry : node) {
        if (entry.Type() != YAML::NodeType::Sequence) {
          throw std::runtime_error(
            "Heterogeneous array types are not supported. Please contact Ben at "
            "ben@miruml.com if you need this feature."
          );
        }
        std::string entry_name = name + DELIMITER + std::to_string(i);
        entries.push_back(parse_yaml_array(entry_name, entry));
        i++;
      }
      return miru::params::Parameter(name, miru::params::NestedArray(entries));
    }
    case YAML::NodeType::Map: {
      std::vector<miru::params::Parameter> entries;
      int i = 0;
      for (const auto& entry : node) {
        if (entry.Type() != YAML::NodeType::Map) {
          throw std::runtime_error(
            "Heterogeneous array types are not supported. Please contact Ben at "
            "ben@miruml.com if you need this feature."
          );
        }
        std::string entry_name = name + DELIMITER + std::to_string(i);
        entries.push_back(parse_yaml_node(entry_name, entry));
        i++;
      }
      return miru::params::Parameter(name, miru::params::MapArray(entries));
    }
  }
  throw std::runtime_error("Unsupported node type");
}

miru::params::Parameter parse_yaml_node(
  const std::string& name, const YAML::Node& node
) {
  switch (node.Type()) {
    case YAML::NodeType::Undefined:
      throw std::runtime_error("Undefined node");
    case YAML::NodeType::Null:
      return miru::params::Parameter(name, nullptr);
    case YAML::NodeType::Scalar:
      return miru::params::Parameter(name, Scalar(node.as<std::string>()));
    case YAML::NodeType::Sequence:
      return parse_yaml_array(name, node);
    case YAML::NodeType::Map: {
      std::vector<miru::params::Parameter> entries;
      for (const auto& it : node) {
        std::string entry_name = name + DELIMITER + it.first.as<std::string>();
        entries.push_back(parse_yaml_node(entry_name, it.second));
      }
      return miru::params::Parameter(name, miru::params::Map(entries));
    }
  }
  throw std::runtime_error("Unsupported node type");
}

miru::params::Parameter parse_structured_data(
  const std::string& name, const std::variant<nlohmann::json, YAML::Node>& node
) {
  if (std::holds_alternative<nlohmann::json>(node)) {
    return parse_json_node(name, std::get<nlohmann::json>(node));
  } else if (std::holds_alternative<YAML::Node>(node)) {
    return parse_yaml_node(name, std::get<YAML::Node>(node));
  }
  throw std::runtime_error("Unsupported node type");
}

miru::params::Parameter parse_file(
  const std::string& name, const miru::filesys::File& file
) {
  std::variant<nlohmann::json, YAML::Node> data = file.read_structured_data();
  return parse_structured_data(name, data);
}

}  // namespace miru::params