#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <random>
#include <exception>
using namespace std;



class GameException : public std::exception {
protected:
    string message;
public:
    explicit GameException(const string& msg) : message(msg) {}
    [[nodiscard]] const char* what() const noexcept override { return message.c_str(); }
};

class InvalidActionException : public GameException {
public:
    explicit InvalidActionException(const string& msg) : GameException("Invalid action: " + msg) {}
};

class CardNotFoundException : public GameException {
public:
    explicit CardNotFoundException(const string& name) : GameException("Card not found in supply: " + name) {}
};

class InsufficientCoinsException : public GameException {
public:
    explicit InsufficientCoinsException(int required, int available)
        : GameException("Insufficient coins. Needed: " + to_string(required) + ", Available: " + to_string(available)) {}
};

class EmptySupplyException : public GameException {
public:
    explicit EmptySupplyException(const string& name) : GameException("Card '" + name + "' is out of supply.") {}
};

/*class NotAValidIndexException : public GameException {
public: // daca am chef o implementez
};*/

class Card {
protected:
    int cost{};
    string nume;
public:
    Card() = default;
    Card(int cost, const string& nume) : cost(cost), nume(nume) {}
    Card(const Card& other)  = default;
    Card& operator=(const Card& other) {
        if (this == &other) return *this;
        cost = other.cost;
        nume = other.nume;
        return *this;
    }
    virtual ~Card() = default;

    [[nodiscard]] string getName() const { return nume; }
    [[nodiscard]] int getCost() const { return cost; }

    virtual void play(class Player& player) const {}

    virtual void print(ostream& os) const {
        os << "cost: " << cost << " nume: " << nume;
    }

    friend ostream& operator<<(ostream& os, const Card& obj) {
        obj.print(os);
        return os;
    }
};

class Action_Card : public Card {
private:
    int extra_actions{}, extra_cards{}, extra_currency{}, extra_buys{};
public:
    Action_Card() = default;
    Action_Card(int cost, const string& nume, int extra_actions, int extra_cards, int extra_currency, int extra_buys)
        : Card(cost, nume), extra_actions(extra_actions), extra_cards(extra_cards), extra_currency(extra_currency), extra_buys(extra_buys) {}

    void play(class Player& player) const override;

    void print(ostream& os) const override {
        Card::print(os);
        os << " [Action +" << extra_actions << "A +" << extra_cards << "C +" << extra_currency << "$ +" << extra_buys << "B]";
    }
};

class Treasury_Card : public Card {
private:
    const int value{};
public:
    Treasury_Card() = default;
    Treasury_Card(int cost, const string& nume, int value) : Card(cost, nume), value(value) {}

    [[nodiscard]] int getValue() const { return value; }

    void print(ostream& os) const override {
        Card::print(os);
        os << " [Treasure " << value << "$]";
    }
};

class Victory_Card : public Card {
private:
    const int points{}; // daca implementez gardens scot const si adaug setter
public:
    Victory_Card() = default;
    Victory_Card(int cost, const string& nume, int points) : Card(cost, nume), points(points) {}

    [[nodiscard]] int getPoints() const { return points; }

    void print(ostream& os) const override {
        Card::print(os);
        os << " [Victory " << points << " pts]";
    }
};

class CardFactory {
public:
    template<typename... Args>
    static std::shared_ptr<Card> createCard(const std::string& name, Args&&... args) {
        if (name == "Copper") return std::make_shared<Treasury_Card>(0, name, 1);
        if (name == "Silver") return std::make_shared<Treasury_Card>(3, name, 2);
        if (name == "Gold") return std::make_shared<Treasury_Card>(6, name, 3);
        if (name == "Estate") return std::make_shared<Victory_Card>(2, name, 1);
        if (name == "Duchy") return std::make_shared<Victory_Card>(5, name, 3);
        if (name == "Province") return std::make_shared<Victory_Card>(8, name, 6);
        if (name == "Curse") return std::make_shared<Victory_Card>(0, name, -1);

        // Action Cards
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
};

class Table {
private:
    unordered_map<string, pair<shared_ptr<Card>, int>> supply;
public:
    void addCard(const shared_ptr<Card>& card, int count) {
        supply[card->getName()] = {card, count};
    }

    shared_ptr<Card> buyCard(const string& name) {
        if (!supply.count(name)) throw CardNotFoundException(name);
        if (supply[name].second == 0) throw EmptySupplyException(name);
        supply[name].second--;
        return supply[name].first;
    }

    bool isEmpty(const string& name) const {
        return supply.count(name) && supply.at(name).second == 0;
    }

    int emptyPiles() const {
        int count = 0;
        for (const auto& [_, entry] : supply)
            if (entry.second == 0) count++;
        return count;
    }

    void showSupply() const {
        for (const auto& [name, entry] : supply) {
            cout << *entry.first << " x" << entry.second << endl;
        }
    }

    bool gameOver() const {
        return isEmpty("Province") || emptyPiles() >= 3;
    }
    Table& operator+=(const pair<shared_ptr<Card>, int>& cardEntry) {
        const auto& card = cardEntry.first;
        int count = cardEntry.second;
        addCard(card, count);
        return *this;
    }
};

class Player {
private:
    string name;
    vector<shared_ptr<Card>> deck, hand, discard, playArea;
    int actions = 1, buys = 1, coins = 0;
    mt19937 rng{random_device{}()};

    void drawCard() {
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

public:
    explicit Player(const string& name) : name(name) {}

    void addCardToDeck(const shared_ptr<Card> &card) {
        discard.push_back(card);
    }

    void startTurn() {
        actions = 1;
        buys = 1;
        coins = 0;
        for (int i = 0; i < 5; ++i) drawCard();
        cout << "\n--- " << name << "'s turn ---\n";
    }

    void playActions() {
        while (actions > 0) {
            cout << "\nActions remaining: " << actions << endl;
            cout << "Your hand:\n";
            for (size_t i = 0; i < hand.size(); ++i) {
                cout << i << " - " << *hand[i] << endl;
            }

            cout << "Enter index of Action card to play (-1 to skip): ";
            int idx;

                cin >> idx;
                if (idx == -1) break;

                if (idx < 0 || idx >= static_cast<int>(hand.size()) ||
                    !dynamic_pointer_cast<Action_Card>(hand[idx])) {
                    cout << "Invalid choice. Try again.\n";
                    continue;
                    }


            shared_ptr<Card> card = hand[idx];
            hand.erase(hand.begin() + idx);
            card->play(*this);
            playArea.push_back(card);
            actions--;
        }
    }

    void playTreasures() {
        cout << "\nPlaying all Treasure cards...\n";
        for (auto it = hand.begin(); it != hand.end(); ) {
            if (auto tc = dynamic_pointer_cast<Treasury_Card>(*it)) {
                coins += tc->getValue();
                playArea.push_back(*it);
                it = hand.erase(it);
            } else {
                ++it;
            }
        }
        cout << "Total coins: " << coins << endl;
    }

    void buyCards(Table& table) {
        while (buys > 0) {
            cout << "\nCoins: " << coins << ", Buys left: " << buys << endl;
            cout << "Enter card name to buy (or 'skip'): ";
            string name;
            cin.ignore();
            getline(cin, name);
            if (name == "skip") break;

            try {
                shared_ptr<Card> card = table.buyCard(name);
                if (card->getCost() > coins)
                    throw InsufficientCoinsException(card->getCost(), coins);

                discard.push_back(card);
                coins -= card->getCost();
                buys--;
                cout << "Bought " << card->getName() << endl;
            } catch (const GameException& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    }

    void endTurn() {
        cout << "\nEnding turn...\n";
        discard.insert(discard.end(), hand.begin(), hand.end());
        discard.insert(discard.end(), playArea.begin(), playArea.end());
        hand.clear();
        playArea.clear();
    }

    [[nodiscard]] int score() const {
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

    [[nodiscard]]string getName() const { return name; }

    void addActions(int a) { actions += a; }
    void addBuys(int b) { buys += b; }
    void addCoins(int c) { coins += c; }
    void drawCards(int n) { for (int i = 0; i < n; ++i) drawCard(); }
};

void Action_Card::play(Player& player) const {
    player.addActions(extra_actions);
    player.drawCards(extra_cards);
    player.addCoins(extra_currency);
    player.addBuys(extra_buys);
}

class Game {
private:
    Table table;
    vector<Player> players;
    bool supplyInitialized = false;
    static int ct;
    Game() { initializeSupply(); }
    void initializeSupply();

public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    void addPlayer(const string& name);
    void startGame();
};

void Game::addPlayer(const string& name) {
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
            player.addCardToDeck(make_shared<Treasury_Card>(0, "Copper", 1));
        for (int i = 0; i < 3; ++i)
            player.addCardToDeck(make_shared<Victory_Card>(2, "Estate", 1));
    }

    while (!table.gameOver()) {
        ct++;
        cout << "--- Turn " << ct << " ---\n";
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

    cout << "\n--- Game Over ---\n";
    for (auto& player : players) {
        cout << player.getName() << ": " << player.score() << " points\n";
    }
}
int Game::ct = 0;
int main() {
    try {
        Game& game = Game::getInstance();

        int numPlayers;
        cout << "Enter number of players: ";
        cin >> numPlayers;
        cin.ignore();

        for (int i = 0; i < numPlayers; ++i) {
            string name;
            cout << "Enter name for player " << (i + 1) << ": ";
            getline(cin, name);
            game.addPlayer(name);
        }

        game.startGame();
    } catch (const exception& e) {
        cerr << "Game error: " << e.what() << endl;
    }

    return 0;
}
