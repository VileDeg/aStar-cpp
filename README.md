# aStar-cpp

This program takes as input a csv-like file which represents the grid, grid dimensions, start point coords and end point coords in grid.
```
Usage: ./astar <input_file.csv> <R,C> <X,Y> <X,Y> <use_color>

        File to read the grid from, grid dimensions, start point and end point. 
        And wether to use color print (0 or 1).
        Where R - number of rows(Y), C - number of columns(X).
```
To see usage just run the program with wrong agruments.

File contents must be formatted like that`(no whitespaces are allowed between cells)`:
```
8,7,7,8,9,3,9,9,9,9,
7,8,Z,8,7,3,9,Z,9,9,
7,7,Z,2,4,3,9,Z,9,9,
9,8,Z,9,9,3,8,Z,9,9,
Z,Z,Z,Z,3,Z,Z,Z,Z,Z,
8,5,7,8,3,4,6,Z,9,7,
8,7,5,8,3,5,2,Z,9,6,
8,7,6,9,3,Z,Z,Z,9,8,
8,7,6,9,3,9,9,9,9,8,
9,9,8,9,7,3,6,7,9,9,
```
To convert pdf to csv you can for example use any online converter.

This project uses CMake as build system. To generate the Makefile or solution follow for example these steps:
```
mkdir build
cd build
cmake ..
```
Then to build run:
```
cmake --build .
```
After that the executable should appear in the `build` folder and you can run it like that:
```
./astar ../grid.csv 10,10 6,6 3,2 1
```
If the required CMake version is too high for your system you can try to decrease it by changing `cmake_minimum_required (VERSION ...)` on the first line of `CMakeLists.txt`. But in this case it's not guarranteed that the build will succeed.

Program functionality is very limited, bugs are likely.

Project contains copyrighted material:

MIT license: `src/color.h` - https://gist.github.com/Ph0enixKM/5a0d48c440a6dd664d7ae4c807c53008
