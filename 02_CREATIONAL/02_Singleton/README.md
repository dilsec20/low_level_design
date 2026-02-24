# Singleton Pattern

## What is it?
The Singleton Pattern is a creational design pattern that lets you ensure that a class has only one instance, while providing a global access point to this instance.

## Why use it?
- **Shared Resource:** Some components make sense to only have one instance in the entire program. Examples: a database connection pool, a global settings configuration, or a logging instance.
- **Global Access:** You need an easy way to access these shared global objects from anywhere in the code.
- **Lazy Initialization:** (Sometimes) the object is only created when it's first requested, saving resources if it's never used.

## Problems with Singleton (Why it's often considered an Anti-Pattern)
- **Testing Nightmare:** Because it introduces global state, testing classes that depend on singletons is extremely difficult. The state of the singleton carries over between tests, causing unexpected failures. You can't easily replace it with a Mock object.
- **Hidden Dependencies:** If a class just grabs a Singleton out of thin air (e.g., `Database::get().query()`), its dependencies are hidden. It's much better to explicitly pass dependencies into constructors (Dependency Injection).

## The Example (singleton.cpp)
We discuss implementing a basic thread-safe Singleton in C++ using what is known as "Meyers' Singleton". Scott Meyers discovered that static local variables in C++11 (and later) are guaranteed to be initialized in a thread-safe manner.

In the **Bad Design**, we define a hardcoded Singleton database. If we have a `ConfigManager` class that uses this Singleton directly, testing `ConfigManager` becomes very painful because it's permanently glued to the real database.

In the **Good Design**, instead of relying on the Singleton *directly*, we abstract it. We put an interface (`IDatabase`) in front of our Singleton. Then, instead of classes directly accessing `Database::get()`, we pass the dependency via the constructor (Dependency Injection). This allows us to pass a `MockDatabase` during testing, leaving the actual Singleton only for production code!
