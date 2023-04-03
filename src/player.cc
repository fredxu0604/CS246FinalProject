#include "player.h"
#include "gameexception.h"
#include "squareinfo.h"
using namespace std;

Player::Player(string name, char avatar, Square *currSquare,
               vector<Property *> ownedProperties, size_t balance)
    : Subject{}, name{name}, avatar{avatar}, balance{balance},
      currSquare{currSquare}, ownedProperties{ownedProperties} {}

void Player::declareBankruptcy() { // assume that player can go bankrupt
  currState = State::Bankrupt;
  notifyObservers();
}

bool Player::hasMonopoly(string targetGroup, const int groupSize) {
  int soFar = 0;

  for (auto property : ownedProperties) {
    if (property->getInfo().group == targetGroup)
      ++soFar;
  }

  return soFar == groupSize;
}

Property *Player::validateImprovement(string propertyName) {
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
  int targetGroupSize = targetProperty->getGroup().size();

  if (!hasMonopoly(targetPropertyInfo.monopolyGroup, targetGroupSize)) {
    throw Disallowed{"You do not own all properties in this monopoly group."};
  }
  return targetProperty;
}

void Player::buyImprovement(std::string propertyName) {
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
  } catch (Disallowed &e) {
    throw;
  }

  currState = State::Improve;
  notifyObservers();
}

void Player::sellImprovement(string propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateImprovement(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  try {
    targetProperty->unImprove();
    balance += targetPropertyInfo.improvementRefund;

  } catch (Disallowed &e) {
    throw;
  }

  currState = State::Improve;
  notifyObservers();
}

void Player::mortgage(string propertyName) {
  Property *targetProperty = nullptr;

  try {
    targetProperty = validateMortgage(propertyName);
  } catch (...) {
    throw;
  }

  SquareInfo targetPropertyInfo = targetProperty->getInfo();

  try {
    targetProperty->mortgage();
    balance += targetPropertyInfo.mortgageLoan;
  } catch (Disallowed &e) {
    throw;
  }

  currState = State::Mortgage;
  notifyObservers();
}

void Player::unmortgage(string propertyName) {
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
    targetProperty->unmortgage();
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
}

void Player::removeProperty(Property *property) {
  vector<Property *>::iterator it = ownedProperties.begin();

  for (; it != ownedProperties.end(); it++) {
    if (*it == property) {
      ownedProperties.erase(it);
      property->setOwner(nullptr);
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
  return PlayerInfo{name, avatar, balance, ownedProperties, currSquare};
}
