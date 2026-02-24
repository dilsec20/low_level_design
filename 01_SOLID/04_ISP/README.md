# Interface Segregation Principle (ISP)

## What is it?
The Interface Segregation Principle (ISP) states that **no client should be forced to depend on methods it does not use.**

In other words, you shouldn't create "fat" or "bloated" interfaces that have too many methods. Instead, break them down into smaller, highly-cohesive, client-specific interfaces.

## Why use it?
- **Decoupling:** Prevents changes in one part of the system from affecting completely unrelated parts.
- **Ease of Use:** Smaller interfaces are easier to understand and implement.
- **Avoiding Dummy Implementations:** You won't have to write empty or exception-throwing methods just to satisfy an overly large interface.

## Identifying Violations of ISP
Look for classes that implement interfaces but leave half the methods empty or throw a "Not Implemented" exception. This is a clear sign that the interface they are forced to implement is too broad.

## The Example (isp.cpp)
Imagine an office machine that can print, scan, and fax.

In our **Bad Design**, we declare an `IMachine` interface with `print()`, `scan()`, and `fax()` methods. We have a `MultiFunctionPrinter` that implements all three perfectly.
But what if we just have an old, cheap `SimplePrinter`? We are forced to implement `scan()` and `fax()`, even if they do nothing or throw errors. This violates ISP.

In our **Good Design**, we segregate the interfaces. We create `IPrinter`, `IScanner`, and `IFaxer`.
The `SimplePrinter` now only implements `IPrinter`. It doesn't even know about scanning and faxing!
The modern `MultiFunctionPrinter` implements *both* `IPrinter` and `IScanner`.

By keeping interfaces segregated, clients only implement exactly what they need.
