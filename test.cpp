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
    double K = 17;
    double lambda = 1;
    int n = 3;
    std::random_device rd;

    int epochs = 5;
    int color = rank % epochs;
    MPI_Comm sub_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &sub_comm);

    int sub_rank, sub_size;
    MPI_Comm_rank(sub_comm, &sub_rank);
    MPI_Comm_size(sub_comm, &sub_size);

    std::cout << "World Rank: " << rank << " Sub Rank: " << sub_rank << " in a communicator of size " << sub_size
              << std::endl;


    // For a concrete example of how my distribution works, let's say I have 5 epochs, 8 processes, and 17 samples.
    // The distribution will first allocate as many processes as it can for each epoch using MPI_Comm_split. In our example, 3 epochs get 2 processes each, and 2 epochs get 1 process each.
    // Then, for each epoch, we want to do 17 samples. For each epoch with 2 processes each, one process will do 9 samples, and the other will do 8. For each epoch with 1 process each, each process will do 17 samples.
    // This way, we can isolate epochs into individual communication groups, and come up with an alpha estimate for each epoch. Then, we can sum up those estimates and average them out.

    // The completion time is bottlenecked by the process with the highest number of samples, so ideally have the number of processes be evenly divisible by the number of epochs.

    // This is the number of processes allocated to a given epoch
    int num_processes_for_epoch;
    MPI_Comm_size(sub_comm, &num_processes_for_epoch);

    // Given the number of processes for this epoch, this calculates the number of samples for each process.
    int samples = K;
    int samples_per_process = samples / num_processes_for_epoch;
    int extra_samples = samples % num_processes_for_epoch;
    if (sub_rank < extra_samples) {
        samples_per_process++;
    }

    std::cout << "Samples per process:  " << samples_per_process << std::endl;


    // We need to choose a specific seed so that all cores (which don't share memory) have access to the same graph.
    std::vector<int> prufer_sequence = create_prufer_sequence(n, 378912830);
    g = prufer_sequence_to_tree(prufer_sequence);
    auto edges = boost::edges(g);
    std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
    int m = boost::num_edges(g);

    double averaged_alpha = 0;

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

            double local_invalid_sets = 0;
            for (int s = 0; s < samples_per_process; s++) {
                std::set<int> sample = glauber_dynamics(g, T, rd(), lambda);
                if (sample.find(removed_edge_source) != sample.end() && sample.find(removed_edge_target) != sample.end()) {
                    local_invalid_sets++;
                }
            }

            double total_invalid_sets = 0;
            MPI_Reduce(&local_invalid_sets, &total_invalid_sets, 1, MPI_DOUBLE, MPI_SUM, 0, sub_comm);

            if (sub_rank == 0) {
//                std::cout << "Total Invalid Sets: " <<  total_invalid_sets << std::endl;
                // The ratio that I multiply alpha by is the number of sets I sampled from Gi+1 that were also independent sets in Gi over the total number of sets I sampled.
                alpha *= (K - total_invalid_sets) / K;
            }
            else {
                alpha = 0;
            }
        }
    }
//    std::cout << "Alpha: " << alpha << std::endl;
    MPI_Reduce(&alpha, &averaged_alpha, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Averaged Alpha: " << averaged_alpha / epochs << std::endl;
    }

//    for (int i = 0; i < epochs; i++) {
//
//        double local = 0;
//        for (int j = 0; j < samples_per_process; j++) {
//            if (rank == 0 || rank == 1) {
//                local += 1;
//                MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 0, sub_comm);
//            }
//            if (rank == 2 || rank == 3) {
//                local += 2;
//                MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 2, sub_comm);
//            }
//            if (rank == 4) {
//                local += 3;
//                MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 4, sub_comm);
//            }
//            std::cout << local << std::endl;
//
//        }
//        if (rank > 2 || rank == 0) {
//            double local = 0;
//            for (int j = 0; j < samples; j++) {
//                local += 1;
//            }
//
//            MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 0, sub_comm);
//        }
//    }


//    std::vector<int> prufer_sequence = create_prufer_sequence(n, 378912830);
//    g = prufer_sequence_to_tree(prufer_sequence);
//    auto edges = boost::edges(g);
//    std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
//    int m = boost::num_edges(g);
//
//    if (rank == 0) {
//        std::cout << num_ind_sets(g, 0, lambda) << std::endl;
//    }
//
//    for (int epoch = 0; epoch < epochs_per_process; epoch++) {
//        double alpha = 1;
//
//        int cur_process = rank;
//
//        if (rank == cur_process) {
//            std::vector<int> prufer_sequence = create_prufer_sequence(n, 378912830);
//            g = prufer_sequence_to_tree(prufer_sequence);
//            auto edges = boost::edges(g);
//            std::vector<Graph::edge_descriptor> edges_list(edges.first, edges.second);
//            int m = boost::num_edges(g);
//
//            for (int i = 0; i < m; i++) {
//                auto edge = edges_list[i];
//                int removed_edge_source = boost::source(edge, g);
//                int removed_edge_target = boost::target(edge, g);
//
//
//                boost::remove_edge(edge, g);
//                // If this is the last edge (the Mth edge), alpha is 2^n
//                if (i == edges_list.size() - 1 && rank == 0) {
//                    alpha *= pow(2, num_vertices(g));
//                }
//                else {
//                    int samples_per_process = K / num_processes;
//                    int remaining_samples = int(K) % num_processes;
//                    double local_invalid_sets = 0;
//
//
//                    for (int j = 0; j < samples_per_process; j++) {
//                        std::set<int> sample = glauber_dynamics(g, T, rd(), lambda);
//                        if (sample.find(removed_edge_source) != sample.end() && sample.find(removed_edge_target) != sample.end()) {
//                            local_invalid_sets++;
//                        }
//                    }
//
//                    if (rank == 0 && remaining_samples > 0) {
//                        for (int j = 0; j < remaining_samples; j++) {
//                            std::set<int> sample = glauber_dynamics(g, T, rd(), lambda);
//                            if (sample.find(removed_edge_source) != sample.end() && sample.find(removed_edge_target) != sample.end()) {
//                                local_invalid_sets++;
//                            }
//                        }
//                    }
//
//                    double total_invalid_sets = 0;
//                    std::cout << "Process: " << rank << " Num Sets: " << local_invalid_sets << std::endl;
//                    MPI_Reduce(&local_invalid_sets, &total_invalid_sets, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
//                    if (rank == 0) {
//                        // The ratio that I multiply alpha by is the number of sets I sampled from Gi+1 that were also independent sets in Gi over the total number of sets I sampled.
//                        alpha *= (K - total_invalid_sets) / K;
//                    }
//                }
//            }
//        }
//
//        if (rank == 0) {
//            std::cout << "Alpha for epoch " << epoch + 1 << ": " << alpha << std::endl;
//        }
//        alpha_sum_local += alpha;
//    }
//
//    double global_alpha_sum = 0;
//    MPI_Reduce(&alpha_sum_local, &global_alpha_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
//
//
//    MPI_Finalize();
//    if (rank == 0) {
//        double alpha_avg = global_alpha_sum / total_epochs;
//        std::cout << "Average alpha over " << total_epochs << " epochs: " << alpha_avg << std::endl;
//    }
    MPI_Comm_free(&sub_comm);
    MPI_Finalize();
    return 0;
}
