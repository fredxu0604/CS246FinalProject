#include "gameexception.h"

InsufficientFunds::InsufficientFunds(std::string message) : message{message} {}

std::string InsufficientFunds::getMessage() const { return message; }

PropertyNotFound::PropertyNotFound(std::string message) : message{message} {}

std::string PropertyNotFound::getMessage() const { return message; }

Disallowed::Disallowed(std::string message) : message{message} {}

std::string Disallowed::getMessage() const { return message; }
