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
            distance = 2147483647;
            pred = std::list<v_iterator>();
            num_shortest_paths = 0;
            dependency = 0;
        }
        int distance;
        std::list<v_iterator> pred;
        int num_shortest_paths;
        double dependency;
    };

    void resetDependencies(graph &g, std::map<boost::adjacency_list<>::vertex_iterator, vertex>& vertices) {
        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        v_iterator start = vs.first;
        v_iterator end = vs.second;

        while(start != end) {
            vertices.at(start).dependency = 0;
            start++;
        }
    }

    void resetAttributes(graph &g, std::map<boost::adjacency_list<>::vertex_iterator, vertex>& vertices) {
        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        v_iterator start = vs.first;
        v_iterator end = vs.second;

        while(start != end) {
            vertices.at(start).pred = std::list<v_iterator>();
            vertices.at(start).distance = 2147483647;
            vertices.at(start).num_shortest_paths = 0;
            start++;
        }
    }


    std::map<v_iterator, double> edge_betweenness(graph &g) {
        std::map<v_iterator, double> edge_betweenness_values;
        std::queue<v_iterator> v_queue; //Q
        std::stack<v_iterator> v_stack; //S
        std::pair<v_iterator, v_iterator> vs = boost::vertices(g);
        std::map<boost::adjacency_list<>::vertex_iterator, vertex> vertices; //V
        v_iterator start = vs.first;
        v_iterator end = vs.second;

        while (start != end) {
            //std::cout << *start << std::endl;
            edge_betweenness_values.emplace(start, 0); //initialize to 0
            vertices.emplace(start, vertex()); //put v in V and initialize dependency to 0
            start++;
        }
        start = vs.first;
        while (start != end) { //for s in V
            //initialization
            resetAttributes(g, vertices);
            vertices.at(start).num_shortest_paths = 1; //num_shortest_paths[s] <- 1
            vertices.at(start).distance = 0; //dist[s] <- 0

            v_queue.emplace(start); //enqueue s -> Q

            while (!v_queue.empty()) { //while Q not empty
                v_iterator v = v_queue.front(); //v
                v_queue.pop(); //dequeue v <- Q
                v_stack.emplace(v); //push v -> S
                v_iterator w = vs.first; //w
                while (w != end) {//for each
                    if (boost::edge(*v, *w, g).second) { //such that (v,w) are in E
                        //path discovery
                        if (vertices.at(w).distance == 2147483647) {//if dist[w] = infinity
                            vertices.at(w).distance = vertices.at(v).distance + 1; //dist[w] <- dist[v] + 1
                            v_queue.emplace(w); //enqueue w -> Q
                        }
                            //path counting
                        else if (vertices.at(w).distance == vertices.at(v).distance + 1) { //if dist[w] = dist[v] + 1
                            vertices.at(w).num_shortest_paths = vertices.at(w).num_shortest_paths + vertices.at(
                                    v).num_shortest_paths; //num_shortest_paths[w] = /num_shortest_paths[w] + /num_shortest_paths[v]
                            vertices.at(w).pred.push_back(v); //append v -> Pred[w]
                        }
                    }
                    w++;
                }//end foreach
            }//end while Q not empty

            //accumulation
            resetDependencies(g, vertices); //for v in V do dependency[v] <- 0
            while (!v_stack.empty()) { //while S not empty
                v_iterator w = v_stack.top(); //pop w <- S
                v_stack.pop(); //pop w <- S
                std::list<v_iterator> curr_pred = vertices.at(w).pred;
                for (auto &v : curr_pred) {//for v in Pred[w]
                    double test = vertices.at(v).dependency +
                            ((double)vertices.at(v).num_shortest_paths / (double)vertices.at(w).num_shortest_paths)
                            * (1 + vertices.at(w).dependency);
                    vertices.at(v).dependency = test;
                }
                if (*w != *start) {//if w != s
                    edge_betweenness_values.at(w) = edge_betweenness_values.at(w) + vertices.at(w).dependency; //cB[w] <- cB[w] + dependency[w]
                }
            }//end while S not empty
            start++;
        }//end for s in V

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
