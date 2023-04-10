#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#include <string>

class Player;

class InsufficientFunds {
  std::string message;
  size_t owedAmount;
  Player *owedTo;

public:
  InsufficientFunds(size_t amount, Player *owedTo, std::string message = "");
  std::string getMessage() const;
  size_t getOwedAmount() const;
  Player *getOwedTo() const;
};

class PropertyNotFound {
  std::string message;

public:
  PropertyNotFound(std::string message = "");
  std::string getMessage() const;
};

class Disallowed {
  std::string message;

public:
  Disallowed(std::string message = "");
  std::string getMessage() const;
};

#endif
