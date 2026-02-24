# Behavioral Design Patterns - Interview Questions & Practice

This README contains common interview questions and coding problems related to Behavioral patterns.

## 1. Theoretical Questions

**Q1: What is the difference between the Strategy pattern and the State pattern?**
> **Answer:** Both patterns are very similar structurally (they both involve a context delegating to an interface).
> - **Strategy:** Used when a client actively chooses *how* an operation should be done. The context (e.g., a sorting algorithm) rarely changes its own strategy unless explicitly told to.
> - **State:** Used to represent the internal state machines of an object. The State objects themselves often hold rules that dictate when the Context should transition to a *different* State.

**Q2: How does the Observer pattern solve tight coupling?**
> **Answer:** It solves tight coupling by having the Subject (Observable) maintain a list of abstract `Observer` interfaces rather than concrete classes. The Subject doesn't care what the observers are or what they do; it merely calls an `update()` method on them when an event occurs.

---

## 2. Coding Problem 1: Implement an Event Dispatcher (Observer)

**Problem Statement:**
Implement a simplified Event System (Publisher-Subscriber) where a `Button` class can notify multiple anonymous listeners when it is clicked.

### Expected Solution
Instead of raw pointers to heavy interfaces, modern C++ often uses `std::function` for observers, which is much cleaner.

```cpp
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

class Button {
private:
    // C++11 functional approach to Observers
    vector<function<void()>> onClickListeners;

public:
    void addOnClickListener(function<void()> listener) {
        onClickListeners.push_back(listener);
    }

    void click() {
        cout << "Button was clicked! Notifying listeners...\n";
        for (auto& listener : onClickListeners) {
            listener(); // Execute the callback
        }
    }
};

int main() {
    Button btn;

    // Listener 1 (Using completely separate logic)
    btn.addOnClickListener([]() {
        cout << " -> Listener 1: Logging click to database.\n";
    });

    // Listener 2
    btn.addOnClickListener([]() {
        cout << " -> Listener 2: Playing a UI click sound.\n";
    });

    // Fire the event!
    btn.click();

    return 0;
}
```

---

## 3. Coding Problem 2: Payment Gateway Interface (Strategy)

**Problem Statement:**
You are building an e-commerce checkout system. The user can pay via CreditCard or PayPal. The checkout logic should remain exactly the same regardless of the payment method chosen. Implement the Strategy pattern for this.

### Expected Solution

```cpp
#include <iostream>
#include <string>
#include <memory>

using namespace std;

// 1. The Strategy Interface
class PaymentStrategy {
public:
    virtual void pay(int amount) = 0;
    virtual ~PaymentStrategy() = default;
};

// 2. Concrete Strategies
class CreditCardStrategy : public PaymentStrategy {
private:
    string cardNumber;
public:
    CreditCardStrategy(string cn) : cardNumber(cn) {}
    void pay(int amount) override {
        cout << "Paid $" << amount << " using Credit Card ending in " 
             << cardNumber.substr(cardNumber.length() - 4) << "\n";
    }
};

class PayPalStrategy : public PaymentStrategy {
private:
    string email;
public:
    PayPalStrategy(string e) : email(e) {}
    void pay(int amount) override {
        cout << "Paid $" << amount << " using PayPal account: " << email << "\n";
    }
};

// 3. The Context (Shopping Cart)
class ShoppingCart {
private:
    unique_ptr<PaymentStrategy> paymentStrategy;
public:
    void setPaymentStrategy(unique_ptr<PaymentStrategy> strategy) {
        paymentStrategy = std::move(strategy);
    }

    void checkout(int totalAmount) {
        if (!paymentStrategy) {
            cout << "Please select a payment method first!\n";
            return;
        }
        cout << "Processing order for $" << totalAmount << "...\n";
        paymentStrategy->pay(totalAmount);
    }
};

int main() {
    ShoppingCart cart;

    // User chooses PayPal
    cart.setPaymentStrategy(make_unique<PayPalStrategy>("user@example.com"));
    cart.checkout(150);

    cout << "\n";

    // Same cart object, new order, user switches to Credit Card
    cart.setPaymentStrategy(make_unique<CreditCardStrategy>("1234567812349999"));
    cart.checkout(250);

    return 0;
}
```
