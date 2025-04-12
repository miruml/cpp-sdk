// internal
#include <miru/filesys/dir.hpp>

namespace miru::test_utils {

miru::filesys::Dir testdata_dir() {
  miru::filesys::Dir repo_dir = miru::filesys::Dir::from_current_dir().git_root();
  return miru::filesys::Dir(repo_dir.path() / "test" / "testdata");
}

miru::filesys::Dir config_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "config");
}

miru::filesys::Dir config_data_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "config" / "configs");
}

miru::filesys::Dir config_schemas_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "config" / "schemas");
}

miru::filesys::Dir filesys_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "filesys");
}

miru::filesys::Dir params_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "params");
}

miru::filesys::Dir query_testdata_dir() {
  miru::filesys::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::Dir(testdata_dir_.path() / "query");
}

}  // namespace miru::test_utils