#pragma once

class Object {

private:
    int m_width;
    int m_height;

public:

    // constructor 
    Object(int, int);
    Object();

    // getter
    int getWidth() const;
    int getHeight() const;

    // setter
    void setWidth(int);
    void setHeight(int);
};