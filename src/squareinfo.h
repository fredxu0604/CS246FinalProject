#ifndef __SQUAREINFO_H__
#define __SQUAREINFO_H__
#include <string>
#include "player.h"
struct SquareInfo {
    string name;
    bool isProperty;
    bool isOwned;
    size_t cost;
    Player* owner;
    int numImprove;
};

#endif
