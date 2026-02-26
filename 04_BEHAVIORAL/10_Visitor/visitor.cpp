#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// ==========================================
// GOOD DESIGN (Visitor Pattern)
// ==========================================

// Forward declarations for double dispatch
class TextElement;
class ImageElement;
class TableElement;

// --- Visitor Interface ---
class DocumentVisitor {
public:
    virtual ~DocumentVisitor() = default;
    virtual void visit(const TextElement& elem) = 0;
    virtual void visit(const ImageElement& elem) = 0;
    virtual void visit(const TableElement& elem) = 0;
};

// --- Element Interface ---
class DocumentElement {
public:
    virtual ~DocumentElement() = default;
    virtual void accept(DocumentVisitor& visitor) const = 0;  // The "accept" method
};

// --- Concrete Elements ---
class TextElement : public DocumentElement {
    string text_;
public:
    explicit TextElement(string text) : text_(move(text)) {}
    const string& getText() const { return text_; }

    void accept(DocumentVisitor& visitor) const override {
        visitor.visit(*this);  // Double dispatch: calls visit(TextElement&)
    }
};

class ImageElement : public DocumentElement {
    string src_;
    int width_, height_;
public:
    ImageElement(string src, int w, int h) : src_(move(src)), width_(w), height_(h) {}
    const string& getSrc() const { return src_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

    void accept(DocumentVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

class TableElement : public DocumentElement {
    int rows_, cols_;
public:
    TableElement(int rows, int cols) : rows_(rows), cols_(cols) {}
    int getRows() const { return rows_; }
    int getCols() const { return cols_; }

    void accept(DocumentVisitor& visitor) const override {
        visitor.visit(*this);
    }
};

// --- Concrete Visitor 1: HTML Export ---
class HtmlExportVisitor : public DocumentVisitor {
public:
    void visit(const TextElement& elem) override {
        cout << "  <p>" << elem.getText() << "</p>\n";
    }

    void visit(const ImageElement& elem) override {
        cout << "  <img src=\"" << elem.getSrc()
             << "\" width=\"" << elem.getWidth()
             << "\" height=\"" << elem.getHeight() << "\" />\n";
    }

    void visit(const TableElement& elem) override {
        cout << "  <table " << elem.getRows() << "x" << elem.getCols() << ">\n";
        for (int r = 0; r < elem.getRows(); ++r) {
            cout << "    <tr>";
            for (int c = 0; c < elem.getCols(); ++c)
                cout << "<td>cell</td>";
            cout << "</tr>\n";
        }
        cout << "  </table>\n";
    }
};

// --- Concrete Visitor 2: Statistics ---
class StatsVisitor : public DocumentVisitor {
    int wordCount_ = 0;
    int imageCount_ = 0;
    int totalCells_ = 0;
public:
    void visit(const TextElement& elem) override {
        // Count words
        istringstream iss(elem.getText());
        string word;
        while (iss >> word) wordCount_++;
    }

    void visit(const ImageElement& elem) override {
        imageCount_++;
    }

    void visit(const TableElement& elem) override {
        totalCells_ += elem.getRows() * elem.getCols();
    }

    void printStats() const {
        cout << "  📊 Words: " << wordCount_
             << " | Images: " << imageCount_
             << " | Table cells: " << totalCells_ << "\n";
    }
};

int main() {
    cout << "=== Visitor Pattern ===" << endl;

    // Build a document
    vector<unique_ptr<DocumentElement>> doc;
    doc.push_back(make_unique<TextElement>("Hello World! This is a test document."));
    doc.push_back(make_unique<ImageElement>("photo.jpg", 800, 600));
    doc.push_back(make_unique<TextElement>("Another paragraph with more words here."));
    doc.push_back(make_unique<TableElement>(3, 4));
    doc.push_back(make_unique<ImageElement>("diagram.png", 400, 300));

    // Visitor 1: Export to HTML
    cout << "\n--- HTML Export ---\n";
    HtmlExportVisitor htmlVisitor;
    for (auto& elem : doc) {
        elem->accept(htmlVisitor);  // Each element dispatches to correct visit() method
    }

    // Visitor 2: Gather Statistics
    cout << "\n--- Document Statistics ---\n";
    StatsVisitor statsVisitor;
    for (auto& elem : doc) {
        elem->accept(statsVisitor);
    }
    statsVisitor.printStats();

    // Adding a new operation (e.g., MarkdownExport)?
    // Just create a new visitor class — ZERO changes to element classes! ✓

    return 0;
}
