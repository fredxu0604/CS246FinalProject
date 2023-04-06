#include "property.h"
#include "residence.h"
#include <map>
#include <string>
#include "squareinfo.h"


Residence::Residence(string name): Property{name} {}

int Residence::getVisitFee() const {
    int tuition = tuitionCostMap.at(name).at(numImprove);
    return tuition;
}

SquareInfo Residence::getInfo() const{
    bool isOwned;
    bool isProperty = true;
    bool isAcedemic = false;
    if (owner) {
        isOwned = true;
    } else {
        isOwned = false;
    }
    unsigned int numImprove = 0;
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