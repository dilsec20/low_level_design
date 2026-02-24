#include <iostream>
#include <vector>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// TARGET INTERFACE (What our system expects)
// ==========================================
struct Point {
    int x, y;
};

struct Line {
    Point start, end;
};

// Our modern renderer only knows how to draw given individual points.
// (Imagine this is deeply embedded in our codebase and cannot be easily changed).
void DrawPoints(const vector<Point>& points) {
    for (auto& p : points) {
        cout << "* Drawing point at (" << p.x << ", " << p.y << ")\n";
    }
}


// ==========================================
// ADAPTEE (The incompatible external/legacy code)
// ==========================================
// We have this class from a legacy library. 
// It defines rectangles, but as collections of lines, not points!
struct VectorObject {
    virtual vector<Line>::iterator begin() = 0;
    virtual vector<Line>::iterator end() = 0;
};

struct VectorRectangle : VectorObject {
    vector<Line> lines;

    VectorRectangle(int x, int y, int width, int height) {
        // Constructing a rectangle from 4 lines
        lines.emplace_back(Line{ Point{x, y}, Point{x + width, y} });
        lines.emplace_back(Line{ Point{x + width, y}, Point{x + width, y + height} });
        lines.emplace_back(Line{ Point{x, y}, Point{x, y + height} });
        lines.emplace_back(Line{ Point{x, y + height}, Point{x + width, y + height} });
    }

    vector<Line>::iterator begin() override { return lines.begin(); }
    vector<Line>::iterator end() override { return lines.end(); }
};


// ==========================================
// ADAPTER (The bridge)
// ==========================================
// We need to convert lines from the VectorObject into individual points for the renderer.

struct LineToPointAdapter {
    vector<Point> points;

    // The constructor takes the incompatible Line object and does the translation work.
    LineToPointAdapter(Line& line) {
        cout << "--- Adapting a line from (" << line.start.x << "," << line.start.y 
             << ") to (" << line.end.x << "," << line.end.y << ") ---\n";
        
        // Very simplistic "drawing" algorithm to get points on a line
        int left = min(line.start.x, line.end.x);
        int right = max(line.start.x, line.end.x);
        int top = min(line.start.y, line.end.y);
        int bottom = max(line.start.y, line.end.y);

        if (right - left == 0) { // vertical line
            for (int y = top; y <= bottom; ++y) {
                points.emplace_back(Point{left, y});
            }
        } 
        else if (bottom - top == 0) { // horizontal line
            for (int x = left; x <= right; ++x) {
                points.emplace_back(Point{x, top});
            }
        }
    }

    // Provide the interface the modern renderer expects (a collection of points)
    vector<Point>::iterator begin() { return points.begin(); }
    vector<Point>::iterator end() { return points.end(); }
};


int main() {
    cout << "Creating a Vector Rectangle (Legacy object)...\n";
    VectorRectangle rect{10, 10, 2, 2}; // 2x2 rectangle starting at 10,10

    cout << "\nAttempting to draw using modern renderer...\n";
    // We CANNOT do this: DrawPoints(rect); // Error: incompatible types

    // Instead, we iterate over the lines and use the Adapter!
    vector<Point> all_points;
    for (auto& line : rect) {
        // Creating the adapter does the translation from Line -> Points
        LineToPointAdapter adapter{line};
        
        // We can now extract points from the adapter
        for (auto& p : adapter) {
            all_points.push_back(p);
        }
    }

    // Now we can successfully pass the translated points to our modern renderer!
    DrawPoints(all_points);

    return 0;
}
