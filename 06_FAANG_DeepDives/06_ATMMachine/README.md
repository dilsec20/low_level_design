# ATM Machine - Low Level Design

## Problem Statement
Design an ATM machine system that supports:
- Card insertion and PIN authentication
- Balance inquiry, cash withdrawal, and deposit
- State-based behavior (Idle → CardInserted → Authenticated → Transaction)
- Proper error handling for insufficient funds, invalid PIN, etc.

## Key Design Patterns Used
- **State Pattern** — ATM behavior changes based on its current state
- **Strategy Pattern** — Different transaction types (withdraw, deposit, balance)

## Classes
- `ATM` — Context class, delegates to current state
- `ATMState` — Interface for all states
- `IdleState`, `CardInsertedState`, `AuthenticatedState`, `TransactionState`
- `Account` — User's bank account with balance
- `Card` — Contains card number and PIN
