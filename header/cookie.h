#ifndef COOKIE_H
#define COOKIE_H

#include "component.h"
#include <vector>
#include <string>


/**
 * @enum @class CookieType
 * Enumeration of 3 cookie types:
 * - NORMAL: normal cookie reward 10 points to the player
 * - POWERUP: Powerup cookie reward 50 points to the player
 * - FREEZE: Freeze cokkie where player can't move for 5 seconds
 */
enum class CookieType {
    NORMAL, 
    POWERUP,
    FREEZE
};

/**
 * @class Cookie
 * cookie component, has its own xy-coordinate. Player receives points or special effect by moving to them and get the cookie.
 */
class Cookie : public Component {
    int points;
    CookieType cookieType;

public:
    // cookie is constructed with xy position, point and its own cookie type
    Cookie(int xPos, int yPos, int pts = 10, CookieType type = CookieType::NORMAL);

    std::string getType() const override;   ///< type identifier which override gettype in component and return cookies
    int getPoints() const;  ///< getter returning the point value of the cookie

    bool isPowerUpCookie() const;   ///<  Validate whether it's powerup cookie and trigger bonus score 
    bool isFreezeCookie() const;    ///<  Validate whether it's freeze cookie and trigger freeze effects on all toxins
    CookieType getCookieType() const { return cookieType; }

    /*
     * Randomly generate normal and powerup cookies
     * Return vector with random cookies placed on map, normal cookies increase proportionally given the level
     * Positions are random but check for avoiding edges and corners 
     */
    static std::vector<Cookie> generateRandomCookies(
        int level, int width, int height,
        int baseCookies = 20, int cookiesPerLevel = 10,
        int basePowerUps = 2);
    /*
     * Randomly generate freeze cookies
     * Return vector with with exactly 4 cookies, one at each corner of the map
     */
    static std::vector<Cookie> generateFreezeCookies(
        int width, int height);
};


#endif

