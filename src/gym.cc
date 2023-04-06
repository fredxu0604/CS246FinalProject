#include "gym.h"



int Gym::getVisitFee() const {

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