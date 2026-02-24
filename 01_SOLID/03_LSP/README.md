# Liskov Substitution Principle (LSP)

## What is it?
The Liskov Substitution Principle states that **objects of a superclass shall be replaceable with objects of its subclasses without breaking the application.**

In simpler terms: If you have a base class and a derived class, you should be able to substitute the base class pointer/reference with the derived class without the program behaving incorrectly.

## Why use it?
- **Predictability:** The behavior of the program remains correct no matter which subclass is used.
- **Robustness:** Prevents unexpected bugs when utilizing polymorphism.

## Identifying Violations of LSP
A common violation is when a subclass overrides a base class method to do something completely different, or throws an exception when the base class doesn't (like `NotImplementedException`).

The classic violation is the Square-Rectangle problem. In mathematics, a Square is a Rectangle. But in programming, treating a Square as a subclass of Rectangle leads to strange behaviors. Why? Because a Rectangle expects independent modifications to width and height, while a Square forces them to be equal.

## The Example (lsp.cpp)
In the **Bad Design**, `Square` derives from `Rectangle`. In a `Rectangle`, you can change the width without affecting the height. But a `Square` overrides the setters to change *both* width and height simultaneously. When a function expects a `Rectangle` and gets a `Square`, it sets the width to something, the height to something else, and calculates area. The result is wrong for a `Square`!

In the **Good Design**, we don't inherit `Square` from `Rectangle` if they behave fundamentally differently regarding mutation. Instead, we can use a factory pattern (a class that just builds `Rectangle`s that happen to have equal width/height) or just keep them completely separate classes if they have different behaviors.
