#include "Character.hpp"

class Ghost : public Character {

private:
    int m_type;

public: 

    // constructor
    Ghost(int, int, int, int, int, int, Orientation);
    Ghost();

    // getter
    int getType() const;

    // setter
    void setType(int);

};