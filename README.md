# Railway Reservation System

## Project Overview

The Railway Reservation System is a console-based application developed in C that simulates a real-world railway ticket booking system. It implements core functionalities such as ticket booking, seat allocation, waiting list management, cancellation handling, and persistent data storage using file handling techniques.

The system is designed using structured programming principles and demonstrates practical implementation of data structures, modular design, and file-based persistence.

---

## Key Features

### Ticket Booking System
- Supports booking for three types of coaches:
  - AC Coach
  - Non-AC Coach
  - Seater Coach
- Captures passenger details including name, age, gender, source, destination, and journey date.
- Generates a unique PNR for each booking.

### Seat Allocation
- Automatically assigns seats based on availability.
- Maintains independent seat counters for each coach type.
- Prevents overbooking using predefined capacity limits.

### Waiting List Management
- Automatically adds passengers to a waiting list when seats are unavailable.
- Maintains separate waiting lists for each coach type.
- Supports automatic promotion from waiting list to confirmed seat when availability arises.

### Cancellation System
- Cancels bookings using PNR identification.
- Frees allocated seats and updates system state.
- Automatically shifts waiting list passengers to confirmed seats when possible.

### Chart Preparation
- Displays complete reservation details including:
  - Confirmed bookings
  - Waiting list passengers
- Provides structured output for administrative review.

### System Reset
- Clears all booking data and resets system state.

---

## System Architecture

The application is modular and divided into the following functional components:

- Booking Module
- Cancellation Module
- Availability Check Module
- Waiting List Management Module
- Chart Generation Module
- File Handling Module

---

## Data Model

Passenger data is structured using a C `struct` containing:

- Name
- Age
- Gender
- Coach Type
- Seat Number
- PNR
- Source Station
- Destination Station
- Journey Date

---

## File Handling

The system uses persistent file storage to maintain data across sessions:

### reservation.txt
- Stores all booking records in tabular format
- Maintains status fields: CONFIRMED, WAITING, CANCELLED

### system.dat
- Binary file storing system state:
  - Seat counts
  - Waiting list counts
  - Serial number for PNR generation

### chart.txt
- Generates structured report of waiting list passengers
- Used for quick administrative reference

---

## Core Workflow

1. User selects booking option
2. System collects passenger details
3. Seat availability is checked based on selected coach
4. If available:
   - Seat is allocated
   - PNR is generated
   - Data is stored in files
5. If unavailable:
   - Passenger is added to waiting list
6. On cancellation:
   - Seat is released
   - Waiting list is updated automatically
7. All changes are persisted using file handling

---

## Technologies Used

- C Programming Language
- Structures and Enumerations
- Arrays and Strings
- File Handling (Text and Binary Files)
- Modular Programming Approach
- Queue-like Waiting List Implementation

---

## Project Structure

railway-reservation-system/
│
├── src/
│   └── main.c
├── data/
│   └── reservation.txt
│   └── system.dat
│   └── chart.txt
└── README.md

---

## Limitations

- Console-based interface only
- No database integration
- Fixed-size storage using arrays
- No multi-user support

---

## Future Enhancements

- GUI-based interface
- Database integration (MySQL / SQLite)
- User authentication system
- Dynamic memory allocation for scalability

---

## Learning Outcomes

- System design using C
- File handling and persistent storage
- Modular programming approach
- Queue-based waiting list logic
- Input validation and error handling