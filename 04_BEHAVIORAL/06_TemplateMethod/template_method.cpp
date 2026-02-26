#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ==========================================
// GOOD DESIGN (Template Method Pattern)
// ==========================================

// --- Abstract Class with Template Method ---
class DataMiner {
public:
    virtual ~DataMiner() = default;

    // THE TEMPLATE METHOD — defines the algorithm skeleton
    // This is final (non-virtual) — subclasses cannot change the order!
    void mine(const string& path) {
        string raw = openFile(path);     // Step 1
        auto data = extractData(raw);    // Step 2 (varies)
        auto parsed = parseData(data);   // Step 3 (varies)
        auto analysis = analyze(parsed); // Step 4
        generateReport(analysis);        // Step 5

        // Hook — optional step that subclasses CAN override
        cleanup();
    }

protected:
    // Common steps (shared by all subclasses)
    string openFile(const string& path) {
        cout << "  📂 Opening file: " << path << "\n";
        return "raw_file_content";
    }

    // Abstract steps — MUST be overridden by subclasses
    virtual vector<string> extractData(const string& raw) = 0;
    virtual vector<pair<string, int>> parseData(const vector<string>& data) = 0;

    // Common analysis step
    string analyze(const vector<pair<string, int>>& parsed) {
        cout << "  📊 Analyzing " << parsed.size() << " records...\n";
        int total = 0;
        for (auto& [key, val] : parsed) total += val;
        return "Total value: " + to_string(total);
    }

    // Common report step
    void generateReport(const string& analysis) {
        cout << "  📋 Report: " << analysis << "\n";
    }

    // Hook method — default does nothing, subclasses can override
    virtual void cleanup() {
        cout << "  🧹 Default cleanup.\n";
    }
};

// --- Concrete Subclass: CSV Mining ---
class CSVDataMiner : public DataMiner {
protected:
    vector<string> extractData(const string& raw) override {
        cout << "  📄 [CSV] Extracting rows from CSV format...\n";
        return {"name,100", "age,25", "score,95"};
    }

    vector<pair<string, int>> parseData(const vector<string>& data) override {
        cout << "  🔍 [CSV] Parsing comma-separated values...\n";
        vector<pair<string, int>> result;
        for (auto& row : data) {
            auto pos = row.find(',');
            result.push_back({row.substr(0, pos), stoi(row.substr(pos + 1))});
        }
        return result;
    }
};

// --- Concrete Subclass: JSON Mining ---
class JSONDataMiner : public DataMiner {
protected:
    vector<string> extractData(const string& raw) override {
        cout << "  📄 [JSON] Extracting fields from JSON format...\n";
        return {"revenue:500", "users:200", "sessions:800"};
    }

    vector<pair<string, int>> parseData(const vector<string>& data) override {
        cout << "  🔍 [JSON] Parsing key:value pairs...\n";
        vector<pair<string, int>> result;
        for (auto& row : data) {
            auto pos = row.find(':');
            result.push_back({row.substr(0, pos), stoi(row.substr(pos + 1))});
        }
        return result;
    }

    // Override the hook to add custom cleanup
    void cleanup() override {
        cout << "  🧹 [JSON] Closing JSON parser and freeing memory.\n";
    }
};

int main() {
    cout << "=== CSV Data Mining ===" << endl;
    CSVDataMiner csvMiner;
    csvMiner.mine("data/sales.csv");

    cout << "\n=== JSON Data Mining ===" << endl;
    JSONDataMiner jsonMiner;
    jsonMiner.mine("data/metrics.json");

    // Both follow the EXACT same algorithm structure
    // but extract and parse data differently.
    // Adding XMLDataMiner? Just override extractData() and parseData().

    return 0;
}
