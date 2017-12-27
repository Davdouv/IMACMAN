#include <iostream>
#include "../glimac/include/project/Map.hpp"

int main() {

    Map m;
    m.setFileMap("mapTest.txt");
    m.load();std::cout << "done" << std::endl;
    m.display();
    m.play();
    m.getPacGommes()[1]->display();
    std::cout << m.getSuperPacGommes().size() << std::endl;
    m.getSuperPacGommes()[1]->display();
    return 0;
}