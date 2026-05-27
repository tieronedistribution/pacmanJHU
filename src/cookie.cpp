#include "../include/cookie.h"
#include <random>
#include <vector>

/*
* Initialise the cookie which has its own position, point and cookie type
* Returns the cookie object
*/
Cookie::Cookie(int xPos, int yPos, int pts, CookieType type)
    : Component(xPos, yPos, false), points(pts), cookieType(type) {}

/*
* Get cookie type as a string 
* Returns cookie type's stirng
*/
std::string Cookie::getType() const {
    switch(cookieType) {
        case CookieType::POWERUP:
            return "PowerUpCookie";
        case CookieType::FREEZE:
            return "FreezeCookie";
        case CookieType::NORMAL:
        default:
            return "Cookie";
    }
}

int Cookie::getPoints() const { return points; } ///< interger point rewarded as player collects the cookie

//Check if this is a freeze cookie or powerup
bool Cookie::isPowerUpCookie() const { 
    return cookieType == CookieType::POWERUP; 
}
bool Cookie::isFreezeCookie() const { 
    return cookieType == CookieType::FREEZE; 
}

/*
* Generate normal cookies and powerup cookies depending on the level on random positions
* Returns the vector of a random cookie
*/
std::vector<Cookie> Cookie::generateRandomCookies(
    int level, int width, int height,
    int baseCookies, int cookiesPerLevel,
    int basePowerUps)
{
    std::vector<Cookie> cookies;

    int cookieCount = baseCookies + level * cookiesPerLevel;
    int powerUpCount = basePowerUps + level;

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> distX(1, width - 2);
    std::uniform_int_distribution<int> distY(1, height - 2);

    for (int i = 0; i < cookieCount; i++)
    {
        int x = distX(rng);
        int y = distY(rng);
        cookies.emplace_back(x, y, 10, CookieType::NORMAL);
    }
    
    for (int i = 0; i < powerUpCount; i++)
    {
        int x = distX(rng);
        int y = distY(rng);
        cookies.emplace_back(x,y, 50, CookieType::POWERUP);
    }
    return cookies; 

}

/*
* Generate freeze cookies at the corners
* Returns the vector of 4 freeze cookies at corner of map, while taken into consideration of the edges
*/
std::vector<Cookie> Cookie::generateFreezeCookies(int width, int height) {
    std::vector<Cookie> freezeCookies;
    
    freezeCookies.emplace_back(1, 1, 0, CookieType::FREEZE);
    freezeCookies.emplace_back(width - 2, 1, 0, CookieType::FREEZE);
    freezeCookies.emplace_back(1, height - 2, 0, CookieType::FREEZE);
    freezeCookies.emplace_back(width - 2, height - 2, 0, CookieType::FREEZE);
    
    return freezeCookies;
}


