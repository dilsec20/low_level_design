# Memento Pattern

## What is it?
The Memento pattern is a behavioral design pattern that lets you **save and restore the previous state** of an object without revealing its implementation details. It's the pattern behind "Undo" functionality.

## Why use it?
- **Undo/Redo:** Save snapshots of an object's state and restore them later.
- **Encapsulation Preserved:** The saved state is opaque — other objects cannot peek inside the memento.
- **Transaction Rollback:** If an operation fails, restore the previous state.

## Three Participants
1. **Originator:** The object whose state we want to save (e.g., a text editor).
2. **Memento:** The snapshot of the originator's state (a sealed box).
3. **Caretaker:** Manages the history of mementos (the undo stack).

## Analogy
Think of a **video game save system**. You (Originator) play the game. At checkpoints, you save your progress (Memento). The save/load menu (Caretaker) keeps a list of saves. When you die, you load a previous save — your state is fully restored.

## The Example (memento.cpp)
We build a **text editor** with undo functionality. Each edit creates a memento (snapshot). The caretaker maintains a history stack. Undo pops the last memento and restores the editor's state.
