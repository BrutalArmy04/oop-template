#ifndef VICTORY_CARD_H
#define VICTORY_CARD_H
#pragma once
#include "Card.h"

class Victory_Card : public Card {
public:
    Victory_Card(int cost, const std::string& name, int points);
    [[nodiscard]]int  getPoints() const;
    void print(std::ostream& os) const override;
private:
    int points;
};

#endif

