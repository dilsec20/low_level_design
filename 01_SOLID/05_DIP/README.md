# Dependency Inversion Principle (DIP)

## What is it?
The Dependency Inversion Principle states two things:
1. **High-level modules should not depend on low-level modules. Both should depend on abstractions (e.g., interfaces).**
2. **Abstractions should not depend on details. Details should depend on abstractions.**

High-level modules contain the important business logic. Low-level modules are utility classes, databases, APIs, etc.

## Why use it?
- **Flexibility:** If your high-level business logic depends on an abstraction (like an `IDatabase` interface) instead of a concrete class (like `MySQLDatabase`), you can easily swap the database without touching the business logic.
- **Testability:** You can easily pass a "Mock" or "Fake" implementation of the abstraction when unit testing your high-level modules.

## Identifying Violations of DIP
Look for high-level classes that directly instantiate or take concrete low-level classes as constructor arguments or properties. 

## The Example (dip.cpp)
We are building a genealogy application to track parent-child relationships.
We have a low-level module: `Relationships` (which stores data in a vector of tuples).
We have a high-level module: `Research` (which performs analysis to find all children of a specific person).

In our **Bad Design**, the `Research` class directly depends on the concrete `Relationships` class to access its internal vector. This is terrible! If `Relationships` decides to change its internal storage from a `vector` to a `map` or a database, the `Research` class will break.

In our **Good Design**, we introduce an abstraction: the `RelationshipBrowser` interface. 
Now, `Relationships` implements `RelationshipBrowser`. 
The `Research` class only depends on `RelationshipBrowser`. It doesn't care *how* the data is stored; it only uses the abstract methods to get what it needs. If the low-level storage changes, the high-level `Research` module remains utterly unaffected!
