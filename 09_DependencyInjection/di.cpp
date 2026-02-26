#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// BAD DESIGN (No Dependency Injection)
// ==========================================
namespace Bad {
    class EmailSender {
    public:
        void send(const string& to, const string& msg) {
            cout << "  📧 Email to " << to << ": " << msg << "\n";
        }
    };

    class NotificationService {
        EmailSender sender_;  // ← Hardcoded dependency! Created internally.
    public:
        void notify(const string& user, const string& msg) {
            // This class is FOREVER tied to EmailSender.
            // Can't use SMS. Can't mock for tests. Can't extend.
            sender_.send(user, msg);
        }
    };

    void demo() {
        NotificationService svc;
        svc.notify("alice@example.com", "Your order shipped!");
        // How do you test this WITHOUT actually sending an email? You can't! ❌
    }
}

// ==========================================
// GOOD DESIGN (With Dependency Injection)
// ==========================================
namespace Good {

    // --- Abstraction (Interface) ---
    class IMessageSender {
    public:
        virtual ~IMessageSender() = default;
        virtual void send(const string& to, const string& msg) = 0;
    };

    // --- Concrete Implementations ---
    class EmailSender : public IMessageSender {
    public:
        void send(const string& to, const string& msg) override {
            cout << "  📧 [Email] To: " << to << " → " << msg << "\n";
        }
    };

    class SMSSender : public IMessageSender {
    public:
        void send(const string& to, const string& msg) override {
            cout << "  📱 [SMS] To: " << to << " → " << msg << "\n";
        }
    };

    class PushNotificationSender : public IMessageSender {
    public:
        void send(const string& to, const string& msg) override {
            cout << "  🔔 [Push] To: " << to << " → " << msg << "\n";
        }
    };

    // --- Mock for Testing ---
    class MockSender : public IMessageSender {
    public:
        int sendCount = 0;
        string lastTo, lastMsg;

        void send(const string& to, const string& msg) override {
            sendCount++;
            lastTo = to;
            lastMsg = msg;
            cout << "  🧪 [Mock] Recorded send #" << sendCount << "\n";
        }
    };

    // --- Service with CONSTRUCTOR INJECTION ---
    class NotificationService {
        IMessageSender& sender_;  // ← Depends on ABSTRACTION, not concretion
    public:
        // The dependency is INJECTED through the constructor
        explicit NotificationService(IMessageSender& sender) : sender_(sender) {}

        void notify(const string& user, const string& msg) {
            cout << "  [Service] Sending notification...\n";
            sender_.send(user, msg);
        }
    };

    // --- Composite Sender (Multi-channel) ---
    class MultiChannelSender : public IMessageSender {
        vector<IMessageSender*> channels_;
    public:
        void addChannel(IMessageSender* channel) {
            channels_.push_back(channel);
        }

        void send(const string& to, const string& msg) override {
            for (auto* ch : channels_) {
                ch->send(to, msg);
            }
        }
    };
}

int main() {
    cout << "=== BAD DESIGN (No DI — tightly coupled) ===" << endl;
    Bad::demo();

    cout << "\n=== GOOD DESIGN (Constructor Injection) ===" << endl;

    // Production: Use real email sender
    cout << "\n--- Email Channel ---\n";
    Good::EmailSender email;
    Good::NotificationService emailService(email);
    emailService.notify("alice@example.com", "Your order shipped!");

    // Easy swap: Use SMS instead — ZERO changes to NotificationService!
    cout << "\n--- SMS Channel ---\n";
    Good::SMSSender sms;
    Good::NotificationService smsService(sms);
    smsService.notify("+1234567890", "Your OTP is 123456");

    // Testing: Use mock — no real email/SMS sent!
    cout << "\n--- Mock for Testing ---\n";
    Good::MockSender mock;
    Good::NotificationService testService(mock);
    testService.notify("test@test.com", "Test message");
    cout << "  ✅ Assert sendCount == " << mock.sendCount << "\n";

    // Advanced: Multi-channel notification
    cout << "\n--- Multi-Channel ---\n";
    Good::PushNotificationSender pushSender;
    Good::MultiChannelSender multi;
    multi.addChannel(&email);
    multi.addChannel(&sms);
    multi.addChannel(&pushSender);
    Good::NotificationService omniService(multi);
    omniService.notify("bob", "Important alert!");

    return 0;
}
