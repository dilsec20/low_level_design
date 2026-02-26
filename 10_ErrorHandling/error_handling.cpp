#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#include <fstream>

using namespace std;

// ==========================================
// 1. CUSTOM EXCEPTION HIERARCHY
// ==========================================
class AppException : public runtime_error {
public:
    explicit AppException(const string& msg) : runtime_error(msg) {}
};

class ValidationException : public AppException {
    string field_;
public:
    ValidationException(const string& field, const string& msg)
        : AppException("Validation error on '" + field + "': " + msg), field_(field) {}
    const string& getField() const { return field_; }
};

class NotFoundException : public AppException {
public:
    NotFoundException(const string& entity, int id)
        : AppException(entity + " with ID " + to_string(id) + " not found") {}
};

class InsufficientFundsException : public AppException {
    double available_, requested_;
public:
    InsufficientFundsException(double available, double requested)
        : AppException("Insufficient funds: available $" + to_string(available)
                      + ", requested $" + to_string(requested)),
          available_(available), requested_(requested) {}
};

// ==========================================
// 2. RAII RESOURCE MANAGEMENT
// ==========================================
// A simple RAII wrapper for a database connection
class DatabaseConnection {
    string connStr_;
    bool connected_ = false;
public:
    explicit DatabaseConnection(const string& connStr) : connStr_(connStr) {
        cout << "  🔗 [DB] Connecting to: " << connStr_ << "\n";
        connected_ = true;
    }

    ~DatabaseConnection() {
        if (connected_) {
            cout << "  🔗 [DB] Auto-disconnecting from: " << connStr_ << "\n";
            connected_ = false;
        }
        // Connection ALWAYS gets cleaned up, even if exception thrown!
    }

    void execute(const string& sql) {
        if (!connected_) throw AppException("Not connected!");
        cout << "  🔗 [DB] Executing: " << sql << "\n";
    }
};

// RAII file logger
class FileLogger {
    ofstream file_;
public:
    explicit FileLogger(const string& path) : file_(path, ios::app) {
        if (!file_.is_open()) throw AppException("Cannot open log file: " + path);
        cout << "  📝 [Log] File opened: " << path << "\n";
    }

    ~FileLogger() {
        cout << "  📝 [Log] File auto-closed.\n";
    }

    void log(const string& msg) {
        file_ << msg << "\n";
        cout << "  📝 [Log] Written: " << msg << "\n";
    }
};

// ==========================================
// 3. FAIL-FAST VALIDATION
// ==========================================
class BankAccount {
    string owner_;
    double balance_;
public:
    BankAccount(const string& owner, double initialBalance)
        : owner_(owner), balance_(initialBalance) {
        // Fail fast: validate at construction
        if (owner.empty())
            throw ValidationException("owner", "cannot be empty");
        if (initialBalance < 0)
            throw ValidationException("initialBalance", "cannot be negative");
        cout << "  🏦 Account created for " << owner_ << " with $" << balance_ << "\n";
    }

    void deposit(double amount) {
        if (amount <= 0)
            throw ValidationException("amount", "deposit must be positive");
        balance_ += amount;
        cout << "  💰 Deposited $" << amount << ", new balance: $" << balance_ << "\n";
    }

    void withdraw(double amount) {
        if (amount <= 0)
            throw ValidationException("amount", "withdrawal must be positive");
        if (amount > balance_)
            throw InsufficientFundsException(balance_, amount);
        balance_ -= amount;
        cout << "  💸 Withdrew $" << amount << ", new balance: $" << balance_ << "\n";
    }

    double getBalance() const { return balance_; }
    const string& getOwner() const { return owner_; }
};

// ==========================================
// 4. STRONG EXCEPTION SAFETY (Transactional)
// ==========================================
void transfer(BankAccount& from, BankAccount& to, double amount) {
    double fromOriginal = from.getBalance();

    try {
        from.withdraw(amount);
        to.deposit(amount);
        cout << "  ✅ Transfer complete: $" << amount
             << " from " << from.getOwner() << " to " << to.getOwner() << "\n";
    } catch (...) {
        // Rollback: restore the from account if deposit failed
        // This provides the STRONG exception guarantee
        cout << "  ⚠️ Transfer failed! Rolling back...\n";
        // In a real system: from.rollback(fromOriginal);
        throw;  // Re-throw so the caller handles it
    }
}

int main() {
    cout << "=== 1. Custom Exception Hierarchy ===" << endl;
    try {
        throw NotFoundException("User", 42);
    } catch (const AppException& e) {
        cout << "  Caught: " << e.what() << "\n";
    }

    cout << "\n=== 2. RAII Resource Management ===" << endl;
    try {
        DatabaseConnection db("postgres://localhost/myapp");
        db.execute("SELECT * FROM users");
        db.execute("UPDATE users SET active = true WHERE id = 1");
        // Even if an exception is thrown here, ~DatabaseConnection() runs automatically!
    } catch (const exception& e) {
        cout << "  Error: " << e.what() << "\n";
    }
    cout << "  (Connection was auto-closed by RAII)\n";

    cout << "\n=== 3. Fail-Fast Validation ===" << endl;
    try {
        BankAccount invalid("", 100);  // Should fail immediately
    } catch (const ValidationException& e) {
        cout << "  Caught: " << e.what() << "\n";
    }

    cout << "\n=== 4. Banking Transactions ===" << endl;
    try {
        BankAccount alice("Alice", 1000);
        BankAccount bob("Bob", 500);

        alice.withdraw(200);
        bob.deposit(200);

        cout << "\n  Attempting overdraft...\n";
        alice.withdraw(5000);  // Should throw InsufficientFundsException

    } catch (const InsufficientFundsException& e) {
        cout << "  💢 " << e.what() << "\n";
    } catch (const AppException& e) {
        cout << "  ❌ App error: " << e.what() << "\n";
    } catch (const exception& e) {
        cout << "  ❌ Unexpected: " << e.what() << "\n";
    }

    return 0;
}
