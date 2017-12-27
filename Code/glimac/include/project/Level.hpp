#pragma once
#include "Map.hpp"

class Level {

private:
    int m_id;
    Map m_map;
    int m_life;
    int m_points;

public:

    // getters 
    int getId() const;
    Map* getMap() const;
    int getLife() const;
    int getPoints() const;

    // setters 
    void setId(int);
    void setMap(Map);
    void setLife(int);
    void setPoints(int);

    /* Level initialization
     * 0 points
     * 3 lives
     * Map("level_"+id+".txt")
    */
    void initialization();

};