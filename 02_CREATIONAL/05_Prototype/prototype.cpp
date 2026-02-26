#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// BAD DESIGN (Manual copying with type checks)
// ==========================================
namespace Bad {
    struct Block {
        string type;  // "text" or "image" — we use strings to track type. Yikes.
        string content;
        int width = 0, height = 0;  // only used by images
    };

    struct Document {
        string title;
        vector<Block> blocks;

        // Manual deep copy — must know about every field and every type
        Document copy() const {
            Document doc;
            doc.title = title + " (Copy)";
            for (auto& b : blocks) {
                Block newBlock;
                newBlock.type = b.type;
                newBlock.content = b.content;
                if (b.type == "image") {  // type-checking with strings!
                    newBlock.width = b.width;
                    newBlock.height = b.height;
                }
                doc.blocks.push_back(newBlock);
            }
            return doc;
        }
    };

    void demo() {
        Document templateDoc;
        templateDoc.title = "Report Template";
        templateDoc.blocks.push_back({"text", "Introduction", 0, 0});
        templateDoc.blocks.push_back({"image", "logo.png", 200, 100});

        Document myDoc = templateDoc.copy();
        cout << "Copied: " << myDoc.title << " with " << myDoc.blocks.size() << " blocks\n";
        // Problem: Adding a new block type (e.g., TableBlock) means modifying copy()
        // Problem: String-based type checking is error-prone
    }
}

// ==========================================
// GOOD DESIGN (Prototype Pattern with clone())
// ==========================================
namespace Good {

    // --- Prototype Interface ---
    class Block {
    public:
        virtual ~Block() = default;
        virtual unique_ptr<Block> clone() const = 0;  // THE key method
        virtual void print() const = 0;
    };

    // --- Concrete Prototypes ---
    class TextBlock : public Block {
        string text_;
    public:
        explicit TextBlock(string text) : text_(move(text)) {}

        unique_ptr<Block> clone() const override {
            return make_unique<TextBlock>(*this);  // Copy constructor does the work
        }

        void print() const override {
            cout << "  [TEXT] " << text_ << "\n";
        }

        void setText(const string& t) { text_ = t; }
    };

    class ImageBlock : public Block {
        string filename_;
        int width_, height_;
    public:
        ImageBlock(string filename, int w, int h)
            : filename_(move(filename)), width_(w), height_(h) {}

        unique_ptr<Block> clone() const override {
            return make_unique<ImageBlock>(*this);  // Deep copy via copy constructor
        }

        void print() const override {
            cout << "  [IMAGE] " << filename_ << " (" << width_ << "x" << height_ << ")\n";
        }
    };

    class TableBlock : public Block {
        int rows_, cols_;
    public:
        TableBlock(int rows, int cols) : rows_(rows), cols_(cols) {}

        unique_ptr<Block> clone() const override {
            return make_unique<TableBlock>(*this);
        }

        void print() const override {
            cout << "  [TABLE] " << rows_ << "x" << cols_ << "\n";
        }
    };

    // --- Document that uses prototypes ---
    class Document {
        string title_;
        vector<unique_ptr<Block>> blocks_;
    public:
        explicit Document(string title) : title_(move(title)) {}

        void addBlock(unique_ptr<Block> block) {
            blocks_.push_back(move(block));
        }

        // Clone the ENTIRE document — we don't need to know concrete types!
        unique_ptr<Document> clone(const string& newTitle) const {
            auto doc = make_unique<Document>(newTitle);
            for (const auto& block : blocks_) {
                doc->addBlock(block->clone());  // Polymorphic cloning
            }
            return doc;
        }

        void print() const {
            cout << "Document: " << title_ << "\n";
            for (const auto& block : blocks_) {
                block->print();
            }
        }
    };
}

int main() {
    cout << "=== BAD DESIGN ===" << endl;
    Bad::demo();

    cout << "\n=== GOOD DESIGN (Prototype Pattern) ===" << endl;

    // Create a template document (the prototype)
    Good::Document templateDoc("Quarterly Report Template");
    templateDoc.addBlock(make_unique<Good::TextBlock>("Executive Summary"));
    templateDoc.addBlock(make_unique<Good::ImageBlock>("chart.png", 800, 600));
    templateDoc.addBlock(make_unique<Good::TableBlock>(5, 3));

    cout << "--- Original ---\n";
    templateDoc.print();

    // Clone it! We get a deep copy without knowing concrete block types.
    auto q1Report = templateDoc.clone("Q1 2024 Report");
    auto q2Report = templateDoc.clone("Q2 2024 Report");

    cout << "\n--- Cloned Documents ---\n";
    q1Report->print();
    q2Report->print();

    // Adding a new block type (e.g., CodeBlock) requires:
    //   1. Create CodeBlock with clone() method — that's it!
    //   2. ZERO changes to Document class → Open-Closed Principle ✓

    return 0;
}
