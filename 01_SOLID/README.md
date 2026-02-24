# SOLID Principles - Interview Questions & Practice

This README contains common interview questions and coding problems related to the SOLID principles.

## 1. Theoretical Questions

**Q1: What are the SOLID principles and why do we use them?**
> **Answer:** 
> SOLID is an acronym for 5 design principles intended to make software designs more understandable, flexible, and maintainable.
> - **S**ingle Responsibility Principle: A class should have one reason to change.
> - **O**pen/Closed Principle: Open for extension, closed for modification.
> - **L**iskov Substitution Principle: Subtypes must be substitutable for their base types.
> - **I**nterface Segregation Principle: Clients shouldn't be forced to depend on methods they don't use.
> - **D**ependency Inversion Principle: High-level modules should not depend on low-level modules; both should depend on abstractions.

**Q2: Which SOLID principle is violated when you have to modify a class every time a new feature is added?**
> **Answer:** The Open/Closed Principle (OCP). The class is not "closed for modification". You should instead extend behavior through polymorphism or interfaces.

---

## 2. Coding Problem 1: Fix the SRP Violation

**Problem Statement:**
You are given a `User` class that handles both user properties (name, email) and database operations (saving to the db). This violates SRP. Refactor the code so it adheres to SRP.

### Bad Code (Given in Interview)
```cpp
class User {
public:
    string name;
    string email;
    
    User(string n, string e) : name(n), email(e) {}
    
    void saveToDatabase() {
        cout << "Saving " << name << " to database..." << endl;
        // DB connection logic...
    }
};
```

### Expected Solution
Create a separate repository class that handles the database operations for a User.
```cpp
// Responsibility 1: Managing User Data
class User {
public:
    string name;
    string email;
    User(string n, string e) : name(n), email(e) {}
};

// Responsibility 2: Managing Database Operations
class UserRepository {
public:
    void save(const User& user) {
        cout << "Saving " << user.name << " to database..." << endl;
        // DB connection logic...
    }
};
```

---

## 3. Coding Problem 2: Liskov Substitution Principle (LSP)

**Problem Statement:**
Why is it a bad idea for a `Square` class to inherit from a `Rectangle` class? Write code to demonstrate the problem.

### Solution / Explanation Code
If `Square` inherits from `Rectangle`, a function that takes a `Rectangle*` might naturally assume changing the width won't affect the height. Setting the width on a `Square` usually artificially alters the height too, breaking the base class expectation.

```cpp
class Rectangle {
protected:
    int width, height;
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    int getArea() { return width * height; }
};

class Square : public Rectangle {
public:
    void setWidth(int w) override { width = height = w; }
    void setHeight(int h) override { height = width = h; }
};

void testArea(Rectangle& r) {
    r.setWidth(5);
    r.setHeight(4);
    // If r is a Square, this will return 16 (since height set width to 4 as well).
    // An assumption about Rectangles is broken!
}
```
*To fix this, `Square` and `Rectangle` should not inherit from each other; they can both implement a `Shape` interface instead.*
