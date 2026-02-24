#include <iostream>

using namespace std;

struct Document {};

// ==========================================
// BAD DESIGN (Violating ISP)
// ==========================================
// This interface is too "fat". It forces everyone who wants to be a machine
// to implement print, scan, and fax.

struct IMachine {
    virtual void print(Document& doc) = 0;
    virtual void scan(Document& doc) = 0;
    virtual void fax(Document& doc) = 0;
};

// This printer can do everything. It happily implements IMachine.
struct MultiFunctionPrinter : IMachine {
    void print(Document& doc) override { cout << "MFP printing..." << endl; }
    void scan(Document& doc) override { cout << "MFP scanning..." << endl; }
    void fax(Document& doc) override { cout << "MFP faxing..." << endl; }
};

// However, a simple printer ONLY knows how to print.
// But because of the fat IMachine interface, we are FORCED to implement scan and fax!
struct SimplePrinter : IMachine {
    void print(Document& doc) override { cout << "SimplePrinter printing..." << endl; }

    // What do we do here? Leave them empty? Throw an exception?
    // Either way, it's a code smell and a violation of ISP.
    void scan(Document& doc) override { cout << "Error: Cannot scan!" << endl; }
    void fax(Document& doc) override { cout << "Error: Cannot fax!" << endl; }
};

// ==========================================
// GOOD DESIGN (Following ISP)
// ==========================================
// Segregate the interfaces into smaller, cohesive units!

struct IPrinter {
    virtual void print(Document& doc) = 0;
};

struct IScanner {
    virtual void scan(Document& doc) = 0;
};

struct IFaxer {
    virtual void fax(Document& doc) = 0;
};

// Now, a simple printer ONLY implements what it actually supports!
struct BetterSimplePrinter : IPrinter {
    void print(Document& doc) override {
        cout << "BetterSimplePrinter perfectly printing..." << endl;
    }
    // No need to implement scan or fax here. We are not forced to!
};

// A multi-function device can simply implement multiple interfaces.
struct BetterMultiFunctionPrinter : IPrinter, IScanner {
    void print(Document& doc) override {
        cout << "BetterMFP perfectly printing..." << endl;
    }
    void scan(Document& doc) override {
        cout << "BetterMFP perfectly scanning..." << endl;
    }
};

// You can even combine interfaces if needed
struct IMultiInterface : IPrinter, IScanner {};

struct CombinedPrinter : IMultiInterface {
    void print(Document& doc) override { cout << "Print!" << endl; }
    void scan(Document& doc) override { cout << "Scan!" << endl; }
};

int main() {
    Document d;
    
    cout << "--- Bad ISP Violations ---" << endl;
    SimplePrinter sp;
    sp.print(d);
    sp.scan(d); // We can incorrectly call this, and it throws an error/does nothing!

    cout << "\n--- Good ISP Fix ---" << endl;
    BetterSimplePrinter bsp;
    bsp.print(d);
    // bsp.scan(d); // Compilation error! `BetterSimplePrinter` has no member named `scan`.
    // This is much safer and cleaner.

    BetterMultiFunctionPrinter bmfp;
    bmfp.print(d);
    bmfp.scan(d);

    return 0;
}
