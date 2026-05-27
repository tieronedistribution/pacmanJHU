#include "../include/GameEngine.h"
#include <iostream>
#include <chrono>

int main(){
    using std::cout;
    using std::endl;
    cout << "=== Pacman ===" << endl;
    // 60fps
    int FRAMERATE = 60;
    
    try {
        GameEngine gameEngine;

        // initialize the game
        gameEngine.startGame();
        
        // main loop
        while(gameEngine.isGameRunning()){
            // check if game failed
            if (g_sharedData.lives.load() <= 0){
                gameEngine.stopGame();
                gameEngine.setGameState(GameState::GAME_OVER);
                break;
            }
            if(!gameEngine.isGameRunning()){
                gameEngine.stopGame();
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FRAMERATE));
        }
        
        // processes when game ends
        cout << "\n\n\n=== GAME END ===" << endl;
        cout << "Final Score: " << g_sharedData.score.load() << endl;
        
        // Update top score
        gameEngine.getTopScore().update(g_sharedData.score.load(), "Player");
        cout << "\nTop Score: " << gameEngine.getTopScore().getScore() << " by " << gameEngine.getTopScore().getName() << endl; 

    } catch (const std::exception& e){ // exception handles
        std::cerr << "Game crashed with error: " << e.what() << endl;
        return 1;
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}