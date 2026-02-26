# Low Level Design (LLD) — Complete C++ Curriculum

A comprehensive repository for mastering Low-Level Design and Object-Oriented Design in C++.  
Covers the full TakeUForward LLD syllabus + FAANG interview problems.

---

## 📂 Repository Structure

### 01_SOLID — SOLID Principles
- Single Responsibility (SRP)
- Open-Closed (OCP)
- Liskov Substitution (LSP)
- Interface Segregation (ISP)
- Dependency Inversion (DIP)

### 02_CREATIONAL — Creational Design Patterns
- Factory Method
- Singleton
- Builder
- Abstract Factory
- Prototype

### 03_STRUCTURAL — Structural Design Patterns
- Adapter
- Decorator
- Facade
- Composite
- Proxy
- Bridge
- Flyweight

### 04_BEHAVIORAL — Behavioral Design Patterns
- Strategy
- Observer
- Command
- State
- Chain of Responsibility
- Template Method
- Mediator
- Iterator
- Memento
- Visitor

### 05_PROJECTS — Mini LLD Projects
- Tic-Tac-Toe
- Parking Lot
- URL Shortener

### 06_FAANG_DeepDives — FAANG Interview Problems
- Elevator System
- Vending Machine
- BookMyShow
- Chess Game
- Snake & Ladder
- ATM Machine
- Library Management System
- Notification System

### 07_UML — UML Class Diagrams
- Class notation, relationships, multiplicity
- Worked example (Parking Lot)

### 08_Concurrency — Multithreading & Concurrency
- Race conditions, Mutex, Lock Guard
- Producer-Consumer pattern
- Thread-safe Singleton
- Deadlock prevention

### 09_DependencyInjection — Dependency Injection
- Constructor, Setter, Interface injection
- Mocking for tests
- Multi-channel composition

### 10_ErrorHandling — Exceptions & Error Handling
- Custom exception hierarchies
- RAII resource management
- Fail-fast validation
- Exception safety guarantees

---

## 🎯 Each topic includes:
- `README.md` — Theory (What/Why/Analogy) + Interview Q&A
- `.cpp` file — Bad Design → Good Design with runnable `main()`

## 🔧 To compile any example:
```bash
g++ -std=c++17 <file>.cpp -o test && ./test
```
