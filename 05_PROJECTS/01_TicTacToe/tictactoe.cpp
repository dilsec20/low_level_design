#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// MODELS (Entities)
// ==========================================

enum class Piece { Empty, X, O };

// Helper to convert Piece enum to char for rendering
char piece_to_char(Piece p) {
    if (p == Piece::X) return 'X';
    if (p == Piece::O) return 'O';
    return '-';
}

class Player {
private:
    string name;
    Piece piece;

public:
    Player(string name, Piece piece) : name(name), piece(piece) {}
    
    string get_name() const { return name; }
    Piece get_piece() const { return piece; }
};

class Board {
private:
    int size;
    vector<vector<Piece>> grid;

public:
    Board(int size = 3) : size(size) {
        grid.resize(size, vector<Piece>(size, Piece::Empty));
    }

    int get_size() const { return size; }

    bool is_valid_move(int row, int col) const {
        if (row < 0 || row >= size || col < 0 || col >= size) return false;
        return grid[row][col] == Piece::Empty;
    }

    bool place_piece(int row, int col, Piece piece) {
        if (is_valid_move(row, col)) {
            grid[row][col] = piece;
            return true;
        }
        return false;
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                cout << piece_to_char(grid[i][j]) << " ";
            }
            cout << "\n";
        }
    }

    bool check_winner(Piece player_piece) const {
        // Check rows and columns
        for (int i = 0; i < size; ++i) {
            bool row_win = true;
            bool col_win = true;
            for (int j = 0; j < size; ++j) {
                if (grid[i][j] != player_piece) row_win = false;
                if (grid[j][i] != player_piece) col_win = false;
            }
            if (row_win || col_win) return true;
        }

        // Check diagonals
        bool diag1_win = true;
        bool diag2_win = true;
        for (int i = 0; i < size; ++i) {
            if (grid[i][i] != player_piece) diag1_win = false;
            if (grid[i][size - 1 - i] != player_piece) diag2_win = false;
        }
        
        return diag1_win || diag2_win;
    }

    bool is_full() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (grid[i][j] == Piece::Empty) return false;
            }
        }
        return true;
    }
};

// ==========================================
// CONTROLLER 
// ==========================================

class Game {
private:
    Board board;
    vector<Player> players;
    int current_player_index;

public:
    Game(Player p1, Player p2) : current_player_index(0) {
        players.push_back(p1);
        players.push_back(p2);
    }

    void play() {
        bool game_over = false;

        cout << "--- Starting Tic-Tac-Toe ---\n";
        
        while (!game_over) {
            board.print();
            
            Player current_player = players[current_player_index];
            cout << current_player.get_name() << "'s turn (" 
                 << piece_to_char(current_player.get_piece()) << ").\n";

            int row, col;
            bool valid_move = false;

            // In a real app we'd get input from `cin`, but we'll simulate it here
            // to avoid blocking the automated run.
            // cout << "Enter row and column (0-2): "; 
            // cin >> row >> col;
            
            // Simulating a random valid move for demonstration
            for (int i=0; i<3 && !valid_move; ++i) {
                for (int j=0; j<3 && !valid_move; ++j) {
                    if (board.is_valid_move(i, j)) {
                        row = i; col = j;
                        valid_move = true;
                    }
                }
            }

            cout << "> Placed at " << row << ", " << col << "\n\n";

            if (board.place_piece(row, col, current_player.get_piece())) {
                
                if (board.check_winner(current_player.get_piece())) {
                    board.print();
                    cout << "🎉 " << current_player.get_name() << " WINS! 🎉\n";
                    game_over = true;
                } else if (board.is_full()) {
                    board.print();
                    cout << "🤝 It's a DRAW! 🤝\n";
                    game_over = true;
                } else {
                    // Swap turns
                    current_player_index = (current_player_index + 1) % players.size();
                }
            }
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    Player p1("Alice", Piece::X);
    Player p2("Bob", Piece::O);

    Game game(p1, p2);
    game.play();

    return 0;
}
