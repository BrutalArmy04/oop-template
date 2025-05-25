#ifndef CARDFACTORY_H
#define CARDFACTORY_H

#include <memory>
#include <string>
#include "Card.h"
#include "Treasury_Card.h"
#include "Victory_Card.h"
#include "Action_Card.h"
#include <stdexcept>

class CardFactory {
public:
    template<typename... Args>
    static std::shared_ptr<Card> createCard(const std::string& name, Args&&... args);
};

template<typename... Args>
std::shared_ptr<Card> CardFactory::createCard(const std::string &name, Args &&...) {
    if (name == "Copper") return std::make_shared<Treasury_Card>(0, name, 1);
    if (name == "Silver") return std::make_shared<Treasury_Card>(3, name, 2);
    if (name == "Gold") return std::make_shared<Treasury_Card>(6, name, 3);
    if (name == "Estate") return std::make_shared<Victory_Card>(2, name, 1);
    if (name == "Duchy") return std::make_shared<Victory_Card>(5, name, 3);
    if (name == "Province") return std::make_shared<Victory_Card>(8, name, 6);
    if (name == "Curse") return std::make_shared<Victory_Card>(0, name, -1);

    if (name == "Moat") return std::make_shared<Action_Card>(2, name, 0, 2, 0, 0);
    if (name == "Village") return std::make_shared<Action_Card>(3, name, 2, 1, 0, 0);
    if (name == "Smithy") return std::make_shared<Action_Card>(4, name, 0, 3, 0, 0);
    if (name == "Festival") return std::make_shared<Action_Card>(5, name, 2, 0, 2, 1);
    if (name == "Laboratory") return std::make_shared<Action_Card>(5, name, 1, 2, 0, 0);
    if (name == "Throne Room") return std::make_shared<Action_Card>(4, name, 0, 0, 0, 0);
    if (name == "Merchant") return std::make_shared<Action_Card>(3, name, 1, 1, 0, 0);
    if (name == "Gardens") return std::make_shared<Action_Card>(4, name, 0, 0, 0, 0);
    if (name == "Council Room") return std::make_shared<Action_Card>(5, name, 0, 4, 0, 1);
    if (name == "Woodcutter") return std::make_shared<Action_Card>(3, name, 0, 0, 2, 1);

    throw std::invalid_argument("Unknown card name: " + name);
}


#endif
