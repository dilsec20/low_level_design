#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <string>

using namespace std;

enum class Color { WHITE, BLACK };

struct Position {
    int row, col;
    bool is_valid() const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
};

// ==========================================
// ABSTRACT PIECE (Polymorphism / Template Method)
// ==========================================
class Piece {
protected:
    Color color;
public:
    Piece(Color c) : color(c) {}
    virtual ~Piece() = default;
    Color get_color() const { return color; }
    
    // Each subclass defines its own movement rules — this is the CORE OOP concept tested!
    virtual bool can_move(Position from, Position to, Piece* board[8][8]) = 0;
    virtual char symbol() const = 0;
};

// ==========================================
// CONCRETE PIECES
// ==========================================
class King : public Piece {
public:
    King(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        int dr = abs(to.row - from.row);
        int dc = abs(to.col - from.col);
        return dr <= 1 && dc <= 1 && !(dr == 0 && dc == 0);
    }
    char symbol() const override { return color == Color::WHITE ? 'K' : 'k'; }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        int dr = abs(to.row - from.row);
        int dc = abs(to.col - from.col);
        // Rook-like (straight) or Bishop-like (diagonal)
        if (dr == 0 || dc == 0 || dr == dc) {
            // Check path is clear
            int step_r = (to.row == from.row) ? 0 : (to.row > from.row ? 1 : -1);
            int step_c = (to.col == from.col) ? 0 : (to.col > from.col ? 1 : -1);
            int r = from.row + step_r, c = from.col + step_c;
            while (r != to.row || c != to.col) {
                if (board[r][c] != nullptr) return false; // Blocked
                r += step_r; c += step_c;
            }
            return true;
        }
        return false;
    }
    char symbol() const override { return color == Color::WHITE ? 'Q' : 'q'; }
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        if (from.row != to.row && from.col != to.col) return false;
        int step_r = (to.row == from.row) ? 0 : (to.row > from.row ? 1 : -1);
        int step_c = (to.col == from.col) ? 0 : (to.col > from.col ? 1 : -1);
        int r = from.row + step_r, c = from.col + step_c;
        while (r != to.row || c != to.col) {
            if (board[r][c] != nullptr) return false;
            r += step_r; c += step_c;
        }
        return true;
    }
    char symbol() const override { return color == Color::WHITE ? 'R' : 'r'; }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        int dr = abs(to.row - from.row);
        int dc = abs(to.col - from.col);
        if (dr != dc || dr == 0) return false;
        int step_r = (to.row > from.row) ? 1 : -1;
        int step_c = (to.col > from.col) ? 1 : -1;
        int r = from.row + step_r, c = from.col + step_c;
        while (r != to.row) {
            if (board[r][c] != nullptr) return false;
            r += step_r; c += step_c;
        }
        return true;
    }
    char symbol() const override { return color == Color::WHITE ? 'B' : 'b'; }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        int dr = abs(to.row - from.row);
        int dc = abs(to.col - from.col);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
    char symbol() const override { return color == Color::WHITE ? 'N' : 'n'; }
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c) {}
    bool can_move(Position from, Position to, Piece* board[8][8]) override {
        int direction = (color == Color::WHITE) ? -1 : 1; // White moves up
        int dr = to.row - from.row;
        int dc = abs(to.col - from.col);

        // Standard 1-step forward
        if (dc == 0 && dr == direction && board[to.row][to.col] == nullptr) return true;
        // 2-step from starting position
        int start_row = (color == Color::WHITE) ? 6 : 1;
        if (dc == 0 && from.row == start_row && dr == 2 * direction && board[to.row][to.col] == nullptr)
            return true;
        // Diagonal capture
        if (dc == 1 && dr == direction && board[to.row][to.col] != nullptr)
            return true;
        return false;
    }
    char symbol() const override { return color == Color::WHITE ? 'P' : 'p'; }
};

// ==========================================
// BOARD
// ==========================================
class Board {
public:
    Piece* grid[8][8] = {};

    Board() { setup(); }

    void setup() {
        // Black pieces (top)
        grid[0][0] = new Rook(Color::BLACK);   grid[0][7] = new Rook(Color::BLACK);
        grid[0][1] = new Knight(Color::BLACK); grid[0][6] = new Knight(Color::BLACK);
        grid[0][2] = new Bishop(Color::BLACK); grid[0][5] = new Bishop(Color::BLACK);
        grid[0][3] = new Queen(Color::BLACK);  grid[0][4] = new King(Color::BLACK);
        for (int c = 0; c < 8; c++) grid[1][c] = new Pawn(Color::BLACK);

        // White pieces (bottom)
        grid[7][0] = new Rook(Color::WHITE);   grid[7][7] = new Rook(Color::WHITE);
        grid[7][1] = new Knight(Color::WHITE); grid[7][6] = new Knight(Color::WHITE);
        grid[7][2] = new Bishop(Color::WHITE); grid[7][5] = new Bishop(Color::WHITE);
        grid[7][3] = new Queen(Color::WHITE);  grid[7][4] = new King(Color::WHITE);
        for (int c = 0; c < 8; c++) grid[6][c] = new Pawn(Color::WHITE);
    }

    void display() const {
        cout << "\n  a b c d e f g h\n";
        for (int r = 0; r < 8; r++) {
            cout << (8 - r) << " ";
            for (int c = 0; c < 8; c++) {
                cout << (grid[r][c] ? grid[r][c]->symbol() : '.') << " ";
            }
            cout << (8 - r) << "\n";
        }
        cout << "  a b c d e f g h\n";
    }

    bool move_piece(Position from, Position to, Color turn) {
        if (!from.is_valid() || !to.is_valid()) {
            cout << "Invalid position!\n"; return false;
        }
        Piece* piece = grid[from.row][from.col];
        if (!piece) { cout << "No piece at source!\n"; return false; }
        if (piece->get_color() != turn) { cout << "Not your piece!\n"; return false; }

        Piece* target = grid[to.row][to.col];
        if (target && target->get_color() == turn) { 
            cout << "Can't capture your own piece!\n"; return false; 
        }

        if (!piece->can_move(from, to, grid)) {
            cout << "Invalid move for this piece!\n"; return false;
        }

        // Execute move
        if (target) cout << "Captured " << target->symbol() << "!\n";
        delete target;
        grid[to.row][to.col] = piece;
        grid[from.row][from.col] = nullptr;
        return true;
    }

    ~Board() {
        for (int r = 0; r < 8; r++)
            for (int c = 0; c < 8; c++)
                delete grid[r][c];
    }
};

// ==========================================
// GAME
// ==========================================
class ChessGame {
    Board board;
    Color current_turn = Color::WHITE;
    
    string color_name() const { return current_turn == Color::WHITE ? "White" : "Black"; }
public:
    void play_move(int fr, int fc, int tr, int tc) {
        cout << "\n" << color_name() << "'s turn: (" << fr << "," << fc << ") -> (" << tr << "," << tc << ")\n";
        if (board.move_piece({fr, fc}, {tr, tc}, current_turn)) {
            current_turn = (current_turn == Color::WHITE) ? Color::BLACK : Color::WHITE;
        }
        board.display();
    }

    void start() {
        cout << "=== CHESS GAME START ===\n";
        board.display();
    }
};

int main() {
    ChessGame game;
    game.start();

    // Classic opening moves
    game.play_move(6, 4, 4, 4); // White: Pawn e2 -> e4
    game.play_move(1, 4, 3, 4); // Black: Pawn e7 -> e5
    game.play_move(7, 1, 5, 2); // White: Knight b1 -> c3
    game.play_move(0, 1, 2, 2); // Black: Knight b8 -> c6

    // Invalid move test
    game.play_move(7, 0, 5, 0); // White Rook: blocked by pawn!

    return 0;
}
