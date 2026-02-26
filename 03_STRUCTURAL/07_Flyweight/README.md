# Flyweight Pattern

## What is it?
The Flyweight pattern is a structural design pattern that minimizes memory usage by **sharing common state** across many objects. Instead of each object storing all its data, you separate the **intrinsic state** (shared, immutable) from the **extrinsic state** (unique, context-dependent).

## Why use it?
- **Memory Optimization:** When you have millions of similar objects (particles in a game, characters in a text editor), sharing common data saves enormous amounts of memory.
- **Performance:** Less memory means better cache performance and fewer allocations.

## Key Concepts
- **Intrinsic State:** Data that is shared and doesn't change (e.g., tree type, texture, color). Stored inside the flyweight.
- **Extrinsic State:** Data that is unique per instance (e.g., position, health). Passed in from outside.
- **Flyweight Factory:** Creates and caches flyweight objects, returning existing ones when possible.

## Analogy
In a **forest simulation**, you have 10,000 trees. Each tree has a type (Oak, Pine, Birch) with a texture and mesh. Instead of storing the mesh 10,000 times, you store it ONCE per tree type and share it. Each tree instance only stores its unique position and age.

## The Example (flyweight.cpp)
We are building a **text editor**. Every character on screen could be an object. Instead of each character storing its font, size, and color (intrinsic), we share that data via flyweights. Each character only stores its position (extrinsic).
