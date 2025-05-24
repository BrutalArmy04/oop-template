#ifndef TABLE_H
#define TABLE_H

#include <unordered_map>
#include <memory>
#include <string>
#include "Card.h"

class Table {
private:
    std::unordered_map<std::string, std::pair<std::shared_ptr<Card>, int>> supply;
public:
    void addCard(const std::shared_ptr<Card>& card, int count);
    std::shared_ptr<Card> buyCard(const std::string& name);
    bool isEmpty(const std::string& name) const;
    int emptyPiles() const;
    void showSupply() const;
    bool gameOver() const;
    Table& operator+=(const std::pair<std::shared_ptr<Card>, int>& cardEntry);
};

#endif