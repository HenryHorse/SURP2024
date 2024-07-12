# Hello World

This is a simple Hello World program in C++, which prints both Hello World! and the C++ version (which should always be C++20). If running via VS Code, ensure that the tasks.json file has -std=c++20 as an argument. 

If using VS code, 
Windows: Use the VS Code run button to run
Mac: Use the VS Code run button to compile and then run via command line using the following line:

```sh
./hello_world
```

Ensure g++ is installed. If on Windows, install using MinGW. 

If you prefer to compile manually rather than through VS Code, you can compile using
```sh
g++ -std=c++20 -g -o hello_world hello_world.cpp
```