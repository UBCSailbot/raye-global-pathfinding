# Sample project base

Fork this repo for all new projects.

## Setup

### Initial Setup
Before you can build the project, you first need to install the dependencies.


#### Option 1 (Recommended)
For UNIX, a handy install script is provided at the top level.
Simply run `./configure`, and dependencies will be installed and the project will be built.

This works for both macOS and Debian users.

#### macOS

1.  [Buy a Mac](http://store.apple.com)
2.  `./install_deps/install_deps_osx.sh`

Depending on your env you may run into permission issues. Here's how to fix them:
```bash
sudo chown -R `whoami` /usr/local
```

#### Debian Linux (Ubuntu)
```bash
./install_deps/install_deps_linux.sh
```

#### Windows
Not supported at this time. Feel free to create a PR with instructions on how to get it working.
Go to the CMake Website: http://www.cmake.org/download/

## Building

If you do not want to use the configure script:
1. Question yourself.
2. Run `mkdir build` in the repo root directory, which will create a folder called `build`.
3. Navigate into the folder using `cd build`.
4. Then run `cmake ..`, which will tell `CMake` to build a project out of the repo directory into your `build` directory.

## Style Guide
Conforming to a common style guide helps us iterate quickly and avoid creating bugs.

We follow [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
Please familiarize yourself with it.

If you see something that doesn't conform to it, submit a PR.

## Linting
Linting helps us keep our code clean, easy to maintain, and free of bugs.

You can run the cpp lint script from our friends at Google with:

```bash
./scripts/run_cpplint.sh
```

### Testing

##### Creating & Running Tests
Whenever you add new tests, you will need to add the required `.cpp` and `.h` files to the `TEST_FILES` parameter in `test/basic_tests/CMakelists.txt`.

For example:
```cmake
set(TEST_FILES
    example_tests/NewTest.cpp
    example_tests/NewTest.h)
```

To run the tests, use CMake to compile the project, then run `basic_tests` in `build`.
It will notify you if tests pass or fail.

If your tests fail, it will make the build fail which will make people upset.

## Structure of the code
The directories `src` and `test/basic_tests` should mirror each other. That is, any unit testing code for the file `src/a/b.cpp` should be in `test/basic_tests/a/bTest.cpp`.

-   **src/** - Source code. For more details, check out the README files in each sub-directory.
    -   **core/** - The foundation of the source code. We avoid putting target or device-specific code here if we can, as it makes it easier to compile the fundamentals.
    -   **main/** - All the files that contain binary targets.
-   **test/** - Testing, including both unit tests and manually-run tests.
    -   **lib/** - Libraries that are required for testing.
    -   Refer to documentation on **src/** for a guide to the other sub-directories here.
-   **lib/** - Libraries that are required for the core binaries.

## If you have problems
 - Check that you've initiated the git submodules.
 - Check that all dependencies are met.
 - Re-run `cmake ..`
 - If all else fails, delete your build directory and try again.

## Contributing
Please read the [contribution guide](CONTRIBUTING.md).
