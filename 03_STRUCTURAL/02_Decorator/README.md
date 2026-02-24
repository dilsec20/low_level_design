# Decorator Pattern

## What is it?
The Decorator is a structural design pattern that lets you attach new behaviors to objects dynamically by placing these objects inside special wrapper objects (decorators) that contain the behaviors.

## Why use it?
- **Alternative to Subclassing:** If you have a class `Shape` and you want colored shapes and transparent shapes, you could subclass it: `ColoredShape`, `TransparentShape`. But what if you want a Colored AND Transparent shape? You'd need `ColoredTransparentShape`. This leads to a class explosion.
- **Dynamic Behavior:** Decorators allow you to add or remove responsibilities from an object at runtime, rather than being hardcoded at compile time.
- **Open-Closed Principle:** You can extend the behavior of a class without modifying the class itself.

## Analogy
Think about ordering coffee. You start with a basic coffee. You want milk? You don't ask the barista to invent a new "CoffeeWithMilk" bean. The barista takes your coffee and *decorates* it with milk. Want sugar? They decorate it with sugar. At the end, you still have a Coffee, but its price and ingredients have been dynamically modified.

## The Example (decorator.cpp)
We have a base interface `Shape` with a method `str()` that returns a description of the shape. We implement a concrete `Circle` and `Square`.

Now, we want to add *Color* and *Transparency* to these shapes without creating a `RedCircle`, `BlueCircle`, `HalfTransparentSquare` class for every exact combination.

In the **Good Design**, we create a `ColoredShape` decorator and a `TransparentShape` decorator. Both decorators take a generic `Shape` in their constructor and "wrap" it. The decorators implement the SAME `Shape` interface as the object they wrap, meaning a decorated shape IS a shape! When `str()` is called on the decorator, it first delegates to the wrapped shape's `str()`, then adds its own extra description (like color or transparency).
