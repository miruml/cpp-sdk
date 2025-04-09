#!/bin/bash

# to the source directory
cd ../

# Run clang-format on all files in the miru directory
find miru \( -name "*.hpp" -o -name "*.cpp" \) \
    -not -path "miru/client/models/*" \
    -exec clang-format -i {} +

# Run clang-format on all files in the test directory
find test \( -name "*.hpp" -o -name "*.cpp" \) \
    -exec clang-format -i {} +