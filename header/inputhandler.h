#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "player.h"
#include "map.h"
#include <atomic>
#include <thread>
#include <unordered_map>

/**
 * @class InputHandler
 * Synchonize the keyboard input for responsive game design. input reading using termios and conio
 * Debouncing to prevent unintended repeat input. The thread continueously read input with cooldown time
 */
class InputHandler {
public:

    /*
     * Initialising input handler referecing player, map and gameRunning
     * All references remain valid given the handler lifetime
     */
    InputHandler(Player& player, Map& map, std::atomic<bool>& gameRunning);

    ~InputHandler();
    
    void startInputThread();    ///<  Spawn another separate thread which reads keyboard input. Run until gameRunning is false or stopInputThread() is called
    void stopInputThread();     ///<  Stop input processing and wait for termination
    void keypress();    ///<  low-level function reading one key press at a time, called by processInput()

private:
    Player& playerRef;  ///<  Reference to player
    Map& gameMap;   ///<  Reference to game map
    std::atomic<bool>& gameRunning; ///<  Reference to game running flag
    std::atomic<bool> inputThreadRunning; ///<  Input thread is active 
    std::thread inputThread; ///<  input process thread
    std::unordered_map<char, bool> keyPressed;
    std::mutex Keymutex;
    std::chrono::steady_clock::time_point lastMoveTime; ///<  time stamp for last movement
    std::chrono::milliseconds moveCooldown{200}; ///<  Minimum time between moves set to 200ms

    void processInput();
    bool isNewKeyPress(char key);
    void updateKeyState(char key, bool pressed);
};

#endif 
