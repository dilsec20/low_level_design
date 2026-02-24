#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

using namespace std;

// ==========================================
// BAD DESIGN (Using Enums and Switch Statements)
// ==========================================
enum class OutputFormat { markdown, html };

struct TextProcessorBad {
    string output;

    // The processor does ALL the formatting itself based on an enum!
    void append_list(const vector<string>& items, OutputFormat format) {
        if (format == OutputFormat::html) {
            output += "<ul>\n";
            for (auto& item : items)
                output += "  <li>" + item + "</li>\n";
            output += "</ul>\n";
        } 
        else if (format == OutputFormat::markdown) {
            for (auto& item : items)
                output += " * " + item + "\n";
        }
    }
    string str() const { return output; }
};

// ==========================================
// GOOD DESIGN (Strategy Pattern)
// ==========================================
// We extract the algorithms into separate interface and classes.

// 1. The Strategy Interface
struct ListStrategy {
    virtual void start(ostringstream& oss) {};
    virtual void add_list_item(ostringstream& oss, const string& item) = 0;
    virtual void end(ostringstream& oss) {};
};

// 2. Concrete Strategy A
struct HtmlListStrategy : ListStrategy {
    void start(ostringstream& oss) override { oss << "<ul>\n"; }
    void add_list_item(ostringstream& oss, const string& item) override {
        oss << "  <li>" << item << "</li>\n";
    }
    void end(ostringstream& oss) override { oss << "</ul>\n"; }
};

// 3. Concrete Strategy B
struct MarkdownListStrategy : ListStrategy {
    void add_list_item(ostringstream& oss, const string& item) override {
        oss << " * " << item << "\n";
    }
};

// 4. The Context Class
// It maintains a reference to a Strategy object and delegates the work to it.
struct TextProcessorBetter {
    ostringstream oss;
    unique_ptr<ListStrategy> list_strategy;

    // We can swap strategies dynamically at runtime!
    void set_output_format(OutputFormat format) {
        switch(format) {
            case OutputFormat::markdown:
                list_strategy = make_unique<MarkdownListStrategy>();
                break;
            case OutputFormat::html:
                list_strategy = make_unique<HtmlListStrategy>();
                break;
        }
    }

    void append_list(const vector<string>& items) {
        list_strategy->start(oss);
        for (auto& item : items)
            list_strategy->add_list_item(oss, item);
        list_strategy->end(oss);
    }

    string str() const { return oss.str(); }
};

int main() {
    vector<string> items{"foo", "bar", "baz"};

    cout << "--- Bad Processor ---" << endl;
    TextProcessorBad bad_tp;
    bad_tp.append_list(items, OutputFormat::markdown);
    cout << bad_tp.str() << endl;

    cout << "--- Good Processor (Strategy Pattern) ---" << endl;
    
    TextProcessorBetter tp;
    
    // Set strategy to Markdown
    tp.set_output_format(OutputFormat::markdown);
    tp.append_list(items);
    cout << "Markdown Output:\n" << tp.str() << endl;

    // We can reuse the same context, but swap the algorithm dynamically!
    tp.oss.str(""); // clear the stream
    tp.set_output_format(OutputFormat::html);
    tp.append_list(items);
    cout << "HTML Output:\n" << tp.str() << endl;

    return 0;
}
