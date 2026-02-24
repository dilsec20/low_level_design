# Strategy Pattern

## What is it?
The Strategy pattern is a behavioral design pattern that allows you to define a family of algorithms, encapsulate each one as an object, and make them interchangeable. This lets the algorithm vary independently from the clients that use it.

## Why use it?
- **Avoid Giant `switch` or `if-else` Statements:** If you have an object that needs to do a specific task in many different ways (e.g., sorting an array, rendering text, routing a path), doing it all in one class leads to massive conditional blocks.
- **Open-Closed Principle:** You can add new strategies (algorithms) without changing the context code that uses them.
- **Runtime Swapping:** You can change the algorithm an object uses dynamically at runtime!

## Analogy
Think about a navigation app. You want to get from Point A to Point B. The app has different *strategies* for finding a route:
- Walking Strategy
- Driving Strategy
- Public Transit Strategy

You (the Context) tell the app what you want to do (navigate). You select the Strategy at runtime. The app executes the selected Strategy without needing to change its core map interface.

## The Example (strategy.cpp)
We want to render a list of items. Sometimes we want to render them as an HTML unordered list (`<ul><li>item</li></ul>`). Sometimes we want to render them as a Markdown list (`* item`).

In the **Bad Design**, we have an `enum` for the OutputFormat and a massive `switch` statement in our `TextProcessor` class. If we ever want to add a third format (like JSON), we have to open up the `TextProcessor` class and modify the `switch` statement.

In the **Good Design**, we define a `ListStrategy` interface with an `add_list_item()` method. We create concrete implementations: `HtmlListStrategy` and `MarkdownListStrategy`. The `TextProcessor` simply holds a pointer to a `ListStrategy` and asks it to do the work. The `TextProcessor` doesn't know *how* the formatting happens. We can even change the strategy dynamically at runtime!
