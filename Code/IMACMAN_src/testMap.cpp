#include <iostream>
#include "../glimac/include/project/Map.hpp"
#include "project/GameManager.hpp"

int main() {

    AudioManager audioManager = AudioManager();
    audioManager.addMusic(audioManager.createMusic("../Code/assets/audio/mainTheme.mp3"));
    audioManager.addMusic(audioManager.createMusic("../Code/assets/audio/mainThemeFast.mp3"));
    audioManager.fillSounds();
    audioManager.playMusic(0);
    Map m;
    m.setFileMap("classicMap.txt");
    GameManager gameManager = GameManager(&m);
    gameManager.load(false);
    m.initialization();
    m.display();
   // gameManager.play(&audioManager);
    
    std::vector<std::vector<int>> passage;
    for (int i = 0; i < m.getNbX(); i++) {
        std::vector<int> tab;
        for (int j = 0; j < m.getNbY(); j++) {
            tab.push_back(0);
        }
        passage.push_back(tab);
    }
   std::cout << gameManager.countShortestWay(1,2, 1,13, passage) << std::endl;
    // m.~Map();
    return 0;
}