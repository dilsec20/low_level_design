# Visitor Pattern

## What is it?
The Visitor pattern is a behavioral design pattern that lets you **add new operations to existing classes without modifying them**. You create a Visitor class that implements the operation for each target class, and the target classes accept the visitor through a method.

## Why use it?
- **Add Operations Without Modifying Classes:** When you have a stable class hierarchy and frequently need new operations, Visitor lets you add them externally.
- **Single Responsibility:** Each visitor encapsulates one specific operation (export, validation, rendering).
- **Double Dispatch:** The combination of visitor type + element type determines which method runs.

## When NOT to use it?
- If classes change frequently (adding new element types requires updating ALL visitors).
- For simple hierarchies where a virtual method does the job.

## Analogy
Think of a **tax inspector** visiting different businesses. The inspector (Visitor) applies different tax calculations for each business type (Restaurant, Shop, Factory). The businesses don't need to know tax logic — they just "accept" the inspector and the inspector does the work.

## The Example (visitor.cpp)
We have a document with different element types (TextElement, ImageElement, TableElement). We create visitors for different operations: `ExportVisitor` (exports to HTML), `WordCountVisitor` (counts words). Adding a new operation means creating a new visitor — no changes to existing element classes.
