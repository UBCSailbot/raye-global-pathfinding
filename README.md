# Global Pathfinding

A pathfinding solution that will find the fastest and safest way for a sailboat to get between two locations on earth. When finished, this program will take in the following inputs:

- Wind angle and magnitude (Downloaded from PredictWind)
- Current GPS position and goal coordinates

It will then create a path consisting of a list of (latitude, longitude) pairs that will be sent to the boat.

## Setup

Instructions to setup and build this repository can be found [here](install_deps/README.md). Make sure to rebuild the code every time you make a code change to see the change make a difference.

## Running Program
To run the program to start from Port Renfrew (lat=48, lon=235) to Maui (lat=21, lon=203), simply run
```bash
./build/bin/pathfinder_cli --navigate 48 235 21 203
```
This will generate a path based on current weather conditions and will produce 2 KML files:

- `build/bin/Path.kml`, a visualization of the path generated, based on the GPS coordinates of the path

- `build/bin/Wind.kml`, a visualization of wind data over time (each color represents a 3 hour increment, based on the estimated speed of the boat)

- TODO: Make this generate a file with the date/time in the folder name or in the filename so we don't overwrite every time.

To see these files, do one of the following:
  1. (Recommended) Follow the instructions at [this link](https://linuxconfig.org/how-to-install-google-earth-on-ubuntu-18-04-bionic-beaver-linux), then File->Open and open the files listed above.
  2. Navigate to [Google Earth](https://earth.google.com) and from the menu on the side, select `Projects>New Project>Import KML file from computer`.

An example of what you would see in Google Earth is shown below:

![Sailbot](https://user-images.githubusercontent.com/26510814/120241952-a947f100-c218-11eb-93a1-e3aa7698384d.png)

An example with user-specified wind values can be shown below. Note that the path avoids the high wind region.
![image](https://user-images.githubusercontent.com/26510814/132959406-bc68fba0-12cb-4861-8d08-81ab3e85c42e.png)

The program will also generate a string consisting of the [Longitude, Latitude] of each GPS waypoint. Additionally, there is an unused variable of type vector<pair<double, double>> in `./src/pathfinding/PathfinderResultPrinter.cpp` that contains the GPS data in the same format.

## Additional Details

### Adjusting Cost Function

It is possible to adjust the weight given to wind measurements (with respect to weight given to path length) in the pathfinding cost function by adding the argument `-w <int>` to the pathfinder_cli command. For example:
```bash
./build/bin/pathfinder_cli -w 1000 --navigate 48 235 21 203
```
The default value for the weather factor is 3000.

### Adjusting planet size (resoution)

It is possible to adjust the planet size, which effectively changes the resolution of the generated path. For example:
```bash
./build/bin/pathfinder_cli -p 12 --navigate 48 235 21 203
```
The `-p 12` refers to a planet size of 12. We have decided to consistently use planet size of X, which takes approximately Y to run (TODO).

### Network Table

To use the network table information to get the current latlon position of the boat, add the `--table` parameter to the arguments. Note that the `--navigate <lat> <lon> <lat> <lon>` argument should still be included. Essentially, if the `--table` parameter is provided, then it will try to use the network table to get the starting latlon, but if it fails to connect, it will instead use the starting latlon value to be robust to connection issues. TODO: Finish implementing this behavior.

For example, you can run:

```
./build/bin/pathfinder_cli --navigate 48 235 21 203 --table
```

### Continuous Pathfinding

To create a more realistic pathfinding route, open runpathfinder.sh in the root folder and adjust the values as indicated by the comments. Then run in the root folder:
```bash
bash runpathfinder
```
You should see a list of coordinates, each calculated starting from the nth waypoint of the previous run.
To-do: Add ability to read custom grib files here to simulate changing weather conditions

### Print nth coordinate

TODO: Describe how that works

### Custom wind scenarios

To control how global pathfinding gets its weather data, we use the `-g` parameter. There are 3 possible ways to control the wind input:

1. By not including the `-g` parameter at all, global pathfinding will by default grab the most recent, up-to-date weather data and use that for planning.

2. By including a path to a stored `.grb` file like `-g <my_path>/data.grb`, global pathfinding will read in the weather data from that stored `.grb` file for planning.

3. By including the specific text `-g csv` (not a path, but the exact text here), global pathfinding will read from the following files for planning (in the `input_csvs` directory):

```
angles2d-0.csv  angles2d-1.csv  angles2d-2.csv  angles2d-3.csv  lats2d.csv  lons2d.csv  magnitudes2d-0.csv  magnitudes2d-1.csv  magnitudes2d-2.csv  magnitudes2d-3.csv
```

These files can be directly modified to change the inputs to planning. Note that the last number refers to the time-step value. We will be using 4 time steps for all global pathfinding purposes.

Note that in options 1 and 2, the used weather data is saved into csv files in `output_csvs`, which are can be copied over to the `input_csvs` file to be used for future testing. It will overwrite files in `output_csvs`.

### Linting
Linting helps us keep our code clean, easy to maintain, and free of bugs.

You can run the cpp lint script from our friends at Google with:

```bash
./scripts/run_cpplint.sh
```

### Testing

To test global pathfinding with old weather data:
  1. Make sure the GRIB file is in your /build/ folder and named `data.grb`
  2. Add the option -g to the pathfinder_cli command above, eg.
  ```bash
  ./build/bin/pathfinder_cli -g -p 10 --navigate 48 235 21 203
  ```
  
### Creating & Running Tests
Whenever you add new tests, you will need to add the required `.cpp` and `.h` files to the `TEST_FILES` parameter in `test/basic_tests/CMakelists.txt`.

For example:
```cmake
set(TEST_FILES
    example_tests/NewTest.cpp
    example_tests/NewTest.h)
```

To run the tests, use CMake to compile the project, then run `basic_tests` in `build/bin`.
It will notify you if tests pass or fail.

If your tests fail, it will make the build fail which will make people upset.

### Structure of the code
The directories `src` and `test/basic_tests` should mirror each other. That is, any unit testing code for the file `src/a/b.cpp` should be in `test/basic_tests/a/bTest.cpp`.

-   **src/** - Source code. For more details, check out the README files in each sub-directory.
    -   **core/** - The foundation of the source code. We avoid putting target or device-specific code here if we can, as it makes it easier to compile the fundamentals.
    -   **main/** - All the files that contain binary targets.
-   **test/** - Testing, including both unit tests and manually-run tests.
    -   **lib/** - Libraries that are required for testing.
    -   Refer to documentation on **src/** for a guide to the other sub-directories here.
-   **lib/** - Libraries that are required for the core binaries.

### If you have problems
 - Check that you've initiated the git submodules.
 - Check that all dependencies are met.
 - Re-run `cmake ..`
 - If all else fails, delete your build directory and try again.

### Style Guide
Conforming to a common style guide helps us iterate quickly and avoid creating bugs.

We follow [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
Please familiarize yourself with it.

If you see something that doesn't conform to it, submit a PR.

### Contributing
Please read the [contribution guide](CONTRIBUTING.md).
