#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// BAD DESIGN (Class Explosion — M×N classes)
// ==========================================
namespace Bad {
    // Without Bridge, every combination of Shape × Renderer needs its own class:
    // VectorCircle, RasterCircle, VectorSquare, RasterSquare, ...
    // Adding OpenGLRenderer means 2 MORE classes. Adding Triangle means 3 MORE classes.
    struct VectorCircle {
        void draw() { cout << "  Drawing circle as vector lines\n"; }
    };
    struct RasterCircle {
        void draw() { cout << "  Drawing circle as pixels\n"; }
    };
    struct VectorSquare {
        void draw() { cout << "  Drawing square as vector lines\n"; }
    };
    struct RasterSquare {
        void draw() { cout << "  Drawing square as pixels\n"; }
    };
    // 2 shapes × 2 renderers = 4 classes. This grows MULTIPLICATIVELY!
}

// ==========================================
// GOOD DESIGN (Bridge Pattern)
// ==========================================
namespace Good {

    // --- Implementation Interface ---
    // This is one "dimension" of variation.
    class Renderer {
    public:
        virtual ~Renderer() = default;
        virtual void renderCircle(float radius) const = 0;
        virtual void renderSquare(float side) const = 0;
    };

    // --- Concrete Implementations ---
    class VectorRenderer : public Renderer {
    public:
        void renderCircle(float radius) const override {
            cout << "  Drawing circle as vector lines (r=" << radius << ")\n";
        }
        void renderSquare(float side) const override {
            cout << "  Drawing square as vector lines (s=" << side << ")\n";
        }
    };

    class RasterRenderer : public Renderer {
    public:
        void renderCircle(float radius) const override {
            cout << "  Drawing circle as " << (int)(radius * 10) << " pixels\n";
        }
        void renderSquare(float side) const override {
            cout << "  Drawing square as " << (int)(side * side * 100) << " pixels\n";
        }
    };

    // --- Abstraction ---
    // This holds a REFERENCE to the implementation (the "bridge").
    class Shape {
    protected:
        const Renderer& renderer_;  // ← This is the BRIDGE
    public:
        explicit Shape(const Renderer& renderer) : renderer_(renderer) {}
        virtual ~Shape() = default;
        virtual void draw() const = 0;
        virtual void resize(float factor) = 0;
    };

    // --- Refined Abstractions ---
    class Circle : public Shape {
        float radius_;
    public:
        Circle(const Renderer& renderer, float radius)
            : Shape(renderer), radius_(radius) {}

        void draw() const override {
            renderer_.renderCircle(radius_);  // Delegates to the implementation
        }

        void resize(float factor) override {
            radius_ *= factor;
        }
    };

    class Square : public Shape {
        float side_;
    public:
        Square(const Renderer& renderer, float side)
            : Shape(renderer), side_(side) {}

        void draw() const override {
            renderer_.renderSquare(side_);
        }

        void resize(float factor) override {
            side_ *= factor;
        }
    };

    // Adding a new shape (Triangle) → 1 new class only
    // Adding a new renderer (OpenGL) → 1 new class only
    // Total classes grow ADDITIVELY (M+N), not MULTIPLICATIVELY (M×N)!
}

int main() {
    cout << "=== BAD DESIGN (Class Explosion) ===" << endl;
    Bad::VectorCircle vc; vc.draw();
    Bad::RasterCircle rc; rc.draw();
    Bad::VectorSquare vs; vs.draw();
    Bad::RasterSquare rs; rs.draw();
    cout << "  → 4 classes for 2 shapes × 2 renderers. Adding 1 shape = 2 more classes!\n";

    cout << "\n=== GOOD DESIGN (Bridge Pattern) ===" << endl;

    Good::VectorRenderer vectorRenderer;
    Good::RasterRenderer rasterRenderer;

    // Same shape, different renderers — just swap the bridge!
    Good::Circle c1(vectorRenderer, 5);
    Good::Circle c2(rasterRenderer, 5);

    Good::Square s1(vectorRenderer, 3);
    Good::Square s2(rasterRenderer, 3);

    cout << "Vector rendering:\n";
    c1.draw();
    s1.draw();

    cout << "\nRaster rendering:\n";
    c2.draw();
    s2.draw();

    cout << "\n  → Only 4 classes total (2 shapes + 2 renderers). Adding 1 shape = 1 class!\n";

    return 0;
}
