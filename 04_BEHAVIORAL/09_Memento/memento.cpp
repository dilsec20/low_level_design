#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// GOOD DESIGN (Memento Pattern)
// ==========================================

// --- Memento (Snapshot) ---
// Stores a snapshot of the Originator's state.
// Only the Originator can read its contents (friend).
class EditorMemento {
    string content_;
    int cursorPos_;

    friend class TextEditor;  // Only TextEditor can access internals

    EditorMemento(string content, int cursor)
        : content_(move(content)), cursorPos_(cursor) {}

public:
    // Caretaker can see the name (for display) but NOT the actual state
    string getName() const {
        string preview = content_.substr(0, min((int)content_.size(), 20));
        return "Snapshot [\"" + preview + "...\"] cursor@" + to_string(cursorPos_);
    }
};

// --- Originator ---
class TextEditor {
    string content_;
    int cursorPos_ = 0;
public:
    void type(const string& text) {
        content_.insert(cursorPos_, text);
        cursorPos_ += text.size();
        cout << "  ✏️ Typed: \"" << text << "\" → Content: \"" << content_ << "\"\n";
    }

    void moveCursor(int pos) {
        cursorPos_ = min(pos, (int)content_.size());
        cout << "  📍 Cursor moved to position " << cursorPos_ << "\n";
    }

    void deleteBack(int count) {
        int start = max(0, cursorPos_ - count);
        content_.erase(start, cursorPos_ - start);
        cursorPos_ = start;
        cout << "  🗑️ Deleted " << count << " chars → Content: \"" << content_ << "\"\n";
    }

    // Create a memento (snapshot)
    unique_ptr<EditorMemento> save() const {
        cout << "  💾 Saving state...\n";
        return unique_ptr<EditorMemento>(new EditorMemento(content_, cursorPos_));
    }

    // Restore from a memento
    void restore(const EditorMemento& memento) {
        content_ = memento.content_;
        cursorPos_ = memento.cursorPos_;
        cout << "  ↩️ Restored! Content: \"" << content_ << "\" cursor@" << cursorPos_ << "\n";
    }

    void print() const {
        cout << "  📄 \"" << content_ << "\" (cursor@" << cursorPos_ << ")\n";
    }
};

// --- Caretaker (History Manager) ---
class UndoHistory {
    vector<unique_ptr<EditorMemento>> history_;
public:
    void push(unique_ptr<EditorMemento> memento) {
        history_.push_back(move(memento));
    }

    unique_ptr<EditorMemento> pop() {
        if (history_.empty()) {
            cout << "  ⚠️ No more history to undo!\n";
            return nullptr;
        }
        auto memento = move(history_.back());
        history_.pop_back();
        return memento;
    }

    void showHistory() const {
        cout << "  📜 History (" << history_.size() << " snapshots):\n";
        for (int i = 0; i < (int)history_.size(); ++i) {
            cout << "    [" << i << "] " << history_[i]->getName() << "\n";
        }
    }
};

int main() {
    cout << "=== Memento Pattern: Text Editor with Undo ===" << endl;

    TextEditor editor;
    UndoHistory history;

    // Type some text, saving state before each edit
    history.push(editor.save());
    editor.type("Hello");

    history.push(editor.save());
    editor.type(" World");

    history.push(editor.save());
    editor.type("! How are you?");

    cout << "\nCurrent state:\n";
    editor.print();

    cout << "\n--- History ---\n";
    history.showHistory();

    // Undo!
    cout << "\n--- Undo 1 ---\n";
    auto m1 = history.pop();
    if (m1) editor.restore(*m1);

    cout << "\n--- Undo 2 ---\n";
    auto m2 = history.pop();
    if (m2) editor.restore(*m2);

    cout << "\n--- Undo 3 (back to empty) ---\n";
    auto m3 = history.pop();
    if (m3) editor.restore(*m3);

    cout << "\nFinal state:\n";
    editor.print();

    return 0;
}
