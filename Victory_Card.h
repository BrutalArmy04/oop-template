#ifndef VICTORY_CARD_H
#define VICTORY_CARD_H
#pragma once
#include "Card.h"

class Victory_Card : public Card {
public:
    Victory_Card(int cost, const std::string& name, int points);
    [[nodiscard]]int  getPoints() const;
    void print(std::ostream& os) const override;
    std::ostream& operator<<(std::ostream& os, const Victory_Card& obj) const;


private:
    int points;
};
#endif

