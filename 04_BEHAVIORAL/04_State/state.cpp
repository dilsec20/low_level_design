#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// BAD DESIGN (Switch statements everywhere)
// ==========================================
namespace Bad {
    enum class State { Idle, HasMoney, Dispensing, OutOfStock };

    class VendingMachine {
        State state_ = State::Idle;
        int stock_ = 2;
        int money_ = 0;
    public:
        void insertMoney(int amount) {
            switch (state_) {
                case State::Idle:
                    money_ = amount;
                    state_ = State::HasMoney;
                    cout << "  Money inserted: $" << amount << "\n";
                    break;
                case State::HasMoney:
                    cout << "  Already has money!\n"; break;
                case State::Dispensing:
                    cout << "  Wait, dispensing...\n"; break;
                case State::OutOfStock:
                    cout << "  Out of stock! Returning money.\n"; break;
            }
            // Adding a new state? Modify EVERY method. ❌
        }

        void selectProduct() {
            switch (state_) {
                case State::Idle:
                    cout << "  Insert money first!\n"; break;
                case State::HasMoney:
                    cout << "  Product selected, dispensing...\n";
                    state_ = State::Dispensing;
                    break;
                case State::Dispensing:
                    cout << "  Already dispensing!\n"; break;
                case State::OutOfStock:
                    cout << "  Out of stock!\n"; break;
            }
        }

        void dispense() {
            switch (state_) {
                case State::Idle:
                case State::HasMoney:
                    cout << "  Can't dispense yet!\n"; break;
                case State::Dispensing:
                    stock_--;
                    money_ = 0;
                    cout << "  ✅ Product dispensed! Stock: " << stock_ << "\n";
                    state_ = (stock_ > 0) ? State::Idle : State::OutOfStock;
                    break;
                case State::OutOfStock:
                    cout << "  Out of stock!\n"; break;
            }
        }
    };
}

// ==========================================
// GOOD DESIGN (State Pattern)
// ==========================================
namespace Good {

    // Forward declaration
    class VendingMachine;

    // --- State Interface ---
    class State {
    public:
        virtual ~State() = default;
        virtual void insertMoney(VendingMachine& vm, int amount) = 0;
        virtual void selectProduct(VendingMachine& vm) = 0;
        virtual void dispense(VendingMachine& vm) = 0;
        virtual string name() const = 0;
    };

    // --- Context ---
    class VendingMachine {
        unique_ptr<State> state_;
        int stock_;
        int money_ = 0;
    public:
        VendingMachine(int stock);  // defined after state classes

        void setState(unique_ptr<State> state) {
            cout << "  [State Change] " << state_->name() << " → " << state->name() << "\n";
            state_ = move(state);
        }

        void insertMoney(int amount) { state_->insertMoney(*this, amount); }
        void selectProduct()         { state_->selectProduct(*this); }
        void dispense()              { state_->dispense(*this); }

        int getStock() const { return stock_; }
        void decrementStock() { stock_--; }
        void setMoney(int m) { money_ = m; }
        int getMoney() const { return money_; }
    };

    // --- Concrete States ---
    class IdleState : public State {
    public:
        string name() const override { return "Idle"; }

        void insertMoney(VendingMachine& vm, int amount) override {
            vm.setMoney(amount);
            cout << "  💰 Money inserted: $" << amount << "\n";
            vm.setState(make_unique<class HasMoneyState>());
        }

        void selectProduct(VendingMachine& vm) override {
            cout << "  ⚠️ Insert money first!\n";
        }

        void dispense(VendingMachine& vm) override {
            cout << "  ⚠️ Insert money and select product first!\n";
        }
    };

    class HasMoneyState : public State {
    public:
        string name() const override { return "HasMoney"; }

        void insertMoney(VendingMachine& vm, int amount) override {
            cout << "  ⚠️ Already has money. Select a product.\n";
        }

        void selectProduct(VendingMachine& vm) override {
            cout << "  🛒 Product selected! Dispensing...\n";
            vm.setState(make_unique<class DispensingState>());
        }

        void dispense(VendingMachine& vm) override {
            cout << "  ⚠️ Select a product first!\n";
        }
    };

    class OutOfStockState : public State {
    public:
        string name() const override { return "OutOfStock"; }

        void insertMoney(VendingMachine& vm, int amount) override {
            cout << "  🚫 Out of stock! Returning $" << amount << "\n";
        }

        void selectProduct(VendingMachine& vm) override {
            cout << "  🚫 Out of stock!\n";
        }

        void dispense(VendingMachine& vm) override {
            cout << "  🚫 Out of stock!\n";
        }
    };

    class DispensingState : public State {
    public:
        string name() const override { return "Dispensing"; }

        void insertMoney(VendingMachine& vm, int amount) override {
            cout << "  ⚠️ Please wait, dispensing in progress...\n";
        }

        void selectProduct(VendingMachine& vm) override {
            cout << "  ⚠️ Already dispensing!\n";
        }

        void dispense(VendingMachine& vm) override {
            vm.decrementStock();
            vm.setMoney(0);
            cout << "  ✅ Product dispensed! Remaining stock: " << vm.getStock() << "\n";

            if (vm.getStock() > 0) {
                vm.setState(make_unique<IdleState>());
            } else {
                vm.setState(make_unique<OutOfStockState>());
            }
        }
    };

    // Constructor definition (needs to know about IdleState)
    VendingMachine::VendingMachine(int stock)
        : state_(make_unique<IdleState>()), stock_(stock) {}
}

int main() {
    cout << "=== BAD DESIGN (Switch Hell) ===" << endl;
    Bad::VendingMachine badVM;
    badVM.insertMoney(5);
    badVM.selectProduct();
    badVM.dispense();
    badVM.insertMoney(5);
    badVM.selectProduct();
    badVM.dispense();
    badVM.insertMoney(5);  // Should say out of stock

    cout << "\n=== GOOD DESIGN (State Pattern) ===" << endl;
    Good::VendingMachine vm(2);

    cout << "\n--- Transaction 1 ---\n";
    vm.insertMoney(5);
    vm.selectProduct();
    vm.dispense();

    cout << "\n--- Transaction 2 ---\n";
    vm.insertMoney(5);
    vm.selectProduct();
    vm.dispense();

    cout << "\n--- Transaction 3 (Out of stock) ---\n";
    vm.insertMoney(5);  // Should refuse

    return 0;
}
