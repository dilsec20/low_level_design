# LLD Project: Basic Parking Lot System

## Problem Statement
Design a classic Parking Lot system.
- The parking lot has multiple floors.
- Each floor has multiple parking spots.
- There are different types of vehicles (Motorcycle, Car, Bus).
- There are different types of spots (Compact, Large).
- Motorcycles can park only in Compact spots. Cars only in Large spots. Buses take up 3 Large spots in a row.

## Object-Oriented Design Approach

### 1. Identifying Entities
- **Vehicle:** An abstract base class or Interface.
    - **Motorcycle, Car, Bus:** Concrete implementations of Vehicle.
- **ParkingSpot:** Represents a single spot. Knows its size (Compact/Large), its floor, its spot number, and if it's currently occupied.
- **Level/Floor:** Contains a collection of ParkingSpots.
- **ParkingLot:** The main managing class (Facade/Singleton). Contains Levels.

### 2. Identifying the Logic
- **Parking a Vehicle:** 
  1. The `ParkingLot` receives a vehicle.
  2. It iterates through its `Levels` asking "can you park this vehicle?".
  3. The `Level` iterates through its `ParkingSpots` looking for an empty one of the correct size (or consecutive spots for a Bus).
  4. If a spot is found, the vehicle is assigned to the spot, and the spot is marked as occupied.
- **Leaving:**
  1. The vehicle leaves the spot.
  2. The spot is marked as empty.

## Pattern Highlights in this Code
In `parking_lot.cpp`, you will see:
1. **Liskov Substitution Principle (LSP):** We can pass a `Motorcycle`, `Car`, or `Bus` anywhere a `Vehicle` is expected (like when asking the `ParkingLot` to park it).
2. **Facade:** The `ParkingLot` class acts as a facade. The user just calls `parkingLot.park_vehicle()`, without knowing about `Levels`, `Spots`, or the logic of finding three consecutive spots for a bus.
3. **Encapsulation:** The internal arrays/vectors of spots belong purely to the `Level`. Outwardly, the level only exposes `park_vehicle()` and `free_spot()` methods.
