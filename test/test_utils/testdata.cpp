// internal
#include <miru/filesys/dir.hpp>

namespace miru::test_utils {

miru::filesys::Dir testdata_dir() {
    miru::filesys::Dir repo_dir = miru::filesys::Dir::current_dir().git_repo_root_dir();
    return miru::filesys::Dir(repo_dir.path() / "test" / "testdata");
}

miru::filesys::Dir filesys_testdata_dir() {
    miru::filesys::Dir testdata_dir_ = testdata_dir();
    return miru::filesys::Dir(testdata_dir_.path() / "filesys");
}

} // namespace miru::test_utils