#include <fstream>
#include "girvan_newman.h"

int main() {
    std::ifstream input("../input.grapml");
    algos::parse(input);
    //algos::hi();
}
