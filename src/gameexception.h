#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__
#include <string>

class InsufficientFunds {
  std::string message;

public:
  InsufficientFunds(std::string message = "");
  std::string getMessage() const;
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

class CannotImprove {
  std::string message;
public:
  CannotImprove(std::string message = "");
  std::string getMessage() const;
};

#endif
