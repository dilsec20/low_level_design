# Mediator Pattern

## What is it?
The Mediator is a behavioral design pattern that reduces direct dependencies between objects by making them communicate through a **central mediator** instead of directly with each other. Objects no longer call each other; they notify the mediator, which handles coordination.

## Why use it?
- **Reduce Coupling:** Without mediator, N objects can have up to N×(N-1) direct dependencies. With mediator, each object only depends on the mediator.
- **Centralized Control:** Complex communication logic lives in one place instead of being scattered across objects.
- **Easier to Extend:** Adding a new component only requires updating the mediator, not all existing components.

## Analogy
Think of an **air traffic control tower**. Planes don't communicate directly with each other (chaos!). Instead, all planes communicate with the control tower (mediator), which coordinates takeoffs, landings, and routing.

## The Example (mediator.cpp)
We build a **chat room** application. Without mediator, each user would need references to all other users. With a ChatRoom mediator, users only know about the chat room, which handles message routing.
