#include "property.h"
#include "squareinfo.h"
#include "gameexception.h"
#include <string>
#include <map>
using namespace std;

Property::Property(string name): Square(name) {
    owner = nullptr;
    numImprove = 0;
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
        owner,
        numImprove
    };
}

void Property::improve(){
    if (numImprove == 5) {
        throw CannotImprove{"Reaches Improvement Maximum"};
    } else {
        numImprove++;
    }
}

void Property::unimprove(){
    if (numImprove == 0) {
        throw CannotImprove{"No Improvement to be unimproved"};
    } else {
        numImprove--;
    }
}
