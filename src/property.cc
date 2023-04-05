#include "property.h"
#include "squareinfo.h"
#include <string>
#include <map>
using namespace std;

Property::Property(string name): Square(name) {
    owner = nullptr;
    group = groupMap.at(name);   //using at because map[] only support non constant value; here is constant map
}


void Property::setOwner(Player* player) {
    owner = player;
}

SquareInfo Property::getInfo() const {
    bool isOwned;
    bool isProperty = true;
    if (owner) {
        isOwned = true;
    }
    isOwned = false;
    unsigned int cost = purchaseCostMap.at(name);
    return SquareInfo{
        name,
        isProperty,
        isOwned,
        cost,
        owner
    };
}