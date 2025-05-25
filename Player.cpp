#include "Player.h"
#include <iostream>
#include <algorithm>
#include <random>
#include "Action_Card.h"
#include "Table.h"
#include "Treasury_Card.h"
#include "Exceptions.h"
#include "Victory_Card.h"

void Player::drawCard() {
    if (deck.empty()) {
        shuffle(discard.begin(), discard.end(), rng);
        deck = discard;
        discard.clear();
    }
    if (!deck.empty()) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
}

Player::Player(const std::string &name) : name(name) {}
void Player::addCardToDeck(const std::shared_ptr<Card> &card) {
    discard.push_back(card);
}

void Player::startTurn() {
    actions = 1;
    buys = 1;
    coins = 0;
    for (int i = 0; i < 5; ++i) drawCard();
    std::cout << "\n--- " << name << "'s turn ---\n";
}

void Player::playActions() {
    while (actions > 0) {
        std::cout << "\nActions remaining: " << actions << std::endl;
        std::cout << "Your hand:\n";
        for (size_t i = 0; i < hand.size(); ++i) {
            std::cout << i << " - " << *hand[i] << std::endl;
        }

        std::cout << "Enter index of Action card to play (-1 to skip): ";
        int idx;

        std::cin >> idx;
        if (idx == -1) break;

        if (idx < 0 || idx >= static_cast<int>(hand.size()) ||
            !std::dynamic_pointer_cast<Action_Card>(hand[idx])) {
            std::cout << "Invalid choice. Try again.\n";
            continue;
            }


        std::shared_ptr<Card> card = hand[idx];
        hand.erase(hand.begin() + idx);
        card->play(*this);
        playArea.push_back(card);
        actions--;
    }
}

void Player::playTreasures() {
    std::cout << "Your hand:\n";
    for (size_t i = 0; i < hand.size(); ++i) {
        std::cout << i << " - " << *hand[i] << std::endl;
    }
    std::cout << "\nPlaying all Treasure cards...\n";
    for (auto it = hand.begin(); it != hand.end(); ) {
        if (auto tc = std::dynamic_pointer_cast<Treasury_Card>(*it)) {
            coins += tc->getValue();
            playArea.push_back(*it);
            it = hand.erase(it);
        } else {
            ++it;
        }
    }
    std::cout << "Total coins: " << coins << std::endl;
}

void Player::buyCards(Table& table) {
    while (buys > 0) {
        std::cout << "\nCoins: " << coins << ", Buys left: " << buys << std::endl;
        std::cout << "Enter card name to buy (or 'skip'): ";
        std::string namebuycard;
        std::cin.ignore();
        getline(std::cin, namebuycard);
        if (namebuycard == "skip") break;

        try {
            std::shared_ptr<Card> card = table.buyCard(namebuycard);
            if (card->getCost() > coins)
                throw InsufficientCoinsException(card->getCost(), coins);

            discard.push_back(card);
            coins -= card->getCost();
            buys--;
            std::cout << "Bought " << card->getName() << std::endl;
        } catch (const GameException& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

void Player::endTurn() {
    std::cout << "\nEnding turn...\n";
    discard.insert(discard.end(), hand.begin(), hand.end());
    discard.insert(discard.end(), playArea.begin(), playArea.end());
    hand.clear();
    playArea.clear();
}

int Player::score() const {
    int points = 0;
    for (const auto& card : deck) {
        if (auto vc = dynamic_pointer_cast<Victory_Card>(card))
            points += vc->getPoints();
    }
    for (const auto& card : discard) {
        if (auto vc = dynamic_pointer_cast<Victory_Card>(card))
            points += vc->getPoints();
    }
    for (const auto& card : hand) {
        if (auto vc = dynamic_pointer_cast<Victory_Card>(card))
            points += vc->getPoints();
    }
    return points;
}

const std::string & Player::getName() const& { return name;}

void Player::addActions(int a) { actions += a; }
void Player::addBuys(int b) { buys += b; }
void Player::addCoins(int c) { coins += c; }
void Player::drawCards(int n) { for (int i = 0; i < n; ++i) drawCard(); }