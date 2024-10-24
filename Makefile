CXX = mpic++
CXXFLAGS = -std=c++2a -Wall
BOOST_LIBS = -lboost_graph

all: test

test: test.o dynamic_programming.o glauber_dynamics.o random_trees.o
	$(CXX) $(CXXFLAGS) -o test test.o dynamic_programming.o glauber_dynamics.o random_trees.o $(BOOST_LIBS)
# Object file compilation
test.o: test.cpp dynamic_programming.h glauber_dynamics.h random_trees.h
	$(CXX) $(CXXFLAGS) -c test.cpp
dynamic_programming.o: dynamic_programming.cpp dynamic_programming.h
	$(CXX) $(CXXFLAGS) -c dynamic_programming.cpp
glauber_dynamics.o: glauber_dynamics.cpp glauber_dynamics.h
	$(CXX) $(CXXFLAGS) -c glauber_dynamics.cpp
random_trees.o: random_trees.cpp random_trees.h
	$(CXX) $(CXXFLAGS) -c random_trees.cpp


clean:
	rm -f *.o test
