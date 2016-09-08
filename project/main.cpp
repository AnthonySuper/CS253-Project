#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <number_list.hpp>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "No filename given" << std::endl;
        return -1;
    }
    try {
        NumberList l(argv[1]);
        l.printStemAndLeaf();
        return 0;
    }
    catch(const std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
