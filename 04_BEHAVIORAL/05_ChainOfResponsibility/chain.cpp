#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// BAD DESIGN (All checks in one function)
// ==========================================
namespace Bad {
    void handleRequest(const string& user, const string& token, int requestCount) {
        // Authentication check
        if (token.empty()) { cout << "  ❌ No auth token!\n"; return; }
        if (token != "valid_token") { cout << "  ❌ Invalid token!\n"; return; }

        // Rate limiting check
        if (requestCount > 100) { cout << "  ❌ Rate limit exceeded!\n"; return; }

        // Logging
        cout << "  [LOG] Request from " << user << "\n";

        // Business logic
        cout << "  ✅ Processing request for " << user << "\n";

        // Adding a new check (e.g., IP whitelist) means modifying this giant function. ❌
    }
}

// ==========================================
// GOOD DESIGN (Chain of Responsibility)
// ==========================================
namespace Good {

    struct Request {
        string user;
        string token;
        int requestCount;
    };

    // --- Handler Interface ---
    class Handler {
    protected:
        shared_ptr<Handler> next_;
    public:
        virtual ~Handler() = default;

        shared_ptr<Handler> setNext(shared_ptr<Handler> next) {
            next_ = move(next);
            return next_;  // enables chaining: a->setNext(b)->setNext(c)
        }

        virtual void handle(const Request& req) {
            if (next_) next_->handle(req);  // Pass to next handler
        }
    };

    // --- Concrete Handlers ---
    class AuthHandler : public Handler {
    public:
        void handle(const Request& req) override {
            if (req.token.empty()) {
                cout << "  🔐 [Auth] REJECTED: No token provided.\n";
                return;  // Stop the chain
            }
            if (req.token != "valid_token") {
                cout << "  🔐 [Auth] REJECTED: Invalid token.\n";
                return;
            }
            cout << "  🔐 [Auth] ✅ Token valid.\n";
            Handler::handle(req);  // Pass to next handler
        }
    };

    class RateLimitHandler : public Handler {
    public:
        void handle(const Request& req) override {
            if (req.requestCount > 100) {
                cout << "  ⏱️ [RateLimit] REJECTED: Too many requests (" << req.requestCount << ").\n";
                return;
            }
            cout << "  ⏱️ [RateLimit] ✅ Within limits (" << req.requestCount << "/100).\n";
            Handler::handle(req);
        }
    };

    class LoggingHandler : public Handler {
    public:
        void handle(const Request& req) override {
            cout << "  📝 [Log] Request from user: " << req.user << "\n";
            Handler::handle(req);
        }
    };

    class BusinessLogicHandler : public Handler {
    public:
        void handle(const Request& req) override {
            cout << "  ✅ [Business] Processing request for: " << req.user << "\n";
        }
    };
}

int main() {
    cout << "=== BAD DESIGN ===" << endl;
    Bad::handleRequest("Alice", "valid_token", 50);
    Bad::handleRequest("Bob", "", 10);

    cout << "\n=== GOOD DESIGN (Chain of Responsibility) ===" << endl;

    // Build the chain: Auth → RateLimit → Logging → BusinessLogic
    auto auth = make_shared<Good::AuthHandler>();
    auto rateLimit = make_shared<Good::RateLimitHandler>();
    auto logging = make_shared<Good::LoggingHandler>();
    auto business = make_shared<Good::BusinessLogicHandler>();

    auth->setNext(rateLimit);
    rateLimit->setNext(logging);
    logging->setNext(business);

    cout << "\n--- Valid Request ---\n";
    auth->handle({"Alice", "valid_token", 50});

    cout << "\n--- No Token ---\n";
    auth->handle({"Bob", "", 10});

    cout << "\n--- Rate Limited ---\n";
    auth->handle({"Charlie", "valid_token", 200});

    cout << "\n--- Invalid Token ---\n";
    auth->handle({"Dave", "bad_token", 5});

    // Adding IP whitelist? Just create IPWhitelistHandler and insert into chain.
    // ZERO changes to existing handlers! → Open-Closed Principle ✓

    return 0;
}
