#pragma once

// std
#include <string>

// internal
#include <miru/filesys/dir.hpp>

namespace miru::test_utils {

miru::filesys::Dir testdata_dir(); 
miru::filesys::Dir filesys_testdata_dir(); 

} // namespace miru::test_utils

