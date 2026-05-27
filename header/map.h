#ifndef MAP_H
#define MAP_H

#include "common.h"
#include "wall.h"
#include "cookie.h"
#include "player.h"
#include "toxin.h"
#include <vector>
#include <set>

// Forward declarations so there won't be circular dependencies 
class Player;
class ToxinManager;

/**
 * @class Map
 * Game map consist of grid, walls, and cookies. Maintain 22 x 22 character map. Walls are generated randomly while validating the walls are permissiable.
 * Place cookies of 3 different types. Detect Player movement and player-toxin collision. It also renders map.
 * 
 * Representation:
 * - '#' : Wall
 * - '.' : Normal cookie
 * - '*' : Power-up cookie
 * - '?' : Freeze cookie
 * - '@' : Player
 * - '#' : Toxin
 * - ' ' : Empty space
 */
class Map{
private:
    static const int WIDTH = 22;
    static const int HEIGHT = 22;
    char grid[HEIGHT][WIDTH];

    // Collection for walls and cookie vectors
    std::vector<Wall> walls;
    std::vector<Cookie> cookies;

    bool isPositionOccupied(int x, int y) const; // Check if the position has cookie, called during random wall generation. True if it is occupied
    bool isReachable(int startX, int startY, int targetX, int targetY) const; //BFS pathway verification on whether player can reach one cookie
    bool areAllCookiesReachable(int playerX, int playerY) const; //BFS pathway verification on whether player can reach ALL cookie
     
public:
    Map();
    
    /*
     * Map Initialisation based on current level.
     * Complete map set up i.e. Clear previous walls and cookies, generate edges and wall, place all types of cookie, validate the reachability of the cookies
     */
    void initializeLevel(int level = 1);
    void addWall(const Wall& wall);
    void addCookie(const Cookie& cookie);
    char getCell(int x, int y) const;
    void setCell(int x, int y, char value);
    void clearCell(int x, int y);
    void display(const Player& player, const ToxinManager& toxins) const;

    // movement validation
    bool isValidMove(int x, int y) const;
    // map info fetch
    bool isEdge(int x, int y) const;
    bool isWall(int x, int y) const;
    bool isCookie(int x, int y) const;
    bool isFreezeCookie(int x, int y) const;  // NEW: Check for freeze cookie
    int getWidth() const {return WIDTH;}
    int getHeight() const {return HEIGHT;}
    Cookie* getCookieAt(int x, int y) const;
    void removeCookie(int x, int y);

    const std::vector<Wall>& getWall() const{return walls;}
    const std::vector<Cookie>& getCookies() const{return cookies;}

    //Wall generation methods
    void generateRandomWalls(int level, int playerX, int playerY);
    void clearWalls();  ///< reset walls
};



#endif