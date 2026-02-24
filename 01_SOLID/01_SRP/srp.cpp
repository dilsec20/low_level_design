#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// BAD DESIGN (Violating SRP)
// ==========================================
// The JournalBad class takes on two responsibilities:
// 1. Managing journal entries.
// 2. Persisting (saving) the journal to a file.
class JournalBad {
public:
    string title;
    vector<string> entries;

    JournalBad(const string& title) : title(title) {}

    // Responsibility 1: Managing entries
    void add_entry(const string& entry) {
        static int count = 1;
        entries.push_back(to_string(count++) + ": " + entry);
    }

    // Responsibility 2: Persistence (Saving to file)
    // This violates SRP because if we want to change how we save 
    // (e.g., save to a database instead of a file), we have to change the Journal class.
    void save(const string& filename) {
        ofstream ofs(filename);
        for (const auto& e : entries) {
            ofs << e << endl;
        }
    }
};


// ==========================================
// GOOD DESIGN (Following SRP)
// ==========================================

// Now, the Journal class only has ONE responsibility: managing entries.
class Journal {
public:
    string title;
    vector<string> entries;

    Journal(const string& title) : title(title) {}

    void add_entry(const string& entry) {
        static int count = 1;
        entries.push_back(to_string(count++) + ": " + entry);
    }
};

// We create a separate class for the persistence responsibility.
// If the saving mechanism changes, only this class needs to change.
class PersistenceManager {
public:
    static void save(const Journal& j, const string& filename) {
        ofstream ofs(filename);
        for (const auto& e : j.entries) {
            ofs << e << endl;
        }
        cout << "Journal saved to " << filename << endl;
    }
};

int main() {
    cout << "--- Demonstrating Single Responsibility Principle (SRP) ---" << endl;

    // Bad Design Usage
    JournalBad bad_journal("My Diary");
    bad_journal.add_entry("I ate a bug.");
    bad_journal.add_entry("I cried.");
    bad_journal.save("bad_diary.txt");

    // Good Design Usage
    Journal good_journal("My Better Diary");
    good_journal.add_entry("I learned C++ LLD.");
    good_journal.add_entry("I feel amazing.");

    // The journal doesn't save itself. We use the specialized PersistenceManager.
    PersistenceManager::save(good_journal, "good_diary.txt");

    return 0;
}
