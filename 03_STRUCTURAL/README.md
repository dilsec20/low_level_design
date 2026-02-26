# Structural Design Patterns - Interview Questions & Practice

This README contains common interview questions and coding problems related to Structural patterns.

## 1. Theoretical Questions

**Q1: What is the primary difference between a Decorator and a Proxy?**
> **Answer:** 
> - A **Decorator** adds behavior to an object at runtime without changing its interface. Example: adding milk to a coffee object.
> - A **Proxy** provides a surrogate or placeholder for another object to control access to it. Example: a caching proxy that returns saved data, or an authentication proxy that checks permissions.

**Q2: What is the difference between an Adapter and a Facade?**
> **Answer:**
> - An **Adapter** takes an existing interface and changes it into another interface the client expects. It makes two incompatible systems work together.
> - A **Facade** simplifies a complex system by providing a new, high-level interface. It does not try to adapt an interface; it just hides complexity.

---

## 2. Coding Problem 1: Implement an Adapter

**Problem Statement:**
You have a modern application that expects logging to be done via an interface `ILogger` with a method `logInfo(string format)`.
However, you are forced to use a 3rd-party legacy library `OldThirdPartyLogger` that only supports `writeErrorLine(string error)` and `writeWarningLine(string warning)`. Write an Adapter so your modern app can use the 3rd-party logger.

### Expected Solution

```cpp
#include <iostream>

using namespace std;

// 1. The Target Interface (Modern Code)
class ILogger {
public:
    virtual void logInfo(const string& message) = 0;
};

// 2. The Adaptee (Legacy Code)
class OldThirdPartyLogger {
public:
    void writeWarningLine(const string& warning) {
        cout << "[WARNING]: " << warning << "\n";
    }
    void writeErrorLine(const string& error) {
        cout << "[ERROR]: " << error << "\n";
    }
};

// 3. The Adapter
class LoggerAdapter : public ILogger {
private:
    OldThirdPartyLogger* legacyLogger;
public:
    LoggerAdapter(OldThirdPartyLogger* legacyLogger) : legacyLogger(legacyLogger) {}

    // We implement the modern interface method...
    void logInfo(const string& message) override {
        // ...and translate it into commands the legacy logger understands!
        // In this case, we might adapt 'info' to 'warning' since that's all it supports.
        legacyLogger->writeWarningLine("Adapted Info: " + message);
    }
};

int main() {
    OldThirdPartyLogger* oldLogger = new OldThirdPartyLogger();
    ILogger* adapter = new LoggerAdapter(oldLogger);

    // The client only knows about logInfo()
    adapter->logInfo("Application started!");

    delete adapter;
    delete oldLogger;
    return 0;
}
```

---

## 3. Coding Problem 2: Decorator Runtime Wrapping

**Problem Statement:**
Demonstrate adding behaviors at runtime without creating a subclass explosion. You are asked to build a `TextFormatter` that prints text. Create decorators that can `Bold` and `Italicize` the text dynamically.

### Expected Solution

```cpp
#include <iostream>
#include <string>

// Component
class Text {
public:
    virtual std::string render() = 0;
    virtual ~Text() = default;
};

// Concrete Component
class PlainText : public Text {
private:
    std::string text;
public:
    PlainText(std::string text) : text(text) {}
    std::string render() override { return text; }
};

// Base Decorator
class TextDecorator : public Text {
protected:
    Text* wrapped;
public:
    TextDecorator(Text* text) : wrapped(text) {}
    std::string render() override { return wrapped->render(); }
};

// Concrete Decorator 1
class BoldDecorator : public TextDecorator {
public:
    BoldDecorator(Text* text) : TextDecorator(text) {}
    std::string render() override { return "<b>" + wrapped->render() + "</b>"; }
};

// Concrete Decorator 2
class ItalicDecorator : public TextDecorator {
public:
    ItalicDecorator(Text* text) : TextDecorator(text) {}
    std::string render() override { return "<i>" + wrapped->render() + "</i>"; }
};

int main() {
    Text* plain = new PlainText("hello world");
    Text* bolded = new BoldDecorator(plain);
    Text* italicBolded = new ItalicDecorator(bolded);

    std::cout << italicBolded->render() << std::endl; 
    // Output: <i><b>hello world</b></i>

    delete italicBolded;
    delete bolded;
    delete plain;
    return 0;
}
```

---

## 4. Additional Theoretical Questions

**Q3: What is the Composite pattern and when would you use it?**
> **Answer:** Composite lets you compose objects into tree structures and treat individual objects and groups uniformly. Use it when your data naturally forms a hierarchy (file systems, UI component trees, organization charts). Both leaves and containers implement the same interface, so client code doesn't need to distinguish between them.

**Q4: Explain the Bridge pattern. How does it prevent class explosion?**
> **Answer:** Bridge separates abstraction from implementation by using composition instead of inheritance. Without Bridge, M abstractions × N implementations = M×N classes. With Bridge, you only need M+N classes. Example: 3 shapes × 4 renderers = 12 classes without Bridge, but only 7 with Bridge.

**Q5: When would you use a Flyweight pattern?**
> **Answer:** When you have a huge number of similar objects consuming excessive memory. Flyweight separates **intrinsic state** (shared, stored in flyweight) from **extrinsic state** (unique, passed by caller). Common in game development (particles, tiles), text editors (character formatting), and caching systems.
