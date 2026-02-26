# Proxy Pattern

## What is it?
The Proxy pattern is a structural design pattern that provides a **substitute or placeholder** for another object. A proxy controls access to the original object, allowing you to add logic before or after the request reaches it.

## Why use it?
- **Lazy Initialization (Virtual Proxy):** Delay creating an expensive object until it's actually needed (e.g., load a large image only when displayed).
- **Access Control (Protection Proxy):** Check permissions before forwarding a request.
- **Logging/Caching (Smart Proxy):** Add cross-cutting concerns like logging, caching, or rate-limiting without modifying the real object.
- **Remote Proxy:** Represent a remote object locally (e.g., RPC stubs).

## Analogy
A **credit card** is a proxy for your bank account. It has the same interface (you can pay with it), but it adds extra logic: it checks your balance, verifies your PIN, logs the transaction, and then forwards the payment.

## The Example (proxy.cpp)
We have an `Image` interface. `RealImage` loads a huge file from disk (expensive). We create a `ProxyImage` that delays loading until `display()` is actually called (lazy initialization). We also show a **Protection Proxy** that checks user roles before allowing access.
