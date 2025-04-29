# Building from Source

## Organization

| Directory | Description |
|-----------|-------------|
| examples  | simple examples using the miru sdk. |
| cmake     | cmake files for importing and configuring external libraries. |
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

Various CMake options are available to customize the builds.

| Option | Description | Default | Requires |
|--------|-------------|---------|----------|
| `BUILD_TESTING` | turn off to disable all testing and only build the SDK targets. | On | N/A  |

## Build from Source

Navigate to the root of the git repository. Configure CMake with

```bash
# use fetch content to compile with boost instead of the system boost dependency
# don't build the tests
cmake -B build -S . -DMIRU_USE_SYSTEM_BOOST=Off -DBUILD_TESTING=Off 
```

Then build with
```bash
cmake --build build
```

## Installing

To install the built project:

```bash
cmake --build . --target install
```

Note: On Linux/macOS, you might need to use `sudo` for installation to system directories.

## Troubleshooting

- If CMake can't find dependencies, ensure they are installed and their paths are properly set in CMAKE_PREFIX_PATH
- Check that all required development packages are installed on your system

## Additional Notes

- The build artifacts will be located in the `build` directory
- Debug builds will have additional debugging symbols and reduced optimization
- Release builds will have optimizations enabled and debugging symbols stripped