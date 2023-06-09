#include "acedemic.h"
#include "gameexception.h"
#include "property.h"
#include <map>

Acedemic::Acedemic(string name)
    : Property{name}, numImprove{0}, group{propertyGroupMap.at(name)} {}

void Acedemic::improve() {
  if (numImprove == 5) {
    throw Disallowed{"Reaches Improvement Maximum"};
  } else {
    numImprove++;
  }
}
void Acedemic::unimprove() {
  if (numImprove == 0) {
    throw Disallowed{"No Improvement to be unimproved"};
  } else {
    numImprove--;
  }
}

size_t Acedemic::getVisitFee() const {
  if (isMortgaged) {
    return 0;
  }
  if (owner) {
    size_t tuition = tuitionCostMap.at(name).at(numImprove);
    return tuition;
  }
  return 0;
}

SquareInfo Acedemic::getInfo() const {
  bool isOwned;
  bool isProperty = true;
  if (owner) {
    isOwned = true;
  } else {
    isOwned = false;
  }
  size_t purchaseCost = purchaseCostMap.at(name);
  size_t improveCost = improvementCostMap.at(group);
  size_t mortgageLoan = purchaseCost / 2;
  size_t unmortgageCost = (purchaseCost * 6 / 10) + unmortgageFeeAddon;
  size_t improveRefund = improveCost / 2;
  return SquareInfo{name,         isProperty,     PropertyType::Acedemic,
                    isOwned,      isMortgaged,    purchaseCost,
                    owner,        numImprove,     improveCost,
                    mortgageLoan, unmortgageCost, improveRefund};
}
