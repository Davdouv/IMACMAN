#include <iostream>
#include "../glimac/include/project/Map.hpp"
#include "project/GameManager.hpp"

int main() {

    Map m;
    m.setFileMap("mapTest.txt");
    m.initialization();
    m.display();
    GameManager gameManager = GameManager(&m);
    std::cout << gameManager.nextMove(1,2,1,1) << std::endl;
   // m.~Map();
    return 0;
}