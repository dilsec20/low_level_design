# FAANG LLD: Design Snake & Ladder Game

**Frequently Asked at:** Amazon, Flipkart, Google, Intuit

---

## Requirements
- Supports 2-N players.
- Board has 100 cells.
- Snakes move a player DOWN. Ladders move a player UP.
- Players take turns rolling a single die (1-6).
- First player to reach exactly cell 100 wins.
- Players should NOT overshoot cell 100.

## Design Patterns Used
- **Strategy:** The dice rolling can be a strategy (single die, double die, or even a biased die for testing).
- **Template Method:** `Game.play()` defines the skeleton: roll → move → check snake/ladder → check winner.
- **SRP:** `Board` manages snakes/ladders. `Player` manages position. `Dice` manages random numbers.

## Class Diagram
```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│    Game      │────►│   Board     │     │   Player    │
├─────────────┤     ├─────────────┤     ├─────────────┤
│ - players[] │     │ - snakes{}  │     │ - name      │
│ - board     │     │ - ladders{} │     │ - position  │
│ - dice      │     ├─────────────┤     └─────────────┘
├─────────────┤     │ + getNewPos()│
│ + play()    │     └─────────────┘
└─────────────┘
       │
       ▼
┌─────────────┐
│    IDice    │ ← Strategy
├─────────────┤
│ + roll()    │
└──────┬──────┘
  ┌────┴────┐
 SingleDie  DoubleDice
```
