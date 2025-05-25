#include "Treasury_Card.h"
#include "Player.h"

Treasury_Card::Treasury_Card(int cost, const std::string& name, int value)
    : Card(cost, name), value(value) {}

int Treasury_Card::getValue() const {return value;}

void Treasury_Card::print(std::ostream& os) const{
    Card::print(os);
        os << " [Treasure " << value << "$]";
    }
std::ostream& operator<<(std::ostream& os, const Treasury_Card& obj) {
    obj.print(os);
    return os;
}