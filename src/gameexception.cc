#include "gameexception.h"

InsufficientFunds::InsufficientFunds(size_t amount, Player *owedTo,
                                     std::string message)
    : message{message}, owedAmount{amount}, owedTo{owedTo} {}

std::string InsufficientFunds::getMessage() const { return message; }

Player *InsufficientFunds::getOwedTo() const { return owedTo; }

size_t InsufficientFunds::getOwedAmount() const { return owedAmount; }

PropertyNotFound::PropertyNotFound(std::string message) : message{message} {}

std::string PropertyNotFound::getMessage() const { return message; }

Disallowed::Disallowed(std::string message) : message{message} {}

std::string Disallowed::getMessage() const { return message; }
