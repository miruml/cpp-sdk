# Miru C++ SDK
This repository contains the [miru](https://www.miruml.com/) C++ SDK for retrieving configs from the miru agent on your device.

## Organization

This repository aims to follow the organization recommended by [Modern CMake](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)

| Directory | Description |
|-----------|-------------|
| cmake     | files for configuring and building with cmake |
| examples  | simple examples using the miru sdk |
| include   | public header files for the sdk |
| scripts   | scripts for the development and release process |
| src       | miru sdk implementation |
| test      | test cases |

## Build Requirements

1. C++17 and above
2. CMake 3.16 or higher
3. POSIX Threads (should be provided by your system)

All other dependencies are fetched via CMake (`Boost::asio`, `Boost::beast`, `nlohmann_json` and `yaml-cpp`). You can optionally be set to use the system `Boost` installation but fetching with CMake is more convenient since `Boost::asio` and `Boost::beast` are both header-only libraries.

GoogleTest is used for testing.

## CMake Options

CMake options are available to customize the builds.

| Option | Description | Default |
|--------|-------------|---------|
| `MIRU_BUILD_TESTS` | turn off to disable all testing and only build the SDK targets. | On |
| `MIRU_BUILD_EXAMPLES` | turn off to disable all examples and only build the SDK targets. | On |
| `MIRU_FETCH_BOOST` | fetch Boost packages with FetchContent as opposed to using the Boost package located on the build system | On |

## Build from Source

Navigate to the root of the git repository. Configure CMake with

```bash
# don't build the tests
cmake -B build -S . -DMIRU_BUILD_TESTS=Off
```

Then build with
```bash
cmake --build build
```

## Integrate via `FetchContent`

This is the simplest method to begin using the SDK.

```cmake
# this should be somewhere in your cmake file
include(FetchContent)

...

# set the desired miru options (feel free to change these)
set(MIRU_FETCH_BOOST ON)
set(MIRU_BUILD_TESTS OFF)
set(MIRU_BUILD_EXAMPLES ON)

FetchContent_Declare(
    miru
    GIT_REPOSITORY https://github.com/miruml/cpp-sdk.git  # Replace with actual repo URL
    GIT_TAG main # replace with desired tag/branch/commit
    GIT_SUBMODULES "" # disable submodule cloning since the openapi repository inside the cpp-sdk repository is private
)

FetchContent_MakeAvailable(miru)

...

# link the miru sdk to your executable
target_link_libraries(your-app PRIVATE miru::miru)
```

## Integrate via `add_subdirectory`

Alternatively, you can clone the repository locally and add it as a subdirectory

```bash
# replace with desired tag
git clone -b v0.1.0 https://github.com/miruml/cpp-sdk.git
```

```cmake
# set the desired miru options (feel free to change these)
set(MIRU_FETCH_BOOST ON)
set(MIRU_BUILD_TESTS OFF)
set(MIRU_BUILD_EXAMPLES ON)

add_subdirectory("path/to/miru/sdk/repo", "${CMAKE_BINARY_DIR}/miru")

...

# link the miru sdk to your executable
target_link_libraries(your-app PRIVATE miru::miru)
```

## Other Integration Methods

We are working to support more integration methods and package managers. Please reach out to ben@miruml.com for adding support for a particular integration method or package manager.

## Examples

Examples should be executed from the root of the repository so that the file system paths evaluate correctly. After you've built the examples with CMake, execute them with

```bash
./build/examples/<examples-directory-name>/<example-binary-name>
```

For instance, the `from_file` example is executed by invoking

```bash
./build/examples/from_file/from-file
```

from the root of the repository. The binary name is simply the example directory name with the underscores (`_`) replaced with hyphens (`-`)

## Supported Platforms

The Miru C++ SDK has been tested and verified to work on the following operating systems:

- Ubuntu 20.04 LTS
- Ubuntu 22.04 LTS
- Ubuntu 24.04 LTS
- NVIDIA Jetson (JetPack 5.1)
- NVIDIA Jetson (JetPack 6.1)
- Raspberry Pi OS (64-bit)

Other Linux distributions and versions should also work, but have not been explicitly tested.  
If you encounter issues on a different platform, please email ben@miruml.com.
