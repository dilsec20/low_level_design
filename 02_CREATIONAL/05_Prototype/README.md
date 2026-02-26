# Prototype Pattern

## What is it?
The Prototype pattern is a creational design pattern that lets you create new objects by **cloning an existing object** (the "prototype") rather than constructing from scratch. The clone is a deep copy that can then be customized independently.

## Why use it?
- **Expensive Construction:** When creating an object involves costly operations (database queries, file I/O, complex calculations), cloning an existing object is much cheaper.
- **Unknown Concrete Types:** When you have a pointer to a base class and don't know the actual derived type, you can call `clone()` to get a copy without knowing the concrete class.
- **Pre-configured Templates:** You can set up a few "template" objects with common configurations and clone them whenever you need a similar object.

## Key Idea
Every class that supports cloning implements a `clone()` method. In C++, this typically returns a `unique_ptr<Base>` to support polymorphism.

## Analogy
Think of a **stamp**. You design a stamp once (the prototype), and then you can stamp (clone) as many copies as you want. Each stamped copy can then be customized (e.g., you write a different date on each form).

## The Example (prototype.cpp)
We are building a **document system** with different types of content blocks (TextBlock, ImageBlock). We want to create "template documents" and then clone them to create new documents quickly.

In the **Bad Design**, we manually copy every field and use `dynamic_cast` to figure out what type each block is. Adding a new block type means updating the copy logic everywhere.

In the **Good Design**, each block implements a `clone()` method. The `Document` class clones the entire structure polymorphically. We never need to know the concrete types!
