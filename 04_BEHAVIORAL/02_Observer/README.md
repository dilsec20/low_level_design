# Observer Pattern

## What is it?
The Observer is a behavioral design pattern that lets you define a subscription mechanism to notify multiple objects about any events that happen to the object they're observing.

It defines a one-to-many relationship. The object being watched is the **Subject** (or Observable). The objects watching are the **Observers**.

## Why use it?
- **Loose Coupling:** The Subject doesn't need to know anything about the concrete classes of its Observers. It only knows they implement a specific interface (e.g., `update()`). This means you can add new kinds of Observers without touching the Subject's code.
- **Event-Driven Architecture:** UI elements (buttons reacting to clicks), Model-View-Controller (views updating when the model changes), and pub/sub systems all rely heavily on the Observer pattern.

## Analogy
Think of a YouTube channel. You are the Viewer (Observer). The YouTube channel is the Creator (Subject).
Instead of you constantly refreshing the channel page every hour to see if a new video is posted (Poling - Bad Design), you hit the "Subscribe" button. By doing this, you've added yourself to a list. Now, whenever a new video drops, the Creator automatically sends out a notification to everyone on the list.

## The Example (observer.cpp)
We have a model representing a `Person`. A person has an `age`. We want to do something whenever the person's age changes (e.g., print a logging message, check if they can vote now, or update a UI element).

In the **Bad Design**, the `Person` class would have to call `Logger::log_age(...)` or `VotingRegistry::check(...)` directly inside its `set_age()` method. This tightly couples the Person class to all those external systems.

In the **Good Design**, we make `Person` an `Observable`. It keeps a list (vector) of `Observer*` pointers. Any class that implements the `Observer` interface can "subscribe" to the Person. 
When `Person::set_age()` is called, it iterates through its list of observers and calls `notify()`. Now, our `ConsoleLogger` can react to the change independently of the `Person` implementation!
