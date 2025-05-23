// std
#include <stdexcept>

// internal
#include <configs/instance_impl.hpp>
#include <filesys/file.hpp>
#include <miru/details/errors.hpp>
#include <utils.hpp>

namespace miru::config {

class FromAgentOptionsError : public std::runtime_error {
 public:
  explicit FromAgentOptionsError(
    const std::string& message,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(
        message + miru::details::errors::format_source_location(trace)
      ) {}
};

#define THROW_FROM_AGENT_OPTIONS_ERROR(message) \
  throw FromAgentOptionsError(message, ERROR_TRACE)

class ConfigTypeSlugNotFound : public std::runtime_error {
 public:
  explicit ConfigTypeSlugNotFound(
    const miru::filesys::File& schema_file,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "Unable to find config type slug in schema file '" +
           schema_file.abs_path().string() + "' (expected the field '" +
           miru::config::MIRU_CONFIG_TYPE_SLUG_FIELD + "') " +
           "at the root of the schema file" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_CONFIG_TYPE_SLUG_NOT_FOUND(schema_file) \
  throw ConfigTypeSlugNotFound(schema_file, ERROR_TRACE)

class EmptyConfigTypeSlug : public std::runtime_error {
 public:
  explicit EmptyConfigTypeSlug(
    const miru::filesys::File& schema_file,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "Config type slug is in schema file '" + schema_file.abs_path().string() +
           "' cannot be empty ('')" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_CONFIG_TYPE_SLUG(schema_file) \
  throw EmptyConfigTypeSlug(schema_file, ERROR_TRACE)

class InvalidConfigSchemaFileTypeError : public std::runtime_error {
 public:
  explicit InvalidConfigSchemaFileTypeError(
    const miru::filesys::File& schema_file,
    const std::vector<std::string>& expected_file_types,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(schema_file, expected_file_types, trace)) {}

  static std::string format_message(
    const miru::filesys::File& schema_file,
    const std::vector<std::string>& expected_file_types,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "Invalid config schema file type '" + schema_file.abs_path().string() +
           "'. Expected one of: " + miru::utils::to_string(expected_file_types) +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_INVALID_CONFIG_SCHEMA_FILE_TYPE(schema_file, expected_file_types) \
  throw InvalidConfigSchemaFileTypeError(schema_file, expected_file_types, ERROR_TRACE)

class EmptyConfigInstance : public std::runtime_error {
 public:
  explicit EmptyConfigInstance(
    const std::string& config_type_slug,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(config_type_slug, trace)) {}

  static std::string format_message(
    const std::string& config_type_slug,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "The config instance loaded for config type slug '" + config_type_slug +
           "' is empty" + miru::details::errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_CONFIG_INSTANCE(config_type_slug) \
  throw EmptyConfigInstance(config_type_slug, ERROR_TRACE)

}  // namespace miru::config
