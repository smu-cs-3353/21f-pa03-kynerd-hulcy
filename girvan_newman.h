#ifndef INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
#define INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <vector>




namespace algos {

    struct vertex {
        vertex() {
            distance = -1;
            pred = std::list<vertex>();
            num_shortest_paths = 0;
        }
        int distance;
        std::list<vertex> pred;
        int num_shortest_paths;
    };


    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<>::vertex_iterator v_iterator;


    std::vector<double> edge_betweenness(graph &g) {
        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        std::map<boost::adjacency_list<>::vertex_iterator, vertex> vertices;
        v_iterator start = vs.first;
        v_iterator end = vs.second;

        while (start != end) {
        std::cout << *start << std::endl;
            vertices.emplace(start, vertex());
            start++;
        }
    }

    void girvan_newman(graph &g) {
        edge_betweenness(g);

    }


    void parse(std::ifstream &graphml_file) {
        graph g;
        boost::dynamic_properties dp;
        boost::read_graphml(graphml_file, g, dp);
        std::ofstream test("output.dot");
        boost::write_graphviz(test, g);

        girvan_newman(g);
    }



}


#endif //INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
