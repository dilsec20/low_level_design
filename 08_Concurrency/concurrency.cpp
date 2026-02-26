#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
#include <chrono>
#include <string>

using namespace std;

// ==========================================
// 1. RACE CONDITION (Unsafe)
// ==========================================
namespace RaceCondition {
    int counter = 0;

    void increment(int times) {
        for (int i = 0; i < times; ++i) {
            counter++;  // NOT atomic! Multiple threads can corrupt this.
        }
    }

    void demo() {
        counter = 0;
        thread t1(increment, 100000);
        thread t2(increment, 100000);
        t1.join();
        t2.join();
        // Expected: 200000, Actual: unpredictable (e.g., 143287)
        cout << "  Unsafe counter: " << counter << " (expected 200000)\n";
    }
}

// ==========================================
// 2. MUTEX FIX (Thread-Safe)
// ==========================================
namespace MutexFix {
    int counter = 0;
    mutex mtx;

    void increment(int times) {
        for (int i = 0; i < times; ++i) {
            lock_guard<mutex> lock(mtx);  // RAII lock — auto-unlocks on scope exit
            counter++;
        }
    }

    void demo() {
        counter = 0;
        thread t1(increment, 100000);
        thread t2(increment, 100000);
        t1.join();
        t2.join();
        cout << "  Safe counter:   " << counter << " (expected 200000)\n";
    }
}

// ==========================================
// 3. PRODUCER-CONSUMER Pattern
// ==========================================
namespace ProducerConsumer {
    queue<int> buffer;
    mutex mtx;
    condition_variable cv;
    bool done = false;
    const int MAX_BUFFER = 5;

    void producer() {
        for (int i = 1; i <= 10; ++i) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] { return buffer.size() < MAX_BUFFER; });

            buffer.push(i);
            cout << "  [Producer] Produced: " << i
                 << " (buffer size: " << buffer.size() << ")\n";

            lock.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        {
            lock_guard<mutex> lock(mtx);
            done = true;
        }
        cv.notify_all();
    }

    void consumer(const string& name) {
        while (true) {
            unique_lock<mutex> lock(mtx);
            cv.wait(lock, [] { return !buffer.empty() || done; });

            if (buffer.empty() && done) break;

            int item = buffer.front();
            buffer.pop();
            cout << "  [" << name << "] Consumed: " << item << "\n";

            lock.unlock();
            cv.notify_one();
            this_thread::sleep_for(chrono::milliseconds(80));
        }
    }

    void demo() {
        done = false;
        while (!buffer.empty()) buffer.pop();

        thread prod(producer);
        thread cons1(consumer, "Consumer-1");
        thread cons2(consumer, "Consumer-2");

        prod.join();
        cons1.join();
        cons2.join();
    }
}

// ==========================================
// 4. THREAD-SAFE SINGLETON (Meyers' Singleton)
// ==========================================
namespace SafeSingleton {
    class Database {
        Database() { cout << "  [DB] Connection established.\n"; }
    public:
        Database(const Database&) = delete;
        Database& operator=(const Database&) = delete;

        static Database& getInstance() {
            // C++11 guarantees thread-safe initialization of static locals
            static Database instance;
            return instance;
        }

        void query(const string& sql) {
            cout << "  [DB] Query: " << sql << "\n";
        }
    };

    void demo() {
        // Both threads get the SAME instance — no double initialization
        thread t1([] {
            Database::getInstance().query("SELECT * FROM users");
        });
        thread t2([] {
            Database::getInstance().query("SELECT * FROM orders");
        });
        t1.join();
        t2.join();
    }
}

// ==========================================
// 5. DEADLOCK PREVENTION
// ==========================================
namespace DeadlockPrevention {
    mutex mtxA, mtxB;

    // BAD: Can deadlock if thread1 locks A then waits for B,
    //      while thread2 locks B then waits for A.
    void badThread1() {
        lock_guard<mutex> lockA(mtxA);
        this_thread::sleep_for(chrono::milliseconds(10));
        // lock_guard<mutex> lockB(mtxB);  // <- DEADLOCK risk!
        cout << "  [Bad Thread 1] Would deadlock here!\n";
    }

    // GOOD: Use std::scoped_lock (C++17) to lock multiple mutexes atomically
    void goodThread1() {
        scoped_lock lock(mtxA, mtxB);  // Locks both atomically — no deadlock!
        cout << "  [Good Thread 1] Both locks acquired safely.\n";
    }

    void goodThread2() {
        scoped_lock lock(mtxA, mtxB);  // Same order guaranteed by scoped_lock
        cout << "  [Good Thread 2] Both locks acquired safely.\n";
    }

    void demo() {
        thread t1(goodThread1);
        thread t2(goodThread2);
        t1.join();
        t2.join();
    }
}

int main() {
    cout << "=== 1. Race Condition (Unsafe) ===" << endl;
    RaceCondition::demo();

    cout << "\n=== 2. Mutex Fix (Thread-Safe) ===" << endl;
    MutexFix::demo();

    cout << "\n=== 3. Producer-Consumer ===" << endl;
    ProducerConsumer::demo();

    cout << "\n=== 4. Thread-Safe Singleton ===" << endl;
    SafeSingleton::demo();

    cout << "\n=== 5. Deadlock Prevention ===" << endl;
    DeadlockPrevention::demo();

    return 0;
}
