#ifndef WALL_H
#define WALL_H

#include <string>
#include "component.h"


/**
 * @class Wall
 * Non-movable walls that block both player and toxin movement. 
 */
class Wall: public Component{
    int x, y;
public:
    Wall(int xPos, int yPos): Component(xPos, yPos, false){}
    std::string getType() const override{return "Wall";}
};

#endif