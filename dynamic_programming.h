#ifndef SURP2024_DYNAMIC_PROGRAMMING_H
#define SURP2024_DYNAMIC_PROGRAMMING_H



typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;

std::vector<int> get_children(int u, const std::vector<int>& postorder, const Graph& g);
std::vector<int> get_grandchildren(const std::vector<int>& children, const std::vector<int>& postorder, const Graph& g);
double num_ind_sets(const Graph& g, int root, double lambda);


#endif //SURP2024_DYNAMIC_PROGRAMMING_H
