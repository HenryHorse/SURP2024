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

int main() {
//    Graph g(5);
//
//    boost::add_edge(0, 1, g);
//    boost::add_edge(0, 2, g);
//    boost::add_edge(1, 3, g);
//    boost::add_edge(1, 4, g);

    //Graph g(11);
   // boost::add_edge(0, 1, g);
   // boost::add_edge(0, 2, g);
   // boost::add_edge(1, 3, g);
   // boost::add_edge(1, 4, g);
   // boost::add_edge(1, 5, g);
   // boost::add_edge(4, 10, g);
   // boost::add_edge(2, 6, g);
   // boost::add_edge(6, 7, g);
   // boost::add_edge(7, 8, g);
   // boost::add_edge(7, 9, g);



    //std::cout << "Number of independent sets: " << num_ind_sets(g) << std::endl;
    std::vector<int> prufer_sequence = create_prufer_sequence(1000000);
    print_prufer_sequence(prufer_sequence);
    Graph g = prufer_sequence_to_tree(prufer_sequence);

    tree_to_dot(g, "random_tree.dot");

    return 0;
}


