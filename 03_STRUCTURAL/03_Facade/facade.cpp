#include <iostream>

using namespace std;

// ==========================================
// COMPLEX SUBSYSTEMS
// ==========================================
// These are the individual, complex components of a system.
struct TV {
    void turn_off() { cout << "TV: Powering down.\n"; }
    void turn_on() { cout << "TV: Hello! Playing channel 1.\n"; }
};

struct ClimateControl {
    void set_temperature(int temp) { cout << "Climate: Setting temperature to " << temp << " degrees.\n"; }
    void eco_mode_on() { cout << "Climate: Entering eco mode (saving power).\n"; }
};

struct SecuritySystem {
    void arm() { cout << "Security: Alarms ARMED. Motion detection active.\n"; }
    void disarm() { cout << "Security: Alarms disarmed. Welcome home.\n"; }
};

// ==========================================
// BAD DESIGN (Client handles complexity)
// ==========================================
// The client has to know about every single subsystem and how to interact with it.
// This is tedious and error-prone. What if they forget to arm the security system?
void leave_house_bad(TV& tv, ClimateControl& climate, SecuritySystem& security) {
    cout << "--- Leaving the house (BAD way) ---\n";
    tv.turn_off();
    climate.set_temperature(18); // Set a low temp
    climate.eco_mode_on();
    security.arm();
}

// ==========================================
// GOOD DESIGN (Facade Pattern)
// ==========================================
// The Facade provides a simple interface to the complex subsystems.
// The client only talks to the Facade!
struct SmartHomeFacade {
    // The facade holds references (or pointers) to the subsystems it manages
    TV tv;
    ClimateControl climate;
    SecuritySystem security;

    // A simple, single method for a complex operation
    void leave_home() {
        cout << "--- Leaving the house (GOOD way via Facade) ---\n";
        tv.turn_off();
        climate.set_temperature(18); // The facade handles the logic of what temp to set
        climate.eco_mode_on();
        security.arm();
    }

    void come_home() {
        cout << "--- Coming home (via Facade) ---\n";
        security.disarm();
        climate.set_temperature(22);
        tv.turn_on();
    }
};


int main() {
    
    // The Bad Way requires the client to instantiate and manage all components manually.
    TV a_tv;
    ClimateControl a_climate;
    SecuritySystem a_security;
    leave_house_bad(a_tv, a_climate, a_security);


    cout << "\n";


    // The Good Way using a Facade
    // The client doesn't need to know that a TV, ClimateControl, or SecuritySystem even exists!
    SmartHomeFacade home;
    
    // One simple function call replaces 4 complex ones.
    home.leave_home();
    
    cout << "\n  [Time passes]\n\n";

    home.come_home();

    return 0;
}
