# Creational Design Patterns - Interview Questions & Practice

This README contains common interview questions and coding problems related to Creational patterns.

## 1. Theoretical Questions

**Q1: What is the difference between Simple Factory, Factory Method, and Abstract Factory?**
> **Answer:**
> - **Simple Factory:** Just a single class/method holding a massive switch statement to create objects.
> - **Factory Method:** Relies on inheritance. A base class declares a factory method (often virtual/pure virtual), and subclasses override it to create specific products.
> - **Abstract Factory:** A factory of factories. It groups object creation families together (e.g., `GUIFactory` creates `Button`, `Checkbox`, `Window`).

**Q2: What is the primary drawback of the Singleton pattern and why is it considered an anti-pattern by many?**
> **Answer:** Singletons introduce hidden global state into the application. This makes code very difficult to unit test (as state carries over between tests, and you can't easily swap the singleton out for a mock object). In modern C++, Dependency Injection is preferred over accessing Singletons directly.

---

## 2. Coding Problem 1: Thread-Safe Singleton

**Problem Statement:**
Implement a thread-safe Singleton in C++. Your code should prove that it is impossible to copy or assign the singleton instance.

### Expected Solution
The best and most modern way to build a thread-safe Singleton in C++ (C++11 and onwards) is "Meyers' Singleton":

```cpp
#include <iostream>

class DatabaseConnection {
private:
    // Private constructor
    DatabaseConnection() {
        std::cout << "Database Initialized\n";
    }

public:
    // Delete copy constructor and assignment operator to prevent duplicates
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;

    static DatabaseConnection& getInstance() {
        // C++11 guarantees that static local variables are initialized in a thread-safe manner
        static DatabaseConnection instance;
        return instance;
    }

    void query(const std::string& sql) {
        std::cout << "Executing: " << sql << "\n";
    }
};

int main() {
    auto& db1 = DatabaseConnection::getInstance();
    auto& db2 = DatabaseConnection::getInstance();
    // They are the exact same instance in memory
    return 0;
}
```

---

## 3. Coding Problem 2: Builder Pattern

**Problem Statement:**
You have a complex `Computer` class with many optional components (RAM size, SSD size, dedicated GPU, liquid cooling). Write a Builder to construct this object cleanly without relying on telescoping constructors.

### Expected Solution

```cpp
#include <iostream>
#include <string>

class Computer {
public:
    int ram;
    int ssd;
    bool hasGPU;
    bool hasLiquidCooling;
    
    void print() {
        std::cout << "PC: " << ram << "GB RAM, " << ssd << "GB SSD, GPU: " << hasGPU << "\n";
    }
};

class ComputerBuilder {
private:
    Computer computer;
public:
    ComputerBuilder() {
        // Set defaults
        computer.ram = 8;
        computer.ssd = 256;
        computer.hasGPU = false;
        computer.hasLiquidCooling = false;
    }

    // Fluent interfaces (returning reference)
    ComputerBuilder& setRam(int gigabytes) {
        computer.ram = gigabytes;
        return *this;
    }

    ComputerBuilder& setSSD(int gigabytes) {
        computer.ssd = gigabytes;
        return *this;
    }

    ComputerBuilder& addGPU() {
        computer.hasGPU = true;
        return *this;
    }

    ComputerBuilder& addLiquidCooling() {
        computer.hasLiquidCooling = true;
        return *this;
    }

    Computer build() {
        return computer;
    }
};

// Usage:
// Computer pc = ComputerBuilder().setRam(32).setSSD(1024).addGPU().build();
```
