// internal
#include <miru/filesys/details/dir.hpp>

namespace miru::test_utils {

miru::filesys::details::Dir testdata_dir() {
  miru::filesys::details::Dir repo_dir = miru::filesys::details::Dir::from_current_dir().git_root();
  return miru::filesys::details::Dir(repo_dir.path() / "test" / "testdata");
}

miru::filesys::details::Dir config_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "config");
}

miru::filesys::details::Dir config_data_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "config" / "configs");
}

miru::filesys::details::Dir config_schemas_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "config" / "schemas");
}

miru::filesys::details::Dir filesys_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "filesys");
}

miru::filesys::details::Dir params_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "params");
}

miru::filesys::details::Dir query_testdata_dir() {
  miru::filesys::details::Dir testdata_dir_ = testdata_dir();
  return miru::filesys::details::Dir(testdata_dir_.path() / "query");
}

}  // namespace miru::test_utils