#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
#include <memory>
#include <cmath>

using namespace std;

// ==========================================
// ENUMS
// ==========================================
enum class Direction { UP, DOWN, IDLE };
enum class ElevatorState { IDLE, MOVING, DOOR_OPEN };

string dir_to_string(Direction d) {
    if (d == Direction::UP) return "UP";
    if (d == Direction::DOWN) return "DOWN";
    return "IDLE";
}

// ==========================================
// REQUEST (Data Object)
// ==========================================
struct Request {
    int floor;
    Direction direction;
    Request(int f, Direction d) : floor(f), direction(d) {}
};

// ==========================================
// ELEVATOR CAR
// ==========================================
class ElevatorCar {
private:
    int id;
    int current_floor;
    Direction direction;
    ElevatorState state;
    vector<int> destinations; // Floors this elevator needs to visit

public:
    ElevatorCar(int id) : id(id), current_floor(0), direction(Direction::IDLE), state(ElevatorState::IDLE) {}

    int get_id() const { return id; }
    int get_current_floor() const { return current_floor; }
    Direction get_direction() const { return direction; }
    bool is_idle() const { return state == ElevatorState::IDLE; }

    void add_destination(int floor) {
        destinations.push_back(floor);
        if (floor > current_floor) direction = Direction::UP;
        else if (floor < current_floor) direction = Direction::DOWN;
        state = ElevatorState::MOVING;
    }

    // Simulate one step of movement
    void step() {
        if (destinations.empty()) {
            state = ElevatorState::IDLE;
            direction = Direction::IDLE;
            return;
        }

        int target = destinations.front();

        if (current_floor < target) {
            current_floor++;
            direction = Direction::UP;
            cout << "  Elevator " << id << ": Moving UP to floor " << current_floor << "\n";
        } else if (current_floor > target) {
            current_floor--;
            direction = Direction::DOWN;
            cout << "  Elevator " << id << ": Moving DOWN to floor " << current_floor << "\n";
        }

        if (current_floor == target) {
            cout << "  Elevator " << id << ": *** DOORS OPEN at floor " << current_floor << " ***\n";
            destinations.erase(destinations.begin());
            if (destinations.empty()) {
                state = ElevatorState::IDLE;
                direction = Direction::IDLE;
            }
        }
    }

    void print_status() const {
        cout << "Elevator " << id << ": Floor=" << current_floor 
             << " Dir=" << dir_to_string(direction)
             << " Stops=" << destinations.size() << "\n";
    }
};

// ==========================================
// DISPATCHER STRATEGY (Interface)
// ==========================================
class IDispatcher {
public:
    virtual ElevatorCar* select_elevator(const vector<unique_ptr<ElevatorCar>>& elevators, const Request& request) = 0;
    virtual ~IDispatcher() = default;
};

// Concrete Strategy: Pick the NEAREST idle or same-direction elevator
class NearestDispatcher : public IDispatcher {
public:
    ElevatorCar* select_elevator(const vector<unique_ptr<ElevatorCar>>& elevators, const Request& request) override {
        ElevatorCar* best = nullptr;
        int min_distance = numeric_limits<int>::max();

        for (const auto& e : elevators) {
            int distance = abs(e->get_current_floor() - request.floor);

            // Prefer idle elevators or those moving toward the request
            bool is_suitable = e->is_idle() || 
                (e->get_direction() == Direction::UP && request.floor >= e->get_current_floor()) ||
                (e->get_direction() == Direction::DOWN && request.floor <= e->get_current_floor());

            if (is_suitable && distance < min_distance) {
                min_distance = distance;
                best = e.get();
            }
        }

        // Fallback: if no suitable elevator, just pick the absolutely closest one
        if (!best) {
            for (const auto& e : elevators) {
                int distance = abs(e->get_current_floor() - request.floor);
                if (distance < min_distance) {
                    min_distance = distance;
                    best = e.get();
                }
            }
        }
        return best;
    }
};

// ==========================================
// ELEVATOR SYSTEM (Facade / Controller)
// ==========================================
class ElevatorSystem {
private:
    vector<unique_ptr<ElevatorCar>> elevators;
    unique_ptr<IDispatcher> dispatcher;

public:
    ElevatorSystem(int num_elevators, unique_ptr<IDispatcher> disp) : dispatcher(move(disp)) {
        for (int i = 0; i < num_elevators; ++i) {
            elevators.push_back(make_unique<ElevatorCar>(i + 1));
        }
    }

    void handle_request(const Request& req) {
        cout << "\n>> External Request: Floor " << req.floor << " " << dir_to_string(req.direction) << "\n";
        ElevatorCar* selected = dispatcher->select_elevator(elevators, req);
        if (selected) {
            cout << "   Dispatched Elevator " << selected->get_id() << " to floor " << req.floor << "\n";
            selected->add_destination(req.floor);
        }
    }

    void press_floor_button(int elevator_id, int destination_floor) {
        cout << "\n>> Internal Button: Elevator " << elevator_id << " → Floor " << destination_floor << "\n";
        elevators[elevator_id - 1]->add_destination(destination_floor);
    }

    void simulate_steps(int steps) {
        cout << "\n--- Simulating " << steps << " time steps ---\n";
        for (int s = 0; s < steps; ++s) {
            for (auto& e : elevators) {
                e->step();
            }
        }
    }

    void print_status() const {
        cout << "\n--- Elevator Status ---\n";
        for (const auto& e : elevators) {
            e->print_status();
        }
    }
};

// ==========================================
// MAIN
// ==========================================
int main() {
    // Create a system with 3 elevators and a NearestDispatcher strategy
    ElevatorSystem system(3, make_unique<NearestDispatcher>());

    system.print_status();

    // User on floor 5 presses UP
    system.handle_request(Request(5, Direction::UP));
    system.simulate_steps(5);

    // User inside elevator 1 presses floor 8
    system.press_floor_button(1, 8);
    system.simulate_steps(3);

    // Simultaneously, user on floor 2 presses DOWN
    system.handle_request(Request(2, Direction::DOWN));
    system.simulate_steps(3);

    system.print_status();

    return 0;
}
