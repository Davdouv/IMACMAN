#include <iostream>
#include "../glimac/include/project/Map.hpp"
#include "project/GameManager.hpp"

int main() {

    Map m;
    m.setFileMap("classicMap.txt");
    m.initialization();
    m.display();
    GameManager gameManager = GameManager(&m);
    gameManager.play();
    m.~Map();
    return 0;
}