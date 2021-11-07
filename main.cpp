#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/array.hpp>
#include <array>
#include <utility>
#include <algorithm>
#include <iostream>
#include "girvan_newman.h"
#include <fstream>

int main() {
//    std::ifstream input("../input.grapml");
//    algos::parse(input);
    //algos::hi();

    enum { topLeft, topRight, bottomRight, bottomLeft };
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<>::vertex_iterator v_iterator;
    std::ifstream input("../input.grapml");
    graph g = algos::parse(input);
    std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
    v_iterator start = vs.first;
    start = start + 3;
    boost::adjacency_list<>::vertex_descriptor d = *start;



    boost::array<boost::adjacency_list<>::vertex_descriptor, 39> predecessors;

    boost::breadth_first_search(g, d,
                                boost::visitor(
                                        boost::make_bfs_visitor(
                                                boost::record_predecessors(predecessors.begin(),
                                                                           boost::on_tree_edge{}))));

    int p = topLeft;
    while (p != bottomRight)
    {
        std::cout << p << '\n';
        p = predecessors[p];
    }
    std::cout << p << '\n';
}
