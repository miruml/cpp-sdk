#!/bin/bash
set -e

this_repo_root_dir=$(git rev-parse --show-toplevel)
this_dir=$this_repo_root_dir/api
openapi_dir=$this_dir/openapi
gen_dir=$this_dir/gen

# Check for any changes (staged, unstaged, or untracked)
cd "$openapi_dir"

echo ""
echo "Checking for any changes (staged, unstaged, or untracked)"
if [ -z "$(git status --porcelain)" ]; then
    echo "There are no changes to commit"
else
    echo "To keep track of what openapi spec the server is using, we store the GitHub commit of the spec used. For this to be accurate, all current changes to the openapi repository must be committed. Please remove or commit your changes to the openapi repository before refreshing the openapi spec."
    exit 1
fi

# generate the models
cd "$this_dir"
make gen

gen_models_dir="$gen_dir/model"
target_models_dir="$this_repo_root_dir/src/http/models"

# replace the target model directories with the generated ones
rm -rf "${target_models_dir:?}"/*

# copy all the files in the generated models dirs to the target models dirs
if [ ! -d "$target_models_dir" ]; then
    mkdir "$target_models_dir"
fi
cp -r "$gen_models_dir"/* "$target_models_dir"
