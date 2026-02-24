# FAANG LLD: Design a Vending Machine

**Frequently Asked at:** Amazon, Google, Goldman Sachs, Flipkart

---

## Requirements
- Machine accepts coins (1, 5, 10, 25 cents).
- Displays available products with prices.
- User selects a product; machine dispenses it if enough money is inserted.
- Returns change.
- Tracks inventory internally.

## Design Patterns Used
- **State Pattern:** The machine behaves completely differently depending on its state (Idle, HasMoney, Dispensing, SoldOut). This is the #1 pattern tested here.
- **SRP:** `Inventory` class only manages stock. `CoinHandler` only manages money. `VendingMachine` orchestrates states.

## Class Diagram
```
┌─────────────────┐
│  VendingMachine  │ (Context holding current State)
├─────────────────┤
│ - currentState   │──────► IState (Interface)
│ - inventory      │        ├─► IdleState
│ - balance        │        ├─► HasMoneyState
├─────────────────┤        ├─► DispensingState
│ + insertCoin()   │        └─► SoldOutState
│ + selectProduct()│
│ + dispense()     │
│ + cancel()       │
└─────────────────┘
         │
         ▼
┌─────────────────┐     ┌─────────────────┐
│    Inventory     │     │     Product      │
├─────────────────┤     ├─────────────────┤
│ - items map      │     │ - name           │
├─────────────────┤     │ - price          │
│ + addProduct()   │     └─────────────────┘
│ + getProduct()   │
│ + isAvailable()  │
│ + reduceStock()  │
└─────────────────┘
```
