# Command Pattern

## What is it?
The Command is a behavioral design pattern that turns a request into a stand-alone object that contains all information about the request. 

This transformation lets you pass requests as a method argument, delay or queue a request's execution, and support undoable operations.

## Why use it?
- **Decoupling UI and Business Logic:** A button shouldn't know *how* to save a file. The button should just execute a `SaveCommand`. The Command object knows who to talk to in the business logic to actually save the file.
- **Undo/Redo:** Since the Command object wraps an action and its parameters, it can also easily store the *reverse* of that action, allowing you to build an undo stack.
- **Macros/Queuing:** You can queue up Command objects to be executed later, or group several commands into a "Composite Command" (Command Macro) that executes them all sequentially.

## Analogy
Think of ordering food at a restaurant. 
You (Client) tell the Waiter (Invoker) what you want. The Waiter writes an Order (Command) on a piece of paper. The Waiter places the order on the spindle. The Chef (Receiver) looks at the paper and cooks the meal. 
The Waiter doesn't need to know how to cook, they just pass the *encapsulated request* to someone who does. The paper order (the Command object) can be queued, prioritized, or even cancelled.

## The Example (command.cpp)
We have a Bank Account with a balance. We can deposit or withdraw money.

In the **Bad Design**, the client directly calls `account.deposit(100)` or `account.withdraw(50)`. Doing it this way makes an undo system very difficult. If someone calls `withdraw(50)`, how do we undo it later if we didn't record what just happened?

In the **Good Design**, we introduce a `Command` interface with `call()` and `undo()` methods.
We implement a `BankAccountCommand` that takes a reference to the account, the action (deposit/withdraw), and the amount.
When we want to deposit money, we don't call the bank account. We create a `BankAccountCommand` and `call()` it. We can then push this command onto a list (an Undo Stack). If the user presses "Undo", we pop the last command off the stack and run its `undo()` method, which automatically executes the reverse action (e.g., if the command was to deposit $50, the undo method will withdraw $50).
