# Composite Pattern

## What is it?
The Composite pattern is a structural design pattern that lets you compose objects into **tree structures** and then work with these structures as if they were individual objects. It lets clients treat individual objects and compositions of objects uniformly.

## Why use it?
- **Tree Structures:** When your data naturally forms a tree (file systems, UI hierarchies, organization charts), Composite is the go-to pattern.
- **Uniform Interface:** Clients don't need to know whether they're dealing with a single leaf object or a complex branch with children — they use the same interface.
- **Recursive Composition:** A composite can contain other composites, enabling infinite nesting.

## Analogy
Think of a **file system**. A `File` is a leaf — it has a name and a size. A `Folder` is a composite — it contains files AND other folders. Yet both can answer "what is your size?" A folder just sums up all its children's sizes.

## The Example (composite.cpp)
We are building a **graphic editor** with shapes. A `Circle` is a simple shape. A `Group` is a collection of shapes. We want to move, resize, or draw either a single shape or an entire group using the same interface.

In the **Bad Design**, we maintain separate vectors for circles, rectangles, and groups — and duplicate logic for each type.

In the **Good Design**, everything implements a `Graphic` interface. A `Group` contains a vector of `Graphic*` children. Operations like `draw()` or `move()` are called uniformly on any element.
