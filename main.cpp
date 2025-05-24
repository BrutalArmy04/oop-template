#include <iostream>
#include <string>
#include "Game.h"

int main() {
    try {
        Game& game = Game::getInstance();

        int numPlayers;
        std::cout << "Enter number of players: ";
        std::cin >> numPlayers;
        std::cin.ignore();

        for (int i = 0; i < numPlayers; ++i) {
            std::string name;
            std::cout << "Enter name for player " << (i + 1) << ": ";
            std::getline(std::cin, name);
            game.addPlayer(name);
        }

        game.startGame();
    } catch (const std::exception& e) {
        std::cerr << "Game error: " << e.what() << std::endl;
    }

    return 0;
}
