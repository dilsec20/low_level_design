# Template Method Pattern

## What is it?
The Template Method is a behavioral design pattern that defines the **skeleton of an algorithm** in a base class, allowing subclasses to override specific steps without changing the algorithm's structure.

## Why use it?
- **Code Reuse:** The overall algorithm structure is written once in the base class. Only the varying steps are overridden.
- **Inversion of Control:** The base class calls the subclass methods (Hollywood Principle: "Don't call us, we'll call you").
- **Consistent Structure:** All implementations follow the same high-level steps, reducing bugs from forgetting steps.

## Analogy
Think of building a **house**. The construction process is always: Foundation → Walls → Roof → Interior. But the materials change: one house uses brick walls, another uses wood. The *template* (order of steps) stays the same; the *details* change per house type.

## The Example (template_method.cpp)
We build a **data mining framework** that extracts and processes data. The algorithm is always: Open file → Extract data → Parse data → Analyze → Report. Different file formats (CSV, JSON) only override the extract and parse steps.
