#include <iostream>
#include <string>
#include <sstream>
#include <memory>

using namespace std;

// ==========================================
// COMPONENT INTERFACE
// ==========================================
struct Shape {
    virtual string str() const = 0;
};

// ==========================================
// CONCRETE COMPONENTS
// ==========================================
struct Circle : Shape {
    float radius;
    Circle(float radius) : radius(radius) {}

    string str() const override {
        ostringstream oss;
        oss << "A circle of radius " << radius;
        return oss.str();
    }
};

struct Square : Shape {
    float side;
    Square(float side) : side(side) {}

    string str() const override {
        ostringstream oss;
        oss << "A square with side " << side;
        return oss.str();
    }
};

// ==========================================
// DECORATORS (The "Wrappers")
// ==========================================
// A Decorator MUST implement the exact same interface as the objects it wraps.
// It holds a reference to the wrapped object and adds its own behavior.

struct ColoredShape : Shape {
    // A reference to the base shape we are wrapping
    Shape& shape;
    string color;

    ColoredShape(Shape& shape, const string& color) : shape(shape), color(color) {}

    // First, we call the wrapped object's method. 
    // Then, we append our new behavior!
    string str() const override {
        ostringstream oss;
        oss << shape.str() << " which is colored " << color;
        return oss.str();
    }
};

struct TransparentShape : Shape {
    Shape& shape;
    uint8_t transparency; // 0 to 255

    TransparentShape(Shape& shape, uint8_t transparency) 
        : shape(shape), transparency(transparency) {}

    string str() const override {
        ostringstream oss;
        // Notice how we don't care what 'shape' is. It could be a Circle,
        // a Square, or even a ColoredShape! We just wrap it and add transparency.
        oss << shape.str() << " and has " 
            << static_cast<float>(transparency) / 255.f * 100.f 
            << "% transparency";
        return oss.str();
    }
};


int main() {
    cout << "--- Base Shapes ---" << endl;
    Circle circle{5.0f};
    Square square{10.0f};
    
    cout << circle.str() << endl;
    cout << square.str() << endl;


    cout << "\n--- Adding One Decoration ---" << endl;
    // We dynamically attach "Red" behavior to the Circle.
    // Notice how ColoredShape takes a Shape and behaves as a Shape.
    ColoredShape red_circle{circle, "red"};
    cout << red_circle.str() << endl;


    cout << "\n--- Adding Multiple Decorations! ---" << endl;
    // We can nest decorations infinitely!
    // We wrap a Square inside a ColoredShape, and wrap that inside a TransparentShape!
    ColoredShape green_square{square, "green"};
    TransparentShape half_transparent_green_square{green_square, 127};
    
    // Output: A square with side 10 which is colored green and has 49.8039% transparency
    cout << half_transparent_green_square.str() << endl;

    // What if we did that with inheritance? We would need a class called HalfTransparentGreenSquare!
    // With decorators, we achieve the exact same result dynamically without class explosion.

    return 0;
}
