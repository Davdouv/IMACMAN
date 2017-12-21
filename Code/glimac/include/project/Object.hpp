#pragma once

class Object {

private:
    int m_width;
    int m_height;
    int m_posX;
    int m_posY;

public:

    // constructor 
    Object(int, int);
    Object();

    // getter
    int getWidth() const;
    int getHeight() const;
    int getPosX() const;
    int getPosY() const;

    // setter
    void setWidth(int);
    void setHeight(int);
    void setPosX(int);
    void setPosY(int);
};