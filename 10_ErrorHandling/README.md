# Exceptions & Error Handling in LLD

## Why is this important?
Good error handling is a key quality signal in LLD interviews. Interviewers look for:
- Meaningful error messages
- Proper use of exception hierarchies
- RAII for resource management
- Graceful failure vs crash

## Key Principles

### 1. Exception Hierarchies
Create domain-specific exceptions that extend `std::exception`:
```
std::exception
  └── AppException
        ├── ValidationException
        ├── NotFoundException
        ├── AuthorizationException
        └── DatabaseException
```

### 2. RAII (Resource Acquisition Is Initialization)
Resources (memory, files, locks, connections) should be tied to object lifetimes. Use smart pointers (`unique_ptr`, `shared_ptr`), `lock_guard`, and custom RAII wrappers.

### 3. Error Codes vs Exceptions
| Feature | Error Codes | Exceptions |
|---------|------------|------------|
| Control flow | Caller checks return value | Automatic propagation |
| Performance | No overhead | Some overhead on throw |
| Forgettable | Caller can ignore | Cannot be ignored |
| Use when | High-performance paths | Business logic errors |

### 4. Fail Fast
Validate inputs early and throw meaningful exceptions immediately rather than letting bad data propagate.

### 5. Exception Safety Guarantees
- **No-throw:** Function never throws (e.g., destructors, `swap`).
- **Strong:** If exception occurs, state is rolled back (transactional).
- **Basic:** If exception occurs, object is in a valid but unspecified state.

## The Example (error_handling.cpp)
Demonstrates custom exception hierarchy, RAII resource management, the fail-fast principle, and proper cleanup in a banking transaction scenario.
