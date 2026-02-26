#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

using namespace std;

// ==========================================
// Notification System — Observer + Strategy
// ==========================================

// --- Priority ---
enum class Priority { LOW, MEDIUM, HIGH, CRITICAL };
string priorityStr(Priority p) {
    switch(p) {
        case Priority::LOW: return "LOW";
        case Priority::MEDIUM: return "MEDIUM";
        case Priority::HIGH: return "HIGH";
        case Priority::CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}

// --- Notification ---
struct Notification {
    string title;
    string message;
    Priority priority;
};

// --- Channel Interface (Strategy) ---
class NotificationChannel {
public:
    virtual ~NotificationChannel() = default;
    virtual void send(const string& recipient, const Notification& notif) = 0;
    virtual string channelName() const = 0;
};

class EmailChannel : public NotificationChannel {
public:
    void send(const string& recipient, const Notification& notif) override {
        cout << "  📧 [Email → " << recipient << "] "
             << "[" << priorityStr(notif.priority) << "] "
             << notif.title << ": " << notif.message << "\n";
    }
    string channelName() const override { return "Email"; }
};

class SMSChannel : public NotificationChannel {
public:
    void send(const string& recipient, const Notification& notif) override {
        cout << "  📱 [SMS → " << recipient << "] "
             << "[" << priorityStr(notif.priority) << "] "
             << notif.title << "\n";
    }
    string channelName() const override { return "SMS"; }
};

class PushChannel : public NotificationChannel {
public:
    void send(const string& recipient, const Notification& notif) override {
        cout << "  🔔 [Push → " << recipient << "] "
             << notif.title << "\n";
    }
    string channelName() const override { return "Push"; }
};

// --- Logging Decorator ---
class LoggingChannelDecorator : public NotificationChannel {
    unique_ptr<NotificationChannel> wrapped_;
public:
    explicit LoggingChannelDecorator(unique_ptr<NotificationChannel> channel)
        : wrapped_(move(channel)) {}

    void send(const string& recipient, const Notification& notif) override {
        cout << "  📝 [LOG] Sending via " << wrapped_->channelName()
             << " to " << recipient << "...\n";
        wrapped_->send(recipient, notif);
        cout << "  📝 [LOG] ✅ Sent successfully.\n";
    }
    string channelName() const override { return wrapped_->channelName() + " (Logged)"; }
};

// --- User ---
class User {
    string name_;
    string email_;
    string phone_;
    vector<string> preferredChannels_;  // e.g., {"Email", "Push"}
public:
    User(string name, string email, string phone, vector<string> channels)
        : name_(move(name)), email_(move(email)), phone_(move(phone)),
          preferredChannels_(move(channels)) {}

    const string& getName() const { return name_; }
    const string& getEmail() const { return email_; }
    const string& getPhone() const { return phone_; }
    const vector<string>& getChannels() const { return preferredChannels_; }

    string getContactFor(const string& channel) const {
        if (channel == "Email") return email_;
        if (channel == "SMS") return phone_;
        if (channel == "Push") return name_;
        return name_;
    }
};

// --- Notification Service (Observer) ---
class NotificationService {
    unordered_map<string, unique_ptr<NotificationChannel>> channels_;
    unordered_map<string, vector<User*>> subscriptions_;  // event_type → users

public:
    void registerChannel(const string& name, unique_ptr<NotificationChannel> channel) {
        channels_[name] = move(channel);
        cout << "  ✅ Channel registered: " << name << "\n";
    }

    void subscribe(const string& eventType, User* user) {
        subscriptions_[eventType].push_back(user);
        cout << "  ✅ " << user->getName() << " subscribed to: " << eventType << "\n";
    }

    // Notify all subscribers of an event
    void notify(const string& eventType, const Notification& notif) {
        cout << "\n  🔔 Event: " << eventType << "\n";
        auto it = subscriptions_.find(eventType);
        if (it == subscriptions_.end()) {
            cout << "  No subscribers for this event.\n";
            return;
        }

        for (auto* user : it->second) {
            for (auto& channelName : user->getChannels()) {
                auto ch = channels_.find(channelName);
                if (ch != channels_.end()) {
                    string contact = user->getContactFor(channelName);
                    ch->second->send(contact, notif);
                }
            }
        }
    }

    // Direct notification to a specific user
    void sendDirect(User& user, const Notification& notif) {
        for (auto& channelName : user.getChannels()) {
            auto ch = channels_.find(channelName);
            if (ch != channels_.end()) {
                string contact = user.getContactFor(channelName);
                ch->second->send(contact, notif);
            }
        }
    }
};

int main() {
    cout << "=== Notification System ===" << endl;

    NotificationService service;

    // Register channels
    cout << "\n--- Registering Channels ---\n";
    service.registerChannel("Email", make_unique<EmailChannel>());
    service.registerChannel("SMS", make_unique<SMSChannel>());
    service.registerChannel("Push", make_unique<PushChannel>());

    // Create users with preferences
    User alice("Alice", "alice@example.com", "+1111111111", {"Email", "Push"});
    User bob("Bob", "bob@example.com", "+2222222222", {"Email", "SMS"});
    User charlie("Charlie", "charlie@example.com", "+3333333333", {"SMS"});

    // Subscribe to events
    cout << "\n--- Subscriptions ---\n";
    service.subscribe("ORDER_PLACED", &alice);
    service.subscribe("ORDER_PLACED", &bob);
    service.subscribe("ORDER_SHIPPED", &alice);
    service.subscribe("ORDER_SHIPPED", &charlie);
    service.subscribe("PAYMENT_FAILED", &bob);

    // Trigger events
    cout << "\n--- Triggering Events ---";
    service.notify("ORDER_PLACED", {
        "Order Confirmed",
        "Your order #1234 has been placed successfully.",
        Priority::MEDIUM
    });

    service.notify("ORDER_SHIPPED", {
        "Order Shipped",
        "Your order #1234 has been shipped!",
        Priority::HIGH
    });

    service.notify("PAYMENT_FAILED", {
        "Payment Failed",
        "Your payment for order #5678 has failed. Please retry.",
        Priority::CRITICAL
    });

    // Direct notification
    cout << "\n--- Direct Notification ---\n";
    service.sendDirect(alice, {
        "Welcome!",
        "Thanks for joining our platform.",
        Priority::LOW
    });

    return 0;
}
