# Iterator Pattern

## What is it?
The Iterator pattern is a behavioral design pattern that provides a way to **access the elements of a collection sequentially** without exposing the collection's underlying representation (array, linked list, tree, graph, etc.).

## Why use it?
- **Uniform Traversal:** Clients can iterate over any collection type using the same interface.
- **Encapsulation:** The collection's internal structure (array, tree, hash map) is hidden from the client.
- **Multiple Iterators:** You can have multiple independent iterators on the same collection simultaneously.

## Analogy
A **book's table of contents** is an iterator. It lets you access chapters sequentially without knowing how the book is physically printed (single volume, multi-volume, digital, etc.). You just call "next chapter."

## The Example (iterator.cpp)
We build an `IntRange` class that generates numbers in a range and a `BinaryTree` with in-order traversal. Both provide iterators that work with C++ range-based for loops, demonstrating how different data structures can present a uniform iteration interface.
