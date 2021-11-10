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
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>::edge_iterator e_iterator;
    //typedef boost::graph_traits<graph> v_descriptor;
    typedef boost::graph_traits<graph>::edge_descriptor e_descriptor;


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

    struct edge {
        edge() {
            distance = -1;
            pred = std::vector<e_iterator>();
            num_shortest_paths = 0;
            dependency = 0;
        }
        int distance;
        std::vector<e_iterator> pred;
        int num_shortest_paths;
        double dependency;
    };

    bool shareNode(e_descriptor first, e_descriptor second) {
        if(first == second) {
            return false;
        }
        if(first.m_source == second.m_source || first.m_source == second.m_target || first.m_target== second.m_source || first.m_target== second.m_target) {
            return true;
        }
        else {
            return false;
        }
    }


    std::map<e_descriptor, double> edge_betweenness(graph &g) {
        std::map<e_descriptor, edge> edge_properties;
        std::map<e_descriptor, double> centrality_map;

        auto es = boost::edges(g);
        auto edge_counter = es.first;
        auto current_edge = es.first;
        auto end_of_graph = es.second;
        while (edge_counter != end_of_graph) { // initialization
            centrality_map.emplace(*edge_counter, 0);
            edge_properties.emplace(*edge_counter, edge());
            edge_counter++;
        }


        while (current_edge != end_of_graph) {
            std::queue<e_iterator> e_queue;
            std::stack<e_iterator> e_stack;

            auto edge_counter2 = es.first;
            while (edge_counter2 != end_of_graph) { // reinitialization for new edge
                edge_properties.at(*edge_counter2).pred.clear();
                edge_properties.at(*edge_counter2).distance = -1;
                edge_properties.at(*edge_counter2).num_shortest_paths = 0;
                edge_counter2++;
            }

            edge_properties.at(*current_edge).distance = 0;
            edge_properties.at(*current_edge).num_shortest_paths = 1;
            e_queue.push(current_edge);

            while (!e_queue.empty()) {
                auto edge1 = e_queue.front();
                e_queue.pop();
                e_stack.push(edge1);

                auto edge2 = es.first;
                while (edge2 != end_of_graph) {
                    if (shareNode(*edge1, *edge2)) {
                        if (edge_properties.at(*edge2).distance == -1) {
                            e_queue.push(edge2);
                            edge_properties.at(*edge2).distance = edge_properties.at(*edge1).distance + 1;
                        }
                        if (edge_properties.at(*edge2).distance == edge_properties.at(*edge1).distance + 1) {
                            edge_properties.at(*edge2).num_shortest_paths += edge_properties.at(*edge1).num_shortest_paths;
                            edge_properties.at(*edge2).pred.push_back(edge1);
                        }
                    }
                    edge2++;
                } // end of edge checks

            } // end of inner loop

            edge_counter = es.first;
            while (edge_counter != end_of_graph) {
                edge_properties.at(*edge_counter).dependency = 0;
                edge_counter++;
            }

            while (!e_stack.empty()) {
                auto edge2 = e_stack.top();
                e_stack.pop();
                auto edge2_pred = edge_properties.at(*edge2).pred;
                    for (auto &predecessor: edge2_pred) {
                        edge_properties.at(*predecessor).dependency += ((double) edge_properties.at(*predecessor).num_shortest_paths / (double) edge_properties.at(*edge2).num_shortest_paths) * (1 + edge_properties.at(*edge2).dependency);
                    }
                    if (edge2 != current_edge) {
                        centrality_map.at(*edge2) += edge_properties.at(*edge2).dependency;
                    }
            } // end of stack loop

            current_edge++;
        } // end of outer loop
        return centrality_map;
    }


    void girvan_newman(graph &g) {
        std::map<e_descriptor, double> e_b_values1 = edge_betweenness(g);

        boost::shared_array_property_map<double, boost::property_map<graph, boost::vertex_index_t>::const_type>
                centrality_map(num_vertices(g), get(boost::vertex_index, g));
        boost::brandes_betweenness_centrality(g, centrality_map);
//
//        std::cout << "Correct:" << std::endl;
//        std::cout << "0: " << centrality_map[0] << std::endl;
//        std::cout << "1: " << centrality_map[1] << std::endl;
//        std::cout << "2: " << centrality_map[2] << std::endl;
//        std::cout << "3: " << centrality_map[3] << std::endl;
//        std::cout << "4: " << centrality_map[4] << std::endl;
//        std::cout << "5: " << centrality_map[5] << std::endl;
//        std::cout << "6: " << centrality_map[6] << std::endl;
//        std::cout << "7: " << centrality_map[7] << std::endl;
//        std::cout << "8: " << centrality_map[8] << std::endl;
//        std::cout << "9: " << centrality_map[9] << std::endl;
//        std::cout << "10: " << centrality_map[10] << std::endl;
//        std::cout << "11: " << centrality_map[11] << std::endl;
//        //std::cout << "12: " << centrality_map[12] << std::endl << std::endl;

        std::map<e_descriptor, double> edge_centralities;
        auto ecm = boost::make_assoc_property_map(edge_centralities);
        boost::brandes_betweenness_centrality(g, boost::edge_centrality_map(ecm));

        std::cout << "done c" << std::endl;

        for (auto it = edge_centralities.begin(); it != edge_centralities.end(); it++)
        {
            std::cout << it->first    // string (key)
                      << ':'
                      << it->second   // string's value
                      << std::endl;
        }




        std::cout << "Ours:" << std::endl;
        for (auto& pair : e_b_values1 ) {
            std::cout << pair.first << ": " << pair.second << std::endl;
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
