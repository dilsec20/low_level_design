#include <iostream>
#include <vector>
#include <memory>

using namespace std;

// ==========================================
// RECEIVER 
// ==========================================
// The actual business logic object that performs the work.
struct BankAccount {
    int balance = 0;
    int overdraft_limit = -500;

    void deposit(int amount) {
        balance += amount;
        cout << "Deposited " << amount << ", balance is now " << balance << "\n";
    }

    bool withdraw(int amount) {
        if (balance - amount >= overdraft_limit) {
            balance -= amount;
            cout << "Withdrew " << amount << ", balance is now " << balance << "\n";
            return true;
        }
        cout << "Withdrawal of " << amount << " failed. Insufficient funds.\n";
        return false;
    }
};

// ==========================================
// THE COMMAND INTERFACE
// ==========================================
struct Command {
    virtual void call() = 0;
    virtual void undo() = 0;
    
    // Virtual destructor is important for polymorphic base classes
    virtual ~Command() = default; 
};


// ==========================================
// CONCRETE COMMAND
// ==========================================
// Encapsulates a request (deposit/withdraw), the receiver (the account), 
// and the arguments (amount).
struct BankAccountCommand : Command {
    BankAccount& account;

    enum Action { deposit, withdraw } action;
    int amount;
    
    // We need to track if the command actually succeeded, 
    // otherwise we shouldn't attempt to undo it!
    bool succeeded = false; 

    BankAccountCommand(BankAccount& account, Action action, int amount)
        : account(account), action(action), amount(amount) {}

    void call() override {
        switch (action) {
            case deposit:
                account.deposit(amount);
                succeeded = true; // Deposits always succeed in this simple model
                break;
            case withdraw:
                succeeded = account.withdraw(amount);
                break;
        }
    }

    // The logic to reverse the action!
    void undo() override {
        if (!succeeded) return; // Can't undo something that failed!

        switch (action) {
            case deposit:
                // Undo a deposit by withdrawing
                account.withdraw(amount);
                break;
            case withdraw:
                // Undo a withdrawal by depositing
                account.deposit(amount);
                break;
        }
    }
};

// ==========================================
// MAIN (The Invoker / Client)
// ==========================================
int main() {
    BankAccount account; // The Receiver
    vector<unique_ptr<BankAccountCommand>> command_history; // The Undo Stack

    cout << "--- Initial Account Status ---" << endl;
    cout << "Balance: " << account.balance << endl;

    cout << "\n--- Executing Commands ---" << endl;
    // Client creates commands and pushes them to history
    command_history.push_back(make_unique<BankAccountCommand>(account, BankAccountCommand::deposit, 100));
    command_history.push_back(make_unique<BankAccountCommand>(account, BankAccountCommand::withdraw, 50));
    command_history.push_back(make_unique<BankAccountCommand>(account, BankAccountCommand::withdraw, 500)); // Should fail

    // Execute them all!
    for (auto& cmd : command_history) {
        cmd->call();
    }

    cout << "\n--- Attempting to Undo Everything! ---" << endl;
    
    // To undo, we iterate through the history backwards!
    for (auto it = command_history.rbegin(); it != command_history.rend(); ++it) {
        (*it)->undo();
    }

    cout << "\n--- Final Account Status (Should be 0) ---" << endl;
    cout << "Balance: " << account.balance << endl;

    return 0;
}
