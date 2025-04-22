// std
#include <stdexcept>

// internal
#include <miru/errors.hpp>
#include <miru/filesys/file.hpp>
#include <miru/utils.hpp>

namespace miru::config {

class FromAgentOptionsError : public std::runtime_error {
 public:
  explicit FromAgentOptionsError(
    const std::string& message,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(message + errors::format_source_location(trace)) {}
};

#define THROW_FROM_AGENT_OPTIONS_ERROR(message) \
  throw FromAgentOptionsError(message, ERROR_TRACE)

class ConfigSlugNotFound : public std::runtime_error {
 public:
  explicit ConfigSlugNotFound(
    const miru::filesys::File& schema_file,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const errors::ErrorTrace& trace
  ) {
    return "Unable to find config slug in schema file '" +
           schema_file.abs_path().string() + "'" +
           errors::format_source_location(trace);
  }
};

#define THROW_CONFIG_SLUG_NOT_FOUND(schema_file) \
  throw ConfigSlugNotFound(schema_file, ERROR_TRACE)

class EmptyConfigSlug : public std::runtime_error {
 public:
  explicit EmptyConfigSlug(
    const miru::filesys::File& schema_file,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const errors::ErrorTrace& trace
  ) {
    return "Config slug is in schema file '" + schema_file.abs_path().string() +
           "' cannot be empty ('')" + errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_CONFIG_SLUG(schema_file) \
  throw EmptyConfigSlug(schema_file, ERROR_TRACE)

class InvalidConfigSchemaFileTypeError : public std::runtime_error {
 public:
  explicit InvalidConfigSchemaFileTypeError(
    const miru::filesys::File& schema_file,
    const std::vector<std::string>& expected_file_types,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, expected_file_types, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const std::vector<std::string>& expected_file_types,
    const errors::ErrorTrace& trace
  ) {
    return "Invalid config schema file type '" + schema_file.abs_path().string() +
           "'. Expected one of: " + miru::utils::to_string(expected_file_types) +
           errors::format_source_location(trace);
  }
};

#define THROW_INVALID_CONFIG_SCHEMA_FILE_TYPE(schema_file, expected_file_types) \
  throw InvalidConfigSchemaFileTypeError(schema_file, expected_file_types, ERROR_TRACE)

class EmptyConcreteConfig : public std::runtime_error {
 public:
  explicit EmptyConcreteConfig(
    const std::string& config_slug,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(config_slug, trace)) {}

  static std::string
  format_message(const std::string& config_slug, const errors::ErrorTrace& trace) {
    return "The concrete config loaded for config slug '" + config_slug + "' is empty" +
           errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_CONCRETE_CONFIG(config_slug) \
  throw EmptyConcreteConfig(config_slug, ERROR_TRACE)

}  // namespace miru::config
