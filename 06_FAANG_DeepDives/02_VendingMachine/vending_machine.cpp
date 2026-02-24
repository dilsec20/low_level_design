#include <iostream>
#include <unordered_map>
#include <string>
#include <memory>

using namespace std;

// ==========================================
// PRODUCT
// ==========================================
struct Product {
    string name;
    int price; // in cents
    Product(string n = "", int p = 0) : name(n), price(p) {}
};

// ==========================================
// INVENTORY
// ==========================================
class Inventory {
private:
    unordered_map<string, pair<Product, int>> items; // code -> {Product, quantity}
public:
    void add_product(const string& code, const Product& product, int qty) {
        items[code] = {product, qty};
    }
    bool is_available(const string& code) const {
        auto it = items.find(code);
        return it != items.end() && it->second.second > 0;
    }
    Product get_product(const string& code) const {
        return items.at(code).first;
    }
    void reduce_stock(const string& code) {
        items[code].second--;
    }
    void display() const {
        cout << "--- Available Products ---\n";
        for (const auto& [code, item] : items) {
            cout << "[" << code << "] " << item.first.name 
                 << " - " << item.first.price << " cents"
                 << " (Stock: " << item.second << ")\n";
        }
    }
};

// Forward declaration
class VendingMachine;

// ==========================================
// STATE INTERFACE
// ==========================================
class IState {
public:
    virtual void insert_coin(VendingMachine& vm, int amount) = 0;
    virtual void select_product(VendingMachine& vm, const string& code) = 0;
    virtual void dispense(VendingMachine& vm) = 0;
    virtual void cancel(VendingMachine& vm) = 0;
    virtual ~IState() = default;
};

// Forward declare concrete states
class IdleState;
class HasMoneyState;
class DispensingState;

// ==========================================
// VENDING MACHINE (Context)
// ==========================================
class VendingMachine {
public:
    Inventory inventory;
    int balance = 0;
    string selected_product_code;
    
    shared_ptr<IState> idle_state;
    shared_ptr<IState> has_money_state;
    shared_ptr<IState> dispensing_state;
    shared_ptr<IState> current_state;

    VendingMachine(); // Defined after states

    void set_state(shared_ptr<IState> state) { current_state = state; }
    void insert_coin(int amount) { current_state->insert_coin(*this, amount); }
    void select_product(const string& code) { current_state->select_product(*this, code); }
    void dispense() { current_state->dispense(*this); }
    void cancel() { current_state->cancel(*this); }
};

// ==========================================
// CONCRETE STATES
// ==========================================
class IdleState : public IState {
public:
    void insert_coin(VendingMachine& vm, int amount) override {
        vm.balance += amount;
        cout << "Inserted " << amount << " cents. Balance: " << vm.balance << "\n";
        vm.set_state(vm.has_money_state);
    }
    void select_product(VendingMachine& vm, const string& code) override {
        cout << "Please insert coins first!\n";
    }
    void dispense(VendingMachine& vm) override {
        cout << "Please insert coins and select a product.\n";
    }
    void cancel(VendingMachine& vm) override {
        cout << "Nothing to cancel.\n";
    }
};

class HasMoneyState : public IState {
public:
    void insert_coin(VendingMachine& vm, int amount) override {
        vm.balance += amount;
        cout << "Inserted " << amount << " cents. Balance: " << vm.balance << "\n";
    }
    void select_product(VendingMachine& vm, const string& code) override {
        if (!vm.inventory.is_available(code)) {
            cout << "Product " << code << " is SOLD OUT!\n";
            return;
        }
        Product p = vm.inventory.get_product(code);
        if (vm.balance < p.price) {
            cout << "Insufficient balance! Need " << (p.price - vm.balance) << " more cents.\n";
            return;
        }
        vm.selected_product_code = code;
        vm.set_state(vm.dispensing_state);
        vm.dispense(); // Auto-trigger dispense
    }
    void dispense(VendingMachine& vm) override {
        cout << "Please select a product first.\n";
    }
    void cancel(VendingMachine& vm) override {
        cout << "Transaction cancelled. Returning " << vm.balance << " cents.\n";
        vm.balance = 0;
        vm.set_state(vm.idle_state);
    }
};

class DispensingState : public IState {
public:
    void insert_coin(VendingMachine& vm, int amount) override {
        cout << "Please wait, dispensing in progress...\n";
    }
    void select_product(VendingMachine& vm, const string& code) override {
        cout << "Please wait, dispensing in progress...\n";
    }
    void dispense(VendingMachine& vm) override {
        Product p = vm.inventory.get_product(vm.selected_product_code);
        vm.inventory.reduce_stock(vm.selected_product_code);
        
        int change = vm.balance - p.price;
        cout << "🎉 Dispensing: " << p.name << "!\n";
        if (change > 0) {
            cout << "💰 Returning change: " << change << " cents.\n";
        }
        
        vm.balance = 0;
        vm.selected_product_code = "";
        vm.set_state(vm.idle_state);
    }
    void cancel(VendingMachine& vm) override {
        cout << "Cannot cancel during dispensing!\n";
    }
};

// Constructor definition (after states are defined)
VendingMachine::VendingMachine() {
    idle_state = make_shared<IdleState>();
    has_money_state = make_shared<HasMoneyState>();
    dispensing_state = make_shared<DispensingState>();
    current_state = idle_state;
}

// ==========================================
// MAIN
// ==========================================
int main() {
    VendingMachine vm;

    // Stock the machine
    vm.inventory.add_product("A1", Product("Coca-Cola", 150), 5);
    vm.inventory.add_product("A2", Product("Pepsi", 125), 3);
    vm.inventory.add_product("B1", Product("Chips", 100), 2);

    vm.inventory.display();

    cout << "\n--- Transaction 1: Buy Coca-Cola ---\n";
    vm.insert_coin(100);
    vm.insert_coin(50);
    vm.select_product("A1"); // Exact change

    cout << "\n--- Transaction 2: Buy Chips with extra money ---\n";
    vm.insert_coin(100);
    vm.insert_coin(25);
    vm.select_product("B1"); // Gets 25 cents change

    cout << "\n--- Transaction 3: Cancel ---\n";
    vm.insert_coin(50);
    vm.cancel(); // Get refund

    cout << "\n--- Transaction 4: Insufficient funds ---\n";
    vm.insert_coin(25);
    vm.select_product("A2"); // Not enough!

    return 0;
}
