#include <iostream>
#include <string>

using namespace std;

// ==========================================
// MEYERS' SINGLETON
// ==========================================
// In C++11 and later, static local variables are guaranteed to be initialized thread-safely.
class Database {
private:
    // 1. Private constructor: no one can instantiate it directly.
    Database() {
        cout << "[Database Output]: Initializing real database! (This should only print ONCE)\n";
    }

public:
    // 2. Delete copy constructor and copy assignment operator.
    // This prevents cloning the singleton by copy or assignment.
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // 3. Static method that returns the SINGLE instance.
    static Database& get() {
        static Database instance;
        return instance;
    }

    string read_data(const string& key) {
        return "RealData for " + key;
    }
};


// ==========================================
// BAD DESIGN (Tight Coupling to Singleton)
// ==========================================
// The client class directly accesses the Singleton.
class ConfigManagerBad {
public:
    void print_setting() {
        // VIOLATION: Hidden dependency! 
        // We are glued to the real database. We can't test this with a fake database.
        string data = Database::get().read_data("Setting1");
        cout << "[ConfigManagerBad] Pulled data: " << data << endl;
    }
};


// ==========================================
// GOOD DESIGN (Dependency Injection)
// ==========================================
// Instead of depending directly on the Singleton, depend on an interface!

class IDatabase {
public:
    virtual string read_data(const string& key) = 0;
};

// Make our real Singleton implement the interface
class BetterDatabase : public IDatabase {
private:
    BetterDatabase() {}
public:
    BetterDatabase(const BetterDatabase&) = delete;
    BetterDatabase& operator=(const BetterDatabase&) = delete;

    static BetterDatabase& get() {
        static BetterDatabase instance;
        return instance;
    }

    string read_data(const string& key) override {
        return "RealData for " + key;
    }
};

// Create a Fake implementation for testing
class MockDatabase : public IDatabase {
public:
    string read_data(const string& key) override {
        return "FakeData for " + key;
    }
};

// Our client class now explicitly declares its dependency.
// It doesn't know or care if the database is a Singleton!
class ConfigManagerGood {
private:
    IDatabase& db; // Can point to the singleton or a mock
public:
    // Inject the dependency
    ConfigManagerGood(IDatabase& db) : db(db) {}

    void print_setting() {
        string data = db.read_data("Setting1");
        cout << "[ConfigManagerGood] Pulled data: " << data << endl;
    }
};

int main() {
    cout << "--- Basic Singleton Usage ---" << endl;
    auto& db1 = Database::get();
    auto& db2 = Database::get();
    
    // Address comparison shows they are the exact same object
    cout << "Address of db1: " << &db1 << "\n";
    cout << "Address of db2: " << &db2 << "\n";


    cout << "\n--- Bad Client ---" << endl;
    ConfigManagerBad bad;
    bad.print_setting(); // Forced to use real database.


    cout << "\n--- Good Client (Testable) ---" << endl;
    
    // Use real database in production code:
    BetterDatabase& singleton_db = BetterDatabase::get();
    ConfigManagerGood good_prod(singleton_db);
    cout << "In Production: ";
    good_prod.print_setting();

    // Use fake database in unit tests:
    MockDatabase mock_db;
    ConfigManagerGood good_test(mock_db);
    cout << "In Unit Tests: ";
    good_test.print_setting();

    return 0;
}
