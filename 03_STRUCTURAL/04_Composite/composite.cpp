#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// BAD DESIGN (Separate handling for each type)
// ==========================================
namespace Bad {
    struct Circle { string name; void draw() { cout << "  Drawing circle: " << name << "\n"; }};
    struct Rectangle { string name; void draw() { cout << "  Drawing rect: " << name << "\n"; }};

    // Must maintain separate lists and duplicate logic for groups
    void drawAll(vector<Circle>& circles, vector<Rectangle>& rects) {
        for (auto& c : circles) c.draw();
        for (auto& r : rects) r.draw();
        // Adding Triangle? Another vector, another loop. ❌
    }
}

// ==========================================
// GOOD DESIGN (Composite Pattern)
// ==========================================
namespace Good {

    // --- Component Interface ---
    // Both individual shapes AND groups implement this.
    class Graphic {
    public:
        virtual ~Graphic() = default;
        virtual void draw(int indent = 0) const = 0;
        virtual int getSize() const = 0;  // e.g., number of primitives
    };

    // --- Leaf: Circle ---
    class Circle : public Graphic {
        string name_;
        double radius_;
    public:
        Circle(string name, double r) : name_(move(name)), radius_(r) {}

        void draw(int indent = 0) const override {
            cout << string(indent, ' ') << "● Circle \"" << name_
                 << "\" (r=" << radius_ << ")\n";
        }

        int getSize() const override { return 1; }
    };

    // --- Leaf: Rectangle ---
    class Rectangle : public Graphic {
        string name_;
        double w_, h_;
    public:
        Rectangle(string name, double w, double h)
            : name_(move(name)), w_(w), h_(h) {}

        void draw(int indent = 0) const override {
            cout << string(indent, ' ') << "▭ Rectangle \"" << name_
                 << "\" (" << w_ << "x" << h_ << ")\n";
        }

        int getSize() const override { return 1; }
    };

    // --- Composite: Group ---
    // A Group IS a Graphic, but also CONTAINS Graphics.
    // This is the core of the Composite pattern.
    class Group : public Graphic {
        string name_;
        vector<shared_ptr<Graphic>> children_;
    public:
        explicit Group(string name) : name_(move(name)) {}

        void add(shared_ptr<Graphic> child) {
            children_.push_back(move(child));
        }

        void draw(int indent = 0) const override {
            cout << string(indent, ' ') << "📁 Group \"" << name_
                 << "\" [" << getSize() << " elements]\n";
            for (const auto& child : children_) {
                child->draw(indent + 4);  // Recursive! Groups inside groups work.
            }
        }

        int getSize() const override {
            int total = 0;
            for (const auto& child : children_) {
                total += child->getSize();  // Polymorphic — works for leaves AND groups
            }
            return total;
        }
    };
}

int main() {
    cout << "=== BAD DESIGN ===" << endl;
    vector<Bad::Circle> circles = {{"Sun"}, {"Moon"}};
    vector<Bad::Rectangle> rects = {{"House"}};
    Bad::drawAll(circles, rects);

    cout << "\n=== GOOD DESIGN (Composite Pattern) ===" << endl;

    // Build a tree structure
    auto sun = make_shared<Good::Circle>("Sun", 50);
    auto moon = make_shared<Good::Circle>("Moon", 20);
    auto house = make_shared<Good::Rectangle>("House", 100, 80);
    auto door = make_shared<Good::Rectangle>("Door", 20, 40);
    auto window = make_shared<Good::Circle>("Window", 10);

    // Group: house details
    auto houseGroup = make_shared<Good::Group>("House Details");
    houseGroup->add(house);
    houseGroup->add(door);
    houseGroup->add(window);

    // Group: sky
    auto skyGroup = make_shared<Good::Group>("Sky");
    skyGroup->add(sun);
    skyGroup->add(moon);

    // Top-level group (groups of groups!)
    auto scene = make_shared<Good::Group>("Full Scene");
    scene->add(skyGroup);
    scene->add(houseGroup);

    // Draw the entire scene with ONE call — Composite power!
    scene->draw();

    cout << "\nTotal primitives in scene: " << scene->getSize() << endl;

    return 0;
}
