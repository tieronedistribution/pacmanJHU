#ifndef COMPONENT_H
#define COMPONENT_H

#include<string>


/**
 * @class Component
 * @brief Abstract base class (ABC)
 * 
 * @details All critical objects i.e. Player, Toxin, Cookie, Wall inherits from this class. Type identification given getType
 *          Player and toxins are moveable object, while the cookie and wall are not moveable. All protected members. 
 */ 
class Component {
protected:
    int x, y;   ///< xy coordinate in the map grid
    bool movable;

public:

    // Construct one component given the xy position, where status and position are constructed. Only derived classes can be instantiated. 
    Component(int xPos, int yPos, bool isMovable)
        : x(xPos), y(yPos), movable(isMovable) {}
        
    virtual ~Component() = default;     ///< Virtual destructor called as game ends and deleting through base class pointer
    virtual std::string getType() const = 0;    ///< Getter for the type identifier of the component. Virtual and implemented by components.

    // Return current x,y position of the component. Then update with new x-y coordinate and set it as the new position for the component
    int getX() const {return x;}
    int getY() const {return y;}
    void setPosition(int newX, int newY){
        x = newX;
        y = newY;
    }

    bool isMovable() const {return movable;}    ///< Return whether the object is moveable during construction. Cookies, wall will return false. Player and toxins will return true.  
};

#endif