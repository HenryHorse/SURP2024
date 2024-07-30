#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <vector>
#include <iostream>
#include <random>
#include "random_trees.h"

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

    std::vector<int> postorder_to_vertex(n);
    for (int i = 0; i < n; ++i) {
//        std::cout << i; // Vertex number
//        std::cout << postorder[i] << std::endl; // Postorder index
        postorder_to_vertex[postorder[i]] = i;
    }

    std::unordered_map<int, int> num_ind_sets;

    for (int i = 0; i < n; ++i) {
        int vertex = postorder_to_vertex[i];
        auto children = get_children(vertex, postorder, g);
        auto grandchildren = get_grandchildren(children, postorder, g);
//        std::cout << "Vertex " << vertex << ":\n";
//        std::cout << "  Children: ";
//        for (int child : children) {
//            std::cout << child << " ";
//        }
//        std::cout << "\n  Grandchildren: ";
//        for (int grandchild : grandchildren) {
//            std::cout << grandchild << " ";
//        }
//        std::cout << std::endl;

        if (children.empty() && grandchildren.empty()) {
            num_ind_sets[vertex] = 2;
        }
        else if (grandchildren.empty()) {
            int product = 1;
            for (int child : children) {
                product *= num_ind_sets[child];
            }
            num_ind_sets[vertex] = product + 1;
        }
        else {
            int product_children = 1;
            for (int child : children) {
                product_children *= num_ind_sets[child];
            }

            int product_grandchildren = 1;
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
        if (independent_set.find(neighbor) != independent_set.end()) {
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

        if (independent_set.find(vertex) != independent_set.end()) {
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





