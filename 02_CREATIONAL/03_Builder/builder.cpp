#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// ==========================================
// BAD DESIGN (String Concatenation)
// ==========================================
// Building a complex object (like an HTML string) manually is brittle and messy.
void build_html_badly() {
    string words[] = { "hello", "world" };
    
    // We try to build a <ul> list manually
    string output = "<ul>\n";
    for (auto w : words) {
        output += "  <li>" + w + "</li>\n";
    }
    output += "</ul>";
    
    cout << output << endl;
    // What if we want to nest deeper? What if we want attributes? 
    // This approach scales terribly.
}


// ==========================================
// GOOD DESIGN (Builder Pattern)
// ==========================================
// We define an object representation for the HTML elements.
struct HtmlElement {
    string name;
    string text;
    vector<HtmlElement> elements;
    const size_t indent_size = 2;

    HtmlElement() {}
    HtmlElement(const string& name, const string& text) : name(name), text(text) {}

    string str(int indent = 0) const {
        ostringstream oss;
        string i(indent_size * indent, ' ');
        oss << i << "<" << name << ">\n";
        
        if (text.size() > 0)
            oss << string(indent_size * (indent + 1), ' ') << text << "\n";

        for (const auto& e : elements)
            oss << e.str(indent + 1);

        oss << i << "</" << name << ">\n";
        return oss.str();
    }
};

// This is the BUILDER. Its entire job is to assemble an HtmlElement piece by piece.
struct HtmlBuilder {
    HtmlElement root;

    HtmlBuilder(string root_name) {
        root.name = root_name;
    }

    // A normal method to add a child element
    void add_child(string child_name, string child_text) {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
    }

    // FLUENT INTERFACE!
    // By returning a reference to the builder itself, we can chain these calls.
    HtmlBuilder& add_child_fluent(string child_name, string child_text) {
        HtmlElement e{child_name, child_text};
        root.elements.emplace_back(e);
        return *this;
    }

    string str() const { return root.str(); }

    // Optionally provide a method to get the final constructed object
    HtmlElement build() { return root; }
};


int main() {
    cout << "--- Bad String Concatenation ---" << endl;
    build_html_badly();

    cout << "\n--- Good Builder Usage ---" << endl;
    HtmlBuilder builder{"ul"};
    
    // Step-by-step piecewise construction
    builder.add_child("li", "hello");
    builder.add_child("li", "world");
    
    cout << builder.str() << endl;

    cout << "--- Fluent Builder Usage ---" << endl;
    HtmlBuilder fluent_builder{"ul"};
    
    // We can chain the methods! This makes configuring complex objects a breeze.
    fluent_builder
        .add_child_fluent("li", "first item")
        .add_child_fluent("li", "second item")
        .add_child_fluent("li", "third item");

    cout << fluent_builder.str() << endl;

    return 0;
}
