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
#include <boost/graph/connected_components.hpp>

namespace Girvan_Newman {



    struct VertexProperty {
        std::string label;
        long value;
        long node_id;
    };

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    //typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty> graph; // typedef for football.graphml
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS>::vertex_iterator v_iterator;
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


    std::map<e_descriptor, double> edge_betweenness(graph &g) {
        std::map<v_iterator, vertex> vertex_properties;
        std::map<v_iterator, double> centrality_map;
        std::map<e_descriptor, double> edge_centrality_map;

        auto vs = boost::vertices(g);
        auto counter = vs.first;
        auto current_vertex = vs.first;
        auto end_of_graph = vs.second;
        while (counter != end_of_graph) { // initialization
            centrality_map.emplace(counter, 0);
            vertex_properties.emplace(counter, vertex());
            counter++;
        }

        auto es = boost::edges(g);
        auto e_counter = es.first;
        auto e_end_of_graph = es.second;
        while (e_counter != e_end_of_graph) { // initialization
            edge_centrality_map.emplace(*e_counter, 0);
            e_counter++;
        }

        while (current_vertex != end_of_graph) {
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
                        double c = ((double) vertex_properties.at(predecessor).num_shortest_paths / (double) vertex_properties.at(vertex2).num_shortest_paths) * (1 + vertex_properties.at(vertex2).dependency);
                        e_descriptor t = boost::edge(*predecessor, *vertex2, g).first;
                        edge_centrality_map.at(t) += c;
                        vertex_properties.at(predecessor).dependency += c;

                    }
                    if (vertex2 != current_vertex) {
                        centrality_map.at(vertex2) += vertex_properties.at(vertex2).dependency/2;
                    }
            } // end of stack loop

            current_vertex++;

        } // end of outer loop
        for (auto & it : edge_centrality_map)
        {
            it.second = it.second/2;
        }
        return edge_centrality_map;
    }


    void girvan_newman(graph &g) {
        std::vector<int> component (boost::num_vertices (g));
        size_t num_components = boost::connected_components (g, &component[0]);
        std::map<e_descriptor , double> betweenness_map = edge_betweenness(g);

        while (num_components < 4 && !betweenness_map.empty()) {
            double highest_betweenness = 0;
            e_descriptor edge_to_remove;
            for (auto & pair : betweenness_map) {
                if (pair.second > highest_betweenness) {
                    highest_betweenness = pair.second;
                    edge_to_remove = pair.first;
                }
            }
            g.remove_edge(edge_to_remove);
            betweenness_map.erase(edge_to_remove);
            num_components = boost::connected_components (g, &component[0]);
            betweenness_map = edge_betweenness(g);
        }
    };


    void run(std::ifstream &graphml_file) {
        graph g;

        boost::dynamic_properties dp;

//        dynamic properties for football graphml
//        dp.property("label", boost::get(&VertexProperty::label, g));
//        dp.property("value", boost::get(&VertexProperty::value, g));
//        dp.property("node_id", boost::get(&VertexProperty::node_id, g));

        boost::read_graphml(graphml_file, g, dp);

        girvan_newman(g);

        std::ofstream testOutput("../output/output.graphml");
        boost::write_graphml(testOutput, g, dp);
    };

}


#endif //INC_21F_PA03_KYNERD_HULCY_GIRVAN_NEWMAN_H
