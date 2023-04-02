#ifndef __PLAYERINFO_H__
#define __PLAYERINFO_H__
#include <string>
#include <vector>
#include "square.h"

struct PlayerInfo {
    std::string name;
    char avatar;
    size_t balance;
    std::vector<Square> ownedProperties;
    Square &currSquare;
};

#endif