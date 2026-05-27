#ifndef TOXIN_H
#define TOXIN_H

#include "common.h"
#include "map.h"
#include "component.h"
#include <vector>
#include <random>

/**
 * @class Toxin
 * AI enemy where the player needs to avoid collision with.
 * Enemy chases player when he is in detection range. Move randomly while not in the range.
 * Calculates mahattan distance for targeting and track previous position when it is rendering
 * 
 */
class Toxin: public Component{
private:
    Position position;
    Position previousPosition;
    int id;
    bool active;
    int detection_range;
    
public:
    Toxin(int id, int startX, int startY, int detection_range = 3);
    
    /*
     * Movement
     * Check if inside detection range. if yes > calculate direction with the player. if no > choose random direction that is valid. 
     * Moves are validated before updating of the position.
     */
    void move(const Map& map, const Position& playerPosition, int detectionRange);
    Position findPlayerDirection(const Map& map, const Position& playerPosition, int detectionRange) const;
    Position getRandomDirection(const Map& map) const;
    
    /*
     * State Query
     * Get either current position or previous position. Check if toxin is active, which can move and hunt player. 
     * return true if active
     */
    Position getPosition() const { return position; }
    Position getPreviousPosition() const { return previousPosition; }
    bool isActive() const { return active; }
    void setActive(bool state) { active = state; }
    
    // type identifier
    std::string getType() const override { return "Toxin"; }
    
    /*
     * Utility Functions
     * Check if toxins can see player based on manhattan distance check. 
     */
    bool canSeePlayer(const Map& map, const Position& playerPosition, int range) const;
    int calculateDistance(const Position& pos1, const Position& pos2) const;
};

class ToxinManager {
private:
    std::vector<Toxin> toxins;
    std::mt19937 rng;
    
public:
    ToxinManager();
    
    // Toxin management
    void initializeToxins(int count, const Map& map);
    void updateToxins(const Map& map, const Position& playerPosition, int detectionRange);
    void drawToxins(Map& map) const;
    
    // Collision detection
    bool checkPlayerCollision(const Position& playerPosition) const;
    
    // Information retriveal
    size_t getToxinCount() const { return toxins.size(); }
    const std::vector<Toxin>& getToxins() const { return toxins; }
};

#endif