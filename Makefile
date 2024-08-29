CXX = mpic++
CXXFLAGS = -std=c++2a -Wall
BOOST_LIBS = -lboost_graph

all: test

test: test.o graph.o random_trees.o
	$(CXX) $(CXXFLAGS) -o test test.o graph.o random_trees.o $(BOOST_LIBS)
# Object file compilation
test.o: test.cpp graph.h random_trees.h
	$(CXX) $(CXXFLAGS) -c test.cpp
graph.o: graph.cpp graph.h
	$(CXX) $(CXXFLAGS) -c graph.cpp
random_trees.o: random_trees.cpp random_trees.h
	$(CXX) $(CXXFLAGS) -c random_trees.cpp

clean:
	rm -f *.o test