#ifndef CARD_H
#define CARD_H


#include <string>

class Player;

class Card {
protected:
    int cost{};
    std::string nume;
public:
    Card();
    Card(int cost, const std::string& nume);
    virtual ~Card() = default;

    Card(const Card&) = default;
    Card& operator=(const Card& other);

    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] const int &getCost() const;

    virtual void play(Player& player) const;
    virtual void print(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Card& obj);
};

#endif