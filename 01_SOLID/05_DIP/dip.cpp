#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;

// ==========================================
// SHARED DOMAIN LOGIC
// ==========================================
enum class Relationship { parent, child, sibling };

struct Person {
    string name;
};

// ==========================================
// BAD DESIGN (Violating DIP)
// ==========================================
// LOW-LEVEL MODULE (Stores data)
struct RelationshipsBad {
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person& parent, const Person& child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }
};

// HIGH-LEVEL MODULE (Processes data)
struct ResearchBad {
    // VIOLATION: The high-level module depends DIRECTLY on the low-level module.
    // It even accesses the internal implementation details (the 'relations' vector) directly!
    // If RelationshipsBad changes its storage to a Map or a Database, ResearchBad breaks entirely.
    ResearchBad(const RelationshipsBad& relbad) {
        auto& relations = relbad.relations;
        for (auto&& [first, rel, second] : relations) {
            if (first.name == "John" && rel == Relationship::parent) {
                cout << "John has a child called " << second.name << endl;
            }
        }
    }
};


// ==========================================
// GOOD DESIGN (Following DIP)
// ==========================================
// We introduce an ABSTRACTION (an interface).
// Both the high-level and low-level modules will depend on this.

struct RelationshipBrowser {
    virtual vector<Person> find_all_children_of(const string& name) = 0;
};

// LOW-LEVEL MODULE
// Now it implements the abstraction.
struct RelationshipsGood : RelationshipBrowser {
    vector<tuple<Person, Relationship, Person>> relations;

    void add_parent_and_child(const Person& parent, const Person& child) {
        relations.push_back({parent, Relationship::parent, child});
        relations.push_back({child, Relationship::child, parent});
    }

    // Implementing the abstract interface
    vector<Person> find_all_children_of(const string& name) override {
        vector<Person> result;
        for (auto&& [first, rel, second] : relations) {
            if (first.name == name && rel == Relationship::parent) {
                result.push_back(second);
            }
        }
        return result;
    }
};

// HIGH-LEVEL MODULE
struct ResearchGood {
    // GOOD: The high-level module now depends on the ABSTRACTION.
    // It doesn't know or care how 'browser' stores the data.
    ResearchGood(RelationshipBrowser& browser) {
        for (auto& child : browser.find_all_children_of("John")) {
            cout << "John has a child called " << child.name << endl;
        }
    }
};

int main() {
    Person parent{"John"};
    Person child1{"Chris"}, child2{"Matt"};

    cout << "--- Bad DIP Violation ---" << endl;
    RelationshipsBad rel_bad;
    rel_bad.add_parent_and_child(parent, child1);
    rel_bad.add_parent_and_child(parent, child2);
    
    // Research directly consumes the low-level concretions
    ResearchBad _research(rel_bad);

    
    cout << "\n--- Good DIP Fix ---" << endl;
    RelationshipsGood rel_good;
    rel_good.add_parent_and_child(parent, child1);
    rel_good.add_parent_and_child(parent, child2);

    // Research consumes the abstraction
    ResearchGood _research_better(rel_good);

    return 0;
}
