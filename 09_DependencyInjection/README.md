# Dependency Injection (DI)

## What is it?
Dependency Injection is a technique where an object receives its dependencies from the outside rather than creating them internally. Instead of a class saying "I'll create what I need," it says "give me what I need."

## Why use it?
- **Testability:** You can inject mock/fake dependencies during testing.
- **Flexibility:** Swap implementations without changing the class (e.g., switch from MySQL to PostgreSQL).
- **Decoupling:** Classes depend on abstractions, not concretions (Dependency Inversion Principle).

## Three Types of DI

### 1. Constructor Injection (Preferred)
Dependencies are provided through the constructor. The object is fully initialized upon creation.
```cpp
class Service {
    IDatabase& db_;
public:
    Service(IDatabase& db) : db_(db) {}  // ← Injected!
};
```

### 2. Setter Injection
Dependencies are provided through setter methods after construction.
```cpp
class Service {
    IDatabase* db_ = nullptr;
public:
    void setDatabase(IDatabase* db) { db_ = db; }
};
```

### 3. Interface Injection
The dependency provides an injector method that the client must implement.
```cpp
class DatabaseAware {
public:
    virtual void injectDatabase(IDatabase& db) = 0;
};
```

## The Example (di.cpp)
We build a `NotificationService` that sends notifications. Without DI, it creates its own `EmailSender` internally (tightly coupled). With DI, we inject the sender — so we can easily switch to SMS, use a mock for testing, or compose multiple senders.
