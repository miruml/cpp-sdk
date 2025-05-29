#!/bin/bash
set -e

this_repo_root_dir=$(git rev-parse --show-toplevel)
this_dir=$this_repo_root_dir/api
gen_dir=$this_dir/gen

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
