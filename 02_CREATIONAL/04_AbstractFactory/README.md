# Abstract Factory Pattern

## What is it?
The Abstract Factory is a creational design pattern that provides an interface for creating **families of related objects** without specifying their concrete classes. Think of it as a "factory of factories" — a super-factory that groups individual factories together under a common interface.

## Why use it?
- **Family Consistency:** When you need to ensure that related objects (e.g., Button + Checkbox + Menu) all belong to the same family (e.g., all Windows-style or all Mac-style), Abstract Factory guarantees you won't mix them.
- **Decoupling from Concrete Classes:** Client code works only with abstract interfaces (`Button`, `Checkbox`), never touching `WindowsButton` or `MacCheckbox` directly.
- **Easy to Swap Families:** Switching the entire product family (e.g., from Windows to Mac theme) is a single-line change — just swap the factory object.

## How is it different from Factory Method?
| Aspect | Factory Method | Abstract Factory |
|--------|---------------|-----------------|
| Creates | One product | A family of related products |
| Mechanism | Subclass overrides a method | Object composition (factory object) |
| Scale | Single product variation | Multiple related product variations |

## Analogy
Imagine you are furnishing a room. You pick a **style** (Modern, Victorian, Art Deco). Once you pick a style, ALL your furniture (Chair, Sofa, Table) comes from the same style family. The "style catalog" is your Abstract Factory — it guarantees everything matches.

## The Example (abstract_factory.cpp)
We are building a **cross-platform UI toolkit**. Our app needs `Button` and `Checkbox` widgets, but they must look different on Windows vs Mac.

In the **Bad Design**, we use `if-else` checks on the OS type everywhere we create a widget. Adding a new OS (Linux) means modifying every single creation site.

In the **Good Design**, we define a `GUIFactory` interface with `createButton()` and `createCheckbox()` methods. `WindowsFactory` and `MacFactory` implement this interface. The client code receives a factory and creates widgets — it never knows which OS it's running on!
