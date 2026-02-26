#include <iostream>
#include <memory>
#include <string>

using namespace std;

// ==========================================
// GOOD DESIGN (Proxy Pattern)
// ==========================================

// --- Subject Interface ---
class Image {
public:
    virtual ~Image() = default;
    virtual void display() const = 0;
    virtual string getFilename() const = 0;
};

// --- Real Subject (Expensive to create) ---
class RealImage : public Image {
    string filename_;
public:
    explicit RealImage(string filename) : filename_(move(filename)) {
        loadFromDisk();  // Expensive operation happens at construction
    }

    void display() const override {
        cout << "  📷 Displaying image: " << filename_ << "\n";
    }

    string getFilename() const override { return filename_; }

private:
    void loadFromDisk() {
        cout << "  ⏳ Loading heavy image from disk: " << filename_ << " ...\n";
        // Simulate expensive loading (DB query, network fetch, etc.)
    }
};

// --- Virtual Proxy (Lazy Loading) ---
// Delays creation of RealImage until display() is actually called.
class LazyImageProxy : public Image {
    mutable unique_ptr<RealImage> realImage_;  // mutable for lazy init in const method
    string filename_;
public:
    explicit LazyImageProxy(string filename) : filename_(move(filename)) {
        cout << "  ✅ Proxy created for: " << filename_ << " (NO loading yet!)\n";
    }

    void display() const override {
        if (!realImage_) {
            cout << "  [Proxy] First access — loading now...\n";
            realImage_ = make_unique<RealImage>(filename_);
        }
        realImage_->display();
    }

    string getFilename() const override { return filename_; }
};

// --- Protection Proxy (Access Control) ---
class ProtectedImageProxy : public Image {
    unique_ptr<RealImage> realImage_;
    string userRole_;
public:
    ProtectedImageProxy(string filename, string role)
        : realImage_(make_unique<RealImage>(move(filename))), userRole_(move(role)) {}

    void display() const override {
        if (userRole_ == "admin" || userRole_ == "editor") {
            realImage_->display();
        } else {
            cout << "  🚫 Access Denied! User role '" << userRole_
                 << "' cannot view: " << realImage_->getFilename() << "\n";
        }
    }

    string getFilename() const override { return realImage_->getFilename(); }
};

int main() {
    cout << "=== Virtual Proxy (Lazy Loading) ===" << endl;
    cout << "Creating 3 image proxies...\n";
    LazyImageProxy img1("photo_4k_01.jpg");
    LazyImageProxy img2("photo_4k_02.jpg");
    LazyImageProxy img3("photo_4k_03.jpg");

    cout << "\nOnly displaying image 1 (only this one loads!):\n";
    img1.display();

    cout << "\nDisplaying image 1 again (already loaded, no reload):\n";
    img1.display();

    cout << "\n=== Protection Proxy (Access Control) ===" << endl;
    ProtectedImageProxy adminImg("secret_doc.png", "admin");
    ProtectedImageProxy guestImg("secret_doc2.png", "guest");

    cout << "\nAdmin tries to view:\n";
    adminImg.display();

    cout << "\nGuest tries to view:\n";
    guestImg.display();

    return 0;
}
