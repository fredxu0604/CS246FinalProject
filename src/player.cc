#include "player.h"
#include "gameexception.h"
#include "property.h"
#include "squareinfo.h"
#include <cstdlib>
#include <ctime>
using namespace std;

bool Player::owns(const string &propertyName) const {
  for (auto property : ownedProperties) {
    if (property->getInfo().name == propertyName)
      return true;
  }
  return false;
}

bool Player::owns(Property *p) const {
  for (auto property : ownedProperties) {
    if (property == p)
      return true;
  }
  return false;
}

Player::Player(const string &name, char avatar, Square *currSquare,
               vector<Property *> ownedProperties, size_t balance, 
               bool isBankrupt, bool stuck, int timsCups)
    : name{name}, avatar{avatar}, balance{balance}, assets{0},
      currSquare{currSquare}, ownedProperties{ownedProperties}, isBankrupt{isBankrupt}, 
      isStuck{stuck}, timsCups{timsCups} {

  for (auto property : ownedProperties) {
    SquareInfo pInfo = property->getInfo();
    assets += pInfo.cost + (pInfo.improvementCost * pInfo.numImprove);
  }
}

void Player::declareBankruptcy() { // assume that player can go bankrupt
  isBankrupt = true;
}

bool Player::hasMonopoly(const string &propertyName) {
  int soFar = 0;

  vector<string> group;

  for (auto &p : groupPropertyMap) {
    bool found = false;
    for (auto &bldg : p.second) {

      if (bldg == propertyName) {
        group = p.second;
        found = true;
        break;
      }
    }
    if (found)
      break;
  }

  int groupSize = group.size();

  for (auto &candidateName : group) {
    if (owns(candidateName))
      ++soFar;
  }

  return soFar == groupSize;
}

Property *Player::validateImprovement(const string &propertyName) {
  Property *targetProperty = nullptr;
  for (auto property : ownedProperties) {
    if (property->getInfo().name == propertyName) {
      targetProperty = property;
      break;
    }
  }

  if (targetProperty == nullptr) {
    throw PropertyNotFound{"You do not own any property named '" +
                           propertyName + "'."};
  }

  // property has been found and is now pointed to.

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  if (!hasMonopoly(targetPropertyInfo.name)) {
    throw Disallowed{"You do not own all properties in this monopoly group."};
  }
  return targetProperty;
}

void Player::buyImprovement(const string &propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateImprovement(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  if (targetPropertyInfo.improvementCost > balance) {
    throw InsufficientFunds{
        "You do not have enough money to purchase this improvement."};
  }

  try {
    targetProperty->improve();
    makePayment(targetPropertyInfo.improvementCost);
    assets += targetPropertyInfo.improvementCost;
  } catch (Disallowed &e) {
    throw;
  }
}

void Player::sellImprovement(const string &propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateImprovement(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  try {
    targetProperty->unimprove();
    balance += targetPropertyInfo.improvementRefund;
    assets -= targetPropertyInfo.improvementCost;

  } catch (Disallowed &e) {
    throw;
  }
}

void Player::mortgage(const string &propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateMortgage(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  try {
    targetProperty->setMortgaged();
    balance += targetPropertyInfo.mortgageLoan;
  } catch (Disallowed &e) {
    throw;
  }
}

void Player::unmortgage(const string &propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateMortgage(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  if (targetPropertyInfo.unmortgageCost > balance)
    throw InsufficientFunds{"Not enough money to unmortgage this property."};

  try {
    targetProperty->setUnmortgaged();
    makePayment(targetPropertyInfo.unmortgageCost);
  } catch (Disallowed &e) {
    throw;
  }
}

bool Player::makePayment(size_t amount) {
  if (amount > balance)
    return false;

  balance -= amount;
  return true;
}

void Player::moveTo(Square *newLocation) {
  if (isStuck)
    throw Disallowed{"Cannot move a stuck player!"};
  currSquare->removeVisitor(this);
  currSquare = newLocation;
  newLocation->addVisitor(this);
}

void Player::addFunds(size_t amount) { balance += amount; }

void Player::addProperty(Property *property) {
  ownedProperties.emplace_back(property);
  property->setOwner(this);
  SquareInfo pInfo = property->getInfo();
  assets += pInfo.cost + (pInfo.improvementCost * pInfo.numImprove);
}

void Player::removeProperty(Property *property) {
  vector<Property *>::iterator it = ownedProperties.begin();

  for (; it != ownedProperties.end(); it++) {
    if (*it == property) {
      ownedProperties.erase(it);
      property->setOwner(nullptr);
      SquareInfo pInfo = property->getInfo();
      assets -= pInfo.cost + (pInfo.improvementCost * pInfo.numImprove);
      break;
    }
  }
}

void Player::buyProperty(Property *property) {
  SquareInfo propertyInfo = property->getInfo();

  if (propertyInfo.cost > balance)
    throw InsufficientFunds{"not enough money to buy this property"};

  makePayment(propertyInfo.cost);
  addProperty(property);
}

PlayerInfo Player::getInfo() const {
  return PlayerInfo{
      name, avatar,balance, balance + assets, ownedProperties, currSquare, isBankrupt, isStuck};
}

void Player::lockPosition() {
  isStuck = true;
}

void Player::unlockPosition() {
  isStuck = false;
}

void Player::trade(Player *o, Property *give, Property *receive) {
  if (!owns(give) || !o->owns(receive))
    throw Disallowed{"Both players must own the properties they're trading!"};

  removeProperty(give);
  addProperty(receive);
  o->removeProperty(receive);
  o->addProperty(give);
}

void Player::trade(Player *o, size_t give, Property *receive) {
  if (!o->owns(receive))
    throw Disallowed{o->name + " Does not own the property they're trading!"};

  if (!makePayment(give))
    throw Disallowed{"you have insufficient funds to make this trade."};

  o->addFunds(give);
  o->removeProperty(receive);
  addProperty(receive);

}

void Player::trade(Player *o, Property *give, size_t receive) {
  if (!owns(give))
    throw Disallowed{"You do not own the property you are trying to trade!"};

  if (!o->makePayment(receive))
    throw InsufficientFunds{o->name + " has insufficient funds to make this trade."};

  addFunds(receive);
  removeProperty(give);
  o->addProperty(give);
}

void Player::addTimsCup() {
  ++timsCups;
}

void Player::useTimsCup() {
  if (timsCups == 0)
    throw Disallowed("You don't have any Tims Cups.");

  if (!isStuck)
    throw Disallowed{"Nothing to use Tims Cup on."};

  unlockPosition();
  --timsCups;
}

int Player::findIndex(std::vector<Square *> &squares) {
  int index = -1;
  for (int i = 0; i < squares.size(); i++) {
    if (squares[i] == currSquare) {
        index = i;
        break;
    }
  }
  return index;
}

void Player::winTimsCup() {
  // Seed the random number generator with the current time
  srand(time(0));

  // Generate a random number between 1 and 100
  int rand_num = rand() % 100 + 1;
  if (rand_num == 1) {
    this->addTimsCup();
  }
}
