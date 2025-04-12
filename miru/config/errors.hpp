// std
#include <stdexcept>

// internal
#include <miru/filesys/file.hpp>
#include <miru/errors.hpp>

namespace miru::config {

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
           schema_file.abs_path().string() + "'" + errors::format_source_location(trace);
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
    return "Config slug is in schema file '" + schema_file.abs_path().string() + "' cannot be empty ('')" + errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_CONFIG_SLUG(schema_file) \
    throw EmptyConfigSlug(schema_file, ERROR_TRACE)

}  // namespace miru::config

