#ifndef ACTION_CARD_H
#define ACTION_CARD_H
#pragma once
#include "Card.h"

class Action_Card : public Card {
private:
    int extra_actions{}, extra_cards{}, extra_currency{}, extra_buys{};
public:
    Action_Card() = default;
    Action_Card(int cost, const std::string& nume, int extra_actions, int extra_cards, int extra_currency, int extra_buys);

    void play(class Player& player) const override;

    void print(std::ostream& os) const override;
    friend std::ostream& operator<<(std::ostream& os, const Action_Card& obj);
};

#endif