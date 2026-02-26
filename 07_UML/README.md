# UML Class Diagrams for LLD

## What is UML?
**Unified Modeling Language (UML)** is a standardized visual language for modeling software systems. In LLD interviews, **Class Diagrams** are the most important UML diagrams — they show the structure of your design: classes, attributes, methods, and relationships.

## Why learn it?
- **Interview Requirement:** Most LLD interviews expect you to draw a class diagram before writing code.
- **Communication Tool:** UML provides a universal vocabulary for discussing designs with teammates.
- **Design Validation:** Drawing the diagram often reveals design flaws before you write a single line of code.

---

## Class Notation

A class is drawn as a box with three sections:

```
┌──────────────────────────┐
│        ClassName         │  ← Name (bold, centered)
├──────────────────────────┤
│ - privateField: Type     │  ← Attributes
│ + publicField: Type      │
│ # protectedField: Type   │
├──────────────────────────┤
│ + publicMethod(): Return │  ← Methods
│ - privateHelper(): void  │
│ # protectedOp(): Type    │
└──────────────────────────┘
```

### Visibility Modifiers
| Symbol | Meaning |
|--------|---------|
| `+` | Public |
| `-` | Private |
| `#` | Protected |
| `~` | Package (Java) |

---

## Relationships

### 1. Association (→)
A general "uses" relationship. One class references another.
```
┌─────────┐         ┌─────────┐
│ Student  │───────→│ Course  │
└─────────┘         └─────────┘
Student "knows about" Course
```

### 2. Aggregation (◇→)
"Has-a" relationship where the child **can exist independently** of the parent.
```
┌──────────┐  ◇────→ ┌──────────┐
│Department│         │ Employee │
└──────────┘         └──────────┘
Department has Employees, but Employees can exist without the Department.
```

### 3. Composition (◆→)
"Has-a" relationship where the child **cannot exist** without the parent.
```
┌─────────┐  ◆────→ ┌─────────┐
│  House   │         │  Room   │
└─────────┘         └─────────┘
House has Rooms. If House is destroyed, Rooms are too.
```

### 4. Inheritance / Generalization (△)
"Is-a" relationship. Subclass extends base class.
```
       ┌──────────┐
       │  Shape   │
       └────△─────┘
            │
    ┌───────┼───────┐
    │       │       │
┌───┴──┐┌──┴───┐┌──┴──┐
│Circle││Square││ Tri  │
└──────┘└──────┘└─────┘
```

### 5. Realization / Implementation (△ dashed)
A class implements an interface.
```
       ┌─────────────┐
       │ «interface»  │
       │  Flyable     │
       └──────△───────┘
              ┆ (dashed)
         ┌────┴────┐
         │  Bird   │
         └─────────┘
```

### 6. Dependency (--→ dashed)
A temporary "uses" relationship (e.g., a method parameter).
```
┌──────────┐  - - - → ┌──────────┐
│ Printer  │          │ Document │
└──────────┘          └──────────┘
Printer uses Document temporarily (e.g., print(Document doc))
```

---

## Multiplicity

Placed on relationships to indicate how many instances are involved:

| Notation | Meaning |
|----------|---------|
| `1` | Exactly one |
| `0..1` | Zero or one |
| `*` | Zero or more |
| `1..*` | One or more |
| `3..5` | Three to five |

Example:
```
┌──────────┐  1    *  ┌──────────┐
│ Teacher  │─────────│ Student  │
└──────────┘         └──────────┘
One Teacher has many Students
```

---

## Worked Example: Parking Lot

```
┌───────────────────────────────┐
│         ParkingLot            │
├───────────────────────────────┤
│ - name: string                │
│ - floors: vector<ParkingFloor>│
├───────────────────────────────┤
│ + addFloor(floor): void       │
│ + findSpot(vehicle): Spot*    │
└────────────◆──────────────────┘
             │ 1..*
    ┌────────┴────────┐
    │  ParkingFloor   │
    ├─────────────────┤
    │ - floorNumber   │
    │ - spots: vector │
    ├─────────────────┤
    │ + getAvailable() │
    └────────◆────────┘
             │ *
    ┌────────┴────────┐
    │  ParkingSpot    │
    ├─────────────────┤
    │ - id: string    │
    │ - type: SpotType│
    │ - occupied: bool│
    ├─────────────────┤
    │ + park(v): bool │
    │ + free(): void  │
    └─────────────────┘

    ┌─────────────────┐          ┌──────────────┐
    │    Vehicle      │          │  «enum»      │
    ├─────────────────┤          │  SpotType    │
    │ - plate: string │          ├──────────────┤
    │ - type: VehicleT│          │ COMPACT      │
    └─────────────────┘          │ REGULAR      │
                                 │ LARGE        │
                                 └──────────────┘
```

**Relationships:**
- ParkingLot **composes** ParkingFloors (floors don't exist without the lot)
- ParkingFloor **composes** ParkingSpots
- ParkingSpot **associates** with Vehicle (temporary, while parked)

---

## Quick Reference Cheat Sheet

```
Association:      A ────────→ B     "A uses B"
Aggregation:      A ◇────────→ B    "A has B (B can exist alone)"
Composition:      A ◆────────→ B    "A owns B (B dies with A)"
Inheritance:      A ────────△ B     "A extends B"
Realization:      A ┄┄┄┄┄┄┄△ B     "A implements B"
Dependency:       A ┄┄┄┄┄┄┄→ B     "A temporarily uses B"
```

## Interview Tips
1. Start with **nouns** from the problem statement → these become **classes**
2. Identify **verbs** → these become **methods**
3. Determine relationships (has-a vs is-a)
4. Mark visibility (`+`, `-`, `#`)
5. Add multiplicity
6. Keep it simple — don't UML everything, focus on core classes
