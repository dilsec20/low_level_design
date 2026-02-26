# Multithreading & Concurrency in LLD

## Why is this important?
In FAANG interviews, many LLD problems involve concurrent access: parking lots with multiple entry gates, elevator systems handling simultaneous requests, booking systems preventing double-bookings. Understanding thread safety is essential.

## Key Concepts

### 1. Thread Safety
A class is **thread-safe** if it behaves correctly when accessed by multiple threads simultaneously, without requiring external synchronization.

### 2. Race Condition
Occurs when multiple threads access shared data simultaneously and at least one thread modifies it, leading to unpredictable results.

### 3. Mutex (Mutual Exclusion)
A lock that ensures only one thread can access a critical section at a time.

### 4. Deadlock
Two or more threads waiting for each other to release locks, causing all of them to hang forever.

### 5. Producer-Consumer Pattern
A classic concurrency pattern where producers add items to a shared buffer and consumers remove them, with proper synchronization.

---

## The Example (concurrency.cpp)
Demonstrates:
1. **Race condition** — unsafe counter increment
2. **Mutex fix** — using `std::mutex` and `lock_guard`
3. **Producer-Consumer** — using `condition_variable`
4. **Thread-safe Singleton** — Meyers' Singleton
5. **Deadlock example** and how to prevent it with `std::scoped_lock`
