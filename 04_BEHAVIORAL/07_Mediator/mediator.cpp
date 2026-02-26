#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// ==========================================
// BAD DESIGN (Everyone knows everyone)
// ==========================================
namespace Bad {
    struct User {
        string name;
        vector<User*> contacts;  // Direct references to ALL other users

        void sendToAll(const string& msg) {
            for (auto* u : contacts) {
                cout << "  " << name << " → " << u->name << ": " << msg << "\n";
            }
            // Adding a new user means updating EVERY existing user's contact list! ❌
        }
    };

    void demo() {
        User alice{"Alice", {}};
        User bob{"Bob", {}};
        User charlie{"Charlie", {}};

        // N×(N-1) direct connections for N users!
        alice.contacts = {&bob, &charlie};
        bob.contacts = {&alice, &charlie};
        charlie.contacts = {&alice, &bob};

        alice.sendToAll("Hello everyone!");
    }
}

// ==========================================
// GOOD DESIGN (Mediator Pattern)
// ==========================================
namespace Good {

    class User;  // forward declaration

    // --- Mediator Interface ---
    class ChatMediator {
    public:
        virtual ~ChatMediator() = default;
        virtual void sendMessage(const string& msg, User* sender) = 0;
        virtual void addUser(shared_ptr<User> user) = 0;
    };

    // --- Colleague ---
    class User {
        string name_;
        ChatMediator* mediator_;
    public:
        User(string name, ChatMediator* mediator)
            : name_(move(name)), mediator_(mediator) {}

        string getName() const { return name_; }

        void send(const string& msg) {
            cout << "  📤 " << name_ << " sends: \"" << msg << "\"\n";
            mediator_->sendMessage(msg, this);  // Only talks to mediator!
        }

        void receive(const string& msg, const string& from) {
            cout << "  📥 " << name_ << " received from " << from << ": \"" << msg << "\"\n";
        }
    };

    // --- Concrete Mediator ---
    class ChatRoom : public ChatMediator {
        vector<shared_ptr<User>> users_;
    public:
        void addUser(shared_ptr<User> user) override {
            cout << "  [ChatRoom] " << user->getName() << " joined.\n";
            users_.push_back(move(user));
        }

        void sendMessage(const string& msg, User* sender) override {
            for (auto& user : users_) {
                if (user.get() != sender) {  // Don't send to self
                    user->receive(msg, sender->getName());
                }
            }
        }
    };
}

int main() {
    cout << "=== BAD DESIGN (Direct coupling) ===" << endl;
    Bad::demo();

    cout << "\n=== GOOD DESIGN (Mediator Pattern) ===" << endl;

    Good::ChatRoom chatRoom;

    auto alice = make_shared<Good::User>("Alice", &chatRoom);
    auto bob = make_shared<Good::User>("Bob", &chatRoom);
    auto charlie = make_shared<Good::User>("Charlie", &chatRoom);

    chatRoom.addUser(alice);
    chatRoom.addUser(bob);
    chatRoom.addUser(charlie);

    cout << "\n";
    alice->send("Hey everyone!");
    cout << "\n";
    bob->send("Hi Alice!");

    // Adding a new user is simple — no existing users need to change!
    auto dave = make_shared<Good::User>("Dave", &chatRoom);
    chatRoom.addUser(dave);
    cout << "\n";
    dave->send("I'm new here!");

    return 0;
}
