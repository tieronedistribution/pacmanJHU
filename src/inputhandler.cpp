#include "../include/inputhandler.h"
#include <iostream>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/*
* Check if the player presses keyboard
* Returns 1 if key pressed
*/
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}

/*
* Initialise input handler with current game state, player and map
* Returns the InputHandler object
*/
InputHandler::InputHandler(Player& player, Map& map, std::atomic<bool>& gameRunning)
    : playerRef(player), gameMap(map), gameRunning(gameRunning), inputThreadRunning(false) {
    keyPressed['w'] = false;
    keyPressed['s'] = false;
    keyPressed['a'] = false;
    keyPressed['d'] = false;
}

//Destructor that stops all input threads 
InputHandler::~InputHandler() {
    stopInputThread();
}

/*
* Check if key press is new instead of a repeat
* Return true if it is a new press
*/
bool InputHandler::isNewKeyPress(char key){
    std::lock_guard<std::mutex> lock(Keymutex);
    char lowerKey = std::tolower(key);
    if(keyPressed.find(lowerKey) != keyPressed.end()){
        if(!keyPressed[lowerKey]){
            keyPressed[lowerKey] = true;
            return true;
        }
    }
    return false;
}

//Update whether the key state is pressed or released
void InputHandler::updateKeyState(char key, bool pressed){
    std::lock_guard<std::mutex> lock(Keymutex);
    char lowerKey = std::tolower(key);
    if(keyPressed.find(lowerKey) != keyPressed.end()){
        keyPressed[lowerKey] = pressed;
    }
}

void InputHandler::startInputThread() {
    inputThreadRunning = true;
    inputThread = std::thread(&InputHandler::keypress, this);
}

void InputHandler::stopInputThread() {
    inputThreadRunning = false;
    if (inputThread.joinable()) {
        inputThread.join();
    }
}

void InputHandler::keypress() {
    while (inputThreadRunning && gameRunning) {
        processInput();
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
    }
}

// moves player based on WASD, or quits on Q/ESC)
void InputHandler::processInput() {
    if (!kbhit()) {
        updateKeyState('w', false);
        updateKeyState('s', false);
        updateKeyState('a', false);
        updateKeyState('d', false);
        return;
    }
    auto now = std::chrono::steady_clock::now();
    if (now - lastMoveTime < moveCooldown) {
        getchar(); // do not process input
        return;
    }
    char ch = getchar(); 
    int currentX = playerRef.getX();
    int currentY = playerRef.getY();
    bool moved = false;
    
    switch (ch) {
        case 'w': case 'W':  
            if (playerRef.isValidMove(currentX, currentY - 1, gameMap)) {
                playerRef.moveUp();
            }
            break;
            
        case 's': case 'S':  
            if (playerRef.isValidMove(currentX, currentY + 1, gameMap)) {
                playerRef.moveDown();
            }
            break;
            
        case 'a': case 'A':  
            if (playerRef.isValidMove(currentX - 1, currentY, gameMap)) {
                playerRef.moveLeft();
            }
            break;
            
        case 'd': case 'D':  
            if (playerRef.isValidMove(currentX + 1, currentY, gameMap)) {
                playerRef.moveRight();
            }
            break;
            
        case 'q': case 'Q':  
            gameRunning = false;
            std::cout << "Quit game requested\n";
            break;
            
        case 27: // ESC
            gameRunning = false;
            std::cout << "ESC pressed - quitting game\n";
            break;
            
        default:
            break;
    }
}