#pragma once

// internal
#include <miru/errors.hpp>
#include <miru/filesys/file.hpp>
#include <miru/filesys/path.hpp>
#include <miru/utils.hpp>

namespace miru::filesys {

// file errors
class FileNotFoundError : public std::runtime_error {
 public:
  explicit FileNotFoundError(const std::string& path, const errors::ErrorTrace& trace)
    : std::runtime_error(format_message(path, trace)) {}

  static std::string
  format_message(const std::string& path, const errors::ErrorTrace& trace) {
    return "File '" + path + "' not found" + errors::format_source_location(trace);
  }
};

#define THROW_FILE_NOT_FOUND(path) throw miru::filesys::FileNotFoundError(path, ERROR_TRACE)

class NotAFileError : public std::runtime_error {
 public:
  explicit NotAFileError(const std::string& path, const errors::ErrorTrace& trace)
    : std::runtime_error(format_message(path, trace)) {}

  static std::string
  format_message(const std::string& path, const errors::ErrorTrace& trace) {
    return "File '" + path + "' exists but is not a file" +
           errors::format_source_location(trace);
  }
};

#define THROW_NOT_A_FILE(path) throw miru::filesys::NotAFileError(path, ERROR_TRACE)

class InvalidFileTypeError : public std::runtime_error {
 public:
  explicit InvalidFileTypeError(
    const std::string& file_path,
    const std::vector<std::string>& expected_file_types,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(file_path, expected_file_types, trace)) {}

  static std::string format_message(
    const std::string& file_path,
    const std::vector<std::string>& expected_file_types,
    const errors::ErrorTrace& trace
  ) {
    return "File '" + file_path + "' is not a valid file type. Expected one of: " +
           miru::utils::to_string(expected_file_types) +
           errors::format_source_location(trace);
  }
};

#define THROW_INVALID_FILE_TYPE(file_path, expected_file_types) \
  throw miru::filesys::InvalidFileTypeError(file_path, expected_file_types, ERROR_TRACE)

// directory errors
class DirNotFoundError : public std::runtime_error {
 public:
  explicit DirNotFoundError(const std::string& path, const errors::ErrorTrace& trace)
    : std::runtime_error(format_message(path, trace)) {}

  static std::string
  format_message(const std::string& path, const errors::ErrorTrace& trace) {
    return "Directory '" + path + "' does not exist" +
           errors::format_source_location(trace);
  }
};

#define THROW_DIR_NOT_FOUND(path) throw miru::filesys::DirNotFoundError(path, ERROR_TRACE)

class NotADirError : public std::runtime_error {
 public:
  explicit NotADirError(const std::string& path, const errors::ErrorTrace& trace)
    : std::runtime_error(format_message(path, trace)) {}

  static std::string
  format_message(const std::string& path, const errors::ErrorTrace& trace) {
    return "Path '" + path + "' exists but is not a directory" +
           errors::format_source_location(trace);
  }
};

#define THROW_NOT_A_DIR(path) throw miru::filesys::NotADirError(path, ERROR_TRACE)

class UnableToFindGitRepoError : public std::runtime_error {
 public:
  explicit UnableToFindGitRepoError(
    const std::string& path,
    const errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(path, trace)) {}

  static std::string
  format_message(const std::string& path, const errors::ErrorTrace& trace) {
    return "The path '" + path + "' is not part of a git repository" +
           errors::format_source_location(trace);
  }
};

#define THROW_UNABLE_TO_FIND_GIT_REPO(path) \
  throw miru::filesys::UnableToFindGitRepoError(path, ERROR_TRACE)

}  // namespace miru::filesys