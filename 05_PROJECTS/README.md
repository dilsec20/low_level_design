# Low Level Design Projects - Interview Questions

This README focuses on end-to-end System Design (Low Level Design) interview questions. In these rounds, you are not asked to implement a single pattern, but to design the class hierarchy, interfaces, and relationships for an entire system in 45-60 minutes.

## Important Steps for LLD Interviews
When given an open-ended problem like "Design an Elevator", follow these steps:
1. **Clarify Requirements:** Does the building have multiple elevators? Are there VIP floors? Capacity limits?
2. **Identify Core Entities/Classes:** What are the nouns? (ElevatorContainer, ElevatorCar, Button, Panel, Door, Dispatcher).
3. **Identify Attributes:** What properties do these classes hold? (Current Floor, Direction, Status, Capacity).
4. **Identify Operations/Methods:** What do these entities do? (MoveUp, OpenDoor, RequestStop).
5. **Draw the Class Diagram:** Visualize relationships (Has-a, Is-a).
6. **Code the Core Logic:** Implement the hardest part of the system first.

---

## 1. Practice Problem: Design a Vending Machine

**Problem Statement:**
Design the software for a Vending Machine.
- It accepts coins (1, 5, 10, 25 cents).
- Allows a user to select products (Candy, Soda, Chips).
- Returns inserted coins if canceled.
- Dispenses item and change when a successful transaction occurs.
- Keeps track of inventory and money inside the machine.

**Key Design Pattern to consider:** 
**State Pattern**. 
A vending machine operates completely differently depending on its state.
Possible States: `IdleState`, `HasCoinState`, `DispensingState`, `SoldOutState`.

**Core Classes:**
1. `VendingMachine` (Context holding the State)
2. `Inventory` (Manages item counts and prices)
3. `Coin` (Enum)
4. `Item` (Enum/Class)
5. `State` Interface (`insertCoin()`, `pressButton()`, `dispense()`)

---

## 2. Practice Problem: Design an Elevator System

**Problem Statement:**
Design the logic for multiple elevators in a building. 
- There are M floors and N elevators.
- Users can press 'Up' or 'Down' from a hallway panel.
- Users can press a specific floor number from inside an elevator car.
- The system should efficiently dispatch the closest available elevator heading in the right direction.

**Key Design Patterns to consider:**
- **Strategy Pattern:** For the Dispatching Algorithm (e.g., RoundRobin strategy, ClosestDistance strategy, FCFS strategy).
- **Observer Pattern:** The panels (hallway) are observers of the elevator's current status (floor/direction).

**Core Classes:**
1. `ElevatorCar` (Manages doors, current floor, internal buttons).
2. `ElevatorSystem` (The orchestrator/facade).
3. `Dispatcher` (Strategy interface for resolving requests).
4. `InternalPanel` and `ExternalPanel` (Buttons).
5. `Request` (Data object containing source floor, target floor, direction).

---

## 3. Practice Problem: Design a Logger Library

**Problem Statement:**
Design a logging library that developers can import into their code.
- It should support multiple levels: INFO, WARNING, ERROR, DEBUG.
- It should support different output targets: Console, File, Database.
- The configuration should be updatable at runtime.

**Key Design Patterns to consider:**
- **Singleton:** The actual Logger instance should be single to avoid concurrency issues writing to the same file.
- **Chain of Responsibility:** For handling log levels.
- **Strategy:** For the varying output targets (`ConsoleSink`, `FileSink`).

**Core Classes:**
1. `Logger` (Singleton).
2. `LogMessage` (Data Transfer Object with timestamp, level, text).
3. `LogSink` Interface with concrete implementations.
4. `ConfigurationManager` (To format strings and apply log level filters).
