# ENGG1340/COMP2113 GROUP PROJECT #
## Project title: PACMAN

**Group members:** 
1. Jin Yuanzhuo (UID: 3036291564, GitHub username: xjyxhh)
2. Jin Gioia Jiaying (UID: 3036199895, GitHub username: gjyjin)
3. Dong YuYan (UID:3036291966 , GitHub username: Dong-Yu-Yan)
4. XU Jiexi (UID:3035945431 , GitHub username: tieronedistribution)
5. Cai Yiren(UID:3036445763 , GitHub username: Flicker-one)

[Link to demo video:](https://youtu.be/Bzy1jG2MFSo?si=49M00wtoqdHPa3Wa)  https://youtu.be/Bzy1jG2MFSo?si=49M00wtoqdHPa3Wa

### Application description
#### Game overview description:
- This is a text-based version of the classic arcade game **Pacman**, built in C++ and played entirely in the console.
- The objective is to collect as many points as possible eating the cookies while avoiding the toxins (equivalent of the classic ghosts).
- The player controls the Pacman navigating through a randomly generated map, navigating between walls and toxins.
- For each game the top score is recorded so that the player is able to look back at their performance at the end of each match.
- This design emphasizes real-time movement from the player, collision detection against toxins or walls, scoring.
- Our game introduces novelty twists to the classic arcade game (like the introduction of freeze cookies) while also providing the arcade game in simple text-based format!

### Compilation/execution instructions
To run the game follow the instructions below:
1. Clone or download the repository contaning all the files
```
// To clone the Github repository
git clone https://github.com/Flicker-one/ENGG1340-COMP2113-course-project.git
```
3. Navigate into the project directory
```
cd pacman
```
4. Compile and run the game:

```
make run  
```
That is all the setup needed!

#### Rules for the player: 
- To control Pacman (represented by **@**), use the WASD keys:
  - *W* = up 
  - *A* = left
  - *S* = down
  - *D* = right
  each input is equavalent to one movement from Pacman (eg. *wwaa* = up up left left)
- **Scoring system:**
  - Collect the cookies to increase the score or to freeze the enemies:
    - **.** = simple cookie gives +10 points 
    - *= powerup cookie gives +50 points
    - **?** = freeze cookie stops the toxins' movements for 5 seconds
  - There are 3 levels with increasing difficulty, at completion of every level the player will be rewarded with extra points:
    - Level 1 (Easy) = +100 points
    - Level 2 (Moderate) = +200 points
    - Level 3 (Hard) = +300 points
  - Increasing difficulty involves: increasing speed of toxin movement, increase in number of internal walls.
- The player is confined within the map delineated by **+** and cannot pass over walls inside the map represented by **=**
- The player has a total of 3 lives, if all 3 lives are lost the run will end.
- Toxins run freely in the map. Encountering a toxin will immediately cause -1 lives and end the current round, so make sure to avoid them!
- Once the player collects all the cookies in one level, that level will be considered completed and will be automatically proceed to the next level.
  - At the end of each level the total score will be updated
  - Before the next level starts, there is a 5 seconds stalling perior where the toxins are immobile and the player can familiarize themselves with the new map
- To quit the game simply input *Q* instead of the **WASD** movement commands.

That's all you need to know to play!

### Features
1. **Map rendering:**
  - Generation of wall, cookie, and toxins position is random for each round
  - The map updates dynamically as the player moves and pellets are eaten 
2. **Player movement**
  - Pacman is controlled via keyboard input
  - movement is restricted by walls and the map smoothly redraws after each move 
3. **Toxin AI**
  - Toxins move automatically with simple chase logic where they move towards the player's position
  - Difficulty levels adjust toxins speed
4. **Collision detection**
  - Handles interactions between Pacman and walls, cookies, and toxins
  - Collision with toxins decreases the number of lives left
5. **Lives system**
  - The player has 3 lives throughout their entire run, toxin collision will decrease the total number
  - Loosing all the lives will trigger the Game Over screen
6. **Level progression**
  - Clearing all the cookies (all 3 types) advances the player to the next level
  - Increasing levels mean increasing difficulty difines by: internal wall amount, increased ghost speed
7. **Game-over and winning screen**
  - Display a Game over screen if all lives are lost, or a Win page when all 3 levels are cleared
  - Both screens show the total final score and their historical top score
8. **Memory system for top scores**
  - The game records and stores the highest score by the player across sessions
  - This top score is preserved even after the current run ends, motivating the players to beat their previous record

### Non-standard libraries
No non-standard libraries were used, only standard libraries were utlized.

## Code requirements and snippets
1. **Generation of random events**
- Feature supported = random walls and cookies placement
- Ensures each playthrough is unique, enhancing replayability
- Implementation:
  - ```Cookie::generateRandomCookies()``` creates random cookie positions
  - ```gameMap.generateRandomWalls()``` generates random walls while ensuring playability.

2. **Data structure for storing data**
- Feature supported = game state management and level configuration
- Ensures organized and scalable code structure for managing game elements
- Implementation:
  - ```SharedGameData``` stores score, lives, level, paustate, and messages across threads
  - ```Component``` base class provides a polymorphic foundaiton for walls, cookies, and other entities
  - ```LevelConfig``` struct holds level parameters (size, toxin speed, cookie positions)

3. **Dynamic memory management**
- Feature supported = input handling and modular object management
- Prevents memory leaks and ensures safe and cleanup when objects go out of scope
- Implementation:
  - Smart pointers (```std::unique_ptr```, ```std::shared_ptr```) manage dynamic objects like ```InputHandler```
  - Example from ```GameEngine.cpp```:

```
void GameEngine::initializeGame(){
    ...
    inputHandler.reset(new InputHandler(player, gameMap, gameRunning));
    ...
}

```

4. **File Input/Output**
- Feature supported = persistent top score system
- Allows players to track performance across multiple sessions
- Implementation:
  - ```TopScore``` class loads and saves the highest score to ```topscore.txt```
```

std::cout << "Top Score: " << topScore.getScore()
          << " Name: " << topScore.getName() << std::endl;

```

5. **Program codes in multiple files**
- Feature supported = modular design for maintainability 
- Allows for easier debigging, readability, and future extension
- Implementation: separate source and header files for each module, for example:
  - ```GameEngine.cpp``` and ```GameEngine.h``` are the core game loop and threading
  - ```map.cpp``` and ```map.h``` are the rendering of the map outline with wall generation
  - ```cookie.cpp``` and ```cookie.h``` are the cookie logic
  - ```player.cpp``` and ```player.h``` are for the player movement and scoring 
  - ```toxin.cpp``` and ```toxin.h``` define the toxin behavior

6. **Multiple difficulty levels**
- Feature supported = adjustable challenge for players 
- Players can choose their preferres difficulty, making the game accessible and increasingly challenging 
- Implementation:
  - Difficulty levels (easy, moderate, hard) are defines in ```getToxinSpeed()``` and ```getToxinDetectionRange()```

```

int GameEngine::getToxinSpeed() const{
    //fetch the toxin speed for different level (in millisecond here!)
    switch(curr_level.load()){
        case level::EASY: return 600;
        case level::MODERATE: return 500;
        case level::HARD: return 400; 
        default: return 500;
    }
}

int GameEngine::getToxinDetectionRange() const{
    switch(curr_level.load()){
        case level::EASY: return 3;
        case level::MODERATE: return 6;
        case level::HARD: return 9;
        default: return 2;
    }
}

```

 
### Feature description and implementation ([GameEngine.cpp](https://github.com/Flicker-one/ENGG1340-COMP2113-course-project/blob/main/pacman(test)/src/GameEngine.cpp))

1. **Global shared data and synchronization**
- ``` SharedGameData g_sharedData ``` is a global structure that scores shared state (score, lives, level) so multiple threads can safely access and update it
- ``` std::mutex g_displayMutex ``` ensures that only one thread writes to the display at a time, preventing garbled output.
- ``` std::atomic<bool> gameRunning(false)``` is a thread-safe flag that indicates whether the game is currently running. Mutiple threads check this to know when to stop.

```

SharedGameData g_sharedData; 
std::mutex g_displayMutex;
std::atomic<bool> gameRunning(false);

```

2. **GameEngine constructor & deconstructor**
- Contructor = initializes the game map, player, score, level, and loads the top score from a file (```topscore.txt```). Then calls ```initializeGame()```to set up the first level
- Deconstructor = calls ```cleanup()``` to stop threads and release resources when the object goes out of scope

```

GameEngine::GameEngine()
    : gameMap(), player(), gameRunning(false), curr_level(1), total_score(0),
    topScore("topscore.txt")  
    {
    initializeGame();
}

GameEngine::~GameEngine(){
    cleanup();
}

```

3. **Game initialization**
- Sets the initial game state: score = 0, lives = 3, level = 1
- Initializes the map and player position
- Creates ```InputHandler``` to process keyboard inputs
- Displays the top score from memory
- Generates cookies and random walls
- Initializes toxins

```

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
    ...
}

```

4. **Cookie collection**
- Checks if Pacman is standing on a cookie
- Adds points to the score
- Handles special cookies:
  - Powerup cookies = adds bonus points
  - Freeze cookies = freezes toxins for 5 seconds
- Removes cookies from the map once eaten
  
```

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

```

5. **Level progression**
- Awards bonus points when a level is cleared
- Advances to the next level, increasing diffuculty
- End the game after level 3 with a victory message

```

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
    ...
}

```

6. **Toxin behavior**
- ``` getToxinSpeed() ``` defines the moving speed of the toxins, which increases as difficulty increases
- ``` getToxinDetectionRange()``` defines the distance at which the toxins can sense the player on the map (eg. the bigger the range the harder for the player to avoid them), which like the speed, also increases as difficulty increases.

```

int GameEngine::getToxinSpeed() const{
    //fetch the toxin speed for different level (in millisecond here!)
    switch(curr_level.load()){
        case level::EASY: return 600;
        case level::MODERATE: return 500;
        case level::HARD: return 400; 
        default: return 500;
    }
}

int GameEngine::getToxinDetectionRange() const{
    switch(curr_level.load()){
        case level::EASY: return 3;
        case level::MODERATE: return 6;
        case level::HARD: return 9;
        default: return 2;
    }
}

```

7. **Rendering**
- Clears the screen and redraws the maze
- Displays current level, score, lives, and top score, and game instructions 

```

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

```

8. **Top score memory system**
- Loads the highest score from a file stored as ``` topscore.txt``` 
- Top score udates when player achieves a new high score
- Ensures persistence across sessions

```
// Fetch the top score from the topscore.txt file
GameEngine::GameEngine()
    : gameMap(), player(), gameRunning(false), curr_level(1), total_score(0),
    topScore("topscore.txt")  
    {
    initializeGame();
}

// Command to display the top score
void GameEngine::initializeGame(){
    ...
    std::cout << "\n Top Score: " << topScore.getScore() 
              << " Name: " << topScore.getName() << " ---\n\n";
    ...
}

```
