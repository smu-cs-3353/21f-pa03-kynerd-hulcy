#include <fstream>
#include <iostream>
#include "girvan_newman.h"
#include <Python.h>


using namespace std;

int main(int argc, char** argv) {
    Py_Initialize();

    ifstream input;

    if (argc > 1) { // if input file specified
        string filename = argv[1];
        if (filename.substr(filename.size()-8, filename.size()) == ".graphml") {
            input.open("../" + filename);
        } else { // checks for .graphml file
            cout << "Please enter the name of a .graphml file." << endl;
        }
    } else {
        FILE* graph_generator = fopen("../src/graphs.py", "r");
        if (graph_generator) { // generate random graph using python
            PyRun_SimpleFile(graph_generator, "graphs.py");
            input.open("../input/random_graph.graphml");
        }
    }

    if (input.is_open()) Girvan_Newman::run(input); // runs Girvan-Newman algorithm if file is open
    else cout << "Error reading input file." << endl;

    FILE* graph_printer = fopen("../src/printGraph.py", "r");
    if (graph_printer) PyRun_SimpleFile(graph_printer, "printGraph.py");
}
