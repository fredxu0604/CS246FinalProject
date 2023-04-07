#include "player.h"
#include "gameexception.h"
#include "property.h"
#include "squareinfo.h"
using namespace std;

bool Player::owns(const string &propertyName) {
  for (auto property : ownedProperties) {
    if (property->getInfo().name == propertyName)
      return true;
  }
  return false;
}

Player::Player(const string &name, char avatar, Square *currSquare,
               vector<Property *> ownedProperties, size_t balance)
    : Subject{}, name{name}, avatar{avatar}, balance{balance}, assets{0},
      currSquare{currSquare}, ownedProperties{ownedProperties} {

  for (auto property : ownedProperties) {
    SquareInfo pInfo = property->getInfo();
    assets += pInfo.cost + (pInfo.improvementCost * pInfo.numImprove);
  }
}

void Player::declareBankruptcy() { // assume that player can go bankrupt
  currState = State::Bankrupt;
  notifyObservers();
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
    throw InsufficientFunds(
        "You do not have enough money to purchase this improvement.");
  }

  try {
    targetProperty->improve();
    makePayment(targetPropertyInfo.improvementCost);
    assets += targetPropertyInfo.improvementCost;
  } catch (Disallowed &e) {
    throw;
  }

  currState = State::Improve;
  notifyObservers();
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

  currState = State::Improve;
  notifyObservers();
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

  currState = State::Mortgage;
  notifyObservers();
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

  currState = State::Unmortgage;
  notifyObservers();
}

bool Player::makePayment(size_t amount, bool notify) {
  if (amount > balance)
    return false;

  balance -= amount;
  if (notify)
    notifyObservers();
  return true;
}

void Player::moveTo(Square *newLocation) {
  currSquare = newLocation;
  currState = State::Move;
  notifyObservers();
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
    throw InsufficientFunds("not enough money to buy this property");

  makePayment(propertyInfo.cost);
  addProperty(property);

  currState = State::BuyProperty;
  notifyObservers();
}

PlayerInfo Player::getInfo() const {
  return PlayerInfo{
      name, avatar, balance, balance + assets, ownedProperties, currSquare};
}
