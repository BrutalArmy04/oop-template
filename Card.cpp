#include "Card.h"
#include "Player.h"
#include <iostream>

Card::Card() = default;
Card::Card(int cost, const std::string& nume) : cost(cost), nume(nume) {}

Card& Card::operator=(const Card& other) {
    if (this == &other) return *this;
    cost = other.cost;
    nume = other.nume;
    return *this;
}

const std::string& Card::getName() const { return nume; }
const int& Card::getCost() const { return cost; }

void Card::play(Player&) const {}

void Card::print(std::ostream& os) const {
    os << "cost: " << cost << " nume: " << nume;
}

std::ostream& operator<<(std::ostream& os, const Card& obj) {
    obj.print(os);
    return os;
}