#include "property.h"
#include "residence.h"
#include <map>
#include <string>
#include "squareinfo.h"
#include "player.h"
#include "dice.h"
#include "gameexception.h"

Residence::Residence(string name): Property{name} {}

void Residence::improve() {
    throw Disallowed{"Improvement on residence is not allowed"};
}

void Residence::unimprove() {
    throw Disallowed{"Improvement on residence is not allowed"};
}

size_t Residence::getVisitFee() const {
    // check whether it's owned by someone
    if (! owner) {
        return 0;
    } else {
        //check how many gyms are owned by the player
        int residenceOwned = 0;
        for (auto r : residenceGymMap.at(PropertyType::Residence)) {
            if (owner->owns(r)) {
                residenceOwned++;
            }
        }
        return residenceRentMap.at(residenceOwned);
    }
}


SquareInfo Residence::getInfo() const {
    bool isOwned;
    bool isProperty = true;
    if (owner) {
        isOwned = true;
    } else {
        isOwned = false;
    }
    size_t numImprove = 0;  // set to default 0
    size_t purchaseCost = purchaseCostMap.at(name);
    size_t improveCost = 0;
    size_t mortgageLoan = purchaseCost / 2;
    size_t unmortgageCost = purchaseCost * 6 / 10;
    size_t improveRefund = 0;
    return SquareInfo{
        name,
        isProperty,
        PropertyType::Residence,
        isOwned,
        isMortgaged,
        purchaseCost,
        owner,
        numImprove,
        improveCost,
        mortgageLoan,
        unmortgageCost,
        improveRefund
    };
}