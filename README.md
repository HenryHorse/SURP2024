# Hello World

This is a simple Hello World program in C++, which prints both Hello World! and the C++ version. The desired C++ version is C++20.

The provided compilation method is CMake in the form of the CMakeLists.txt file. To compile using CMake, use the following commands after installing CMake. Minor modifications may be needed for Windows.

```sh
mkdir build
cd build
cmake ..
cmake --build .
./my_program
```

If running via VS Code, ensure that your generated tasks.json file has -std=c++20 as an argument.

If you prefer to compile manually rather than through VS Code, you can compile using the following command, assuming you have g++ installed. 
```sh
g++ -std=c++20 -g -o hello_world hello_world.cpp
```