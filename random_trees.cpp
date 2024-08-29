#include <vector>
#include <iostream>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

std::vector<int> create_prufer_sequence(int num_vertices, unsigned int seed) {
    std::vector<int> prufer_sequence(num_vertices - 2);
    std::mt19937 rng(seed); // Mersenne Twister RNG, C++ way of generating random numbers
    std::uniform_int_distribution<> dist(0, num_vertices - 1);

    for (int i = 0; i < num_vertices - 2; i++) {
        prufer_sequence[i] = dist(rng);
    }
    return prufer_sequence;
}

void print_prufer_sequence(const std::vector<int>& prufer_sequence) {
    std::cout << "Prufer Sequence: ";
    for (int i = 0; i < prufer_sequence.size(); i++) {
        std::cout << prufer_sequence[i];
        if (i != prufer_sequence.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}


Graph prufer_sequence_to_tree(const std::vector<int>& prufer_sequence) {
    int num_vertices = prufer_sequence.size() + 2;
    Graph g(num_vertices);

    std::vector<int> degree(num_vertices, 1);
    for (int i : prufer_sequence) {
        degree[i]++;
    }

    for (int i : prufer_sequence) {
        for (auto node : boost::make_iterator_range(boost::vertices(g))){
            if (degree[node] == 1) {
                boost::add_edge(i, node, g);
                degree[i]--;
                degree[node]--;
                break;
            }
        }
    }

    int u = 0;
    int v = 0;
    for (auto node : boost::make_iterator_range(boost::vertices(g))) {
        if (degree[node] == 1) {
            if (u == 0) {
                u = node;
            }
            else {
                v = node;
                break;
            }
        }
    }
    boost::add_edge(u, v, g);
    degree[u]--;
    degree[v]--;
    return g;
}

void tree_to_dot(const Graph& g, const std::string& filename) {
    std::ofstream file(filename);
    boost::write_graphviz(file, g);
    file.close();
}


