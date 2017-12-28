#pragma once
class Player {

private:
    int m_life;
    int m_points;
    static const int m_maxLives = 9;

public:

    Player();
    // getters 
    int getLife() const;
    int getPoints() const;
    int getMaxLives() const;

    // setters 
    void setLife(int);
    void setPoints(int);

    /* Level initialization
     * 0 points
     * 3 lives
    */
    void initialization();

    void gainLife();
    void loseLife();
    void gainPoints(unsigned int);
    void losePoints(int);

};