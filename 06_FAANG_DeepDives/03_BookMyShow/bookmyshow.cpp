#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>

using namespace std;

// ==========================================
// ENUMS
// ==========================================
enum class SeatType { REGULAR, VIP };
enum class SeatStatus { AVAILABLE, LOCKED, BOOKED };
enum class BookingStatus { PENDING, CONFIRMED, CANCELLED };

// ==========================================
// ENTITIES
// ==========================================
struct Movie {
    string title;
    string genre;
    int duration_min;
};

struct Seat {
    int row;
    int col;
    SeatType type;
    SeatStatus status = SeatStatus::AVAILABLE;
    int price;
    mutex seat_lock; // Critical: Per-seat mutex for thread safety!

    Seat(int r, int c, SeatType t, int p) : row(r), col(c), type(t), price(p) {}

    string label() const {
        return string(1, 'A' + row) + to_string(col);
    }

    // Thread-safe seat locking
    bool try_lock_seat() {
        lock_guard<mutex> guard(seat_lock);
        if (status == SeatStatus::AVAILABLE) {
            status = SeatStatus::LOCKED;
            cout << "  🔒 Seat " << label() << " LOCKED.\n";
            return true;
        }
        cout << "  ❌ Seat " << label() << " is already taken!\n";
        return false;
    }

    void confirm_booking() {
        lock_guard<mutex> guard(seat_lock);
        status = SeatStatus::BOOKED;
        cout << "  ✅ Seat " << label() << " BOOKED.\n";
    }

    void release_lock() {
        lock_guard<mutex> guard(seat_lock);
        if (status == SeatStatus::LOCKED) {
            status = SeatStatus::AVAILABLE;
            cout << "  🔓 Seat " << label() << " released back to AVAILABLE.\n";
        }
    }
};

struct Show {
    Movie movie;
    string time_slot;
    string hall_name;
    vector<shared_ptr<Seat>> seats;
    
    void display_seats() const {
        cout << "\n--- Seats for " << movie.title << " @ " << time_slot << " ---\n";
        for (const auto& s : seats) {
            char status_char = (s->status == SeatStatus::AVAILABLE) ? 'O' : 'X';
            cout << "[" << s->label() << ":" << status_char << "] ";
        }
        cout << "\n";
    }
};

// ==========================================
// PAYMENT STRATEGY
// ==========================================
class IPaymentStrategy {
public:
    virtual bool pay(int amount) = 0;
    virtual ~IPaymentStrategy() = default;
};

class CreditCardPayment : public IPaymentStrategy {
public:
    bool pay(int amount) override {
        cout << "  💳 Charged $" << amount << " to Credit Card.\n";
        return true;
    }
};

class UPIPayment : public IPaymentStrategy {
public:
    bool pay(int amount) override {
        cout << "  📱 Paid $" << amount << " via UPI.\n";
        return true;
    }
};

// ==========================================
// BOOKING
// ==========================================
struct Booking {
    int booking_id;
    string user_name;
    Show* show;
    vector<shared_ptr<Seat>> booked_seats;
    int total_price = 0;
    BookingStatus status = BookingStatus::PENDING;

    void print_ticket() const {
        cout << "\n🎬 ========= TICKET =========\n";
        cout << "Booking ID: " << booking_id << "\n";
        cout << "Movie: " << show->movie.title << "\n";
        cout << "Time: " << show->time_slot << " | Hall: " << show->hall_name << "\n";
        cout << "Seats: ";
        for (const auto& s : booked_seats) cout << s->label() << " ";
        cout << "\nTotal: $" << total_price << "\n";
        cout << "Status: CONFIRMED ✅\n";
        cout << "============================\n";
    }
};

// ==========================================
// BOOKING MANAGER (Controller / Facade)
// ==========================================
class BookingManager {
private:
    int next_booking_id = 1000;
public:
    Booking* create_booking(const string& user, Show* show, 
                            vector<int> seat_indices, 
                            unique_ptr<IPaymentStrategy> payment) {
        
        cout << "\n--- " << user << " attempting to book ---\n";
        
        // Step 1: Lock the requested seats
        vector<shared_ptr<Seat>> locked_seats;
        for (int idx : seat_indices) {
            if (show->seats[idx]->try_lock_seat()) {
                locked_seats.push_back(show->seats[idx]);
            } else {
                // Rollback: Release all already-locked seats
                cout << "  ⚠️ Booking failed! Rolling back...\n";
                for (auto& s : locked_seats) s->release_lock();
                return nullptr;
            }
        }

        // Step 2: Calculate total price
        int total = 0;
        for (auto& s : locked_seats) total += s->price;

        // Step 3: Process payment
        cout << "  Processing payment of $" << total << "...\n";
        if (!payment->pay(total)) {
            cout << "  ⚠️ Payment FAILED! Releasing seats.\n";
            for (auto& s : locked_seats) s->release_lock();
            return nullptr;
        }

        // Step 4: Confirm booking
        for (auto& s : locked_seats) s->confirm_booking();

        Booking* booking = new Booking();
        booking->booking_id = next_booking_id++;
        booking->user_name = user;
        booking->show = show;
        booking->booked_seats = locked_seats;
        booking->total_price = total;
        booking->status = BookingStatus::CONFIRMED;

        return booking;
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    // Setup
    Movie avengers{"Avengers: Endgame", "Action", 181};
    
    Show show;
    show.movie = avengers;
    show.time_slot = "7:00 PM";
    show.hall_name = "Screen 1";
    
    // Create seats (2 rows x 5 cols)
    for (int r = 0; r < 2; r++) {
        for (int c = 1; c <= 5; c++) {
            SeatType type = (r == 0) ? SeatType::VIP : SeatType::REGULAR;
            int price = (type == SeatType::VIP) ? 15 : 10;
            show.seats.push_back(make_shared<Seat>(r, c, type, price));
        }
    }

    show.display_seats();

    BookingManager manager;

    // User 1 books VIP seats A1, A2
    Booking* b1 = manager.create_booking("Alice", &show, {0, 1}, make_unique<CreditCardPayment>());
    if (b1) b1->print_ticket();

    // User 2 tries to book seat A1 (already taken!)
    Booking* b2 = manager.create_booking("Bob", &show, {0, 4}, make_unique<UPIPayment>());
    if (b2) b2->print_ticket();

    // User 3 tries the SAME seat as Alice → should FAIL
    Booking* b3 = manager.create_booking("Charlie", &show, {0}, make_unique<CreditCardPayment>());
    if (!b3) cout << "Charlie's booking failed as expected.\n";

    show.display_seats();

    delete b1; delete b2;
    return 0;
}
