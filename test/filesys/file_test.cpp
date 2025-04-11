// std
#include <filesystem>

// internal
#include <miru/filesys/file.hpp>
#include <miru/filesys/errors.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

enum class FileExceptionType {
  // std errors
  BadOptionalAccess,

  // miru errors
  None,
  FileNotFound,
  NotAFile,
  InvalidFileType,
};

// ============================ file_type:string conversion ======================== //
struct FileTypeStringConversionTestCase {
  std::string test_name;
  std::optional<miru::filesys::FileType> file_type;
  std::string file_type_string;
  FileExceptionType expected_exception;
};

class FileTypeStringConversion
    : public testing::TestWithParam<FileTypeStringConversionTestCase> {};

TEST_P(FileTypeStringConversion, FileTypeStringConversion) {
  const auto& [_, opt_file_type, file_type_string, expected_exception] = GetParam();
  miru::filesys::FileType file_type;
  switch (expected_exception) {
    case FileExceptionType::None:
      file_type = opt_file_type.value();
      EXPECT_EQ(miru::filesys::file_type_to_string(file_type), file_type_string);
      EXPECT_EQ(miru::filesys::string_to_file_type(file_type_string), file_type);
      break;
    case FileExceptionType::BadOptionalAccess:
      EXPECT_THROW(miru::filesys::string_to_file_type(file_type_string),
                   std::bad_optional_access);
      break;
    default:
      FAIL() << "Unexpected exception type: " << static_cast<int>(expected_exception);
  }
}

std::string FileTypeStringConversionTestNameGenerator(
    const testing::TestParamInfo<FileTypeStringConversionTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
    File, FileTypeStringConversion,
    testing::Values(
        FileTypeStringConversionTestCase{"JSON", miru::filesys::FileType::JSON, "JSON",
                                         FileExceptionType::None},
        FileTypeStringConversionTestCase{"YAML", miru::filesys::FileType::YAML, "YAML",
                                         FileExceptionType::None},
        FileTypeStringConversionTestCase{"invalid file type", std::nullopt, "invalid",
                                         FileExceptionType::BadOptionalAccess}),
    FileTypeStringConversionTestNameGenerator);

// ================================ extension() ==================================== //
struct ExtensionTestCase {
  std::string test_name;
  std::filesystem::path path;
  std::string extension;
};

class Extensions : public testing::TestWithParam<ExtensionTestCase> {};

TEST_P(Extensions, Extension) {
  const auto& [test_name, path, extension] = GetParam();
  miru::filesys::File file(path);
  EXPECT_EQ(file.extension(), extension);
}

std::string ExtensionTestNameGenerator(
    const testing::TestParamInfo<ExtensionTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
    File, Extensions,
    testing::Values(
        ExtensionTestCase{"txt file",
                          std::filesystem::path("test") / "file_test" / "test.txt",
                          ".txt"},
        ExtensionTestCase{"json file", std::filesystem::path("arglebargle.json"),
                          ".json"},
        ExtensionTestCase{"yaml file", std::filesystem::path("lebron.james.yaml"),
                          ".yaml"},
        ExtensionTestCase{"xml file", std::filesystem::path("mj.xml"), ".xml"},
        ExtensionTestCase{"is a directory",
                          miru::test_utils::filesys_testdata_dir().path(), ""}),
    ExtensionTestNameGenerator);

// ================================= file_type() =================================== //
struct FileTypeTestCase {
  std::string test_name;
  std::filesystem::path path;
  std::optional<miru::filesys::FileType> file_type;
  FileExceptionType expected_exception;
};

class FileTypes : public testing::TestWithParam<FileTypeTestCase> {};

TEST_P(FileTypes, FileType) {
  const auto& [test_name, path, file_type, expected_exception] = GetParam();
  miru::filesys::File file(path);
  switch (expected_exception) {
    case FileExceptionType::None:
      EXPECT_EQ(file.file_type(), file_type);
      break;
    case FileExceptionType::InvalidFileType:
      EXPECT_THROW(file.file_type(), miru::filesys::InvalidFileTypeError);
      break;
    default:
      FAIL() << "Unexpected exception type: " << static_cast<int>(expected_exception);
  }
}
std::string FileTypeTestNameGenerator(
    const testing::TestParamInfo<FileTypeTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
    File, FileTypes,
    testing::Values(
        FileTypeTestCase{
            "json file", std::filesystem::path("arglebargle.json"),
            std::optional<miru::filesys::FileType>(miru::filesys::FileType::JSON),
            FileExceptionType::None},
        FileTypeTestCase{
            "yaml file", std::filesystem::path("lebron.james.yaml"),
            std::optional<miru::filesys::FileType>(miru::filesys::FileType::YAML),
            FileExceptionType::None},
        FileTypeTestCase{"text file", std::filesystem::path("mj.txt"), std::nullopt,
                         FileExceptionType::InvalidFileType},
        FileTypeTestCase{"xml file", std::filesystem::path("mj.xml"), std::nullopt,
                         FileExceptionType::InvalidFileType},
        FileTypeTestCase{"go file", std::filesystem::path("golang.go"), std::nullopt,
                         FileExceptionType::InvalidFileType}),
    FileTypeTestNameGenerator);

// ================================ assert_exists() ================================ //
struct AssertExistsTestCase {
  std::string test_name;
  std::filesystem::path path;
  FileExceptionType expected_exception;
};

class AssertExistsTest : public testing::TestWithParam<AssertExistsTestCase> {};

TEST_P(AssertExistsTest, AssertExists) {
  const AssertExistsTestCase& test_case = GetParam();
  miru::filesys::File file(test_case.path);
  switch (test_case.expected_exception) {
    case FileExceptionType::None:
      EXPECT_NO_THROW(file.assert_exists());
      break;
    case FileExceptionType::FileNotFound:
      EXPECT_THROW(file.assert_exists(), miru::filesys::FileNotFoundError);
      break;
    case FileExceptionType::NotAFile:
      EXPECT_THROW(file.assert_exists(), miru::filesys::NotAFileError);
      break;
    default:
      FAIL() << "Unexpected exception type: "
             << static_cast<int>(test_case.expected_exception);
  }
}

std::string AssertExistsTestNameGenerator(
    const testing::TestParamInfo<AssertExistsTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
    File, AssertExistsTest,
    testing::Values(
        // file exists
        AssertExistsTestCase{
            "golang file",
            miru::test_utils::filesys_testdata_dir().path() / "golang.go",
            FileExceptionType::None},
        AssertExistsTestCase{
            "json file",
            miru::test_utils::filesys_testdata_dir().path() / "invalid.json",
            FileExceptionType::None},
        AssertExistsTestCase{
            "yaml file",
            miru::test_utils::filesys_testdata_dir().path() / "invalid.yaml",
            FileExceptionType::None},
        AssertExistsTestCase{
            "text file", miru::test_utils::filesys_testdata_dir().path() / "text.txt",
            FileExceptionType::None},

        // file does not exists
        AssertExistsTestCase{
            "doesnt exist",
            miru::test_utils::filesys_testdata_dir().path() / "doesnt/exist.yaml",
            FileExceptionType::FileNotFound},
        AssertExistsTestCase{
            "doesnt exist and is a directory",
            miru::test_utils::filesys_testdata_dir().path() / "doesnt/exist",
            FileExceptionType::FileNotFound},

        // exists but is a directory
        AssertExistsTestCase{"is a directory",
                             miru::test_utils::filesys_testdata_dir().path(),
                             FileExceptionType::NotAFile}),
    AssertExistsTestNameGenerator);

// ================================= read_json() =================================== //
class ReadJson : public ::testing::Test {
 protected:
  miru::filesys::File doesnt_exist{"doesnt/exist.json"};
  miru::filesys::File invalid_file_type =
      miru::test_utils::filesys_testdata_dir().file("text.txt");
  miru::filesys::File invalid_json_file =
      miru::test_utils::filesys_testdata_dir().file("invalid.json");
  miru::filesys::File json_file =
      miru::test_utils::filesys_testdata_dir().file("valid.json");
};

TEST_F(ReadJson, FileNotFound) {
  EXPECT_THROW(doesnt_exist.read_json(), miru::filesys::FileNotFoundError);
}

TEST_F(ReadJson, InvalidFileType) {
  EXPECT_THROW(invalid_file_type.read_json(), miru::filesys::InvalidFileTypeError);
}

TEST_F(ReadJson, InvalidJsonFile) { EXPECT_ANY_THROW(invalid_json_file.read_json()); }

TEST_F(ReadJson, ValidJson) {
  auto json = json_file.read_json();
  EXPECT_EQ(json["name"], "Example Test Data");
}

// ================================= read_yaml() =================================== //
class ReadYaml : public ::testing::Test {
 protected:
  miru::filesys::File doesnt_exist{"doesnt/exist.yaml"};
  miru::filesys::File invalid_file_type =
      miru::test_utils::filesys_testdata_dir().file("text.txt");
  miru::filesys::File invalid_yaml_file =
      miru::test_utils::filesys_testdata_dir().file("invalid.yaml");
  miru::filesys::File json_file =
      miru::test_utils::filesys_testdata_dir().file("valid.json");
  miru::filesys::File yaml_file =
      miru::test_utils::filesys_testdata_dir().file("valid.yaml");
  miru::filesys::File yml_file =
      miru::test_utils::filesys_testdata_dir().file("valid.yml");
};

TEST_F(ReadYaml, FileNotFound) {
  EXPECT_THROW(doesnt_exist.read_yaml(), miru::filesys::FileNotFoundError);
}

TEST_F(ReadYaml, InvalidFileType) {
  EXPECT_THROW(invalid_file_type.read_yaml(), miru::filesys::InvalidFileTypeError);
}

TEST_F(ReadYaml, InvalidYamlFile) { EXPECT_ANY_THROW(invalid_yaml_file.read_yaml()); }

// the yaml-cpp parser can actually parse json files as well (since json is a subset of
// yaml) however if we want to read strict json then we should use the dedicated json
// parser
TEST_F(ReadYaml, ValidJson) {
  auto json = json_file.read_yaml();
  EXPECT_EQ(json["name"].as<std::string>(), "Example Test Data");
}

TEST_F(ReadYaml, ValidYaml) {
  auto yaml = yaml_file.read_yaml();
  EXPECT_EQ(yaml["name"].as<std::string>(), "Example Test Data");
}

TEST_F(ReadYaml, ValidYml) {
  auto yml = yml_file.read_yaml();
  EXPECT_EQ(yml["name"].as<std::string>(), "Example Test Data");
}

// ============================ read_structured_data() ============================= //
TEST(ReadStructuredData, FileNotFound) { EXPECT_EQ(false, true); }