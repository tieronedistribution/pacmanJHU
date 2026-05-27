#include "../include/map.h"
#include "../include/player.h"
#include "../include/toxin.h"
#include <iostream>
#include <queue>
#include <random>
#include <algorithm>
#include <set>
#include <utility>

// map components:
// edge : +
// wall : =
// cookies : .
// toxins : #
// player : @
// freeze cookie : ? 
// Powerup cookie : *


Map::Map(){
    initializeLevel(1); 
}

void Map::initializeLevel(int level){ 
    // initialise all cells (oignally only edges)
    walls.clear();
    cookies.clear();
    for (int y = 0; y < HEIGHT; y++) {  
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH-1 || y == 0 || y == HEIGHT-1) {
                grid[y][x] = '+'; // edge
            }
            //initialise interior to spaces
            else{
                grid[y][x] = ' ';
            }
        }
    }
}

void Map::addWall(const Wall& wall){
    int x = wall.getX();
    int y = wall.getY();
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        grid[y][x] = '=';
        walls.push_back(wall);
    }
}

void Map::addCookie(const Cookie& cookie){
    int x = cookie.getX();
    int y = cookie.getY();
    if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        if(grid[y][x] == ' '){
            if(cookie.isFreezeCookie()){
                grid[y][x] = '?';
            } else if (cookie.isPowerUpCookie()){
                grid[y][x] = '*';
            } else {
                grid[y][x] = '.';
            }
            cookies.push_back(cookie);
        }
    }
}

bool Map::isValidMove(int x, int y) const{
    // check if the movement is valid
    return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1 && !isWall(x, y);
}

bool Map::isEdge(int x, int y) const{
    // check if the component at the position is a edge
    return getCell(x, y) == '+';
}

bool Map::isWall(int x, int y) const{
    // check if the component at the position is a wall
    return getCell(x, y) == '=';
}

bool Map::isCookie(int x, int y) const{
    // check if the component at the position is a cookie
    char cell = getCell(x, y);
    return cell == '.' || cell == '*' || cell == '?'; //all cookie types
}

// check for freeze cookie
bool Map::isFreezeCookie(int x, int y) const{
    return getCell(x,y) == '?';
}

Cookie* Map::getCookieAt(int x, int y) const{
    for(auto& cookie : cookies){
        if(cookie.getX() == x && cookie.getY() == y){
            return const_cast<Cookie*>(&cookie);
        }
    }
    return nullptr;
}

void Map::removeCookie(int x, int y){
    if(isCookie(x, y)){
        clearCell(x, y);
        // remove cookie
        Position target = Position(x, y);
        auto new_end = std::remove_if(cookies.begin(), cookies.end(),
            [target](const Cookie& p) {
                return (p.getX() == target.x && p.getY() == target.y);
            });
    
        cookies.erase(new_end, cookies.end());
    }
}

char Map::getCell(int x, int y) const{
    // return the component at that position
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        return grid[y][x];
    }
    return ' '; // if out of range then return space
}
 
void Map::setCell(int x, int y, char value){
    // fulfill the map with the 'value' passed in
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
        grid[y][x] = value;
    }
}

void Map::clearCell(int x, int y){
    // fulfill the map with " "
    setCell(x, y, ' ');
}

//IMPLEMENTATING RANDOM WALL GENERATION

//check if occupied
bool Map::isPositionOccupied(int x, int y) const{
    if (x <= 0 || x >= WIDTH-1 || y <= 0 || y >= HEIGHT-1) {
        return true;  
    }
    char cell = getCell(x, y);
    return cell != ' '; 
}
//BFS Pathfind, if target is reachable from the start  
bool Map::isReachable(int startX, int startY, int targetX, int targetY) const {
    if (startX <= 0 || startX >= WIDTH-1 || startY <= 0 || startY >= HEIGHT-1) return false;
    if (targetX <= 0 || targetX >= WIDTH-1 || targetY <= 0 || targetY >= HEIGHT-1) return false;
    
    if (isWall(targetX, targetY)) return false;
    
    std::queue<std::pair<int, int>> q;
    std::set<std::pair<int, int>> visited;
    
    q.push({startX, startY});
    visited.insert({startX, startY});
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        int x = current.first;
        int y = current.second;
        q.pop();
        
        if (x == targetX && y == targetY) {
            return true;
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if (nx > 0 && nx < WIDTH-1 && ny > 0 && ny < HEIGHT-1 &&
                !isWall(nx, ny) && visited.find(std::make_pair(nx, ny)) == visited.end()) {
                
                visited.insert(std::make_pair(nx, ny));
                q.push(std::make_pair(nx, ny));
            }
        }
    }
    
    return false; 
}

//Validate if all cookies are reachable from player xy position
bool Map::areAllCookiesReachable(int playerX, int playerY) const {
    for (const auto& cookie : cookies) {
        if (!isReachable(playerX, playerY, cookie.getX(), cookie.getY())) {
            return false;  // Found unreachable cookie
        }
    }
    return true;  // All cookies reachable
}

// remove all the walls
void Map::clearWalls(){
    for (int y = 1; y < HEIGHT - 1; y++){  
        for (int x = 1; x < WIDTH - 1; x++){
            if (grid[y][x] == '='){
                grid[y][x] = ' ';
            }
        }
    }
}

//different wall each game, reachability check, more walls at higher levels, valid ate cookies reachability
void Map::generateRandomWalls(int level, int playerX, int playerY) {
    clearWalls();
    //higher difficulty, more walls
    int baseWalls = 10;
    int wallsPerLevel = 5;
    int targetWallCount = baseWalls + (level - 1) * wallsPerLevel;

    //avoid too crowded with walls
    int maxWalls = (WIDTH - 2) * (HEIGHT - 2) / 2;
    targetWallCount = std::min(targetWallCount, maxWalls);

    //random walls with time-based seed
    std::random_device rd;
    std::mt19937 rng(rd() ^ (std::chrono::steady_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distX(2, WIDTH - 3);
    std::uniform_int_distribution<int> distY(2, HEIGHT - 3);

    //no infinite loops
    int attempts = 0;
    int maxAttempts = targetWallCount * 10; 
    
    while (walls.size() < static_cast<size_t>(targetWallCount) && attempts < maxAttempts) {
        attempts++;
        
        int x = distX(rng);
        int y = distY(rng);
        
        // Skip if position is occupied or too close to player
        if (isPositionOccupied(x, y)) continue;
        if (std::abs(x - playerX) <= 1 && std::abs(y - playerY) <= 1) continue;
        
        // Place wall as a trial
        Wall testWall(x, y);
        addWall(testWall);
        
        // Validate reachability
        if (!areAllCookiesReachable(playerX, playerY)) {
            grid[y][x] = ' ';
            walls.pop_back();
            continue;
        }
    }
}

void Map::display(const Player& player, const ToxinManager& toxins) const{
    //display the map
    using std::cout;
    std::lock_guard<std::mutex> lock(g_displayMutex);
    char tempGrid[HEIGHT][WIDTH];
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            tempGrid[y][x] = grid[y][x];
        }
    }

    // show walls
    for(const auto& wall: walls){
        int x = wall.getX();
        int y = wall.getY();
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
            tempGrid[y][x] = '=';
        }
    }

    // show cookie
    for(const auto& cookie : cookies){
        int x = cookie.getX();
        int y = cookie.getY();
        if(x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT){
            if(cookie.isFreezeCookie()){
                tempGrid[y][x] = '?';
            } else if(cookie.isPowerUpCookie()){
                tempGrid[y][x] = '*';
            } else{
                tempGrid[y][x] = '.';
            }
        }
    }
    
    // show player
    int playerX = player.getX();
    int playerY = player.getY();
    if(playerX >= 0 && playerX < WIDTH && playerY >= 0 && playerY < HEIGHT){
        tempGrid[playerY][playerX] = '@';
    }
    
    // show toxins
    for(const auto& toxin : toxins.getToxins()){
        if(toxin.isActive()){
            Position pos = toxin.getPosition();
            if(pos.x >= 0 && pos.x < WIDTH && pos.y >= 0 && pos.y < HEIGHT){
                // toxin can cover cookies!
                if (tempGrid[pos.y][pos.x] == ' ' || 
                    tempGrid[pos.y][pos.x] == '.' || 
                    tempGrid[pos.y][pos.x] == '*' || 
                    tempGrid[pos.y][pos.x] == '?') {
                    tempGrid[pos.y][pos.x] = '#';
                }
            }
        }
    }
    
    for (int y = 0; y < HEIGHT; y++){
        for (int x = 0; x < WIDTH; x++){
            cout << tempGrid[y][x] << ' ';
        }
        cout << '\n';
    }
    cout << std::endl;
}