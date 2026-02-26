# Chain of Responsibility Pattern

## What is it?
The Chain of Responsibility is a behavioral design pattern that lets you pass requests along a chain of handlers. Each handler decides either to process the request or pass it to the next handler in the chain.

## Why use it?
- **Decoupled Senders and Receivers:** The sender doesn't need to know which handler will process the request.
- **Dynamic Chain:** You can add, remove, or reorder handlers at runtime.
- **Single Responsibility:** Each handler does one thing (logging, authentication, validation, etc.).

## Analogy
Think of a **tech support escalation**. Your request goes through Level 1 (basic FAQ), then Level 2 (technical), then Level 3 (engineering). Each level either resolves your issue or escalates it to the next level.

## The Example (chain.cpp)
We build a **middleware pipeline** for an HTTP server. Each handler in the chain handles one concern: Authentication → Rate Limiting → Logging → Business Logic. Bad design puts all checks in one giant function; good design chains independent handlers.
