#ifndef __SQUAREINFO_H__
#define __SQUAREINFO_H__
#include <string>
class Player;
struct SquareInfo {
    string name;
    bool isProperty;
    PropertyType type;
    bool isOwned;
    bool isMortgaged;
    size_t cost;
    Player* owner;
    size_t numImprove;
    size_t improvementCost;
    size_t mortgageLoan;
    size_t unmortgageCost;
    size_t improvementRefund;
};

#endif
