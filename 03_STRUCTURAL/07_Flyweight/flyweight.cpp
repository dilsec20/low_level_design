#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// ==========================================
// BAD DESIGN (Every character stores everything)
// ==========================================
namespace Bad {
    struct Character {
        char ch;
        string fontFamily;  // "Arial" — duplicated for EVERY character!
        int fontSize;       // 12 — duplicated for EVERY character!
        string color;       // "black" — duplicated for EVERY character!
        int x, y;           // position (unique per character)
    };

    void demo() {
        vector<Character> document;
        string text = "Hello World! This is a long document with thousands of characters...";
        for (int i = 0; i < (int)text.size(); ++i) {
            // Every single character stores "Arial", 12, "black" — massive waste!
            document.push_back({text[i], "Arial", 12, "black", i * 10, 0});
        }
        cout << "  Bad: " << document.size() << " characters, each storing font data.\n";
        cout << "  Memory per char: ~" << sizeof(Character) << " bytes (includes strings)\n";
        cout << "  Total wasted font/color copies: " << document.size() << " × 3 fields!\n";
    }
}

// ==========================================
// GOOD DESIGN (Flyweight Pattern)
// ==========================================
namespace Good {

    // --- Flyweight (Shared Intrinsic State) ---
    // This is the SHARED data. Many characters point to the same CharacterStyle.
    class CharacterStyle {
        string fontFamily_;
        int fontSize_;
        string color_;
    public:
        CharacterStyle(string font, int size, string color)
            : fontFamily_(move(font)), fontSize_(size), color_(move(color)) {}

        void render(char ch, int x, int y) const {
            // In a real app, this would use the font/color to render the character
            cout << "  '" << ch << "' at (" << x << "," << y << ") "
                 << "[" << fontFamily_ << " " << fontSize_ << "px " << color_ << "]\n";
        }

        string getKey() const {
            return fontFamily_ + "_" + to_string(fontSize_) + "_" + color_;
        }
    };

    // --- Flyweight Factory ---
    // Creates and caches flyweight objects. If a style already exists, reuse it.
    class StyleFactory {
        unordered_map<string, shared_ptr<CharacterStyle>> cache_;
    public:
        shared_ptr<CharacterStyle> getStyle(const string& font, int size, const string& color) {
            string key = font + "_" + to_string(size) + "_" + color;
            if (cache_.find(key) == cache_.end()) {
                cout << "  [Factory] Creating NEW style: " << key << "\n";
                cache_[key] = make_shared<CharacterStyle>(font, size, color);
            }
            return cache_[key];
        }

        int uniqueStyles() const { return cache_.size(); }
    };

    // --- Context (Extrinsic State) ---
    // Each character only stores its UNIQUE data + a shared pointer to its style.
    struct Character {
        char ch;
        int x, y;                          // Extrinsic: unique per character
        shared_ptr<CharacterStyle> style;  // Intrinsic: SHARED across many characters

        void render() const {
            style->render(ch, x, y);
        }
    };
}

int main() {
    cout << "=== BAD DESIGN (No Flyweight) ===" << endl;
    Bad::demo();

    cout << "\n=== GOOD DESIGN (Flyweight Pattern) ===" << endl;

    Good::StyleFactory factory;

    // Simulate a document with different formatting
    vector<Good::Character> document;
    string text = "Hello World!";

    // "Hello " in Arial 12 black
    auto normalStyle = factory.getStyle("Arial", 12, "black");
    for (int i = 0; i < 6; ++i) {
        document.push_back({text[i], i * 10, 0, normalStyle});
    }

    // "World" in Arial 16 red (bold heading)
    auto headingStyle = factory.getStyle("Arial", 16, "red");
    for (int i = 6; i < 11; ++i) {
        document.push_back({text[i], i * 10, 0, headingStyle});
    }

    // "!" in Arial 12 black (same as normal — REUSED, not recreated!)
    auto reuseStyle = factory.getStyle("Arial", 12, "black");
    document.push_back({text[11], 110, 0, reuseStyle});

    cout << "\nRendering document:\n";
    for (const auto& ch : document) {
        ch.render();
    }

    cout << "\n  Total characters: " << document.size() << "\n";
    cout << "  Unique styles created: " << factory.uniqueStyles() << "\n";
    cout << "  Memory saved: " << document.size() - factory.uniqueStyles()
         << " duplicate style objects avoided!\n";

    return 0;
}
