//
// Created by Shayan Daijavad on 7/29/24.
//

#ifndef SURP2024_GRAPH_H
#define SURP2024_GRAPH_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <vector>
#include <iostream>
#include <random>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

std::set<int> glauber_dynamics(const Graph& g, int T, unsigned int seed);
std::vector<int> get_children(int u, const std::vector<int>& postorder, const Graph& g);
std::vector<int> get_grandchildren(const std::vector<int>& children, const std::vector<int>& postorder, const Graph& g);
int num_ind_sets(const Graph& g);
bool is_independent_set(const std::set<int>& independent_set, int vertex, const Graph& g);
std::set<int> glauber_dynamics(const Graph& g, int T, unsigned int seed);
double counting_reduction(const Graph& g0, int T, double K);
double alternate_counting_reduction(const Graph& g0, int T, double K);

#endif //SURP2024_GRAPH_H
