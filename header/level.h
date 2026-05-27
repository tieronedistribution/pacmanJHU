#ifndef LEVEL_H
#define LEVEL_H

#include<string>

/**
 * @enum level
 * Game difficulty levels
 * 
 * Levels:
 * - EASY (1): Slow toxins, minimum detection range (3 units)
 * - MODERATE (2): Standard toxins, moderate detection range (5 units)
 * - HARD (3): Quicker toxins, longest detection range (7 units)
 * 
 * @note Integer values allow easy arithmetic operations
 */
enum level{
    EASY = 1,
    MODERATE = 2,
    HARD = 3,
};

class LevelSelector{
    private:

    public:
        static std::string level_to_string(level level_value);
        static level select_level();
};
#endif
