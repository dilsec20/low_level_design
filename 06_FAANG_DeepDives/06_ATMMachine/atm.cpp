#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace std;

// ==========================================
// ATM Machine — State Pattern
// ==========================================

// --- Data Classes ---
class Card {
    string number_;
    string pin_;
public:
    Card(string number, string pin) : number_(move(number)), pin_(move(pin)) {}
    const string& getNumber() const { return number_; }
    bool validatePin(const string& pin) const { return pin_ == pin; }
};

class Account {
    string id_;
    double balance_;
public:
    Account(string id, double balance) : id_(move(id)), balance_(balance) {}

    double getBalance() const { return balance_; }

    bool withdraw(double amount) {
        if (amount > balance_) return false;
        balance_ -= amount;
        return true;
    }

    void deposit(double amount) { balance_ += amount; }
    const string& getId() const { return id_; }
};

// --- Forward Declaration ---
class ATM;

// --- State Interface ---
class ATMState {
public:
    virtual ~ATMState() = default;
    virtual void insertCard(ATM& atm, const Card& card) = 0;
    virtual void enterPin(ATM& atm, const string& pin) = 0;
    virtual void checkBalance(ATM& atm) = 0;
    virtual void withdraw(ATM& atm, double amount) = 0;
    virtual void deposit(ATM& atm, double amount) = 0;
    virtual void ejectCard(ATM& atm) = 0;
    virtual string name() const = 0;
};

// --- ATM Context ---
class ATM {
    unique_ptr<ATMState> state_;
    const Card* currentCard_ = nullptr;
    Account* currentAccount_ = nullptr;
    double cashAvailable_;

    // Simulated bank database
    unordered_map<string, Account*> accounts_;
public:
    ATM(double cash);
    void setState(unique_ptr<ATMState> s);
    void registerAccount(const string& cardNumber, Account* acc) { accounts_[cardNumber] = acc; }

    void insertCard(const Card& card) { state_->insertCard(*this, card); }
    void enterPin(const string& pin) { state_->enterPin(*this, pin); }
    void checkBalance() { state_->checkBalance(*this); }
    void withdraw(double amount) { state_->withdraw(*this, amount); }
    void deposit(double amount) { state_->deposit(*this, amount); }
    void ejectCard() { state_->ejectCard(*this); }

    void setCurrentCard(const Card* c) { currentCard_ = c; }
    const Card* getCurrentCard() const { return currentCard_; }

    Account* lookupAccount(const string& cardNum) {
        auto it = accounts_.find(cardNum);
        return it != accounts_.end() ? it->second : nullptr;
    }

    void setCurrentAccount(Account* acc) { currentAccount_ = acc; }
    Account* getCurrentAccount() const { return currentAccount_; }

    bool dispenseCash(double amount) {
        if (amount > cashAvailable_) return false;
        cashAvailable_ -= amount;
        return true;
    }

    void acceptCash(double amount) { cashAvailable_ += amount; }
};

// --- Concrete States ---
class IdleState : public ATMState {
public:
    string name() const override { return "Idle"; }

    void insertCard(ATM& atm, const Card& card) override {
        cout << "  💳 Card inserted: ****" << card.getNumber().substr(card.getNumber().size() - 4) << "\n";
        atm.setCurrentCard(&card);
        atm.setState(make_unique<class CardInsertedState>());
    }

    void enterPin(ATM& atm, const string&) override { cout << "  ⚠️ Insert card first.\n"; }
    void checkBalance(ATM& atm) override { cout << "  ⚠️ Insert card first.\n"; }
    void withdraw(ATM& atm, double) override { cout << "  ⚠️ Insert card first.\n"; }
    void deposit(ATM& atm, double) override { cout << "  ⚠️ Insert card first.\n"; }
    void ejectCard(ATM& atm) override { cout << "  ⚠️ No card inserted.\n"; }
};

class CardInsertedState : public ATMState {
public:
    string name() const override { return "CardInserted"; }

    void insertCard(ATM& atm, const Card&) override { cout << "  ⚠️ Card already inserted.\n"; }

    void enterPin(ATM& atm, const string& pin) override {
        if (atm.getCurrentCard()->validatePin(pin)) {
            cout << "  ✅ PIN correct. Authenticated.\n";
            auto* acc = atm.lookupAccount(atm.getCurrentCard()->getNumber());
            if (acc) {
                atm.setCurrentAccount(acc);
                atm.setState(make_unique<class AuthenticatedState>());
            } else {
                cout << "  ❌ Account not found. Ejecting card.\n";
                atm.setCurrentCard(nullptr);
                atm.setState(make_unique<IdleState>());
            }
        } else {
            cout << "  ❌ Wrong PIN. Ejecting card.\n";
            atm.setCurrentCard(nullptr);
            atm.setState(make_unique<IdleState>());
        }
    }

    void checkBalance(ATM& atm) override { cout << "  ⚠️ Enter PIN first.\n"; }
    void withdraw(ATM& atm, double) override { cout << "  ⚠️ Enter PIN first.\n"; }
    void deposit(ATM& atm, double) override { cout << "  ⚠️ Enter PIN first.\n"; }

    void ejectCard(ATM& atm) override {
        cout << "  💳 Card ejected.\n";
        atm.setCurrentCard(nullptr);
        atm.setState(make_unique<IdleState>());
    }
};

class AuthenticatedState : public ATMState {
public:
    string name() const override { return "Authenticated"; }

    void insertCard(ATM& atm, const Card&) override { cout << "  ⚠️ Session active.\n"; }
    void enterPin(ATM& atm, const string&) override { cout << "  ⚠️ Already authenticated.\n"; }

    void checkBalance(ATM& atm) override {
        auto* acc = atm.getCurrentAccount();
        cout << "  💰 Balance: $" << acc->getBalance() << "\n";
    }

    void withdraw(ATM& atm, double amount) override {
        auto* acc = atm.getCurrentAccount();
        if (!acc->withdraw(amount)) {
            cout << "  ❌ Insufficient funds.\n";
            return;
        }
        if (!atm.dispenseCash(amount)) {
            acc->deposit(amount);  // Rollback
            cout << "  ❌ ATM has insufficient cash.\n";
            return;
        }
        cout << "  💵 Dispensed $" << amount << ". New balance: $" << acc->getBalance() << "\n";
    }

    void deposit(ATM& atm, double amount) override {
        auto* acc = atm.getCurrentAccount();
        acc->deposit(amount);
        atm.acceptCash(amount);
        cout << "  💵 Deposited $" << amount << ". New balance: $" << acc->getBalance() << "\n";
    }

    void ejectCard(ATM& atm) override {
        cout << "  💳 Card ejected. Session ended.\n";
        atm.setCurrentCard(nullptr);
        atm.setCurrentAccount(nullptr);
        atm.setState(make_unique<IdleState>());
    }
};

// Constructor (needs IdleState defined)
ATM::ATM(double cash) : state_(make_unique<IdleState>()), cashAvailable_(cash) {}

void ATM::setState(unique_ptr<ATMState> s) {
    cout << "  [State] " << state_->name() << " → " << s->name() << "\n";
    state_ = move(s);
}

// --- Main ---
int main() {
    cout << "=== ATM Machine (State Pattern) ===" << endl;

    // Setup
    ATM atm(10000);  // ATM has $10,000 cash
    Account acc1("ACC001", 2500);
    Account acc2("ACC002", 100);
    Card card1("4111222233334444", "1234");
    Card card2("5555666677778888", "5678");

    atm.registerAccount("4111222233334444", &acc1);
    atm.registerAccount("5555666677778888", &acc2);

    // Scenario 1: Successful withdrawal
    cout << "\n--- Scenario 1: Normal Transaction ---\n";
    atm.insertCard(card1);
    atm.enterPin("1234");
    atm.checkBalance();
    atm.withdraw(500);
    atm.checkBalance();
    atm.ejectCard();

    // Scenario 2: Wrong PIN
    cout << "\n--- Scenario 2: Wrong PIN ---\n";
    atm.insertCard(card1);
    atm.enterPin("0000");

    // Scenario 3: Insufficient funds
    cout << "\n--- Scenario 3: Insufficient Funds ---\n";
    atm.insertCard(card2);
    atm.enterPin("5678");
    atm.checkBalance();
    atm.withdraw(500);  // Should fail (only $100)
    atm.deposit(200);
    atm.checkBalance();
    atm.ejectCard();

    return 0;
}
