#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>

using namespace std;

// ==========================================
// BAD DESIGN (Constructor Overloading Issue)
// ==========================================
enum class CoordinateSystem { cartesian, polar };

class PointBad {
public:
    float x, y;

    // We can't have two constructors with (float, float) signatures.
    // So we are forced to use an enum to distinguish what the floats mean.
    // This is ugly, confusing, and violates OCP if we add new coordinate systems.
    PointBad(float a, float b, CoordinateSystem sys = CoordinateSystem::cartesian) {
        if (sys == CoordinateSystem::cartesian) {
            x = a;
            y = b;
        } else {
            // Polar coordinates: a is rho (radius), b is theta (angle)
            x = a * cos(b);
            y = a * sin(b);
        }
    }

    void print() const { cout << "PointBad(x=" << x << ", y=" << y << ")\n"; }
};

// ==========================================
// GOOD DESIGN (Using Factory Methods / Classes)
// ==========================================
// Forward declaration for the separate factory class
class PointFactory;

class Point {
private:
    float x, y;

    // Make constructor private! Now clients MUST use the factory to create a Point.
    Point(float x, float y) : x(x), y(y) {}

public:
    // Option 1: FACTORY METHODS
    // Static methods inside the class itself. They have clear, descriptive names.
    static Point NewCartesian(float x, float y) {
        return {x, y};
    }

    static Point NewPolar(float rho, float theta) {
        return {rho * cos(theta), rho * sin(theta)};
    }
    
    void print() const { cout << "Point(x=" << x << ", y=" << y << ")\n"; }

    // Required for Option 2: giving the factory class access to the private constructor
    friend class PointFactory;
};

// Option 2: FACTORY CLASS
// If object creation logic is very complex, we move it to a dedicated class.
// This follows the Single Responsibility Principle (SRP).
class PointFactory {
public:
    static Point NewCartesian(float x, float y) {
        return Point{x, y};
    }

    static Point NewPolar(float rho, float theta) {
        return Point{rho * cos(theta), rho * sin(theta)};
    }
};

int main() {
    cout << "--- Bad Constructor Approach ---" << endl;
    // Magic numbers! Is 5, 5 x,y or rho,theta? We only know if we read the enum at the end.
    PointBad pb1(5, 5, CoordinateSystem::cartesian);
    PointBad pb2(5, M_PI_4, CoordinateSystem::polar);
    pb1.print();
    pb2.print();

    cout << "\n--- Good Factory Method Approach ---" << endl;
    // The method names make it extremely obvious what the parameters mean!
    auto p1 = Point::NewCartesian(5, 5);
    auto p2 = Point::NewPolar(5, M_PI_4);
    p1.print();
    p2.print();

    cout << "\n--- Good Factory Class Approach ---" << endl;
    // Sometimes we want to group all creation logic into a separate class.
    auto p3 = PointFactory::NewCartesian(5, 5);
    auto p4 = PointFactory::NewPolar(5, M_PI_4);
    p3.print();
    p4.print();

    return 0;
}
