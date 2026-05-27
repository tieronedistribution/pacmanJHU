#include "../include/topscore.h"
#include <fstream>
#include <iostream>

TopScore::TopScore(const std::string& file) 
    : score(0), name("No one"), filename(file) {
    load();
}

void TopScore::load() {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        save();
        return;
    }
    
    // Read score and name
    file >> score;
    //skip if new line
    file.ignore();
    std::getline(file, name);
    
    if (name.empty())
     name = "No one";
    
    file.close();
}

void TopScore::save() {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "ERROR";
        return;
    }
    
    file << score << "\n";
    file << name << "\n";
    
    file.close();
}

void TopScore::update(int newScore, const std::string& newName) {
    if (newScore > score) {
        std::cout << "\n*** NEW TOP SCORE: " << newScore << " ***\n";
        std::cout << "Previous: " << score << " by " << name << "\n\n";
        
        score = newScore;
        name = newName;
        save();
    }
}
