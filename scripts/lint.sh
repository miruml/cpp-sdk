#!/bin/sh
set -e

this_repo_root_dir=$(git rev-parse --show-toplevel)
cd "$this_repo_root_dir"

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