# LLD Project: Tic-Tac-Toe

## Problem Statement
Design a classic Tic-Tac-Toe game.
- A 3x3 grid.
- Two players (X and O).
- Players take turns making a move on an empty cell.
- The game ends when a player gets 3 marks in a row (horizontal, vertical, or diagonal) or the board is full (a draw).

## Object-Oriented Design Approach

When designing an application, we need to identify the core **Entities** (Models), the **Game Logic** (Controller/Service), and the **Execution flow**.

### 1. Identifying Entities
- **Piece (Symbol):** An enum or class representing X, O, or Empty.
- **Board:** Knows about the 3x3 grid, can place pieces, can check if spaces are empty, and can print itself.
- **Player:** Has a name and a playing piece (X or O).

### 2. Identifying the Game Logic (Controller)
- **Game class:** Holds the Board, a list/queue of Players, and manages the main game loop.
- It asks the current player for a move.
- It checks if the move is valid via the Board.
- It applies the move.
- It checks for a win or tie condition.
- It swaps turns.

## Pattern Highlights in this Code
In the provided `tictactoe.cpp`, you will see:
1. **Single Responsibility Principle (SRP):** The `Board` only cares about managing the 2D grid. The `Player` only cares about holding player data. The `Game` only cares about orchestrating the rules.
2. **Encapsulation:** The internal state of the board (`grid`) is private. It can only be modified through the `place_piece()` method which performs validation.
