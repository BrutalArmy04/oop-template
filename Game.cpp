#include "Game.h"
#include <iostream>
#include "CardFactory.h"
#include "Treasury_Card.h"
#include "Victory_Card.h"

int Game::ct = 0;

Game::Game() { initializeSupply(); }

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

void Game::addPlayer(const std::string& name) {
    players.emplace_back(name);
}

void Game::initializeSupply() {
    if (supplyInitialized) return;
    supplyInitialized = true;

    table += {CardFactory::createCard("Copper"), 60};
    table += {CardFactory::createCard("Silver"), 40};
    table += {CardFactory::createCard("Gold"), 30};
    table += {CardFactory::createCard("Estate"), 24};
    table += {CardFactory::createCard("Duchy"), 12};
    table += {CardFactory::createCard("Province"), 12};
    table += {CardFactory::createCard("Curse"), 30};

    table += {CardFactory::createCard("Moat"), 10};
    table += {CardFactory::createCard("Village"), 10};
    table += {CardFactory::createCard("Smithy"), 10};
    table += {CardFactory::createCard("Festival"), 10};
    table += {CardFactory::createCard("Laboratory"), 10};
    table += {CardFactory::createCard("Throne Room"), 10};
    table += {CardFactory::createCard("Merchant"), 10};
    table += {CardFactory::createCard("Gardens"), 10};
    table += {CardFactory::createCard("Council Room"), 10};
    table += {CardFactory::createCard("Woodcutter"), 10};
}

void Game::startGame() {
    for (auto& player : players) {
        for (int i = 0; i < 7; ++i)
            player.addCardToDeck(std::make_shared<Treasury_Card>(0, "Copper", 1));
        for (int i = 0; i < 3; ++i)
            player.addCardToDeck(std::make_shared<Victory_Card>(2, "Estate", 1));
    }

    while (!table.gameOver()) {
        ct++;
        std::cout << "--- Turn " << ct << " ---\n";
        for (auto& player : players) {
            player.startTurn();
            player.playActions();
            player.playTreasures();
            table.showSupply();
            player.buyCards(table);
            player.endTurn();

            if (table.gameOver()) break;
        }
    }

    std::cout << "\n--- Game Over ---\n";
    for (const auto& player : players) {
        std::cout << player.getName() << ": " << player.score() << " points\n";
    }
}