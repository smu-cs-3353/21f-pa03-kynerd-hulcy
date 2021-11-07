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


namespace algos {

    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
    typedef boost::adjacency_list<>::vertex_iterator v_iterator;

    struct vertex {
        vertex() {
            distance = -1;
            pred = std::vector<v_iterator>();
            num_shortest_paths = 0;
            dependency = 0;
        }
         void print() {
            std::cout << "distance: " << distance << std::endl;
             std::cout << "num_shortest_paths: " << num_shortest_paths << std::endl;
             std::cout << "dependency: " << dependency << std::endl;

         }

        int distance;
        std::vector<v_iterator> pred;
        int num_shortest_paths;
        float dependency;
    };

    struct neighbor {
        int target;
        int weight;

        neighbor(int mTarget, int mWeight) : target(mTarget), weight(mWeight) {
        }
    };


    float bfs_SSSP(v_iterator src, int n, std::stack<v_iterator> &visitStack,
                   std::map<boost::adjacency_list<>::vertex_iterator, vertex> &vertices, graph &g) {
        // Closeness counter.
        float closeness = 0;

        // Queue used for the Bfs algorithm.
        std::queue<v_iterator> visitQueue;
        visitQueue.push(src);

        // While there are still elements in the queue.
        while (!visitQueue.empty()) {
            // Pop the first.
            v_iterator v = visitQueue.front();
            visitQueue.pop();
            visitStack.emplace(v);

            // Closeness part aggregation.
            closeness += vertices.at(v).distance;
        }
    }


    // Clears the variables or re-initializes to 0, so that they are ready for the next loop.
    void resetVariables(v_iterator src, std::map<boost::adjacency_list<>::vertex_iterator, vertex>& vertices, v_iterator start, v_iterator end) {
        vertices.at(src).num_shortest_paths = 0;
        vertices.at(src).dependency = 0;
        vertices.at(src).pred.clear();

        //all other dist except src are -1
        while(start != end) {
            vertices.at(start).distance = -1;
            start++;
        }
    }


    std::map<boost::graph_traits<graph>::edge_descriptor, float> edge_betweenness(graph &g) {
        std::map<boost::graph_traits<graph>::edge_descriptor, float> edgeBetweenness; //holds edge iterator and edge betweenness

        std::pair<boost::graph_traits<graph>::edge_iterator, boost::graph_traits<graph>::edge_iterator> es = boost::edges(g);
        boost::graph_traits<graph>::edge_iterator e_start = es.first;
        boost::graph_traits<graph>::edge_iterator e_end = es.second;
        while(e_start != e_end) {
            edgeBetweenness.emplace(*e_start, 0);
            e_start++;
        }


        std::stack<v_iterator> visitStack; // Stack that holds the inverse order of visited nodes.

        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        std::map<boost::adjacency_list<>::vertex_iterator, vertex> vertices;
        v_iterator start = vs.first;
        v_iterator end = vs.second;
        while(start != end) {
            vertices.emplace(start, vertex());
            start++;
        }


        start = vs.first;
        for (v_iterator src = start; src != end; src++) { //for each node in the graph
            resetVariables(src, vertices, start, end);

            std::vector<boost::graph_traits<graph>::vertex_descriptor> predVector;
            //boost::graph::breadth_first_search(g, *src,
//                                        boost::visitor(
//                                                boost::make_bfs_visitor(
//                                                        boost::record_predecessors(predVector.begin(),
//                                                                                   boost::on_tree_edge{}))));


            // Get the inverse order of visited nodes.
            while (!visitStack.empty()) {
                v_iterator w = visitStack.top();
                visitStack.pop();
                std::vector<v_iterator> predVector = vertices.at(w).pred;
                // For each predecessors of node w, do the math!
                for(int v = 0; v < predVector.size(); v++) {
                    float c = ((float) vertices.at(predVector[v]).num_shortest_paths / (float) vertices.at(w).num_shortest_paths) * (1.0 + vertices.at(w).dependency);
                    vertices.at(predVector[v]).dependency += c;

                    // Edge betweenness aggregation part.
                    auto edgeId = boost::edge(*w, *predVector[v], g).first;
                    float tempC = edgeBetweenness.at(edgeId);
                    edgeBetweenness.erase(edgeId);
                    edgeBetweenness.emplace(edgeId, tempC + c);
                }
            }
            //vertices.at(src).print();
        }//end outer for loop
        return edgeBetweenness;
    }

    void girvan_newman(graph &g) {
        std::map<boost::graph_traits<graph>::edge_descriptor, float> e_b_values1 = edge_betweenness(g);

//        for (auto &pair: e_b_values1) {
//            std::cout << pair.first << ": " << pair.second << std::endl;
//        }
    }


    boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS> parse(std::ifstream &graphml_file) {
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
