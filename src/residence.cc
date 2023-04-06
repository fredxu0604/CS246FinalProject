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