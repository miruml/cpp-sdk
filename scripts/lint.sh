#!/bin/sh

# to the source directory
cd ../

lint() {
    dir=$1
    exclude=$2

    find "$dir" \( -name "*.hpp" -o -name "*.cpp" \) \
        -not -path "$exclude" \
        -exec clang-format -i {} +
}

lint include
lint src "src/http/models/*"
lint test 