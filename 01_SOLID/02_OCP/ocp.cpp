#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum class Color { red, green, blue };
enum class Size { small, medium, large };

struct Product {
    string name;
    Color color;
    Size size;
};

// ==========================================
// BAD DESIGN (Violating OCP)
// ==========================================
// Every time a new filtering requirement comes in,
// we have to MODIFY this class by adding a new method.
struct ProductFilter {
    vector<Product*> filter_by_color(vector<Product*> items, Color color) {
        vector<Product*> result;
        for (auto& i : items)
            if (i->color == color)
                result.push_back(i);
        return result;
    }

    vector<Product*> filter_by_size(vector<Product*> items, Size size) {
        vector<Product*> result;
        for (auto& i : items)
            if (i->size == size)
                result.push_back(i);
        return result;
    }

    // New requirement: Filter by both! (We had to open and modify the class to add this)
    vector<Product*> filter_by_size_and_color(vector<Product*> items, Size size, Color color) {
        vector<Product*> result;
        for (auto& i : items)
            if (i->size == size && i->color == color)
                result.push_back(i);
        return result;
    }
};

// ==========================================
// GOOD DESIGN (Following OCP)
// ==========================================
// We use interfaces (abstract classes in C++) to define the "Specification" and "Filter" 
// This is called the Specification Pattern.

template <typename T> struct Specification {
    // Pure virtual function
    virtual bool is_satisfied(T* item) = 0;
};

template <typename T> struct Filter {
    virtual vector<T*> filter(vector<T*> items, Specification<T>& spec) = 0;
};

// Now, the filter class itself is CLOSED for modification.
struct BetterFilter : Filter<Product> {
    vector<Product*> filter(vector<Product*> items, Specification<Product>& spec) override {
        vector<Product*> result;
        for (auto& p : items)
            if (spec.is_satisfied(p))
                result.push_back(p);
        return result;
    }
};

// But OPEN for extension. Let's create new requirements!
struct ColorSpecification : Specification<Product> {
    Color color;
    ColorSpecification(Color color) : color(color) {}
    bool is_satisfied(Product* item) override {
        return item->color == color;
    }
};

struct SizeSpecification : Specification<Product> {
    Size size;
    SizeSpecification(Size size) : size(size) {}
    bool is_satisfied(Product* item) override {
        return item->size == size;
    }
};

// What about filtering by both? We can create a combinator.
// This is completely new code, we didn't touch the existing specification classes!
template <typename T> struct AndSpecification : Specification<T> {
    Specification<T>& first;
    Specification<T>& second;

    AndSpecification(Specification<T>& first, Specification<T>& second) : first(first), second(second) {}

    bool is_satisfied(T* item) override {
        return first.is_satisfied(item) && second.is_satisfied(item);
    }
};


int main() {
    Product apple{"Apple", Color::green, Size::small};
    Product tree{"Tree", Color::green, Size::large};
    Product house{"House", Color::blue, Size::large};

    // Vector of pointers to products
    vector<Product*> all_items{&apple, &tree, &house};

    cout << "--- Bad Filter ---" << endl;
    ProductFilter pf;
    auto green_things = pf.filter_by_color(all_items, Color::green);
    for (auto& x : green_things)
        cout << x->name << " is green\n";

    cout << "\n--- Good (Better) Filter ---" << endl;
    BetterFilter bf;
    ColorSpecification green(Color::green);
    
    // We just pass the specification to the same filter
    for (auto& x : bf.filter(all_items, green))
        cout << x->name << " is green\n";

    cout << "\n--- Complex Good Filter (Large AND Green) ---" << endl;
    SizeSpecification large(Size::large);
    AndSpecification<Product> green_and_large(green, large);

    // Filter by both simultaneously using the combinator
    for (auto& x : bf.filter(all_items, green_and_large))
        cout << x->name << " is green and large\n";

    return 0;
}
