#include "random_trees.h"
#include "graph.h"
#include <thread>

#include <mpi.h>


double lambda = 0.001;

void print_set(std::set<int> set) {
    for (int vertex : set) {
        std::cout << vertex << " ";
    }
}


void try_num_ind_sets() {
    std::random_device rd;
    std::vector<int> prufer_sequence = create_prufer_sequence(68, 18000);
    Graph g = prufer_sequence_to_tree(prufer_sequence);
    std::cout << "Number of independent sets: " << num_ind_sets(g, 0, lambda) << std::endl;

}

void try_prufer_sequence() {
    std::random_device rd;
    std::vector<int> prufer_sequence = create_prufer_sequence(1000, rd());
    print_prufer_sequence(prufer_sequence);
    Graph g = prufer_sequence_to_tree(prufer_sequence);

    tree_to_dot(g, "random_tree.dot");
}

void try_glauber_dynamics(const Graph &g) {
    std::random_device rd;
    std::set<int> independent_set = glauber_dynamics(g, 1000, rd(), lambda);
    for (int vertex : independent_set) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl;
}

void glauber_test1() {
    Graph g(5);
    boost::add_edge(0, 1, g);
    boost::add_edge(0, 2, g);
    boost::add_edge(1, 3, g);
    boost::add_edge(1, 4, g);

    std::set<int> independent_set = glauber_dynamics(g, 1000, 1337, lambda);
    std::set<int> expected_set = {4};

    if (independent_set == expected_set) {
        std::cout << "Glauber Test 1 Passed" << std::endl;
    }
    else {
        std::cout << "Glauber Test 1 Failed" << std::endl;
    }
}

void glauber_test2() {
    Graph g(11);
    boost::add_edge(0, 1, g);
    boost::add_edge(0, 2, g);
    boost::add_edge(1, 3, g);
    boost::add_edge(1, 4, g);
    boost::add_edge(1, 5, g);
    boost::add_edge(4, 10, g);
    boost::add_edge(2, 6, g);
    boost::add_edge(6, 7, g);
    boost::add_edge(7, 8, g);
    boost::add_edge(7, 9, g);

    std::set<int> independent_set = glauber_dynamics(g, 1000, 1337, lambda);
    std::set<int> expected_set = {0, 3, 5};

    if (independent_set == expected_set) {
        std::cout << "Glauber Test 2 Passed" << std::endl;
    }
    else {
        std::cout << "Glauber Test 2 Failed" << std::endl;
    }
}

void glauber_test3() {
    Graph g(11);
    boost::add_edge(0, 1, g);
    boost::add_edge(0, 2, g);
    boost::add_edge(1, 3, g);
    boost::add_edge(1, 4, g);
    boost::add_edge(1, 5, g);
    boost::add_edge(4, 10, g);
    boost::add_edge(2, 6, g);
    boost::add_edge(6, 7, g);
    boost::add_edge(7, 8, g);
    boost::add_edge(7, 9, g);

    std::set<int> independent_set = glauber_dynamics(g, 100000, 007, lambda);
    std::set<int> expected_set = {2, 9, 10};

    if (independent_set == expected_set) {
        std::cout << "Glauber Test 3 Passed" << std::endl;
    }
    else {
        std::cout << "Glauber Test 3 Failed" << std::endl;
    }
}

void glauber_prufer_test() {
    std::vector<int> prufer_sequence = create_prufer_sequence(100000, 1337);
    Graph g = prufer_sequence_to_tree(prufer_sequence);
    tree_to_dot(g, "glauber_prufer.dot");

    std::set<int> independent_set = glauber_dynamics(g, 10000, 1337, lambda);
    print_set(independent_set);
    std::set<int> expected_set = {4};

    if (independent_set == expected_set) {
        std::cout << "Glauber Prufer Test Passed" << std::endl;
    }
    else {
        std::cout << "Glauber Prufer Test Failed" << std::endl;
    }
}


void compare_counting_to_actual() {
    std::random_device rd;
    for (int n = 10; n < 1000; n += 100) {
        std::vector<int> prufer_sequence = create_prufer_sequence(n, rd());
        Graph g = prufer_sequence_to_tree(prufer_sequence);

        std::cout << "Tree of Size: " << n << std::endl;

        double actual_num_sets = num_ind_sets(g, 0, lambda);
        double estimated_num_sets = counting_reduction(g, 10000, 100, lambda);

        std::cout << "Actual Number of Sets: " << actual_num_sets << std::endl;
        std::cout << "Estimated Number of Sets: " << estimated_num_sets << std::endl;
        std::cout << "Difference Between Actual and Estimated: " << actual_num_sets - estimated_num_sets << std::endl;
        std::cout << "Percent Error: " << (abs(actual_num_sets - estimated_num_sets) / actual_num_sets) * 100 << std::endl;
        std::cout << std::endl;

    }

}


void compare_alternate_counting_to_actual() {
    std::random_device rd;
    for (int n = 10; n < 1000; n += 100) {
        std::vector<int> prufer_sequence = create_prufer_sequence(n, rd());
        Graph g = prufer_sequence_to_tree(prufer_sequence);

        std::cout << "Tree of Size: " << n << std::endl;

        double actual_num_sets = num_ind_sets(g, 0, lambda);
        alternate_counting_reduction(g, 10000, 100, lambda);

    }

}


// int main(int argc, char *argv[]) {
//     Graph g1(5);
//     boost::add_edge(0, 1, g1);
//     boost::add_edge(0, 2, g1);
//     boost::add_edge(1, 3, g1);
//     boost::add_edge(1, 4, g1);
//
//     Graph g2(11);
//     boost::add_edge(0, 1, g2);
//     boost::add_edge(0, 2, g2);
//     boost::add_edge(1, 3, g2);
//     boost::add_edge(1, 4, g2);
//     boost::add_edge(1, 5, g2);
//     boost::add_edge(4, 10, g2);
//     boost::add_edge(2, 6, g2);
//     boost::add_edge(6, 7, g2);
//     boost::add_edge(7, 8, g2);
//     boost::add_edge(7, 9, g2);
//
//
// //    tree_to_dot(g1, "g1.dot");
// //    tree_to_dot(g2, "g2.dot");
//
// //    try_num_ind_sets();
// //    try_prufer_sequence();
// //    try_glauber_dynamics(g2);
//
// //    glauber_test1();
// //    glauber_test2();
// //    glauber_test3();
// //    glauber_prufer_test();
//
// //    std::cout << num_ind_sets(g2) << std::endl;
// //    counting_reduction(g2);
//
//
//     compare_counting_to_actual();
// //    compare_alternate_counting_to_actual();
//
//     return 0;
// }




int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Graph g;
    double alpha = 1;
    int T = 10000;
    double K = 100;
    double lambda = 1;
    std::random_device rd;



    std::vector<int> prufer_sequence = create_prufer_sequence(1000, 123421941824);
    g = prufer_sequence_to_tree(prufer_sequence);
    auto edges = boost::edges(g);
    std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
    int m = boost::num_edges(g);

    if (rank == 0) {
        std::cout << num_ind_sets(g, 0, lambda) << std::endl;
    }

    for (int i = 0; i < m; i++) {
        auto edge = edges_list[i];
        int removed_edge_source = boost::source(edge, g);
        int removed_edge_target = boost::target(edge, g);


        boost::remove_edge(edge, g);
        // If this is the last edge (the Mth edge), alpha is 2^n
        if (i == edges_list.size() - 1 && rank == 0) {
            alpha *= pow(2, num_vertices(g));
        }
        else {
            int samples_per_process = K / num_processes;
            int remaining_samples = int(K) % num_processes;
            double local_invalid_sets = 0;

            for (int j = 0; j < samples_per_process; j++) {
                std::set<int> sample = glauber_dynamics(g, T, rd(), lambda);
                if (sample.find(removed_edge_source) != sample.end() && sample.find(removed_edge_target) != sample.end()) {
                    local_invalid_sets++;
                }
            }

            if (rank == 0 && remaining_samples > 0) {
                for (int j = 0; j < remaining_samples; j++) {
                    std::set<int> sample = glauber_dynamics(g, T, rd(), lambda);
                    if (sample.find(removed_edge_source) != sample.end() && sample.find(removed_edge_target) != sample.end()) {
                        local_invalid_sets++;
                    }
                }
            }

            double total_invalid_sets = 0;
            MPI_Reduce(&local_invalid_sets, &total_invalid_sets, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
            if (rank == 0) {
                // The ratio that I multiply alpha by is the number of sets I sampled from Gi+1 that were also independent sets in Gi over the total number of sets I sampled.
                alpha *= (K - total_invalid_sets) / K;
            }
        }
    }

    MPI_Finalize();
    if (rank == 0) {
        std::cout << alpha << std::endl;
    }
    return 0;
}


// int main() {
//     std::random_device rd;
//     std::vector<int> prufer_sequence = create_prufer_sequence(10, rd());
//     Graph g = prufer_sequence_to_tree(prufer_sequence);
//     Graph g1;
//
//     std::size_t numNodes = boost::num_vertices(g);
//     std::size_t numEdges = boost::num_edges(g);
//
//     std::cout << "Number of nodes: " << numNodes << std::endl;
//     std::cout << "Number of edges: " << numEdges << std::endl;
//
//     tree_to_dot(g, "test.dot");
//     dot_to_tree("test.dot", g1);
//
//
//     numNodes = boost::num_vertices(g);
//     numEdges = boost::num_edges(g);
//
//     std::cout << "Number of nodes: " << numNodes << std::endl;
//     std::cout << "Number of edges: " << numEdges << std::endl;
//
// }