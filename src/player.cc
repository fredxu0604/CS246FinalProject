#include "player.h"
#include "gameexception.h"
#include "squareinfo.h"
#include <random>
using namespace std;

Player::Player(string name, char avatar, Square *currSquare,
               vector<Square *> ownedProperties, size_t balance)
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

Square *Player::validateImprovement(string propertyName) {
  Square *targetProperty = nullptr;
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
  Square *targetProperty = nullptr;

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
  Square *targetProperty = nullptr;

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
  Square *targetProperty = nullptr;

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
  Square *targetProperty = nullptr;

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

bool Player::makePayment(size_t amount) {
  if (amount > balance)
    return false;

  balance -= amount;
  return true;
}

void Player::moveTo(Square *newLocation) { currSquare = newLocation; }
