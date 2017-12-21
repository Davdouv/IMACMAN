#include "../include/project/Map.hpp"
#include <iostream>

Map::Map() { 

}
std::string Map::getFileMap() const { return m_fileMap; }

void Map::setFileMap(std::string fileName) { m_fileMap = fileName; }
int Map::load() {

    std::fstream file;
    file.open(m_fileMap, std::ios::binary | std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cout << " error loading the file " << std::endl;
        return 0;
    }
    else {
        int i = 0;
        std::string tmp;
        file.seekg(0);
        while (!file.eof()) {
            getline(file, tmp);
            for (int j = 0; j < tmp.size(); j++) {
                if (!isStaticElement(tmp[j])) tmp[j] = 'V';
                Cell c(i, j, tmp[j]);
                m_cells[i][j] = c;
            }
            i++;
        }
    }   
    file.close();
    return 1;
}

void Map::display() {

    for (int i = 0; i < m_nbX; i++) {
        for (int  j = 0; j < m_nbY; j++) m_cells[i][j].display();
        std::cout << std::endl;
    }
}