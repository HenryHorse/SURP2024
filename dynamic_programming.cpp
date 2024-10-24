#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <vector>
#include <iostream>
#include "random_trees.h"

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


double num_ind_sets(const Graph& g, int root, double lambda) {
    int n = num_vertices(g);

    std::vector<int> postorder(n);

    post_order_visitor vis(postorder);
    boost::depth_first_search(g, visitor(vis).root_vertex(root));

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
            num_ind_sets[vertex] = product_children + lambda * product_grandchildren;
        }
    }

    return num_ind_sets[postorder_to_vertex[n-1]];
}