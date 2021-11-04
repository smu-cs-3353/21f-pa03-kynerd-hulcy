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
#include <queue>
#include <stack>




namespace algos {

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<>::vertex_iterator v_iterator;

    struct vertex {
        vertex() {
            distance = -1;
            pred = std::list<v_iterator>();
            num_shortest_paths = 0;
            dependency = 0;
        }
        int distance;
        std::list<v_iterator> pred;
        int num_shortest_paths;
        double dependency;
    };


    std::map<v_iterator, double> edge_betweenness(graph &g) {
        std::map<v_iterator, double> edge_betweenness_values;
        std::queue<v_iterator> v_queue;
        std::stack<v_iterator> v_stack;
        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        std::map<boost::adjacency_list<>::vertex_iterator, vertex> vertices;
        v_iterator start = vs.first;
        v_iterator end = vs.second;

        while (start != end) {
        //std::cout << *start << std::endl;
            edge_betweenness_values.emplace(start, 0);
            vertices.emplace(start, vertex());
            start++;
        }

        start = vs.first;
        vertices.at(start).num_shortest_paths = 1;
        vertices.at(start).distance = 0;

        v_queue.emplace(start);

        while (!v_queue.empty()) {
            v_iterator vertex1 = v_queue.front();
            v_queue.pop();
            v_stack.emplace(vertex1);
            v_iterator vertex2 = vs.first;
            while (vertex2 != end) {
                if (boost::edge(*vertex1, *vertex2, g).second) {
                    if (vertices.at(vertex2).distance == -1) {

                        int test = vertices.at(vertex1).distance + 1;
                        vertices.at(vertex2).distance = test;
                        std::cout << test << std::endl;
                        v_queue.emplace(vertex2);
                    } else if (vertices.at(vertex2).distance == vertices.at(vertex1).distance + 1) {
                        vertices.at(vertex2).num_shortest_paths = vertices.at(vertex2).num_shortest_paths + vertices.at(vertex1).num_shortest_paths;
                        vertices.at(vertex2).pred.push_back(vertex1);
                    }
                }
                vertex2++;
            }

            start = vs.first;

            while (!v_stack.empty()) {
                v_iterator vertex3 = v_stack.top();
                v_stack.pop();
                std::list<v_iterator> curr_pred = vertices.at(vertex3).pred;
                for (auto &vertex4 : curr_pred) {
                    vertices.at(vertex4).dependency = vertices.at(vertex4).dependency +
                            ((double)vertices.at(vertex4).num_shortest_paths / vertices.at(vertex3).num_shortest_paths)
                            * (1 + vertices.at(vertex3).dependency);
                }
                if (vertex3 != start) {
                    edge_betweenness_values.at(vertex3) = edge_betweenness_values.at(vertex3) + vertices.at(vertex3).dependency;
                }
            }
        }
        return edge_betweenness_values;
    }

    void girvan_newman(graph &g) {
        std::map<v_iterator, double> e_b_values1 = edge_betweenness(g);

        for (auto& pair : e_b_values1 ) {
            std::cout << *pair.first << ": " << pair.second << std::endl;
        }
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
