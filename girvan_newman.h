#ifndef INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
#define INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/array.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/graph/visitors.hpp>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <boost/graph/compressed_sparse_row_graph.hpp>
#include <boost/graph/betweenness_centrality.hpp>


namespace algos {

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<>::vertex_iterator v_iterator;
    typedef boost::adjacency_list<>::vertex_descriptor v_index;

    int n; // Number of nodes
    bool isWeigthed; // Weighted graph check.
    adjacency_list adjList; // Adjacency list.

    // Centrality measures.
    map<string, float> edgeBetweenness;
    vector<float> nodeBetweenness;
    nodeBetweenness.resize(n, 0);
    vector<float> closeness;
    closeness.resize(n, 0);

    // Input is read, and values are set to all the arguments.
    readGraph(n, isWeigthed, adjList, edgeBetweenness);

    list<int> pred[n]; // List of predecessors of node v.
    vector<int> sigma;
    vector<float> delta;
    stack<int> visitStack; // Stack that holds the inverse order of visited nodes.


    void girvan_newman(graph &g) {
        std::map<boost::graph_traits<graph>::edge_descriptor, float> e_b_values1 = edge_betweenness(g);

//        for (auto &pair: e_b_values1) {
//            std::cout << pair.first << ": " << pair.second << std::endl;
//        }
    }


    graph parse(std::ifstream &graphml_file) {
        boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> g;
        boost::dynamic_properties dp;
        boost::read_graphml(graphml_file, g, dp);
        std::ofstream test("output.dot");
        boost::write_graphviz(test, g);
        return g;
        //girvan_newman(g);
    }

}


#endif //INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
