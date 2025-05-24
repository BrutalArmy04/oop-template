#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class InvalidActionException : public GameException {
public:
    explicit InvalidActionException(const std::string& msg) : GameException("Invalid action: " + msg) {}
};

class CardNotFoundException : public GameException {
public:
    explicit CardNotFoundException(const std::string& name) : GameException("Card not found in supply: " + name) {}
};

class InsufficientCoinsException : public GameException {
public:
    InsufficientCoinsException(int required, int available);
};

class EmptySupplyException : public GameException {
public:
    explicit EmptySupplyException(const std::string& name);
};

/*class NotAValidIndexException : public GameException {
public: // daca am chef o implementez
};*/

#endif
