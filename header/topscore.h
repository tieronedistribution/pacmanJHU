#ifndef TOPSCORE_H
#define TOPSCORE_H

#include <string>

/**
 * @class TopScore
 * File I/O for tracking the top score
 * Loads highest score from txt, display it to user. 
 * When game is finished, the current score is comprared with the record, update if current score is higher.
 * 
 * txt format:
 * int Score
 * string Player Name
 */
class TopScore {
private:
    int score;
    std::string name;
    std::string filename;

public:
    // initialise with specified file name and it doesn't load automatically
    TopScore(const std::string& file = "topscore.txt");
    
    void load();
    void save();
    
    int getScore() const { return score; }
    std::string getName() const { return name; }
    
    bool isBeaten(int newScore) const { return newScore > score; }  ///< return true if player scores higher than the record
    void update(int newScore, const std::string& newName = "Player"); ///< update the highest score to new player score
};

#endif
