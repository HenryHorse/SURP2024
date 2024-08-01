#include "random_trees.h"
#include "graph.h"



void print_set(std::set<int> set) {
    for (int vertex : set) {
        std::cout << vertex << " ";
    }
}


void try_num_ind_sets() {
    std::random_device rd;
    std::vector<int> prufer_sequence = create_prufer_sequence(68, 18000);
    Graph g = prufer_sequence_to_tree(prufer_sequence);
    std::cout << "Number of independent sets: " << num_ind_sets(g) << std::endl;

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
    std::set<int> independent_set = glauber_dynamics(g, 1000, rd());
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

    std::set<int> independent_set = glauber_dynamics(g, 1000, 1337);
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

    std::set<int> independent_set = glauber_dynamics(g, 1000, 1337);
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

    std::set<int> independent_set = glauber_dynamics(g, 100000, 007);
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

    std::set<int> independent_set = glauber_dynamics(g, 10000, 1337);
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
    for (int n = 10; n < 1000; n += 10) {
        std::vector<int> prufer_sequence = create_prufer_sequence(n, rd());
        Graph g = prufer_sequence_to_tree(prufer_sequence);

        std::cout << "Tree of Size: " << n << std::endl;

        double actual_num_sets = num_ind_sets(g);
        double estimated_num_sets = counting_reduction(g, 1000, 1000);

        std::cout << "Actual Number of Sets: " << actual_num_sets << std::endl;
        std::cout << "Estimated Number of Sets: " << estimated_num_sets << std::endl;
        std::cout << "Difference Between Actual and Estimated: " << actual_num_sets - estimated_num_sets << std::endl;

    }

}

int main() {
    Graph g1(5);
    boost::add_edge(0, 1, g1);
    boost::add_edge(0, 2, g1);
    boost::add_edge(1, 3, g1);
    boost::add_edge(1, 4, g1);

    Graph g2(11);
    boost::add_edge(0, 1, g2);
    boost::add_edge(0, 2, g2);
    boost::add_edge(1, 3, g2);
    boost::add_edge(1, 4, g2);
    boost::add_edge(1, 5, g2);
    boost::add_edge(4, 10, g2);
    boost::add_edge(2, 6, g2);
    boost::add_edge(6, 7, g2);
    boost::add_edge(7, 8, g2);
    boost::add_edge(7, 9, g2);


//    tree_to_dot(g1, "g1.dot");
//    tree_to_dot(g2, "g2.dot");

//    try_num_ind_sets();
//    try_prufer_sequence();
//    try_glauber_dynamics(g2);

//    glauber_test1();
//    glauber_test2();
//    glauber_test3();
//    glauber_prufer_test();

//    std::cout << num_ind_sets(g2) << std::endl;
//    counting_reduction(g2);
    compare_counting_to_actual();

    return 0;
}
