#include "../include/level.h"
#include <iostream>
#include <limits>

/*
* Convert level enum to readable string, which is a description of the level
*/
std::string LevelSelector::level_to_string(level level_value){
    switch(level_value){
        case level::EASY: return "Ez mode";
        case level::MODERATE: return "moderate";
        case level::HARD: return "hard";
        default: return "not chose";
    }
}

//Deprecated
level LevelSelector::select_level(){
    int choice = 0;
    
    while (true){
        std::cout << "=== Choose the level ===" << std::endl;
        std::cout << "Higher level means MORE DANGEROUS toxins!" << std::endl;
        std::cout << "1. EZ mode" << std::endl;
        std::cout << "2. moderate" << std::endl;
        std::cout << "3. Hard" << std::endl;
        std::cout << "plz input the number 1~3: ";
        
        std::cin >> choice;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice >= 1 && choice <= 3){
            return static_cast<level>(choice);
        } else {
            std::cout << "invalid input. plz enter the number 1~3 :(" << std::endl;
        }
    }
}