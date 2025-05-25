#ifndef TREASURY_CARD_H
#define TREASURY_CARD_H
#pragma once
#include "Card.h"

class Treasury_Card : public Card {
public:
    Treasury_Card(int cost, const std::string& name, int value);
    [[nodiscard]] int getValue() const;
    void print(std::ostream& os) const override;

private:
    const int value;
};
std::ostream& operator<<(std::ostream& os, const Treasury_Card& obj);
#endif