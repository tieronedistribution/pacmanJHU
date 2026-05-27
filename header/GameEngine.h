#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "topscore.h"
#include "map.h"
#include "common.h"
#include "level.h"
#include "player.h"
#include "inputhandler.h"
#include "toxin.h"
#include <thread>
#include <vector>
#include <memory>

// Forward declarations so there won't be circular dependencies 
class Player;
class ToxinManager;

/**
 * @class GameEngine
 * @brief The backbone that orchestrates and coordiantes all the threads / components
 * 
 * @details responsible for:
 *          - Initialise each major game component
 *          - Maintain multiple-thraed system  i.e. Input, Game Controller, Toxin, Freeze Cookie
 *          - Keep track of level, top score, current score and running status
 *          - Manage difficulty progression and completeion of the game
 *          - Rendering the game display
 *          - Top Score Tracking
 *          - Freeze Thread: temporary 5 seconds freeze among all toxins
 * 
 * Usage:
 * GameEngine engine;
 * engine.initializeGame();
 * engine.startGame();
 * Game runs in separate threads
 * engine.stopGame();
 * engine.cleanup();
 */ 

class GameEngine{
private:
    std::atomic<int> curr_level;        ///< Game level in current state, thread-safe
    std::atomic<int> total_score;       ///< Total score gained by player, thread-safe
    TopScore topScore;                  ///< Highest score tracking system
    Map gameMap;                        ///< The world of game consist of cookies, walls and edges
    Player player;                      ///< Player character instance
    ToxinManager toxins;                ///< Enemy (toxin) manager
    std::vector<std::thread> gameThreads; ///< Container storing all threads
    std::unique_ptr<InputHandler> inputHandler; ///< User input processor
    std::atomic<bool> gameRunning{false}; ///< Thread-safe game running flag
    
public:
    // Lifecycle management
    GameEngine();
    ~GameEngine();

    /*
     * Game Lifecycle
     * Methods that determines the whole workflow i.e. Initialise the game > Start the game > Stop the game > Clean up the threads
     */
    void initializeGame();
    void startGame();
    void stopGame();
    void cleanup();

    /*
     * Thread Management
     * Methods managing each game thread
     */
    void startAllThread();
    void stopAllThread();

    /*
     * Thread Functions
     * Individual thread executions function
     */
    void inputThread();
    void gameControllerThread();
    void toxinThread();
    void freezeThread();

    
    /*
     * Toxins Parameters
     * Getters for toxin, where the settings are adjusted based on difficulty level
     */
    int getToxinSpeed() const;          ///< Get toxin speed for current level
    int getToxinDetectionRange() const; ///< Get toxin detection radius for current level

    /*
     * Game State Management
     * various methods that validate current game state and update
     */
    void setGameState(GameState state);     ///< Set up or update existing game state
    bool isGameRunning() const {return gameRunning.load();}
    int getCurrentLevel() const{return curr_level.load();}
    int getTotalScore() const{return total_score.load();}

    /*
     * Message System
     * Separate system that display temporary messages on terminal given corresponding triggers
     */
    void addMessage(const std::string& content, std::chrono::seconds duration);
    void updateMessage();
    void displayMessage();
    
    /*
     * Game Logic
     * Hadle level completion and gameplay mechanics
     */
    void checkCookieCollection();       ///< Validate whether a player is at a cookie position, which reward the player and remove the cookie
    void levelComplete();               ///< Called when player collects all cookies. Either progress to next level or complete the game
    void renderGame();                  ///< Render the complete game display which clears screen and render map

    /*
     * Accessors
     */
    TopScore& getTopScore() { return topScore; }    ///< Provide access for TopScore Object during update of top score 
    Map& getMap() {return gameMap;}                 ///< Other threads can access map for query of map state and detect collision
};

#endif