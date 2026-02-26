#include <iostream>
#include <memory>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// ==========================================
// GOOD DESIGN (Iterator Pattern)
// ==========================================

// --- Example 1: Simple Range Iterator ---
// Makes any numeric range iterable with range-based for loops.
class IntRange {
    int start_, end_;
public:
    IntRange(int start, int end) : start_(start), end_(end) {}

    // --- Iterator ---
    class Iterator {
        int current_;
    public:
        explicit Iterator(int val) : current_(val) {}
        int operator*() const { return current_; }
        Iterator& operator++() { ++current_; return *this; }
        bool operator!=(const Iterator& other) const { return current_ != other.current_; }
    };

    Iterator begin() const { return Iterator(start_); }
    Iterator end() const   { return Iterator(end_); }
};

// --- Example 2: Binary Tree with In-Order Iterator ---
struct TreeNode {
    int value;
    shared_ptr<TreeNode> left, right;

    TreeNode(int v, shared_ptr<TreeNode> l = nullptr, shared_ptr<TreeNode> r = nullptr)
        : value(v), left(move(l)), right(move(r)) {}
};

class BinaryTree {
    shared_ptr<TreeNode> root_;

    // Helper: collect in-order traversal
    void inorder(shared_ptr<TreeNode> node, vector<int>& result) const {
        if (!node) return;
        inorder(node->left, result);
        result.push_back(node->value);
        inorder(node->right, result);
    }

public:
    explicit BinaryTree(shared_ptr<TreeNode> root) : root_(move(root)) {}

    // --- Iterator wrapping in-order traversal ---
    class Iterator {
        vector<int> elements_;
        int index_;
    public:
        Iterator(const vector<int>& elems, int idx) : elements_(elems), index_(idx) {}
        int operator*() const { return elements_[index_]; }
        Iterator& operator++() { ++index_; return *this; }
        bool operator!=(const Iterator& other) const { return index_ != other.index_; }
    };

    Iterator begin() const {
        vector<int> elems;
        inorder(root_, elems);
        return Iterator(elems, 0);
    }

    Iterator end() const {
        vector<int> elems;
        inorder(root_, elems);
        return Iterator(elems, elems.size());
    }
};

// --- Example 3: Custom collection with filtered iteration ---
class EvenFilter {
    const vector<int>& data_;
public:
    explicit EvenFilter(const vector<int>& data) : data_(data) {}

    class Iterator {
        const vector<int>& data_;
        size_t index_;

        void skipOdds() {
            while (index_ < data_.size() && data_[index_] % 2 != 0)
                ++index_;
        }
    public:
        Iterator(const vector<int>& data, size_t idx) : data_(data), index_(idx) { skipOdds(); }
        int operator*() const { return data_[index_]; }
        Iterator& operator++() { ++index_; skipOdds(); return *this; }
        bool operator!=(const Iterator& other) const { return index_ != other.index_; }
    };

    Iterator begin() const { return Iterator(data_, 0); }
    Iterator end() const   { return Iterator(data_, data_.size()); }
};

int main() {
    cout << "=== Iterator Pattern ===" << endl;

    // Example 1: Range iteration
    cout << "\n--- IntRange(1, 6) ---\n  ";
    for (int x : IntRange(1, 6)) {
        cout << x << " ";
    }
    cout << "\n";

    // Example 2: Binary tree in-order iteration
    //       4
    //      / \
    //     2   6
    //    / \ / \
    //   1  3 5  7
    auto tree = make_shared<TreeNode>(4,
        make_shared<TreeNode>(2,
            make_shared<TreeNode>(1),
            make_shared<TreeNode>(3)),
        make_shared<TreeNode>(6,
            make_shared<TreeNode>(5),
            make_shared<TreeNode>(7))
    );

    BinaryTree bt(tree);
    cout << "\n--- BinaryTree In-Order ---\n  ";
    for (int val : bt) {
        cout << val << " ";  // Should print: 1 2 3 4 5 6 7
    }
    cout << "\n";

    // Example 3: Filtered iteration (only even numbers)
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cout << "\n--- EvenFilter over {1..10} ---\n  ";
    for (int x : EvenFilter(nums)) {
        cout << x << " ";  // Should print: 2 4 6 8 10
    }
    cout << "\n";

    // All three DIFFERENT data structures use the SAME for-loop interface!

    return 0;
}
