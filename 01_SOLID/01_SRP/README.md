# Single Responsibility Principle (SRP)

## What is it?
The Single Responsibility Principle (SRP) states that a class should have one, and only one, reason to change. In other words, a class should only have one job or responsibility.

## Why use it?
- **Maintainability:** Classes are smaller and easier to understand.
- **Flexibility:** When a class only does one thing, changing how it does that one thing doesn't break other unrelated functionalities.
- **Testability:** A class with a single responsibility has fewer test cases, making it easier to unit test.
- **Reusability:** Smaller, highly-focused classes are more reusable across different parts of the application or even different projects.

## Analogy
Think of a Swiss Army Knife versus a specialized chef's knife. A Swiss Army Knife does many things (cutting, opening bottles, turning screws) but is mostly decent at them. A chef's knife does one thing—cutting food—and it does it exceptionally well. You want your classes to be like the chef's knife.

## Identifying Violations of SRP
Look for classes with words that combine responsibilities, like `ReportAndDatabaseManager`. Also, if you find yourself using words like "and" or "or" to describe what a class does, it's likely violating SRP.

## The Example (srp.cpp)
In our code example, we start with a `Journal` class that not only manages adding and removing entries (its primary responsibility) but also handles saving itself to a file. Saving to a file is a persistence responsibility, which could change independently of how a journal works (e.g., maybe later we want to save to a database instead).

To fix this, we extract the saving logic into a separate `PersistenceManager` class. Now, the `Journal` only handles journal entries, and the `PersistenceManager` only handles saving things.
