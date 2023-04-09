#include "gym.h"
#include "player.h"
#include <string>
#include "gameexception.h"

Gym::Gym(string name): Property{name} {}
void Gym::improve() {
    throw Disallowed{"Improvement on residence is not allowed"};
}

void Gym::unimprove() {
    throw Disallowed{"Improvement on residence is not allowed"};
}
size_t Gym::getVisitFee() const {
    if (isMortgaged) {
        return 0;
    }
    // check whether it's owned by someone
    if (! owner) {
        return 0;
    } else {
        //check how many gyms are owned by the player
        int gymOwned = 0;
        for (auto g : residenceGymMap.at(PropertyType::Gym)) {
            if (owner->owns(g)) {
                gymOwned++;
            }
        }
        if (gymOwned == 1) {
            size_t fee = (size_t)(owner->roll() + owner->roll()) * 4;
        } else {
            size_t fee = (size_t)(owner->roll() + owner->roll()) * 10;
        }
    }
}


SquareInfo Gym::getInfo() const {
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
        PropertyType::Gym,
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