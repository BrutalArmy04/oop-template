#ifndef GAME_H
#define GAME_H
#pragma once
#include "Table.h"
#include "Player.h"

class Game {
private:
    Table table;
    std::vector<Player> players;
    bool supplyInitialized = false;
    static int ct;
    Game();
    void initializeSupply();

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

    static Game& getInstance();
    void addPlayer(const std::string& name);
    void startGame();
};


#endif