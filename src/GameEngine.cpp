#include "../include/GameEngine.h"
#include<iostream>
#include<chrono>
#include<thread>
#include<memory>

SharedGameData g_sharedData; // use 'extern' to fetch the date
std::mutex g_displayMutex;
std::atomic<bool> gameRunning(false);

/*
* Initialise game engine with default setting
* Returns GameEngine object
*/
GameEngine::GameEngine()
    : gameMap(), player(), gameRunning(false), curr_level(1), total_score(0),
    topScore("topscore.txt") // NEW: add score text 
    {
    initializeGame();
}

// this will stop the game as soon as the object is not in scope.
GameEngine::~GameEngine(){
    cleanup();
}

void GameEngine::initializeGame(){
    //initialize the game
    std::cout << "Initializing Pacman Game..." << std::endl;
    gameRunning = true;
    curr_level = 1;
    total_score = 0;
    g_sharedData.Pause.store(false);
    g_sharedData.score.store(0);
    g_sharedData.lives.store(3);
    g_sharedData.currentLevel.store(1);
    g_sharedData.gameState.store(GameState::RUNNING);
    g_sharedData.freezeEffect.deactivate();  
    gameMap.initializeLevel(curr_level);
    player.resetPosition();
    inputHandler.reset(new InputHandler(player, gameMap, gameRunning));
    std::cout << "Game initialized successfully! level: " << curr_level << std::endl;

    //Show top score
    std::cout << "\n Top Score: " << topScore.getScore() 
              << " Name: " << topScore.getName() << " ---\n\n";

    // Generate cookies 
    auto cookies = Cookie::generateRandomCookies(
        curr_level, 
        gameMap.getWidth(), 
        gameMap.getHeight()
    );
    
    // create regular and power-up cookies
    for (const auto& cookie : cookies) {
        gameMap.addCookie(cookie);
    }

    // create freeze cookie by the corners
    auto freezeCookies = Cookie::generateFreezeCookies(
        gameMap.getWidth(), 
        gameMap.getHeight()
    );
    for (const auto& freezeCookie : freezeCookies) {
        gameMap.addCookie(freezeCookie);
    }

    // generate and validate the random walls
    int playerX = player.getX();
    int playerY = player.getY();
    gameMap.generateRandomWalls(curr_level, playerX, playerY);

    // generate toxins
    toxins.initializeToxins(curr_level * 2, gameMap);

    // output initialize result
    addMessage("Player starting position: (" + std::to_string(player.getX()) + ", " + std::to_string(player.getY()) + ")\n", std::chrono::seconds(5));
    addMessage("Walls count: " + std::to_string(gameMap.getWall().size()) + "\n", std::chrono::seconds(5));
    addMessage("Cookies count: " + std::to_string(gameMap.getCookies().size()) + "\n", std::chrono::seconds(5));
    addMessage("Toxins count: " + std::to_string(toxins.getToxinCount()) + "\n", std::chrono::seconds(5));
    gameRunning = true;

}

// Start all game threads
void GameEngine::startGame(){
    std::cout << "Starting game threads..." << std::endl;
    startAllThread();
}

// Stop all game threads
void GameEngine::stopGame(){
    std::cout << "Stopping game..." << std::endl;
    gameRunning = false;
    g_sharedData.gameState = GameState::GAME_OVER;
    stopAllThread();
}

// thread 2: input handler
void GameEngine::inputThread(){
    inputHandler->startInputThread();
    while(gameRunning){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (player.getLives() <= 0){
            gameRunning = false;
            break;
        }
    }
    inputHandler->stopInputThread();
}

// thread 3: game controller
void GameEngine::gameControllerThread(){
    auto lastUpdateTime = std::chrono::steady_clock::now();
    const std::chrono::milliseconds updateInterval(20); 
    bool Pause = false;
    while(gameRunning){
        while(g_sharedData.Pause.load()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = currentTime - lastUpdateTime;
        if (elapsedTime >= updateInterval){
            checkCookieCollection();
            updateMessage();
            renderGame();
            lastUpdateTime = currentTime;
            if (gameMap.getCookies().empty()){
                Pause = true;
                levelComplete();
            }
        }
        if (player.getLives() <= 0){
            gameRunning = false;
            break;
        }
    }
}

// thread 4: toxins
void GameEngine::toxinThread(){
    while (g_sharedData.gameState == GameState::RUNNING){
        while(g_sharedData.Pause.load()){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        if(!g_sharedData.freezeEffect.isActive()){
            toxins.updateToxins(gameMap, player.getPosition(), getToxinDetectionRange());
        }
        // check for collision
        if(toxins.checkPlayerCollision(player.getPosition())){
            player.loseLife(); 
            g_sharedData.lives.store(player.getLives());
            addMessage("You are caught by toxin!", std::chrono::seconds(3));
            g_sharedData.redrawNeeded = true;
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(getToxinSpeed()));
    }
}

// thread 5: freeze thread (exists for only 5 secs)
void GameEngine::freezeThread(){
    g_sharedData.freezeEffect.activate();
    std::chrono::seconds freezeInterval(5);
    if(g_sharedData.freezeEffect.getRemainingSeconds() <= 0){
        g_sharedData.freezeEffect.deactivate();
    }
}

void GameEngine::startAllThread(){
    addMessage("All thread started", std::chrono::seconds(3));
    gameThreads.emplace_back(&GameEngine::inputThread, this);
    gameThreads.emplace_back(&GameEngine::gameControllerThread, this);
    gameThreads.emplace_back(&GameEngine::toxinThread, this);
}

void GameEngine::stopAllThread(){
    // stop all the thread
    for (auto& thread : gameThreads){
        if (thread.joinable()){
            thread.join();
        }
    }
    gameThreads.clear();
}

// game logic

void GameEngine::checkCookieCollection() {
    Position player_pos = player.getPosition();
    Cookie* cookie = gameMap.getCookieAt(player_pos.x, player_pos.y);
    if(cookie){
        int points = cookie->getPoints();
        player.addScore(points);
        total_score += points;
        g_sharedData.score.store(total_score);
        if(!cookie -> isFreezeCookie() && !cookie -> isPowerUpCookie()){
            addMessage("+ 10", std::chrono::seconds(2));
        }
        // deal with power up cookie
        if(cookie -> isPowerUpCookie()){
            player.addScore(50);
            addMessage("+ 50 ", std::chrono::seconds(1));
            total_score += 50;
        }
        if(cookie->isFreezeCookie()){
            std::cout << "toxins are now freeze for 5 sec!!!" << std::endl;
            gameThreads.emplace_back(&GameEngine::freezeThread, this);
        }
        // remove cookie
        gameMap.removeCookie(player_pos.x, player_pos.y);
    }
}

void GameEngine::levelComplete(){
    if(g_sharedData.Pause.load())return;
    g_sharedData.Pause.store(true);
    std::cout << "=== LEVEL " << curr_level << " COMPLETE! ===" << std::endl;
    int curr_total_score = total_score;
    total_score += curr_level * 100; // bonus point
    int levelBonus = curr_level * 100;
    total_score += levelBonus;
    player.addScore(levelBonus);
    curr_level++;

    // check if all level completed
    if(curr_level > 3){
        std::cout << "=== Congratulations ! You beat the game ! ===" << std::endl;
        gameRunning = false;
        return;
    }
    
    std::cout << "Advancing to level " << curr_level << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    gameMap.initializeLevel(curr_level);
    player.resetPosition();
    auto cookies = Cookie::generateRandomCookies(
        curr_level,
        gameMap.getWidth(),
        gameMap.getHeight()
    );
    for(const auto& cookie : cookies){
        gameMap.addCookie(cookie);
    }
    auto freezeCookies = Cookie::generateFreezeCookies(
        gameMap.getWidth(),
        gameMap.getHeight()
    );
    for(const auto& freezeCookie : freezeCookies){
        gameMap.addCookie(freezeCookie);
    }
    int playerX = player.getX();
    int playerY = player.getY();
    gameMap.generateRandomWalls(curr_level, playerX, playerY);
    toxins.initializeToxins(curr_level * 2, gameMap);
    
    g_sharedData.currentLevel.store(curr_level);
    g_sharedData.score.store(total_score);
    g_sharedData.Pause.store(false);
}

int GameEngine::getToxinSpeed() const{
    //fetch the toxin speed for different level (in millisecond here!)
    switch(curr_level.load()){
        case level::EASY: return 600;// you can change the value as you want!
        case level::MODERATE: return 500;// you can change the value as you want!
        case level::HARD: return 400; //you can change the value as you want!
        default: return 500;// you can change the value as you want!
    }
}

int GameEngine::getToxinDetectionRange() const{
    switch(curr_level.load()){
        case level::EASY: return 3;// you can change the value as you want!
        case level::MODERATE: return 6;// you can change the value as you want!
        case level::HARD: return 9;// you can change the value as you want!
        default: return 2;// you can change the value as you want!
    }
}

void GameEngine::setGameState(GameState state){
    g_sharedData.gameState = state;
}

void GameEngine::addMessage(const std::string& content, std::chrono::seconds duration){
    std::lock_guard<std::mutex> lock(g_sharedData.messageMutex);
    Message newMessage;
    newMessage.content = content;
    newMessage.expiration_time = std::chrono::steady_clock::now() + duration;
    g_sharedData.messages.push_back(newMessage);
    g_sharedData.redrawNeeded.store(true);
}

void GameEngine::updateMessage(){
    std::lock_guard<std::mutex> lock(g_sharedData.messageMutex);
    auto it = g_sharedData.messages.begin();
    while(it != g_sharedData.messages.end()){
        if(it -> isExpired()){
            it = g_sharedData.messages.erase(it);
            g_sharedData.redrawNeeded.store(true);
        }else{
            it++;
        }
    }
}

void GameEngine::displayMessage(){
    std::lock_guard<std::mutex> lock(g_sharedData.messageMutex);
    for(const auto& message : g_sharedData.messages){
        if(!message.isExpired()){
            std::cout << message.content << std::endl;
        }
    }
}

void GameEngine::renderGame(){
    system("clear");
    std::cout << "=== PACMAN === Level: " << curr_level << " Score: " << total_score << " Lives: " << player.getLives() << " Top Score: "<< topScore.getScore() << std::endl;
    std::cout << "Controls: WASD to move, Q to quit" << std::endl << std::endl;
    gameMap.display(player, toxins);
    updateMessage();
    if(g_sharedData.freezeEffect.isActive()){
        std::cout << "Toxins frozen for " << g_sharedData.freezeEffect.getRemainingSeconds() << " seconds" << std::endl;
    }
    displayMessage();
    std::cout << std::endl;
}

void GameEngine::cleanup(){
    stopGame();
}