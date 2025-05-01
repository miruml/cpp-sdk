#pragma once

// std
#include <stdexcept>
#include <string>
#include <vector>

// internal
#include <miru/details/errors.hpp>
#include <miru/params/type.hpp>
#include <utils.hpp>

namespace miru::params {

class EmptyInitializationError : public std::invalid_argument {
 public:
  EmptyInitializationError(
    const std::string& object_to_initialize,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return object_to_initialize + " must be initialized with at least one item" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_INITIALIZATION(object_to_initialize) \
  throw EmptyInitializationError(object_to_initialize, ERROR_TRACE)

class DuplicateFieldNamesError : public std::invalid_argument {
 public:
  DuplicateFieldNamesError(
    const std::string& object_to_initialize,
    const std::string& duplicate_field_name,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(
        format_message(object_to_initialize, duplicate_field_name, trace)
      ) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& duplicate_field_name,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with duplicate field name: " + duplicate_field_name +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_DUPLICATE_FIELD_NAMES(object_to_initialize, duplicate_field_name) \
  throw DuplicateFieldNamesError(                                               \
    object_to_initialize, duplicate_field_name, ERROR_TRACE                     \
  )

class MismatchingParentNamesError : public std::invalid_argument {
 public:
  MismatchingParentNamesError(
    const std::string& object_to_initialize,
    const std::string& param1_name,
    const std::string& param1_parent_name,
    const std::string& param2_name,
    const std::string& param2_parent_name,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(
        object_to_initialize,
        param1_name,
        param1_parent_name,
        param2_name,
        param2_parent_name,
        trace
      )) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& param1_name,
    const std::string& param1_parent_name,
    const std::string& param2_name,
    const std::string& param2_parent_name,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with mismatching parent names. Every field must have "
           "the same parent name but field '" +
           param1_name + "' has parent name '" + param1_parent_name +
           "' while field '" + param2_name + "' has parent name '" +
           param2_parent_name + "'" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_MISMATCHING_PARENT_NAMES( \
  object_to_initialize,                 \
  param1_name,                          \
  param1_parent_name,                   \
  param2_name,                          \
  param2_parent_name                    \
)                                       \
  throw MismatchingParentNamesError(    \
    object_to_initialize,               \
    param1_name,                        \
    param1_parent_name,                 \
    param2_name,                        \
    param2_parent_name,                 \
    ERROR_TRACE                         \
  )

class EmptyParentNameError : public std::invalid_argument {
 public:
  EmptyParentNameError(
    const std::string& object_to_initialize,
    const std::string& param_name,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(object_to_initialize, param_name, trace)) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& param_name,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "unable to initialize " + object_to_initialize +
           " with empty parent name. Every field must have a parent name but field '" +
           param_name + "' has an empty parent name" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_EMPTY_PARENT_NAME(object_to_initialize, param_name) \
  throw EmptyParentNameError(object_to_initialize, param_name, ERROR_TRACE)

class InvalidArrayKeysError : public std::invalid_argument {
 public:
  InvalidArrayKeysError(
    const std::string& object_to_initialize,
    int index,
    const std::vector<std::string>& item_names,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(
        format_message(object_to_initialize, index, item_names, trace)
      ) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    int index,
    const std::vector<std::string>& item_names,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "items in " + object_to_initialize +
           " must have keys that are integers in ascending order. "
           "Unable to find index '" +
           std::to_string(index) + "' in items '" + miru::utils::to_string(item_names) +
           "'" + miru::details::errors::format_source_location(trace);
  }
};

#define THROW_INVALID_ARRAY_KEYS(object_to_initialize, index, item_names) \
  throw InvalidArrayKeysError(object_to_initialize, index, item_names, ERROR_TRACE)

class ChildParentNameMismatchError : public std::invalid_argument {
 public:
  ChildParentNameMismatchError(
    const std::string& object_to_initialize,
    const std::string& child_name,
    const std::string& child_parent_name,
    const std::string& parent_name,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::invalid_argument(format_message(
        object_to_initialize,
        child_name,
        child_parent_name,
        parent_name,
        trace
      )) {}

  static std::string format_message(
    const std::string& object_to_initialize,
    const std::string& child_name,
    const std::string& child_parent_name,
    const std::string& parent_name,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "child parameter with name '" + child_name +
           "' must have its parent parameter name '" + parent_name +
           "' as a prefix but has parent name '" + child_parent_name + "'" +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_CHILD_PARENT_NAME_MISMATCH(                                         \
  object_to_initialize, child_name, child_parent_name, parent_name                \
)                                                                                 \
  throw ChildParentNameMismatchError(                                             \
    object_to_initialize, child_name, child_parent_name, parent_name, ERROR_TRACE \
  )

}  // namespace miru::params