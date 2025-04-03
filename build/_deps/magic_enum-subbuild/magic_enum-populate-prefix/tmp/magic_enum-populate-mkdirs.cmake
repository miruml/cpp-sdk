# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-src"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-build"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/tmp"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/src/magic_enum-populate-stamp"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/src"
  "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/src/magic_enum-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/src/magic_enum-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ben/miru/sdk/cpp/build/_deps/magic_enum-subbuild/magic_enum-populate-prefix/src/magic_enum-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
