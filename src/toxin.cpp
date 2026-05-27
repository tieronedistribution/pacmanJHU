#include "../include/toxin.h"
#include <iostream>
#include <algorithm>
#include <chrono>

// === Toxin Class Implementation ===

Toxin::Toxin(int id, int startX, int startY, int detection_range) 
    :Component(startX, startY, true), id(id), active(true), detection_range(detection_range) {
    position.x = startX;
    position.y = startY;
    previousPosition = position;
}

void Toxin::move(const Map& map, const Position& playerPosition, int detectionRange) {
    if (!active) return;
    
    previousPosition = position;
    
    Position newPosition = position;
    
    // If the player is visable, then chase the player
    if (canSeePlayer(map, playerPosition, detectionRange)) {
        newPosition = findPlayerDirection(map, playerPosition, detectionRange);
    } else {
        // Randommovement
        newPosition = getRandomDirection(map);
    }
    
    // Validate if the movement is valid
    if (map.isValidMove(newPosition.x, newPosition.y)) {
        position = newPosition;
    }
}

Position Toxin::findPlayerDirection(const Map& map, const Position& playerPosition, int detectionRange) const {
    Position direction = position;
    
    // Simple chase algortihm that moves toward player
    if (position.x < playerPosition.x && map.isValidMove(position.x + 1, position.y)) {
        direction.x = position.x + 1;
    } else if (position.x > playerPosition.x && map.isValidMove(position.x - 1, position.y)) {
        direction.x = position.x - 1;
    } else if (position.y < playerPosition.y && map.isValidMove(position.x, position.y + 1)) {
        direction.y = position.y + 1;
    } else if (position.y > playerPosition.y && map.isValidMove(position.x, position.y - 1)) {
        direction.y = position.y - 1;
    }
    
    return direction;
}

Position Toxin::getRandomDirection(const Map& map) const {
    static std::vector<Position> directions = {
        Position(1, 0),   // Right
        Position(-1, 0),  // Left
        Position(0, 1),   // Down
        Position(0, -1)   // Up
    };
    
    std::vector<Position> validMoves;
    
    // Find all effective movement direction
    for (const auto& dir : directions) {
        int newX = position.x + dir.x;
        int newY = position.y + dir.y;
        
        if (map.isValidMove(newX, newY)) {
            validMoves.push_back(Position(newX, newY));
        }
    }
    
    // Randomly choose one valid movement direction
    if (!validMoves.empty()) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, validMoves.size() - 1);
        return validMoves[dist(gen)];
    }
    
    return position;
}

bool Toxin::canSeePlayer(const Map& map, const Position& playerPosition, int range) const {
    int distance = calculateDistance(position, playerPosition);
    return distance <= range;
}

int Toxin::calculateDistance(const Position& pos1, const Position& pos2) const {
    return std::abs(pos1.x - pos2.x) + std::abs(pos1.y - pos2.y);
}

// === ToxinManager Class Implementation ===

ToxinManager::ToxinManager() {
    // Initialise random number generator 
    rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());
}

void ToxinManager::initializeToxins(int count, const Map& map) {
    toxins.clear();
    
    for (int i = 0; i < count; ++i) {
        // Randomly generate toxins at valid positions 
        int x, y;
        bool validPosition = false;
        
        while (!validPosition) {
            std::uniform_int_distribution<int> xDist(1, map.getWidth() - 2);
            std::uniform_int_distribution<int> yDist(1, map.getHeight() - 2);
            
            x = xDist(rng);
            y = yDist(rng);
            
            // Check if the position is valid (not the wall and not overlapping with toxins)and not no close to the player
            Toxin temp = Toxin(i + 1, x, y, 3);
            if (map.isValidMove(x, y) && temp.calculateDistance(Position{x,y}, Position{11, 11}) > 4){
                validPosition = true;
                // Check if overlapping with other toxins
                for (const auto& toxin : toxins) {
                    if (toxin.getPosition().x == x && toxin.getPosition().y == y) {
                        validPosition = false;
                        break;
                    }
                }
            }
        }
        toxins.push_back(Toxin(i + 1, x, y, 3));
    }
}

void ToxinManager::updateToxins(const Map& map, const Position& playerPosition, int detectionRange) {
    for (auto& toxin : toxins) {
        if (toxin.isActive()) {
            toxin.move(map, playerPosition, detectionRange);
        }
    }
}

void ToxinManager::drawToxins(Map& map) const {
    for (const auto& toxin : toxins) {
        if (toxin.isActive()) {
            map.setCell(toxin.getPosition().x, toxin.getPosition().y, '#');
        }
    }
}

bool ToxinManager::checkPlayerCollision(const Position& playerPosition) const {
    for (const auto& toxin : toxins) {
        if (toxin.isActive() && toxin.getPosition() == playerPosition) {
            return true;
        }
    }
    return false;
}