#!/bin/bash
set -e

openapi_dir=../../openapi/all
backend_dir=..
backend_api_configs_dir=./
git_info_file=$backend_api_configs_dir/git-info.txt
gen_dir=./gen
helpers_dir=./helpers

# Check for any changes (staged, unstaged, or untracked)
cd $openapi_dir
pwd

make bundle-frontend

echo ""
echo "Checking for any changes (staged, unstaged, or untracked)"
if [ -z "$(git status --porcelain)" ]; then
    echo "There are no changes to commit. Proceeding to refresh the openapi spec for the backend."
else
    echo "To keep track of what openapi spec the server is using, we store the GitHub commit of the spec used. For this to be accurate, all current changes to the openapi repository must be committed. Please remove or commit your changes to the openapi repository before refreshing the openapi spec for the backend."
    exit 1
fi
repository_name=$(git remote get-url origin)
branch_name=$(git branch --show-current)
commit_hash=$(git rev-parse HEAD)
commit_message=$(git log -1 --pretty=%B)
cd -

# keep track of the branch name + commit hash
{
    echo "Repository Name: $repository_name"
    echo "Branch Name: $branch_name"
    echo "Commit Hash: $commit_hash"
    echo "Commit Message: $commit_message"
} > $git_info_file

# update the models in the docs
make gen

gen_server_models_dir=$gen_dir/server
gen_server_helpers_dir=$helpers_dir/server

gen_client_models_dir=$gen_dir/client
gen_client_helpers_dir=$helpers_dir/client

# copy all the files in the helpers dirs to the models dirs
cp "$gen_server_helpers_dir"/* "$gen_server_models_dir"
cp "$gen_client_helpers_dir"/* "$gen_client_models_dir"

target_server_models_dir=$backend_dir/internal/openapi/server
target_client_models_dir=$backend_dir/internal/openapi/client

# replace the target model directories with the generated ones
rm -rf "${target_server_models_dir:?}"/*
rm -rf "${target_client_models_dir:?}"/*

# copy all the files in the generated models dirs to the target models dirs
if [ ! -d "$target_server_models_dir" ]; then
    mkdir "$target_server_models_dir"
fi
cp -r "$gen_server_models_dir"/* "$target_server_models_dir"

if [ ! -d "$target_client_models_dir" ]; then
    mkdir "$target_client_models_dir"
fi
cp -r "$gen_client_models_dir"/* "$target_client_models_dir"




