#include <fstream>
#include "girvan_newman.h"

int main() {
    std::ifstream input("/Users/kynerd/Desktop/Algorithms/21f-pa03-kynerd-hulcy/input.grapml");
    algos::parse(input);
    //algos::hi();
}
