#include <iostream>

using namespace std;

// ==========================================
// BAD DESIGN (Violating LSP)
// ==========================================
class Rectangle {
protected:
    int m_width, m_height;
public:
    Rectangle(const int width, const int height)
        : m_width(width), m_height(height) {}

    virtual int get_width() const { return m_width; }
    virtual void set_width(const int width) { this->m_width = width; }
    virtual int get_height() const { return m_height; }
    virtual void set_height(const int height) { this->m_height = height; }

    int area() const { return m_width * m_height; }
};

// In mathematics, a Square is a Rectangle.
// But in object-oriented programming, this inheritance is dangerous!
class Square : public Rectangle {
public:
    Square(int size) : Rectangle(size, size) {}

    // A square must have equal sides. So if you set the width,
    // you MUST also set the height to match.
    void set_width(const int width) override {
        this->m_width = height = width;
    }

    void set_height(const int height) override {
        this->m_width = this->m_height = height;
    }
};

// This function expects a Rectangle (or anything derived from it).
// It naturally expects that changing the width will NOT affect the height.
void process(Rectangle& r) {
    int w = r.get_width();
    r.set_height(10); 
    // Expectation: Area should be w * 10.
    cout << "Expected area = " << (w * 10) << 
        ", got " << r.area() << endl;
}


// ==========================================
// GOOD DESIGN (Following LSP)
// ==========================================
// If Square and Rectangle behave differently when mutated, they should not
// inherit from each other. 
// Alternatively, we can use a factory to produce Rectangles that happen to be squares.

class Shape {
public:
    virtual int area() const = 0;
};

class RectangleBetter : public Shape {
protected:
    int m_width, m_height;
public:
    RectangleBetter(const int width, const int height)
        : m_width(width), m_height(height) {}
    
    int area() const override { return m_width * m_height; }
};

// Now Square is a distinct entity. It does not pretend to be a Rectangle
// that allows independent width/height modification.
class SquareBetter : public Shape {
protected:
    int size;
public:
    SquareBetter(int size) : size(size) {}

    int area() const override { return size * size; }
};

int main() {
    cout << "--- Bad LSP Violation ---" << endl;
    Rectangle r{5, 5};
    process(r); // Works fine. Output: Expected area = 50, got 50.

    Square s{5};
    process(s); 
    // Fails! Square changes width when height is changed.
    // Width was 5. Height set to 10. So width also became 10.
    // Area becomes 10 * 10 = 100! Expected 50 !! This is an LSP violation.

    cout << "\n--- Good LSP Fix ---" << endl;
    // With our Better classes, `SquareBetter` doesn't inherit from `RectangleBetter`.
    // We cannot accidentally pass `SquareBetter` into a function that expects
    // the mutation behavior of `RectangleBetter`.
    
    RectangleBetter rb(5, 5);
    cout << "Rectangle area: " << rb.area() << endl;

    SquareBetter sb(5);
    cout << "Square area: " << sb.area() << endl;

    return 0;
}
