#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace std;

// ==========================================
// Library Management System
// ==========================================

// --- Book ---
class Book {
    string isbn_, title_, author_;
    int totalCopies_, availableCopies_;
public:
    Book(string isbn, string title, string author, int copies)
        : isbn_(move(isbn)), title_(move(title)), author_(move(author)),
          totalCopies_(copies), availableCopies_(copies) {}

    const string& getISBN() const { return isbn_; }
    const string& getTitle() const { return title_; }
    const string& getAuthor() const { return author_; }
    int getAvailable() const { return availableCopies_; }

    bool checkout() {
        if (availableCopies_ <= 0) return false;
        availableCopies_--;
        return true;
    }

    void returnCopy() {
        if (availableCopies_ < totalCopies_) availableCopies_++;
    }

    void print() const {
        cout << "  📖 [" << isbn_ << "] \"" << title_ << "\" by " << author_
             << " (" << availableCopies_ << "/" << totalCopies_ << " available)\n";
    }
};

// --- Borrow Record ---
struct BorrowRecord {
    string memberName;
    string bookISBN;
    time_t borrowDate;
    time_t dueDate;
    bool returned = false;

    double calculateFine() const {
        if (returned) return 0;
        time_t now = time(nullptr);
        double daysLate = difftime(now, dueDate) / (60 * 60 * 24);
        if (daysLate <= 0) return 0;
        return daysLate * 1.0;  // $1 per day late
    }
};

// --- Search Strategy ---
class SearchStrategy {
public:
    virtual ~SearchStrategy() = default;
    virtual vector<Book*> search(const vector<unique_ptr<Book>>& books, const string& query) = 0;
};

class SearchByTitle : public SearchStrategy {
public:
    vector<Book*> search(const vector<unique_ptr<Book>>& books, const string& query) override {
        vector<Book*> results;
        for (auto& b : books) {
            if (b->getTitle().find(query) != string::npos)
                results.push_back(b.get());
        }
        return results;
    }
};

class SearchByAuthor : public SearchStrategy {
public:
    vector<Book*> search(const vector<unique_ptr<Book>>& books, const string& query) override {
        vector<Book*> results;
        for (auto& b : books) {
            if (b->getAuthor().find(query) != string::npos)
                results.push_back(b.get());
        }
        return results;
    }
};

class SearchByISBN : public SearchStrategy {
public:
    vector<Book*> search(const vector<unique_ptr<Book>>& books, const string& query) override {
        vector<Book*> results;
        for (auto& b : books) {
            if (b->getISBN() == query)
                results.push_back(b.get());
        }
        return results;
    }
};

// --- Member ---
class Member {
    string id_, name_;
    vector<string> borrowedISBNs_;
    static const int MAX_BOOKS = 5;
public:
    Member(string id, string name) : id_(move(id)), name_(move(name)) {}

    const string& getName() const { return name_; }
    const string& getId() const { return id_; }
    int borrowedCount() const { return borrowedISBNs_.size(); }

    bool canBorrow() const { return (int)borrowedISBNs_.size() < MAX_BOOKS; }

    void addBorrow(const string& isbn) { borrowedISBNs_.push_back(isbn); }

    bool removeBorrow(const string& isbn) {
        auto it = find(borrowedISBNs_.begin(), borrowedISBNs_.end(), isbn);
        if (it == borrowedISBNs_.end()) return false;
        borrowedISBNs_.erase(it);
        return true;
    }

    void print() const {
        cout << "  👤 " << name_ << " (ID: " << id_ << ") — "
             << borrowedISBNs_.size() << "/" << MAX_BOOKS << " books borrowed\n";
    }
};

// --- Library (Singleton) ---
class Library {
    Library() = default;
    vector<unique_ptr<Book>> books_;
    unordered_map<string, unique_ptr<Member>> members_;
    vector<BorrowRecord> records_;

public:
    Library(const Library&) = delete;
    Library& operator=(const Library&) = delete;

    static Library& getInstance() {
        static Library instance;
        return instance;
    }

    void addBook(string isbn, string title, string author, int copies) {
        books_.push_back(make_unique<Book>(move(isbn), move(title), move(author), copies));
        cout << "  ✅ Book added: " << books_.back()->getTitle() << "\n";
    }

    void registerMember(string id, string name) {
        members_[id] = make_unique<Member>(move(id), move(name));
        cout << "  ✅ Member registered: " << members_[id]->getName() << "\n";
    }

    Book* findBook(const string& isbn) {
        for (auto& b : books_)
            if (b->getISBN() == isbn) return b.get();
        return nullptr;
    }

    Member* findMember(const string& id) {
        auto it = members_.find(id);
        return it != members_.end() ? it->second.get() : nullptr;
    }

    bool borrowBook(const string& memberId, const string& isbn) {
        auto* member = findMember(memberId);
        auto* book = findBook(isbn);

        if (!member) { cout << "  ❌ Member not found.\n"; return false; }
        if (!book) { cout << "  ❌ Book not found.\n"; return false; }
        if (!member->canBorrow()) { cout << "  ❌ Borrow limit reached.\n"; return false; }
        if (!book->checkout()) { cout << "  ❌ No copies available.\n"; return false; }

        member->addBorrow(isbn);

        BorrowRecord record;
        record.memberName = member->getName();
        record.bookISBN = isbn;
        record.borrowDate = time(nullptr);
        record.dueDate = record.borrowDate + (14 * 24 * 60 * 60);  // 14 days
        records_.push_back(record);

        cout << "  ✅ " << member->getName() << " borrowed \"" << book->getTitle() << "\"\n";
        return true;
    }

    bool returnBook(const string& memberId, const string& isbn) {
        auto* member = findMember(memberId);
        auto* book = findBook(isbn);

        if (!member || !book) { cout << "  ❌ Invalid member or book.\n"; return false; }
        if (!member->removeBorrow(isbn)) { cout << "  ❌ Book not borrowed by this member.\n"; return false; }

        book->returnCopy();

        for (auto& rec : records_) {
            if (rec.bookISBN == isbn && rec.memberName == member->getName() && !rec.returned) {
                rec.returned = true;
                double fine = rec.calculateFine();
                cout << "  ✅ " << member->getName() << " returned \"" << book->getTitle() << "\"";
                if (fine > 0) cout << " (Fine: $" << fine << ")";
                cout << "\n";
                break;
            }
        }
        return true;
    }

    void searchBooks(SearchStrategy& strategy, const string& query) {
        auto results = strategy.search(books_, query);
        cout << "  🔍 Found " << results.size() << " result(s):\n";
        for (auto* b : results) b->print();
    }

    void showAllBooks() {
        cout << "  📚 Library Catalog (" << books_.size() << " books):\n";
        for (auto& b : books_) b->print();
    }
};

int main() {
    cout << "=== Library Management System ===" << endl;

    auto& library = Library::getInstance();

    // Add books
    cout << "\n--- Adding Books ---\n";
    library.addBook("978-0-13-468599-1", "The Pragmatic Programmer", "Andrew Hunt", 3);
    library.addBook("978-0-201-63361-0", "Design Patterns", "Gang of Four", 2);
    library.addBook("978-0-13-235088-4", "Clean Code", "Robert C. Martin", 4);
    library.addBook("978-0-596-00712-6", "Head First Design Patterns", "Eric Freeman", 2);

    // Register members
    cout << "\n--- Registering Members ---\n";
    library.registerMember("M001", "Alice");
    library.registerMember("M002", "Bob");

    // Show catalog
    cout << "\n--- Catalog ---\n";
    library.showAllBooks();

    // Search
    cout << "\n--- Search by Author ---\n";
    SearchByAuthor authorSearch;
    library.searchBooks(authorSearch, "Robert");

    cout << "\n--- Search by Title ---\n";
    SearchByTitle titleSearch;
    library.searchBooks(titleSearch, "Design");

    // Borrow
    cout << "\n--- Borrowing ---\n";
    library.borrowBook("M001", "978-0-13-468599-1");
    library.borrowBook("M001", "978-0-201-63361-0");
    library.borrowBook("M002", "978-0-13-468599-1");

    cout << "\n--- After Borrowing ---\n";
    library.showAllBooks();

    // Return
    cout << "\n--- Returning ---\n";
    library.returnBook("M001", "978-0-13-468599-1");

    cout << "\n--- After Return ---\n";
    library.showAllBooks();

    return 0;
}
