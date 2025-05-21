#pragma once

// internal
#include <filesys/dir.hpp>

namespace miru::test_utils {

miru::filesys::Dir testdata_dir();
miru::filesys::Dir config_testdata_dir();
miru::filesys::Dir config_instances_testdata_dir();
miru::filesys::Dir config_schemas_testdata_dir();
miru::filesys::Dir filesys_testdata_dir();
miru::filesys::Dir params_testdata_dir();
miru::filesys::Dir query_testdata_dir();

}  // namespace miru::test_utils
