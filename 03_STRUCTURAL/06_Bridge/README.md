# Bridge Pattern

## What is it?
The Bridge pattern is a structural design pattern that separates an object's **abstraction** (what it does) from its **implementation** (how it does it), so that the two can vary independently. It does this by introducing a "bridge" — a composition relationship from the abstraction to the implementation.

## Why use it?
- **Avoid Class Explosion:** Without Bridge, combining M abstractions with N implementations creates M×N classes. With Bridge, you only need M+N classes.
- **Independent Extension:** You can add new abstractions (shapes) without touching implementations (renderers), and vice versa.
- **Runtime Switching:** You can swap the implementation at runtime.

## Analogy
Think of a **TV remote** (abstraction) and a **TV** (implementation). A basic remote and an advanced remote can both work with a Sony TV or Samsung TV. Without Bridge, you'd need: BasicSonyRemote, BasicSamsungRemote, AdvancedSonyRemote, AdvancedSamsungRemote (4 classes). With Bridge, you have 2 remotes + 2 TVs = 4 classes, but they compose freely.

## The Example (bridge.cpp)
We have **Shapes** (Circle, Square) and **Renderers** (VectorRenderer, RasterRenderer). Without Bridge, we'd need VectorCircle, RasterCircle, VectorSquare, RasterSquare (4 classes). With Bridge, shapes hold a reference to a renderer and delegate the drawing.
