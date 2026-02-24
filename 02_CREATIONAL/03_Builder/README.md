# Builder Pattern

## What is it?
The Builder Pattern is a creational design pattern that lets you construct complex objects step by step. The pattern allows you to produce different types and representations of an object using the same construction code.

## Why use it?
- **Telescoping Constructors:** Have you ever seen a constructor with 10 arguments? `Pizza(true, false, true, false, 12, "thin")`. This is unreadable and error-prone. The Builder pattern solves this.
- **Piecewise Construction:** Some objects shouldn't be fully instantiated in a single step. You might need to build it incrementally.
- **Fluent Interfaces:** It allows you to create readable, chainable configuration methods (e.g., `builder.addCheese().addPepperoni().build()`).

## The Example (builder.cpp)
We want to generate HTML code programmatically. An HTML snippet has a root element (`<ul>`) and child elements (`<li>`).

In the **Bad Design**, we try to construct the HTML string by manually concatenating strings and tags together. This is extremely brittle, hard to read, and difficult to format properly (with indentation). As the HTML gets more nested, string concatenation becomes a nightmare.

In the **Good Design**, we introduce an `HtmlBuilder` object. It provides methods like `add_child(name, text)` to build up a tree representation of the HTML. It handles the logic of formatting and indentation internally.
We also make the builder **fluent** by returning a reference to `*this` from the `add_child` method. This allows us to chain method calls beautifully: `builder.add_child("li", "hello").add_child("li", "world")`.
