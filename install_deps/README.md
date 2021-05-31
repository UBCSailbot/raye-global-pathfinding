# Setup Instructions

## Option 1 (Recommended)
For UNIX, a handy install script is provided at the top level.
Simply run `./configure`, and dependencies will be installed and the project will be built.

This works for both macOS and Debian users.

## Option 2

### Step 1: Installing depenendencies

Before you can build the project, you first need to install the dependencies.

#### Debian Linux (Ubuntu)
```bash
./install_deps/install_deps_linux.sh
```

#### macOS

```bash
./install_deps/install_deps_osx.sh
```

Depending on your env you may run into permission issues. Here's how to fix them:
```bash
sudo chown -R `whoami` /usr/local
```

#### Windows
Not supported at this time. Feel free to create a PR with instructions on how to get it working.
Go to the CMake Website: http://www.cmake.org/download/

### Step 2: Building

1. Run `mkdir build` in the repo root directory, which will create a folder called `build`.
2. Navigate into the folder using `cd build`.
3. Then run `cmake ..`, which will tell `CMake` to generate build files for the project into your `build` directory.
4. Then run `make -j4`, which will build the code.
