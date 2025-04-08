#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <iostream>
#include <random>
#include "random_trees.h"
#include "dynamic_programming.h"
#include <cmath>
#include <boost/graph/copy.hpp>
#include <future>
#include <thread>



bool is_independent_set(const std::set<int>& independent_set, int vertex, const Graph& g) {
    for (auto neighbor : boost::make_iterator_range(boost::adjacent_vertices(vertex, g))) {
        if (independent_set.find(neighbor) != independent_set.end()) {
            return false;
        }
    }
    return true;
}

std::set<int> glauber_dynamics(const Graph& g, int T, unsigned int seed, double lambda) {
    std::set<int> independent_set;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<> vertex_picker(0, boost::num_vertices(g) - 1);
    std::uniform_real_distribution<> percent_chance(0.0, 1.0);


    for (int t = 0; t < T; t++) {
        int vertex = vertex_picker(rng);

        if (independent_set.find(vertex) != independent_set.end()) {
            if (percent_chance(rng) <  1 / (1 + lambda)) {
                independent_set.erase(vertex);
            }
        }
        else if (is_independent_set(independent_set, vertex, g)) {
            if (percent_chance(rng) < lambda / (1 + lambda)) {
                independent_set.insert(vertex);
            }
        }
    }

    return independent_set;
}



