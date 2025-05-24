#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <memory>
#include "Card.h"
#include <random>
#include "Table.h"

class Player {
private:
    std::string name;
    int actions = 1, buys = 1, coins = 0;
    std::vector<std::shared_ptr<Card>> deck, hand, discard, playArea;
    void drawCard();
    std::mt19937 rng{std::random_device{}()};
public:
    explicit Player(const std::string& name);
    void addCardToDeck(const std::shared_ptr<Card>& card);
    void startTurn();
    void playActions();
    void playTreasures();
    void buyCards(Table& table);
    void endTurn();
    [[nodiscard]] int score() const;
    [[nodiscard]] const std::string& getName() const&;
    void addActions(int a);
    void addBuys(int b);
    void addCoins(int c);
    void drawCards(int n);
};


#endif