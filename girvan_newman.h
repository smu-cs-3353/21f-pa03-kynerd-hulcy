#ifndef INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
#define INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <boost/concept/assert.hpp>
#include <boost/graph/visitors.hpp>
#include <boost/graph/betweenness_centrality.hpp>




namespace algos {

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>::vertex_iterator v_iterator;
    //typedef boost::graph_traits<graph> v_descriptor;


    struct vertex {
        vertex() {
            distance = -1;
            pred = std::vector<v_iterator>();
            num_shortest_paths = 0;
            dependency = 0;
        }
        int distance;
        std::vector<v_iterator> pred;
        int num_shortest_paths;
        double dependency;
    };


    std::map<v_iterator, double> edge_betweenness(graph &g) {
        std::map<v_iterator, vertex> vertex_properties;
        std::map<v_iterator, double> centrality_map;

        auto vs = boost::vertices(g);
        auto counter = vs.first;
        auto current_vertex = vs.first;
        auto end_of_graph = vs.second;
        while (counter != end_of_graph) { // initialization
            centrality_map.emplace(counter, 0);
            vertex_properties.emplace(counter, vertex());
            counter++;
        }

        while (current_vertex != end_of_graph) {
            if(*current_vertex == 1) {
                std::cout << "current node: "<< *current_vertex << std:: endl;
            }
            std::queue<v_iterator> v_queue;
            std::stack<v_iterator> v_stack;

            counter = vs.first;
            while (counter != end_of_graph) { // reinitialization for new vertex
                vertex_properties.at(counter).pred.clear();
                vertex_properties.at(counter).distance = -1;
                vertex_properties.at(counter).num_shortest_paths = 0;
                counter++;
            }

            vertex_properties.at(current_vertex).distance = 0;
            vertex_properties.at(current_vertex).num_shortest_paths = 1;
            v_queue.push(current_vertex);

            while (!v_queue.empty()) {
                auto vertex1 = v_queue.front();
                v_queue.pop();
                v_stack.push(vertex1);

                auto vertex2 = vs.first;
                while (vertex2 != end_of_graph) {
                    if (boost::edge(*vertex1, *vertex2, g).second) {
                        std::cout << *vertex1 << ", " << *vertex2 << std:: endl;
                        if (vertex_properties.at(vertex2).distance == -1) {
                            v_queue.push(vertex2);
                            vertex_properties.at(vertex2).distance = vertex_properties.at(vertex1).distance + 1;
                        }
                        if (vertex_properties.at(vertex2).distance == vertex_properties.at(vertex1).distance + 1) {
                            vertex_properties.at(vertex2).num_shortest_paths += vertex_properties.at(vertex1).num_shortest_paths;
                            vertex_properties.at(vertex2).pred.push_back(vertex1);
                        }
                    }
                    vertex2++;
                } // end of edge checks

            } // end of inner loop

            counter = vs.first;
            while (counter != end_of_graph) {
                vertex_properties.at(counter).dependency = 0;
                counter++;
            }

            while (!v_stack.empty()) {
                auto vertex2 = v_stack.top();
                v_stack.pop();
                auto vertex2_pred = vertex_properties.at(vertex2).pred;
                    for (auto &predecessor: vertex2_pred) {
                        if(*predecessor == 0) {
                            std::cout << "at 0" << std::endl;
                        }
                        vertex_properties.at(predecessor).dependency += ((double) vertex_properties.at(predecessor).num_shortest_paths / (double) vertex_properties.at(vertex2).num_shortest_paths) * (1 + vertex_properties.at(vertex2).dependency);
                        if (vertex_properties.at(predecessor).dependency > 0) {
                            std::cout << "not 0" << std::endl;
                        }
                    }
                    if (vertex2 != current_vertex) {
                        if(*vertex2 == 0) {
                            std::cout << "at 0" << std::endl;
                        }
                        centrality_map.at(vertex2) += vertex_properties.at(vertex2).dependency/2;
                    }
            } // end of stack loop

            current_vertex++;
        } // end of outer loop
//        for (auto it = centrality_map.begin(); it != centrality_map.end(); it++)
//        {
//            it->second = it->second/2;
//        }
        return centrality_map;
    }


    void girvan_newman(graph &g) {
        std::map<v_iterator, double> e_b_values1 = edge_betweenness(g);

        boost::shared_array_property_map<double, boost::property_map<graph, boost::vertex_index_t>::const_type>
                centrality_map(num_vertices(g), get(boost::vertex_index, g));
        boost::brandes_betweenness_centrality(g, centrality_map);

        std::cout << "Correct:" << std::endl;
        std::cout << "0: " << centrality_map[0] << std::endl;
        std::cout << "1: " << centrality_map[1] << std::endl;
        std::cout << "2: " << centrality_map[2] << std::endl;
        std::cout << "3: " << centrality_map[3] << std::endl;
        std::cout << "4: " << centrality_map[4] << std::endl;
//        std::cout << "5: " << centrality_map[5] << std::endl;
//        std::cout << "6: " << centrality_map[6] << std::endl;
//        std::cout << "7: " << centrality_map[7] << std::endl;
//        std::cout << "8: " << centrality_map[8] << std::endl;
//        std::cout << "9: " << centrality_map[9] << std::endl;
//        std::cout << "10: " << centrality_map[10] << std::endl;
//        std::cout << "11: " << centrality_map[11] << std::endl;
//        std::cout << "12: " << centrality_map[12] << std::endl << std::endl;


        std::cout << "Ours:" << std::endl;
        for (auto& pair : e_b_values1 ) {
            std::cout << *pair.first << ": " << pair.second << std::endl;
        }
    };


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
