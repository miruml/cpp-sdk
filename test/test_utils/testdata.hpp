#pragma once

// internal
#include <miru/filesys/details/dir.hpp>

namespace miru::test_utils {

miru::filesys::details::Dir testdata_dir();
miru::filesys::details::Dir config_testdata_dir();
miru::filesys::details::Dir config_data_testdata_dir();
miru::filesys::details::Dir config_schemas_testdata_dir();
miru::filesys::details::Dir filesys_testdata_dir();
miru::filesys::details::Dir params_testdata_dir();
miru::filesys::details::Dir query_testdata_dir();

}  // namespace miru::test_utils
