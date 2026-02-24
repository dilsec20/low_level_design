# Open-Closed Principle (OCP)

## What is it?
The Open-Closed Principle states that software entities (classes, modules, functions, etc.) should be **open for extension, but closed for modification**.

- **Open for Extension:** You should be able to add new functionality to an existing module.
- **Closed for Modification:** You should not have to change existing, tested, and working code to add this new functionality.

## Why use it?
- **Reduces Bugs:** Changing existing code can break existing features. Extending behavior through new code avoids touching the tested components.
- **Scalability:** It makes adding new features vastly easier and safer as your project grows.
- **Decoupling:** Forces you to use interfaces and abstract base classes, leading to loosely coupled systems.

## Identifying Violations of OCP
Look for long `if-else` or `switch` statements that check the type of an object to do something different. If you have to add a new `if` branch every time a new type is introduced, you are modifying existing code, thus violating OCP.

## The Example (ocp.cpp)
We have products with different traits like Color and Size.
In the **Bad Design**, we define a `ProductFilter` class that has methods like `filter_by_color` and `filter_by_size`. If we suddenly need to filter by Color AND Size, we have to open the `ProductFilter` class and write a new method `filter_by_color_and_size`. We modify the class every time a new criteria is introduced!

In the **Good Design**, we use the *Specification Pattern*. 
We define purely abstract interfaces: `Specification<T>` (which answers "does this item satisfy a condition?") and `Filter<T>` (which takes a list of items and a `Specification` and returns the filtered items). 
Now, if we need a new filter (e.g., filter by Weight), we just create a `WeightSpecification` class without touching the `Filter` class.
