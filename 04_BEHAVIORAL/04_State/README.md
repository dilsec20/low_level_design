# State Pattern

## What is it?
The State pattern is a behavioral design pattern that allows an object to **change its behavior when its internal state changes**. It appears as if the object changed its class. Instead of using massive `if-else` or `switch` statements, each state is encapsulated in its own class.

## Why use it?
- **Eliminate Conditional Hell:** Complex state-dependent behavior (like a vending machine, ATM, or elevator) creates massive conditionals. State pattern replaces each branch with a class.
- **Open-Closed Principle:** Adding a new state means creating a new class — no modification to existing state classes.
- **Clear State Transitions:** Each state class explicitly defines which transitions are valid, making the state machine self-documenting.

## Analogy
Think of a **traffic light**. It has 3 states: Red, Yellow, Green. Each state determines:
- What the light shows
- What happens when the timer expires (transition to next state)
- Whether cars can go

Instead of writing `if (state == RED) {...} else if (state == YELLOW) {...}`, each state is its own class with its own behavior.

## The Example (state.cpp)
We are building a **vending machine** with states: Idle → HasMoney → Dispensing → OutOfStock. Each state handles user actions (insertMoney, selectProduct, dispense) differently.

In the **Bad Design**, every method has a giant `switch` on the current state enum.

In the **Good Design**, each state is a class. The vending machine delegates all actions to its current state object. State transitions are clean and explicit.
