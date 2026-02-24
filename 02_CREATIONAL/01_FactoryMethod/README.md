# Factory Method Pattern

## What is it?
The Factory Method is a creational design pattern that provides an interface for creating objects in a superclass, but allows subclasses to alter the type of objects that will be created. In simpler terms (and in modern C++), a factory is just a method or a separate class (Factory) that handles the complex construction of an object, rather than doing it all directly in the constructor.

## Why use it?
- **Constructor Limitations in C++:** Constructors cannot have names. If you need multiple ways to initialize an object (e.g., initializing a `Point` using Cartesian coordinates vs Polar coordinates), you can't have two constructors taking `(double, double)` because the signatures would conflict.
- **Expressiveness:** `Point::NewCartesian(x, y)` is much clearer than `Point(x, y)`.
- **Decoupling:** You separate the process of creating an object from the object's logic itself.

## Two Flavors of Factories
1. **Factory Method:** A static method within the class itself that returns instances of the class.
2. **Factory Class:** A completely separate class whose sole responsibility is to create instances of another class.

## The Example (factory.cpp)
We want to represent a 2D `Point` class. We might want to construct it using Cartesian coordinates (`x`, `y`) or Polar coordinates (`rho`, `theta`).

In the **Bad Design**, we try to handle both in a single constructor. We are forced to pass an `enum` to tell the constructor which coordinates we are supplying. This makes the code ugly, hard to read, and difficult to extend. What if we add a new coordinate system?

In the **Good Design**, we make the constructor `private`, and we introduce **Factory Methods** (static methods like `NewCartesian` and `NewPolar`). These methods clearly express *how* the object is being constructed. 
Alternatively, we can group these methods into a separate **Factory Class** (`PointFactory`) to follow the Single Responsibility Principle, separating the logic of a point from the logic of creating a point.
