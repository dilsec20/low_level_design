#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <memory>

using namespace std;

// ==========================================
// DICE (Strategy Pattern)
// ==========================================
class IDice {
public:
    virtual int roll() = 0;
    virtual ~IDice() = default;
};

class SingleDie : public IDice {
public:
    int roll() override { return (rand() % 6) + 1; }
};

class DoubleDice : public IDice {
public:
    int roll() override { return (rand() % 6) + 1 + (rand() % 6) + 1; }
};

// ==========================================
// PLAYER
// ==========================================
class Player {
public:
    string name;
    int position = 0;  // Start off the board

    Player(const string& n) : name(n) {}
};

// ==========================================
// BOARD
// ==========================================
class Board {
private:
    static const int SIZE = 100;
    unordered_map<int, int> snakes;  // head -> tail (moves DOWN)
    unordered_map<int, int> ladders; // bottom -> top (moves UP)

public:
    Board() {
        // Add snakes (mouth -> tail)
        snakes[16] = 6;
        snakes[48] = 26;
        snakes[64] = 60;
        snakes[93] = 73;
        snakes[95] = 75;
        snakes[97] = 78;

        // Add ladders (bottom -> top)
        ladders[1] = 38;
        ladders[4] = 14;
        ladders[9] = 31;
        ladders[21] = 42;
        ladders[28] = 84;
        ladders[51] = 67;
        ladders[80] = 99;
    }

    int get_new_position(int pos) const {
        if (snakes.count(pos)) {
            cout << "  🐍 SNAKE! Slides from " << pos << " down to " << snakes.at(pos) << "\n";
            return snakes.at(pos);
        }
        if (ladders.count(pos)) {
            cout << "  🪜 LADDER! Climbs from " << pos << " up to " << ladders.at(pos) << "\n";
            return ladders.at(pos);
        }
        return pos;
    }

    int get_size() const { return SIZE; }

    void display_info() const {
        cout << "--- Board Setup ---\n";
        cout << "Snakes: ";
        for (auto& [h, t] : snakes) cout << h << "→" << t << " ";
        cout << "\nLadders: ";
        for (auto& [b, t] : ladders) cout << b << "→" << t << " ";
        cout << "\n\n";
    }
};

// ==========================================
// GAME (Template Method)
// ==========================================
class SnakeLadderGame {
private:
    Board board;
    queue<Player*> turn_queue;
    vector<unique_ptr<Player>> players;
    unique_ptr<IDice> dice;
    bool game_over = false;

public:
    SnakeLadderGame(const vector<string>& names, unique_ptr<IDice> d) : dice(move(d)) {
        for (const auto& name : names) {
            players.push_back(make_unique<Player>(name));
            turn_queue.push(players.back().get());
        }
    }

    void play() {
        board.display_info();
        cout << "=== GAME START ===\n\n";

        int round = 1;
        while (!game_over) {
            Player* current = turn_queue.front();
            turn_queue.pop();

            int roll = dice->roll();
            int new_pos = current->position + roll;

            cout << "Round " << round << ": " << current->name << " (at " << current->position 
                 << ") rolls a " << roll;

            if (new_pos > board.get_size()) {
                // Cannot overshoot 100!
                cout << " → Overshoots! Stays at " << current->position << "\n";
                turn_queue.push(current);
            } else {
                new_pos = board.get_new_position(new_pos);
                current->position = new_pos;
                cout << " → Moves to " << current->position << "\n";

                if (current->position == board.get_size()) {
                    cout << "\n🎉🎉🎉 " << current->name << " WINS THE GAME! 🎉🎉🎉\n";
                    game_over = true;
                } else {
                    turn_queue.push(current);
                }
            }
            round++;

            // Safety: prevent infinite games in demo
            if (round > 200) {
                cout << "\n--- Game capped at 200 rounds for demo ---\n";
                break;
            }
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    srand(time(0));

    SnakeLadderGame game({"Alice", "Bob"}, make_unique<SingleDie>());
    game.play();

    return 0;
}
