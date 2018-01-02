#include <iostream>
#include "../glimac/include/project/Map.hpp"
#include "project/GameManager.hpp"

int main() {

    Map m;
    m.setFileMap("classicMap.txt");
    m.initialization();
    m.display();
    std::vector<std::vector<int>> passage;
    for (int i = 0; i < m.getNbX(); i++) {
        std::vector<int> tab;
        for (int j = 0; j < m.getNbY(); j++) {
            tab.push_back(0);
        }
        passage.push_back(tab);
    }
    GameManager gameManager = GameManager(&m);
    //gameManager.play();
    std::cout << gameManager.countShortestWay(14,28, 7,1, passage) << std::endl;
    // m.~Map();
    return 0;
}