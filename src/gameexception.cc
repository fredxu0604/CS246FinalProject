#include "gameexception.h"

InsufficientFunds::InsufficientFunds(std::string message) : message{message} {}

std::string InsufficientFunds::getMessage() const { return message; }
