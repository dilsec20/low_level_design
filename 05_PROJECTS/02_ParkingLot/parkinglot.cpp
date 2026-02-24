#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ==========================================
// MODELS (Entities)
// ==========================================

enum class VehicleSize { Motorcycle, Compact, Large };

// Abstract Base Class for all Vehicles
class Vehicle {
protected:
    string license_plate;
    int spots_needed;
    VehicleSize size;

public:
    Vehicle(string plate, int spots, VehicleSize size) 
        : license_plate(plate), spots_needed(spots), size(size) {}
    
    virtual ~Vehicle() = default;

    int get_spots_needed() const { return spots_needed; }
    VehicleSize get_size() const { return size; }
    string get_plate() const { return license_plate; }
    
    // Abstract method to force subclasses to define how they print
    virtual void print() const = 0; 
};

class Motorcycle : public Vehicle {
public:
    Motorcycle(string plate) : Vehicle(plate, 1, VehicleSize::Motorcycle) {}
    void print() const override { cout << "🏍️  Motorcycle [" << license_plate << "]"; }
};

class Car : public Vehicle {
public:
    Car(string plate) : Vehicle(plate, 1, VehicleSize::Compact) {}
    void print() const override { cout << "🚗 Car [" << license_plate << "]"; }
};

class Bus : public Vehicle {
public:
    // A bus needs 5 consecutive large spots
    Bus(string plate) : Vehicle(plate, 5, VehicleSize::Large) {}
    void print() const override { cout << "🚌 Bus [" << license_plate << "]"; }
};

// ==========================================
// INFRASTRUCTURE
// ==========================================

class ParkingSpot {
private:
    Vehicle* vehicle; // Pointer to the vehicle parked here (if any)
    VehicleSize spot_size;
    int row;
    int spot_number;

public:
    ParkingSpot(int r, int n, VehicleSize sz) 
        : vehicle(nullptr), row(r), spot_number(n), spot_size(sz) {}

    bool is_available() const { return vehicle == nullptr; }
    
    // Checking if a specific vehicle CAN fit in this specific spot
    bool can_fit_vehicle(Vehicle* v) const {
        if (!is_available()) return false;
        
        // Motorcycle fits in any spot
        if (v->get_size() == VehicleSize::Motorcycle) return true;
        
        // Car (Compact) fits in Compact or Large
        if (v->get_size() == VehicleSize::Compact) {
            return spot_size == VehicleSize::Compact || spot_size == VehicleSize::Large;
        }

        // Bus (Large) ONLY fits in Large spots
        return spot_size == VehicleSize::Large;
    }

    bool park(Vehicle* v) {
        if (!can_fit_vehicle(v)) return false;
        vehicle = v;
        return true;
    }

    void remove_vehicle() {
        vehicle = nullptr;
    }

    void print() const {
        if (!is_available()) {
            cout << "[ X ] "; // Occupied
        } else {
            if (spot_size == VehicleSize::Large) cout << "[ L ] ";
            else if (spot_size == VehicleSize::Compact) cout << "[ C ] ";
            else cout << "[ M ] ";
        }
    }
};

class Level {
private:
    int floor;
    vector<ParkingSpot> spots;
    int available_spots;
    static const int SPOTS_PER_ROW = 10;

public:
    Level(int flr, int num_spots) : floor(flr), available_spots(num_spots) {
        // Simple assignment: half compact, half large
        for (int i = 0; i < num_spots; ++i) {
            VehicleSize sz = (i < num_spots / 2) ? VehicleSize::Compact : VehicleSize::Large;
            spots.emplace_back(ParkingSpot(i / SPOTS_PER_ROW, i, sz));
        }
    }

    // Attempt to park a vehicle on this level.
    // Handles finding consecutive spots for buses!
    bool park_vehicle(Vehicle* v) {
        if (available_spots < v->get_spots_needed()) return false;

        int spots_needed = v->get_spots_needed();
        int consecutive_spots = 0;
        int start_index = -1;

        for (int i = 0; i < spots.size(); ++i) {
            if (spots[i].can_fit_vehicle(v)) {
                if (start_index == -1) start_index = i;
                consecutive_spots++;

                if (consecutive_spots == spots_needed) {
                    // We found enough space! Park them.
                    for (int j = start_index; j <= i; ++j) {
                        spots[j].park(v);
                    }
                    available_spots -= spots_needed;
                    return true;
                }
            } else {
                // Streak broken, reset 
                consecutive_spots = 0;
                start_index = -1;
            }
        }
        return false;
    }

    void print() const {
        cout << "Floor " << floor << ": ";
        for (int i = 0; i < spots.size(); ++i) {
            spots[i].print();
            if ((i + 1) % SPOTS_PER_ROW == 0) cout << "\n         ";
        }
        cout << "\n";
    }
};


// ==========================================
// THE FACADE (Main System)
// ==========================================
class ParkingLot {
private:
    vector<Level> levels;

public:
    ParkingLot(int num_levels, int spots_per_level) {
        for (int i = 0; i < num_levels; ++i) {
            levels.emplace_back(Level(i, spots_per_level));
        }
    }

    // The client only interacts with this simple method!
    bool park_vehicle(Vehicle* v) {
        cout << "Attempting to park ";
        v->print();
        cout << "...\n";

        for (auto& level : levels) {
            if (level.park_vehicle(v)) {
                cout << "-> Successfully parked!\n";
                return true;
            }
        }
        cout << "-> Lot is Full. Cannot park.\n";
        return false;
    }

    void print() const {
        cout << "--- Parking Lot Status ---\n";
        for (const auto& level : levels) {
            level.print();
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    ParkingLot lot(2, 20); // 2 levels, 20 spots each

    Car c1("CAR-001");
    Car c2("CAR-002");
    Motorcycle m1("MOTO-99");
    Bus b1("BUS-1234");
    Bus b2("BUS-5678"); // Might not fit depending on layout!

    lot.park_vehicle(&c1);
    lot.park_vehicle(&c2);
    lot.park_vehicle(&m1);
    lot.park_vehicle(&b1);

    cout << "\n";
    lot.print();

    cout << "\n";
    lot.park_vehicle(&b2); // Let's see if another bus fits!

    return 0;
}
