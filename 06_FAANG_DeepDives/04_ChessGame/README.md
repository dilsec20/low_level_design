# FAANG LLD: Design a Chess Game

**Frequently Asked at:** Google, Amazon, Microsoft, Directi, Arcesium

---

## Requirements
- Two-player game on an 8×8 board.
- Six piece types: King, Queen, Rook, Bishop, Knight, Pawn — each with unique movement rules.
- Game ends on Checkmate, Stalemate, or Resignation.
- Validate moves before applying them.

## Design Patterns Used
- **Template Method / Polymorphism:** Each `Piece` subclass overrides `canMove()` with its own movement logic. The board doesn't need to know WHICH piece it is — it just calls `piece->canMove(from, to)`.
- **Command:** Each move can be stored as a `MoveCommand` for undo/redo and game replay.
- **Factory:** `PieceFactory` creates the right piece subclass based on type.

## Class Diagram
```
┌──────────────┐        ┌──────────────┐
│    Board     │        │    Player    │
├──────────────┤        ├──────────────┤
│ - grid[8][8] │        │ - name       │
├──────────────┤        │ - color      │
│ + movePiece()│        └──────────────┘
│ + isCheck()  │
│ + display()  │
└──────────────┘
       │ has
       ▼
┌──────────────┐
│  Piece (ABC) │  ← Abstract Base Class
├──────────────┤
│ - color      │
│ + canMove()  │  ← Pure virtual (Template Method)
│ + symbol()   │
└──────┬───────┘
       │ Derived
  ┌────┼────┬────┬────┬────┐
 King Queen Rook Bishop Knight Pawn
```
