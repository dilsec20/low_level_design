# Library Management System - Low Level Design

## Problem Statement
Design a Library Management System that supports:
- Adding/removing books
- Member registration
- Borrowing and returning books
- Search by title, author, or ISBN
- Fine calculation for late returns
- Book availability tracking

## Key Design Patterns Used
- **Singleton** — Library instance
- **Observer** — Notify members when a reserved book becomes available
- **Strategy** — Different search strategies (by title, author, ISBN)

## Classes
- `Library` — Central manager (Singleton)
- `Book` — ISBN, title, author, copies
- `Member` — Name, ID, borrowed books list
- `BorrowRecord` — Tracks borrow date, due date, return date
- `SearchStrategy` — Interface for different search methods
