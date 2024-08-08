#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <vector>
#include <iostream>
#include <random>
#include "random_trees.h"
#include <cmath>
#include <boost/graph/copy.hpp>
#include <future>
#include <thread>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

class post_order_visitor : public boost::default_dfs_visitor {
public:
    post_order_visitor(std::vector<int>& postorder)
        : postorder(postorder), index(0) {}

    template <typename Vertex, typename Graph>
    void finish_vertex(Vertex u, const Graph& g) {
        postorder[u] = index++;
    }

private:
    std::vector<int>& postorder;
    int index;
};

std::vector<int> get_children(int u, const std::vector<int>& postorder, const Graph& g) {
    std::vector<int> children;
    for (auto ei = boost::out_edges(u, g).first; ei != boost::out_edges(u, g).second; ei++) {
        int v = boost::target(*ei, g);
        if (postorder[u] > postorder[v]) {
            children.push_back(v);
        }
    }
    return children;
}

std::vector<int> get_grandchildren(const std::vector<int>& children, const std::vector<int>& postorder, const Graph& g) {
    std::vector<int> grandchildren;
    for (int child : children) {
        auto child_children = get_children(child, postorder, g);
        grandchildren.insert(grandchildren.end(), child_children.begin(), child_children.end());
    }
    return grandchildren;
}


int num_ind_sets(const Graph& g) {
     int n = num_vertices(g);

    std::vector<int> postorder(n);

    post_order_visitor vis(postorder);
    boost::depth_first_search(g, visitor(vis));

    // A list that allows for converting back to the vertex index from its post order number
    std::vector<int> postorder_to_vertex(n);
    for (int i = 0; i < n; i++) {
        postorder_to_vertex[postorder[i]] = i;
    }

    std::unordered_map<int, double> num_ind_sets;

    for (int i = 0; i < n; ++i) {
        int vertex = postorder_to_vertex[i];
        auto children = get_children(vertex, postorder, g);
        auto grandchildren = get_grandchildren(children, postorder, g);

        if (children.empty() && grandchildren.empty()) {
            num_ind_sets[vertex] = 2;
        }
        else if (grandchildren.empty()) {
            double product = 1;
            for (int child : children) {
                product *= num_ind_sets[child];
            }
            num_ind_sets[vertex] = product + 1;
        }
        else {
            double product_children = 1;
            for (int child : children) {
                product_children *= num_ind_sets[child];
            }

            double product_grandchildren = 1;
            for (int grandchild : grandchildren) {
                product_grandchildren *= num_ind_sets[grandchild];
            }
            num_ind_sets[vertex] = product_children + product_grandchildren;
        }
    }

    return num_ind_sets[postorder_to_vertex[n-1]];
}

bool is_independent_set(const std::set<int>& independent_set, int vertex, const Graph& g) {
    for (auto neighbor : boost::make_iterator_range(boost::adjacent_vertices(vertex, g))) {
        if (independent_set.contains(neighbor)) {
            return false;
        }
    }
    return true;
}

std::set<int> glauber_dynamics(const Graph& g, int T, unsigned int seed) {
    std::set<int> independent_set;
    std::mt19937 rng(seed);
    std::uniform_int_distribution<> vertex_picker(0, boost::num_vertices(g) - 1);
    std::uniform_real_distribution<> percent_chance(0.0, 1.0);


    for (int t = 0; t < T; t++) {
        int vertex = vertex_picker(rng);

        if (independent_set.contains(vertex)) {
            if (percent_chance(rng) < 0.5) {
                independent_set.erase(vertex);
            }
        }
        else if (is_independent_set(independent_set, vertex, g)) {
            if (percent_chance(rng) < 0.5) {
                independent_set.insert(vertex);
            }
        }
    }

    return independent_set;
}


double counting_reduction(const Graph& g0, int T, double K) {
    Graph gi;
    boost::copy_graph(g0, gi);

    auto edges = boost::edges(gi);
    std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
    int m = boost::num_edges(gi);

    double alpha = 1;

    std::random_device rd;

    for (int i = 0; i < m; i++) {
        auto edge = edges_list[i];
        int removed_edge_source = boost::source(edge, gi);
        int removed_edge_target = boost::target(edge, gi);



        boost::remove_edge(edge, gi);
        // If this is the last edge (the Mth edge), alpha is 2^n
        if (i == edges_list.size() - 1) {
            alpha *= pow(2, num_vertices(gi));
        }
        else {
            double num_invalid_sets = 0;

            // This is a vector of "futures", which are values (in this case sets of integers) that will be determined at a later time
            // futures.get() as used later will wait until the value is determined before continuing onwards
            // This allows me to asynchronously launch a bunch of samples at one time instead of just one
            std::vector<std::future<std::set<int>>> futures;
            int batch_size = std::thread::hardware_concurrency();

            // K is the total number of samples
            for (int j = 0; j < K; j+=batch_size) {
                for (int k = 0; k < batch_size && j + k < K; k++) {
                    unsigned int seed = rd();
                    futures.push_back(std::async(std::launch::async, glauber_dynamics, std::ref(gi), T, seed));
                }
                for (auto& future : futures) {
                    std::set<int> sample = future.get();
                    if (sample.contains(removed_edge_source) && sample.contains(removed_edge_target)) {
                        num_invalid_sets++;
                    }
                }
                futures.clear();
            }
            // The ratio that I multiply alpha by is the number of sets I sampled from Gi+1 that were also independent sets in Gi over the total number of sets I sampled.
            alpha *= (K - num_invalid_sets) / K;
        }
    }

    return alpha;

}




