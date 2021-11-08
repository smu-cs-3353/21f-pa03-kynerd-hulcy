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

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, uint32_t, uint32_t, boost::no_property> graph;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, uint32_t, uint32_t, boost::no_property>::vertex_iterator v_iterator;
    typedef boost::graph_traits<graph> v_descriptor;
//
//    class MyVisitor : public boost::default_bfs_visitor
//    {
//    public:
//        std::list<std::list<v_descriptor>> paths;
//    };

//
//    class bfs_visitor {
//    public:
//        void initialize_vertex(const graph::vertex_descriptor &s, graph &g) {
//            std::cout << "Initialize: " << g[s] << std::endl;
//        }
//        void discover_vertex(const graph::vertex_descriptor &s, graph &g) {
//            std::cout << "Discover: " << g[s] << std::endl;
//        }
//        void examine_vertex(const graph::vertex_descriptor &s, graph &g) {
//            std::cout << "Examine vertex: " << g[s] << std::endl;
//        }
//        void examine_edge(const graph::edge_descriptor &e, graph &g) {
//            std::cout << "Examine edge: " << g[e] << std::endl;
//        }
//        void tree_edge(const graph::edge_descriptor &e, graph &g) {
//            std::cout << "Tree edge: " << g[e] << std::endl;
//        }
//        void non_tree_edge(const graph::edge_descriptor &e, graph &g) {
//            std::cout << "Non-Tree edge: " << g[e] << std::endl;
//        }
//        void gray_target(const graph::edge_descriptor &e, graph &g) {
//            std::cout << "Gray target: " << g[e] << std::endl;
//        }
//        void black_target(const graph::edge_descriptor &e, graph &g) {
//            std::cout << "Black target: " << g[e] << std::endl;
//        }
//        void finish_vertex(const graph::vertex_descriptor &s, graph &g) {
//            std::cout << "Finish vertex: " << g[s] << std::endl;
//        }
//    };

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
        std::queue<v_iterator> v_queue;
        std::stack<v_iterator> v_stack;
        auto vs = boost::vertices(g);
        auto counter = vs.first;
        auto start_of_graph = vs.first;
        auto end_of_graph = vs.second;
        while (counter != end_of_graph) { // initialization
            centrality_map.emplace(counter, 0);
            vertex_properties.emplace(counter, vertex());
            counter++;
        }


        while (start_of_graph != end_of_graph) {

            counter = start_of_graph;
            while (counter != end_of_graph) { // reinitialization for new vertex
                vertex_properties.at(counter).pred.clear();
                vertex_properties.at(counter).distance = -1;
                vertex_properties.at(counter).num_shortest_paths = 0;
                counter++;
            }

            vertex_properties.at(start_of_graph).distance = 0;
            vertex_properties.at(start_of_graph).num_shortest_paths = 1;
            //vertex_properties.at(start_of_graph).dependency = 0;
            v_queue.push(start_of_graph);

            while (!v_queue.empty()) {
                auto vertex1 = v_queue.front();
                v_queue.pop();
                v_stack.push(vertex1);

                auto vertex2 = vs.first;
                while (vertex2 != end_of_graph) {
                    if (boost::edge(*vertex1, *vertex2, g).second) {
                        if (vertex_properties.at(vertex2).distance == -1) {
                            vertex_properties.at(vertex2).distance = vertex_properties.at(vertex1).distance + 1;
                            //vertex_properties.at(vertex2).pred.push_back(vertex1);
                            v_queue.push(vertex2);
                        }
                        if (vertex_properties.at(vertex2).distance == vertex_properties.at(vertex1).distance + 1) {
                            vertex_properties.at(vertex2).num_shortest_paths += vertex_properties.at(vertex1).num_shortest_paths;
                            vertex_properties.at(vertex2).pred.push_back(vertex1);

                        }
                    }
                    vertex2++;
                } // end of edge checks

                while (!v_stack.empty()) {
                    vertex2 = v_stack.top();
                    v_stack.pop();
                    auto vertex2_pred = vertex_properties.at(vertex2).pred;
                    for (auto & predecessor : vertex2_pred) {
                        vertex_properties.at(predecessor).dependency +=
                                ((double)vertex_properties.at(predecessor).num_shortest_paths / (double)vertex_properties.at(vertex2).num_shortest_paths)
                                * (1 + vertex_properties.at(vertex2).dependency);
                    }
                    if (vertex2 != start_of_graph) {
                        centrality_map.at(vertex2) += vertex_properties.at(vertex2).dependency;
                    }
                } // end of stack loop
//                counter = vs.first;
//                while (counter != end_of_graph) {
//                    vertex_properties.at(counter).dependency = 0;
//                    counter++;
//                }
            } // end of inner loop

            start_of_graph++;
        } // end of outer loop
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
        std::cout << "4: " << centrality_map[4] << std::endl << std::endl;


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
