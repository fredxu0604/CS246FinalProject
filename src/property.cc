#include "property.h"
#include "gameexception.h"
#include "squareinfo.h"
using namespace std;

Property::Property(string name) : Square{name}, group{propertyGroupMap.at(name)}, isMortgaged{false}, numImprove{0}, unmortgageFeeAddon{0} {
  owner = nullptr;
  isMortgaged = false;
  group = propertyGroupMap.at(name); // using at because map[] only support non
                                     // constant value; here is constant map
}

void Property::setOwner(Player *player) { owner = player; }

void Property::setMortgaged() {
  if (this->getInfo().type == PropertyType::Acedemic) {
    if (this->getInfo().numImprove > 0) {
      throw Disallowed("You must sell all your improvements first");
    }
  } else {
    isMortgaged = true;
    numImprove = -1;
  }
}

void Property::setUnmortgaged() {
  isMortgaged = false;
  numImprove = 0;
  unmortgageFeeAddon = 0;
}

void Property::stepUpUnmortgageFee(size_t amount) {
  unmortgageFeeAddon = amount;
}