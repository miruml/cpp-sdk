# Miru C++ SDK
This repository contains the [miru](https://www.miruml.com/) C++ SDK for retrieving configs from the miru agent on your device.

## Organization

| Directory | Description |
|-----------|-------------|
| cmake     | cmake files for configuring and building with cmake |
| examples  | simple examples using the miru sdk. |
| miru      | miru sdk implementation. |
| scripts   | scripts used in the development and release process. |
| tests     | tests for the miru sdk |

## Build Requirements

1. C++17 and above
2. CMake 3.15 or higher
3. `Boost` version 1.83 or higher

Additional dependencies are fetched via CMake (`nlohmann_json` and `yaml-cpp`).

GoogleTest is used for testing.

## CMake Options

CMake options are available to customize the builds.

| Option | Description | Default |
|--------|-------------|---------|
| `MIRU_BUILD_TESTS` | turn off to disable all testing and only build the SDK targets. | On |
| `MIRU_BUILD_EXAMPLES` | turn off to disable all examples and only build the SDK targets. | On |

## Build from Source

Navigate to the root of the git repository. Configure CMake with

```bash
# use fetch content to compile with boost instead of the system boost dependency
# don't build the tests
cmake -B build -S .
```

Then build with
```bash
cmake --build build
```

## Install

To install the built project (from the repository root):

```bash
cmake --build build --target install
```

> [!NOTE]
> Note: You might need to use `sudo` for installation to system directories.

## Include via `add_subdirectory`

```cmake
# Set SDK build options, for example:
set(MIRU_BUILD_SHARED_LIBS On)

add_subdirectory(path-to-miru-sdk-repo)
target_link_libraries(your-app PRIVATE miru::miru)
```

## Include via `find_package`

First install the SDK on the build system via `cmake --install`

```cmake
find_package(miru REQUIRED)
target_link_libraries(your-app PRIVATE miru::miru)
```
