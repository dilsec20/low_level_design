# FAANG LLD: Design an Elevator System

**Frequently Asked at:** Google, Amazon, Microsoft, Uber, Goldman Sachs

---

## Requirements
- A building has N floors and M elevator cars.
- Users press Up/Down buttons on each floor.
- Users press a destination floor button inside the elevator.
- The system dispatches the most efficient elevator.
- Display shows current floor and direction for each elevator.

---

## Class Diagram

```
┌──────────────────┐       ┌──────────────────┐
│   ElevatorSystem │◄──────│     Request      │
│  (Facade/Ctrl)   │       │ (floor, direction)│
├──────────────────┤       └──────────────────┘
│ - elevators[]    │
│ - dispatcher     │       ┌──────────────────┐
├──────────────────┤       │   IDispatcher    │  ← Strategy Pattern
│ + handleRequest()│       │  (Interface)     │
└──────────────────┘       ├──────────────────┤
        │                  │ + selectElevator()│
        │                  └───────┬──────────┘
        ▼                          │
┌──────────────────┐       ┌───────┴──────────┐
│  ElevatorCar     │       │ NearestDispatcher│ ← Concrete Strategy
├──────────────────┤       └──────────────────┘
│ - id             │
│ - currentFloor   │
│ - direction      │
│ - state (IDLE,   │
│   MOVING, DOOR)  │
│ - destinationQueue│
├──────────────────┤
│ + moveUp()       │
│ + moveDown()     │
│ + openDoor()     │
│ + addDestination()│
└──────────────────┘
```

## Design Patterns Used
- **Strategy:** Dispatcher algorithm is swappable (Nearest, FCFS, LOOK).
- **State:** Elevator behavior changes based on state (Idle, MovingUp, MovingDown, DoorOpen).
- **Observer:** Floor displays observe elevator position changes.
- **Singleton:** ElevatorSystem is a single controller instance.
