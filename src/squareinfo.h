#ifndef __SQUAREINFO_H__
#define __SQUAREINFO_H__
#include <string>
#include "player.h"
// Whoever is implementing Square plz implement this too.
struct SquareInfo {
    string name;
    bool isProperty;
    bool isOwned;
    size_t cost;
    vector<Player*> visitors;
    Player* owner;
};

#endif
