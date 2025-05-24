#include "Exceptions.h"
#include <string>

InsufficientCoinsException::InsufficientCoinsException(int required, int available)
    : GameException("Insufficient coins. Needed: " + std::to_string(required) + ", Available: " + std::to_string(available)) {}

EmptySupplyException::EmptySupplyException(const std::string& name)
    : GameException("Card '" + name + "' is out of supply.") {}
