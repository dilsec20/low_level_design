#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// BAD DESIGN (Hardcoded OS checks everywhere)
// ==========================================
namespace Bad {
    enum class OS { Windows, Mac };

    // Every time we create a widget, we check the OS type.
    // Adding a new OS (Linux) means modifying EVERY function.
    string createButton(OS os) {
        if (os == OS::Windows) return "[Windows Button]";
        else return "(Mac Button)";
    }

    string createCheckbox(OS os) {
        if (os == OS::Windows) return "[Windows Checkbox]";
        else return "(Mac Checkbox)";
    }

    void demo() {
        OS os = OS::Windows;
        cout << "Button: " << createButton(os) << endl;
        cout << "Checkbox: " << createCheckbox(os) << endl;
        // Problem: If we add Linux, we modify EVERY function.
        // Problem: If we add a new widget (Slider), we write another if-else.
    }
}

// ==========================================
// GOOD DESIGN (Abstract Factory)
// ==========================================
namespace Good {

    // --- Abstract Products ---
    class Button {
    public:
        virtual ~Button() = default;
        virtual string render() const = 0;
    };

    class Checkbox {
    public:
        virtual ~Checkbox() = default;
        virtual string render() const = 0;
    };

    // --- Concrete Products: Windows Family ---
    class WindowsButton : public Button {
    public:
        string render() const override {
            return "[========= Windows Button =========]";
        }
    };

    class WindowsCheckbox : public Checkbox {
    public:
        string render() const override {
            return "[X] Windows Checkbox";
        }
    };

    // --- Concrete Products: Mac Family ---
    class MacButton : public Button {
    public:
        string render() const override {
            return "( ~~~ Mac Button ~~~ )";
        }
    };

    class MacCheckbox : public Checkbox {
    public:
        string render() const override {
            return "(●) Mac Checkbox";
        }
    };

    // --- Abstract Factory ---
    // This is the "factory of factories" interface.
    // Each concrete factory produces an ENTIRE FAMILY of widgets.
    class GUIFactory {
    public:
        virtual ~GUIFactory() = default;
        virtual unique_ptr<Button> createButton() const = 0;
        virtual unique_ptr<Checkbox> createCheckbox() const = 0;
    };

    // --- Concrete Factory: Windows ---
    class WindowsFactory : public GUIFactory {
    public:
        unique_ptr<Button> createButton() const override {
            return make_unique<WindowsButton>();
        }
        unique_ptr<Checkbox> createCheckbox() const override {
            return make_unique<WindowsCheckbox>();
        }
    };

    // --- Concrete Factory: Mac ---
    class MacFactory : public GUIFactory {
    public:
        unique_ptr<Button> createButton() const override {
            return make_unique<MacButton>();
        }
        unique_ptr<Checkbox> createCheckbox() const override {
            return make_unique<MacCheckbox>();
        }
    };

    // --- Client Code ---
    // Notice: the client NEVER references WindowsButton, MacCheckbox, etc.
    // It only talks to the abstract interfaces. This is the power of Abstract Factory.
    void renderUI(const GUIFactory& factory) {
        auto button = factory.createButton();
        auto checkbox = factory.createCheckbox();

        cout << "Button:   " << button->render() << endl;
        cout << "Checkbox: " << checkbox->render() << endl;
    }

    // To add Linux support, we ONLY:
    //   1. Create LinuxButton, LinuxCheckbox (new files)
    //   2. Create LinuxFactory (new file)
    //   3. ZERO changes to existing code! → Open-Closed Principle ✓
}

int main() {
    cout << "=== BAD DESIGN (if-else everywhere) ===" << endl;
    Bad::demo();

    cout << "\n=== GOOD DESIGN: Windows Factory ===" << endl;
    Good::WindowsFactory winFactory;
    Good::renderUI(winFactory);

    cout << "\n=== GOOD DESIGN: Mac Factory ===" << endl;
    Good::MacFactory macFactory;
    Good::renderUI(macFactory);

    // Switching the ENTIRE look-and-feel is just ONE line:
    // GUIFactory& factory = macFactory; // or winFactory
    // renderUI(factory);

    return 0;
}
