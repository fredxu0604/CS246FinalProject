#include "property.h"
#include "acedemic.h"
#include "gameexception.h"
#include <map>
#include <string>


Acedemic::Acedemic(string name): Property{name} {
    numImprove = 0;
}

void Acedemic::improve(){
    if (numImprove == 5) {
        throw CannotImprove{"Reaches Improvement Maximum"};
    } else {
        numImprove++;
    }
}

void Acedemic::unimprove(){
    if (numImprove == 0) {
        throw CannotImprove{"No Improvement to be unimproved"};
    } else {
        numImprove--;
    }
}

int Acedemic::getVisitFee() const {
    int tuition = tuitionCostMap.at(name).at(numImprove);
    return tuition;
}

SquareInfo Acedemic::getInfo() const {
    bool isOwned;
    bool isProperty = true;
    bool isAcedemic = true;
    if (owner) {
        isOwned = true;
    } else {
        isOwned = false;
    }

    unsigned int purchaseCost = purchaseCostMap.at(name);
    return SquareInfo{
        name,
        isProperty,
        isAcedemic,
        isOwned,
        purchaseCost,
        owner,
        numImprove
    };
}