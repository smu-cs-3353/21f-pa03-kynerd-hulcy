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
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <fstream>

int main() {
    std::ifstream input("../input.grapml");
//    algos::parse(input);
    //algos::hi();

    enum { topLeft, topRight, bottomRight, bottomLeft };

    std::array<std::pair<int, int>, 4> edges{{
                                                     std::make_pair(topLeft, topRight),
                                                     std::make_pair(topRight, bottomRight),
                                                     std::make_pair(bottomRight, bottomLeft),
                                                     std::make_pair(bottomLeft, topLeft)
                                             }};

    typedef boost::adjacency_list<boost::setS, boost::vecS,
            boost::undirectedS> graph;


    graph g = algos::parse(input);

    std::pair<boost::adjacency_list<>::vertex_iterator, boost::adjacency_list<>::vertex_iterator> vs = boost::vertices(g);
    boost::adjacency_list<>::vertex_iterator start = vs.first;
    boost::adjacency_list<>::vertex_iterator end = vs.second;
    boost::adjacency_list<>::vertex_iterator test = start + 6;


    boost::array<boost::adjacency_list<>::vertex_descriptor, 10> predecessors;
    boost::array<int, 10> distances{{0}};
    predecessors[*test] = *test;

    boost::breadth_first_search(g, *test,
                                boost::visitor(
                                        boost::make_bfs_visitor(
                                                std::make_pair(
                                                        boost::record_distances(distances.begin(),
                                                                                boost::on_tree_edge()),
                                                        boost::record_predecessors(predecessors.begin(),
                                                                                   boost::on_tree_edge{})))));


    boost::adjacency_list<>::vertex_descriptor p = *start;

    int count = 0;
    while (p != *test)
    {
        std::cout << p << '\n';
        count++;
        p = predecessors[p];
    }
    std::cout << p << '\n';
    int num_shortest_paths = 0;

    std::cout<< "distances" << std::endl;
    for(int i = 0; i < 10; i++) {
        std::cout << distances[i] << std::endl;
    }



//    std::for_each(distances.begin(), distances.end(),
//                  [](int d){
//        //std::cout << d << '\n';
//        if(d == count) {
//            num_shortest_paths++;
//        }
//    });


}
