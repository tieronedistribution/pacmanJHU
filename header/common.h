#ifndef COMMON_H
#define COMMON_H

#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>


/**
 * @enum GameState
 * Validate if the current game is running, temporarily paused, game has ended or the player has completed the current level
 */
enum class GameState {
    RUNNING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE
};

/**
 * @enum @class Direction
 * All possible movement directions or players based on given coordinates (x,y)
 * None depicts stationary and no movement
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
};

/**
 * @struct Message
 * Temporary message displayed to user given trigger events
 * Shown for fixed period of time and then disappear from terminal
 */
struct Message{
    std::string content;                            ///< Text that is about to display
    std::chrono::steady_clock::time_point expiration_time; ///< Text Expires

    bool isExpired() const{
        return std::chrono::steady_clock::now() >= expiration_time;
    }
};


/**
 * @struct Position
 * Represents the 2D coordinate system that the player is at
 * Equality comparison and validation used for matching xy-grid map
 */
struct Position {
    int x;
    int y;

    // Construct the position with x-y coordinate
    Position(int x = 0, int y = 0){
        this->x = x;
        this->y = y;
    }

    // Compares whether two positions are equal
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


/**
 * @struct FreezeEffect
 * Manage the freeze effects among all toxins. When activated, all toxins can't move for 5 seconds.
 * Atomic operations are used to maintain thread
 */
struct FreezeEffect {
    std::atomic<bool> active{false};        ///< Thread safey active flag
    std::chrono::steady_clock::time_point startTime; ///< Starts the freeze
    std::chrono::seconds duration{5};  // set to freeze for 5 secs

    // Set active to be true and use current time for freeze start time
    void activate() {
        active = true;
        startTime = std::chrono::steady_clock::now();
    }
    
    // Check if currently active, only return true when it's activated and the duration has passed
    bool isActive() const {
        if (!active) return false;
        
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        return elapsed < duration;
    }
    
    // Manually deactivate, called by freeze thread when duration passed 
    void deactivate() {
        active = false;
    }
    
    // Calculates remaining duration in seconds
    int getRemainingSeconds() const {
        if (!active) return 0;
        
        auto elapsed = std::chrono::steady_clock::now() - startTime;
        auto remaining = duration - std::chrono::duration_cast<std::chrono::seconds>(elapsed);
        return std::max(0, static_cast<int>(remaining.count()));
    }
};

/**
 * @struct SharedGameData
 * Safe Container which all threads can use its data
 * Safe because atomic variables are adopted, syncornize current game state, mechanic, live, score, rewards
 */
struct SharedGameData {
    std::atomic<GameState> gameState{GameState::RUNNING};   ///< Current game state
    std::atomic<int> score{0};  ///< Current scores
    std::atomic<int> lives{3};  ///< Current lives that player has
    std::atomic<int> currentLevel{1};  ///< Current difficulty level
    std::atomic<bool> redrawNeeded{true};   ///< Trigger redraw flag
    std::atomic<bool> Pause{false};     ///< Stop current game state
    
    FreezeEffect freezeEffect;
    std::vector<Message> messages;
    std::mutex messageMutex;
};

// Global shared data defined in map.cpp, accessible through the program
extern SharedGameData g_sharedData;
extern std::mutex g_displayMutex; ///< Protects console display operations and locked before output
extern std::atomic<bool> gameRunning; ///< Set to true when game starts, false when it ends. All threads access this flag to validate if the game is running

#endif