#include "Table.h"
#include <iostream>
#include "Exceptions.h"

void Table::addCard(const std::shared_ptr<Card>& card, int count) {
    supply[card->getName()] = {card, count};
}

std::shared_ptr<Card> Table::buyCard(const std::string& name) {
    if (!supply.count(name)) throw CardNotFoundException(name);
    if (supply[name].second == 0) throw EmptySupplyException(name);
    supply[name].second--;
    return supply[name].first;
}

bool Table::isEmpty(const std::string& name) const {
    return supply.count(name) && supply.at(name).second == 0;
}

int Table::emptyPiles() const {
    int count = 0;
    for (const auto& [_, entry] : supply)
        if (entry.second == 0) count++;
    return count;
}

void Table::showSupply() const {
    for (const auto& [name, entry] : supply) {
        std::cout << *entry.first << " x" << entry.second << std::endl;
    }
}

bool Table::gameOver() const {
    return isEmpty("Province") || emptyPiles() >= 3;
}

Table& Table::operator+=(const std::pair<std::shared_ptr<Card>, int>& cardEntry) {
    const auto& card = cardEntry.first;
    int count = cardEntry.second;
    addCard(card, count);
    return *this;
}