#include <iostream>
#include <string>
#include <unordered_map>
#include <chrono>

using namespace std;

// ==========================================
// MODELS (Entities)
// ==========================================
struct UrlMapping {
    string short_url;
    string long_url;
    string user_id;
    int click_count = 0;
    // In a real app we'd add creation_date, expiration_date here
};


// ==========================================
// UTILITIES (Base62 Strategy)
// ==========================================
class Base62Encoder {
public:
    // A simple method to generate a quasi-random string of a given length
    static string generate_random_string(int length = 6) {
        const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        string random_string;
        for (int i = 0; i < length; ++i) {
            random_string += characters[rand() % 62];
        }
        return random_string;
    }
};

// ==========================================
// REPOSITORY (Following DIP)
// ==========================================
// The interface for our storage. 
class IUrlRepository {
public:
    virtual void save(const UrlMapping& mapping) = 0;
    virtual UrlMapping* get_by_short_url(const string& short_url) = 0;
    virtual bool alias_exists(const string& alias) = 0;
    virtual void increment_click(const string& short_url) = 0;
    virtual ~IUrlRepository() = default;
};

// A concrete implementation using memory (a hash map)
class InMemoryUrlRepository : public IUrlRepository {
private:
    unordered_map<string, UrlMapping> db;

public:
    void save(const UrlMapping& mapping) override {
        db[mapping.short_url] = mapping;
    }

    UrlMapping* get_by_short_url(const string& short_url) override {
        if (db.find(short_url) != db.end()) {
            return &db[short_url];
        }
        return nullptr;
    }

    bool alias_exists(const string& alias) override {
        return db.find(alias) != db.end();
    }

    void increment_click(const string& short_url) override {
        if (db.find(short_url) != db.end()) {
            db[short_url].click_count++;
        }
    }
};


// ==========================================
// CONTROLLER (The Facade/Service)
// ==========================================
class UrlShortenerService {
private:
    IUrlRepository& repository;
    const string BASE_DOMAIN = "http://tinylink.co/";

public:
    // Dependency Injection!
    UrlShortenerService(IUrlRepository& repo) : repository(repo) {
        srand(time(0)); // Seed random for Base62
    }

    // 1. Core feature: Shorten a URL
    string shorten(const string& long_url, const string& user_id = "anonymous", string custom_alias = "") {
        string short_hash;

        // Handle Custom Aliases
        if (!custom_alias.empty()) {
            if (repository.alias_exists(custom_alias)) {
                return "Error: Alias '" + custom_alias + "' is already registered!";
            }
            short_hash = custom_alias;
        } 
        // Handle Random Generation
        else {
            short_hash = Base62Encoder::generate_random_string();
            // Ensure collision does not happen (very low probability, but required logic)
            while (repository.alias_exists(short_hash)) {
                short_hash = Base62Encoder::generate_random_string();
            }
        }

        // Save to Database
        UrlMapping mapping{short_hash, long_url, user_id, 0};
        repository.save(mapping);

        return BASE_DOMAIN + short_hash;
    }

    // 2. Core Feature: Expand / Redirect
    string expand(const string& full_short_url) {
        // Extract the hash from the full URL string
        string short_hash = full_short_url.substr(BASE_DOMAIN.length());

        UrlMapping* mapping = repository.get_by_short_url(short_hash);
        
        if (mapping != nullptr) {
            // Analytics handling
            repository.increment_click(short_hash);
            
            // "Redirect"
            cout << "[Redirecting...] -> " << full_short_url << " resolves to " << mapping->long_url << "\n";
            return mapping->long_url;
        }

        return "Error: URL Not Found! (404)";
    }

    // 3. Extra Feature: Analytics
    void print_analytics(const string& full_short_url) {
        string short_hash = full_short_url.substr(BASE_DOMAIN.length());
        UrlMapping* mapping = repository.get_by_short_url(short_hash);
        
        if (mapping) {
            cout << "Analytics for " << full_short_url << ": " 
                 << mapping->click_count << " clicks.\n";
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    cout << "--- Initializing URL Shortener System ---\n";
    
    // Setup infrastructure
    InMemoryUrlRepository memory_db;
    UrlShortenerService service(memory_db);

    cout << "\n--- Generating URLs ---\n";
    // Shorten a random URL
    string short_url_1 = service.shorten("https://en.wikipedia.org/wiki/Design_Patterns");
    cout << "Random Short URL: " << short_url_1 << "\n";

    // Shorten with a custom alias
    string short_url_2 = service.shorten("https://google.com", "user123", "g");
    cout << "Custom Short URL: " << short_url_2 << "\n";

    // Attempting to steal an alias
    string short_url_failed = service.shorten("https://yahoo.com", "user456", "g");
    cout << short_url_failed << "\n";

    cout << "\n--- Simulating User Clicks ---\n";
    // User clicks the links
    service.expand(short_url_1);
    
    service.expand(short_url_2);
    service.expand(short_url_2); // Clicked twice!

    // Try to visit an invalid link
    cout << "Attempting to visit invalid link: " << service.expand("http://tinylink.co/invalid123") << "\n";

    cout << "\n--- Viewing Analytics ---\n";
    service.print_analytics(short_url_1);
    service.print_analytics(short_url_2);

    return 0;
}
