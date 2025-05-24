#include "Victory_Card.h"
#include "Player.h"

Victory_Card::Victory_Card(int cost, const std::string& name, int points)
    : Card(cost, name), points(points) {}

int Victory_Card::getPoints() const {
    return points;
}
void Victory_Card::print(std::ostream& os) const{
    Card::print(os);
    os << " [Victory " << points << " pts]";
}