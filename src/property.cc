#include "property.h"
#include "gameexception.h"
#include "squareinfo.h"
using namespace std;

Property::Property(string name) : Square{name}, isMortgaged{false}, numImprove{0}, unmortgageFeeAddon{0} {
  owner = nullptr;
  isMortgaged = false;
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