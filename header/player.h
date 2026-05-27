#ifndef PLAYER_H
#define PLAYER_H

#include <atomic>
#include <mutex>
#include "common.h"
#include "component.h"

class Map;

/**
 * @class Player
 * Represents the player @ 
 * Stores position, current scores, how many lives left, direction state
 * Thread safety ensured given mutex and tomics variables
 */

class Player: public Component {
public:
    

    Player();    ///< Instantiate player with default position, 3 lives and o score
    ~Player() = default;
    
    // various movement method (upward, downward, left or right). Position and direction state updated if the move is valid 
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void move(Direction dir, const Map& map);
    
    // Various method to update current player state
    void resetPosition();  ///< Reset to default player position
    void loseLife();    ///< Lives decrement by one
    void respawn();     ///< Reset position, isAlive and currentDirection
    
    // Getters returning the current player state
    int getX() const;
    int getY() const;
    int getLives() const;
    int getScore() const;
    Direction getCurrentDirection() const;
    bool isAlive() const;
    Position getPosition() const;
    

    void setPosition(int x, int y);
    void addScore(int points);
    
    
    bool isValidMove(int newX, int newY, const Map& map) const;      ///< Validating the move and update the player position only when it returns true
    std::string getType() const override{return "Player";}

private:

    int posX, posY;                      ///< Current xy coordinates
    int startX, startY;                  ///< Starting position
    std::atomic<int> lives;              ///< How many lives left
    std::atomic<int> score;              ///< How much scores
    std::atomic<Direction> currentDirection; ///< Movement direction
    std::atomic<bool> alive;             ///< Alive flag
    mutable std::mutex positionMutex;
};

#endif 
