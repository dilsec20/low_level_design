# Adapter Pattern

## What is it?
The Adapter Pattern is a structural design pattern that allows objects with incompatible interfaces to collaborate. It acts as a wrapper between two objects. It catches calls for one object and transforms them to format and interface recognizable by the second object.

## Why use it?
- **Legacy Code Integration:** You have an existing class with a great interface, but you need to use a new 3rd-party library (or an old legacy component) that has a completely different and incompatible interface. You cannot change the 3rd-party library code.
- **Reusability:** Instead of modifying existing code to match a needed interface, you write an adapter to bridge the gap, keeping both original systems intact and independent.

## Analogy
Think of a real-world power adapter. You travel from the US (120V, flat pins) to Europe (230V, round pins). You can't plug your laptop directly into the European wall socket. You need a travel adapter that accepts your flat US plug and has round European pins to go into the wall.

## The Example (adapter.cpp)
We have a modern rendering system that only knows how to draw `Line` objects (which are defined by two abstract `Point`s). 

However, we are given a legacy system composed of `VectorRectangle` objects (which are defined by x, y, width, and height). Our rendering system has no idea what a `VectorRectangle` is! 

In the **Adapter Approach**, instead of rewriting the rendering engine or the legacy rectangle class, we create an `LineToPointAdapter`. This adapter takes the line segments of the rectangle and converts them into individual points that our renderer can understand and draw.
