#include "random_trees.h"
#include "dynamic_programming.h"
#include "glauber_dynamics.h"
#include <thread>

#include <mpi.h>




int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int num_processes, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Graph g;
    double alpha = 1;
    int T = 100;
    double K = 200;
    double lambda = 0.001;
    std::random_device rd;

    int total_epochs = 5;
    int epochs_per_process = total_epochs / num_processes;
    int extra_epochs = total_epochs % num_processes;
    if (rank < extra_epochs) {
        epochs_per_process++;
    }

    double alpha_sum_local = 0.0;


    std::vector<int> prufer_sequence = create_prufer_sequence(50, 378912830);
    g = prufer_sequence_to_tree(prufer_sequence);
    auto edges = boost::edges(g);
    std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
    int m = boost::num_edges(g);

    if (rank == 0) {
        std::cout << num_ind_sets(g, 0, lambda) << std::endl;
    }

    for (int epoch = 0; epoch < epochs_per_process; epoch++) {
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

        if (rank == 0) {
            alpha_sum_local += alpha;
            std::cout << "Process " << rank << " - Alpha for epoch " << epoch + 1 << ": " << alpha << std::endl;
        }
    }

    double global_alpha_sum = 0;
    MPI_Reduce(&alpha_sum_local, &global_alpha_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);



    MPI_Finalize();
    if (rank == 0) {
        double alpha_avg = global_alpha_sum / total_epochs;
        std::cout << "Average alpha over " << total_epochs << " epochs: " << alpha_avg << std::endl;
    }
    return 0;
}
