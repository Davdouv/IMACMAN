#include <iostream>
#include "../glimac/include/project/Map.hpp"

int main() {

    Map m;
    m.setFileMap("/home/laure/IMACMAN/Code/data/mapTest.txt");
    m.load();std::cout << "done" << std::endl;
    m.display();
    return 0;
}