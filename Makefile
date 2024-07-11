CXX = g++

CXXFLAGS = -std=c++20 -g

all: hello_world

hello_world: hello_world.cpp
	$(CXX) $(CXXFLAGS) hello_world.cpp -o hello_world

clean:
	rm -f hello_world