# 🚗 Vehicle Rental System (C++ Project)

A simple and efficient **Vehicle Rental Management System** built using **Object-Oriented Programming (OOP) in C++**.  
This project supports adding vehicles, renting vehicles, applying offers, deleting entries, and maintaining records using file handling.

---

## 📌 Features

### 🔹 1. Add Vehicle
- Add new vehicles with details:
  - Vehicle ID  
  - Type (Car/Bike/Truck)  
  - Name  
  - Rate per day  
- Automatically saved in `vehicles.txt`

### 🔹 2. Display All Vehicles
Shows all vehicles along with:
- ID  
- Type  
- Name  
- Rate  
- Status (Available / Rented)

### 🔹 3. Rent a Vehicle
- Enter customer details
- Check if the vehicle is available
- Apply **discount offers** based on number of days
- Store rental details in `rentals.txt`

### 🔹 4. Delete a Vehicle
- Delete vehicle data using Vehicle ID
- Safely updates the file and removes entry

### 🔹 5. View Offers
Offers are loaded from `offers.txt`:
