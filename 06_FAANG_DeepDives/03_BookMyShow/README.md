# FAANG LLD: Design BookMyShow (Movie Ticket Booking)

**Frequently Asked at:** Amazon, Flipkart, Uber, Swiggy, PhonePe

---

## Requirements
- Users can browse movies currently showing in their city.
- Users can select a movie, a showtime, a cinema hall, and specific seats.
- The system should lock selected seats to prevent double booking.
- Users make a payment. On success, a ticket is generated. On failure/timeout, the seat lock is released.
- Admin can add movies, cinema halls, and shows.

## Design Patterns Used
- **Builder:** Building complex `Booking` objects step by step (movie → show → seats → payment).
- **Strategy:** Payment can be done via CreditCard, UPI, or Wallet (swappable strategy).
- **Observer:** Notify the user via email/SMS when booking is confirmed.
- **Singleton:** `BookingManager` is a centralized coordinator.

## Class Diagram
```
┌───────────────┐     ┌───────────────┐     ┌───────────────┐
│     Movie     │     │   CinemaHall  │     │     Show      │
├───────────────┤     ├───────────────┤     ├───────────────┤
│ - title       │     │ - name        │     │ - movie       │
│ - genre       │     │ - city        │     │ - hall        │
│ - duration    │     │ - screens[]   │     │ - startTime   │
└───────────────┘     └───────────────┘     │ - seats[]     │
                                            └───────┬───────┘
                                                    │
                                            ┌───────┴───────┐
                                            │     Seat      │
                                            ├───────────────┤
                                            │ - row, col    │
                                            │ - type (VIP,  │
                                            │   Regular)    │
                                            │ - status      │
                                            │  (AVAILABLE,  │
                                            │   LOCKED,     │
                                            │   BOOKED)     │
                                            └───────────────┘

┌───────────────┐     ┌───────────────────┐
│    Booking    │     │  IPaymentStrategy │ ← Strategy
├───────────────┤     ├───────────────────┤
│ - user        │     │ + pay(amount)     │
│ - show        │     └────────┬──────────┘
│ - seats[]     │              │
│ - totalPrice  │     ┌────────┼──────────┐
│ - status      │     │        │          │
└───────────────┘  CreditCard  UPI     Wallet
```

## Key Concurrency Challenge: Seat Locking
The critical part interviewers test: **Two users selecting the same seat simultaneously.**

```
User A selects Seat 5A at 10:00:00.001
User B selects Seat 5A at 10:00:00.002

Without a lock, BOTH users see Seat 5A as AVAILABLE and complete their booking!
```

**Solution:** Use a **Pessimistic Lock (Mutex)** or a **Temporary Reservation with TTL**.
When User A selects Seat 5A, the system:
1. Acquires a lock on Seat 5A.
2. Sets status to `LOCKED` with a 5-minute TTL.
3. If User A completes payment within 5 min → status becomes `BOOKED`.
4. If User A doesn't pay → TTL expires → status reverts to `AVAILABLE`.
5. User B sees Seat 5A as `LOCKED` and cannot select it.
