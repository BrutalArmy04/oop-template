#include "Action_Card.h"
#include "Player.h"

Action_Card::Action_Card(int cost, const std::string& nume, int extra_actions, int extra_cards, int extra_currency, int extra_buys)
    : Card(cost, nume), extra_actions(extra_actions), extra_cards(extra_cards), extra_currency(extra_currency), extra_buys(extra_buys) {}

void Action_Card::play(Player& player) const {
    player.addActions(extra_actions);
    player.drawCards(extra_cards);
    player.addCoins(extra_currency);
    player.addBuys(extra_buys);
}

void Action_Card::print(std::ostream& os) const{
    Card::print(os);
    os << " [Action +" << extra_actions << "A +" << extra_cards << "C +" << extra_currency << "$ +" << extra_buys << "B]";
}
std::ostream& operator<<(std::ostream& os, const Action_Card& obj) {
    obj.print(os);
    return os;
}