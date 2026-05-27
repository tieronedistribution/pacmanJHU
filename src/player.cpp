#include "../include/player.h"
#include "map.h"
#include <iostream>

/*
* Initialize player at starting position with default values, 3 lives and 0 score
*/
Player::Player() 
    : Component(startX, startY, true),
      posX(11), posY(11), startX(11), startY(11), 
      lives(3), score(0), 
      currentDirection(Direction::RIGHT), 
      alive(true) {
}

//move up by one unit
void Player::moveUp() {
    std::lock_guard<std::mutex> lock(positionMutex);
    posY--;
    currentDirection = Direction::UP;
}

// move down by one unit
void Player::moveDown() {
    std::lock_guard<std::mutex> lock(positionMutex);
    posY++;
    currentDirection = Direction::DOWN;
}

// move left by one unit
void Player::moveLeft() {
    std::lock_guard<std::mutex> lock(positionMutex);
    posX--;
    currentDirection = Direction::LEFT;
}

// move right by one unit
void Player::moveRight() {
    std::lock_guard<std::mutex> lock(positionMutex);
    posX++;
    currentDirection = Direction::RIGHT;
}
// move player in certain direction only if the move is valid
void Player::move(Direction direction, const Map& map){
    int newX = this->posX;
    int newY = this->posY;
    switch(direction){
        case Direction::UP:
            newY--;
            break;
        case Direction::DOWN:
            newY++;
            break;
        case Direction::LEFT:
            newX--;
            break;
        case Direction::RIGHT:
            newX++;
            break;
        default:
            break;
    }
    if(isValidMove(newX, newY, map)){
        std::lock_guard<std::mutex> lock(positionMutex);
        this->posX = newX;
        this->posY = newY;
        this->currentDirection = direction;
    }else{
    }
}

//reset player to starting position
void Player::resetPosition() {
    std::lock_guard<std::mutex> lock(positionMutex);
    posX = startX;
    posY = startY;
    currentDirection = Direction::RIGHT;
    alive = true;
}

//decrease lives by one and respawn or end game
void Player::loseLife() {
    lives--;
    alive = false;
    
    if (lives <= 0) {
        gameRunning = false;
    } else {
        respawn();
    }
}

void Player::respawn() { 
    resetPosition();
}

bool Player::isValidMove(int newX, int newY, const Map& map) const {
    return map.isValidMove(newX, newY);
}

//gettor for player crrent x coordinate
int Player::getX() const {
    std::lock_guard<std::mutex> lock(positionMutex);
    return posX;
}

//gettor for player crrent y coordinate
int Player::getY() const {
    std::lock_guard<std::mutex> lock(positionMutex);
    return posY;
}

//gettor for player crrent Position Object
Position Player::getPosition() const{
    std::lock_guard<std::mutex> lock(positionMutex);
    return Position(posX, posY);
}

//gettor for player crrent lives
int Player::getLives() const {
    return lives.load();
}

//gettor for player crrent score
int Player::getScore() const {
    return score.load();
}

//gettor for player crrent facing direction
Direction Player::getCurrentDirection() const {
    return currentDirection.load();
}

//gettor to check if player is currently alive
bool Player::isAlive() const {
    return alive.load();
}

//set player to specific coordinates
void Player::setPosition(int x, int y) {
    std::lock_guard<std::mutex> lock(positionMutex);
    posX = x;
    posY = y;
}

//add score to player
void Player::addScore(int points) {
    score += points;
}
