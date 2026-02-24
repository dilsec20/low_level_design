#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Forward declaration
class Person;

// ==========================================
// THE OBSERVER (The "Subscriber")
// ==========================================
// Anything that wants to be notified must implement this interface.
struct PersonObserver {
    virtual void field_changed(Person& source, const string& field_name) = 0;
};

// ==========================================
// THE OBSERVABLE (The "Publisher" or "Subject")
// ==========================================
// Contains the boilerplate for adding/removing subscribers and notifying them.
template <typename T>
struct Observable {
    vector<PersonObserver*> observers; // List of subscribers

    void subscribe(PersonObserver* observer) {
        observers.push_back(observer);
    }

    void unsubscribe(PersonObserver* observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }

    // This is called when something happens!
    void notify(T& source, const string& name) {
        for (auto obs : observers) {
            obs->field_changed(source, name);
        }
    }
};

// ==========================================
// CONCRETE IMPLEMENTATIONS
// ==========================================

// Our actual Domain Object. It inherits from Observable so it can emit events!
class Person : public Observable<Person> {
private:
    int _age;
    string _name;
public:
    Person(string name, int age) : _name(name), _age(age) {}

    int get_age() const { return _age; }
    string get_name() const { return _name; }

    void set_age(const int age) {
        if (this->_age == age) return;

        this->_age = age;

        // The Magic happens here! 
        // We notify all subscribers that the "age" field changed.
        notify(*this, "age");
    }
};

// A concrete Observer that simply prints to the console when something changes.
struct ConsolePersonObserver : PersonObserver {
    void field_changed(Person& source, const string& field_name) override {
        cout << "[ConsoleObserver] Person's " << field_name 
             << " has changed to ";
        
        // We can inspect the source object to see the new value
        if (field_name == "age") cout << source.get_age();
        cout << ".\n";
    }
};

// Another totally independent observer!
struct VotingEligibilityChecker : PersonObserver {
    void field_changed(Person& source, const string& field_name) override {
        if (field_name == "age") {
            if (source.get_age() == 18) {
                cout << "[VotingChecker] Congratulations " << source.get_name() 
                     << ", you are exactly 18 and can now vote!\n";
            } else if (source.get_age() > 18) {
                 cout << "[VotingChecker] " << source.get_name() 
                     << " is eligible to vote.\n";
            } else {
                 cout << "[VotingChecker] " << source.get_name() 
                     << " is NOT eligible to vote yet.\n";
            }
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    Person p{"Alice", 16};
    
    // Create our observers
    ConsolePersonObserver console_logger;
    VotingEligibilityChecker vote_checker;

    // Alice subscribes to both!
    p.subscribe(&console_logger);
    p.subscribe(&vote_checker);

    cout << "--- It is Alice's Birthday! ---\n";
    p.set_age(17); 
    // ConsoleObserver fires. VotingChecker fires. No one modified the Person class to do this!

    cout << "\n--- Next Year! ---\n";
    p.set_age(18);

    cout << "\n--- Unsubscribing the Console Logger ---\n";
    p.unsubscribe(&console_logger);

    cout << "\n--- Next Year! ---\n";
    p.set_age(19); 
    // Only the VotingChecker fires now!

    return 0;
}
