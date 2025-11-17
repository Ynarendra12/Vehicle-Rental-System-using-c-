#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <map>
using namespace std;

// ===================== VEHICLE CLASS =====================
class Vehicle {
protected:
    string vehicleID;
    string name;
    string type;
    double rate;
    bool isRented;

public:
    Vehicle() {}
    Vehicle(string id, string n, string t, double r, bool rented = false)
        : vehicleID(id), name(n), type(t), rate(r), isRented(rented) {}

    string getID() { return vehicleID; }
    string getName() { return name; }
    string getType() { return type; }
    double getRate() { return rate; }
    bool getStatus() { return isRented; }

    void setStatus(bool status) { isRented = status; }

    void display() {
        cout << left << setw(10) << vehicleID
             << setw(12) << type
             << setw(12) << name
             << setw(8) << rate
             << setw(10) << (isRented ? "Rented" : "Available") << endl;
    }

    string toFileFormat() {
        return vehicleID + "," + type + "," + name + "," + to_string(rate) + "," + (isRented ? "1" : "0") + "\n";
    }
};

// ===================== CUSTOMER CLASS =====================
class Customer {
    string custID;
    string name;
    string phone;
public:
    Customer() {}
    Customer(string id, string n, string p) : custID(id), name(n), phone(p) {}

    string getID() { return custID; }
    string getName() { return name; }
    string getPhone() { return phone; }

    string toFileFormat() {
        return custID + "," + name + "," + phone + "\n";
    }
};

// ===================== RENTAL CLASS =====================
class Rental {
    Customer customer;
    Vehicle vehicle;
    int days;
    double totalCost;
    double discountApplied;

public:
    Rental() {}
    Rental(Customer c, Vehicle v, int d, double discount)
        : customer(c), vehicle(v), days(d), discountApplied(discount) {
        double base = vehicle.getRate() * days;
        totalCost = base - (base * discount / 100);
    }

    void display() {
        cout << "\n------ RENTAL DETAILS ------\n";
        cout << "Customer: " << customer.getName()
             << " | Vehicle: " << vehicle.getName()
             << " | Days: " << days
             << " | Discount: " << discountApplied << "%"
             << " | Total Bill: Rs." << totalCost << endl;
    }

    string toFileFormat() {
        return customer.getID() + "," + vehicle.getID() + "," + to_string(days) + "," + to_string(discountApplied) + "," + to_string(totalCost) + "\n";
    }
};

// ===================== FILE HANDLING =====================
void saveVehicle(Vehicle v) {
    ofstream file("vehicles.txt", ios::app);
    file << v.toFileFormat();
    file.close();
}

void saveRental(Rental r) {
    ofstream file("rentals.txt", ios::app);
    file << r.toFileFormat();
    file.close();
}

void displayAllVehicles() {
    ifstream file("vehicles.txt");
    if (!file) {
        cout << "No vehicle records found.\n";
        return;
    }

    cout << left << setw(10) << "ID"
         << setw(12) << "Type"
         << setw(12) << "Name"
         << setw(8) << "Rate"
         << setw(10) << "Status" << endl;
    cout << "-----------------------------------------------------\n";

    string id, type, name, rate, status;
    while (getline(file, id, ',')) {
        getline(file, type, ',');
        getline(file, name, ',');
        getline(file, rate, ',');
        getline(file, status, '\n');
        Vehicle v(id, name, type, stod(rate), status == "1");
        v.display();
    }
    file.close();
}

// ===================== OFFERS HANDLING =====================
map<int, double> loadOffers() {
    map<int, double> offers;
    ifstream file("offers.txt");
    if (!file) {
        cout << "No offers file found. Creating default offers...\n";
        ofstream newFile("offers.txt");
        newFile << "1,0\n2,5\n3,10\n4,12\n5,15\n6,18\n7,20\n";
        newFile.close();
        offers = {{1,0},{2,5},{3,10},{4,12},{5,15},{6,18},{7,20}};
        return offers;
    }

    string days, discount;
    while (getline(file, days, ',')) {
        getline(file, discount, '\n');
        offers[stoi(days)] = stod(discount);
    }
    file.close();
    return offers;
}

void viewOffers() {
    map<int, double> offers = loadOffers();
    cout << "\n------ CURRENT RENTAL OFFERS ------\n";
    cout << "Days\tDiscount (%)\n";
    cout << "--------------------\n";
    for (auto &offer : offers) {
        cout << offer.first << "\t" << offer.second << "%" << endl;
    }
}

// ===================== DELETE VEHICLE =====================
void deleteVehicleByID() {
    string deleteID;
    cout << "Enter Vehicle ID to delete: ";
    cin.ignore();
    getline(cin, deleteID);

    ifstream inFile("vehicles.txt");
    ofstream outFile("temp.txt");
    if (!inFile) {
        cout << "No vehicles found.\n";
        return;
    }

    string id, type, name, rate, status;
    bool found = false;

    while (getline(inFile, id, ',')) {
        getline(inFile, type, ',');
        getline(inFile, name, ',');
        getline(inFile, rate, ',');
        getline(inFile, status, '\n');

        if (id != deleteID) {
            outFile << id << "," << type << "," << name << "," << rate << "," << status << "\n";
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();

    remove("vehicles.txt");
    rename("temp.txt", "vehicles.txt");

    if (found)
        cout << "Vehicle with ID " << deleteID << " deleted successfully!\n";
    else
        cout << "Vehicle ID not found.\n";
}

// ===================== RENT VEHICLE =====================
void rentVehicle() {
    map<int, double> offers = loadOffers();
    string custID, custName, phone, vehicleID;
    int days;
    cout << "Enter Customer ID: ";
    cin.ignore();
    getline(cin, custID);
    cout << "Enter Customer Name: ";
    getline(cin, custName);
    cout << "Enter Phone Number: ";
    getline(cin, phone);
    cout << "Enter Vehicle ID to Rent: ";
    getline(cin, vehicleID);
    cout << "Enter Number of Days: ";
    cin >> days;

    double discount = (offers.count(days) ? offers[days] : 0);

    ifstream inFile("vehicles.txt");
    ofstream outFile("temp.txt");
    string id, type, name, rate, status;
    bool found = false;
    Vehicle rentedVehicle;

    while (getline(inFile, id, ',')) {
        getline(inFile, type, ',');
        getline(inFile, name, ',');
        getline(inFile, rate, ',');
        getline(inFile, status, '\n');

        if (id == vehicleID) {
            if (status == "1") {
                cout << "Vehicle is already rented!\n";
                outFile << id << "," << type << "," << name << "," << rate << "," << status << "\n";
            } else {
                cout << "Vehicle rented successfully!\n";
                rentedVehicle = Vehicle(id, name, type, stod(rate), true);
                outFile << id << "," << type << "," << name << "," << rate << ",1\n";
                found = true;
            }
        } else {
            outFile << id << "," << type << "," << name << "," << rate << "," << status << "\n";
        }
    }

    inFile.close();
    outFile.close();

    remove("vehicles.txt");
    rename("temp.txt", "vehicles.txt");

    if (found) {
        Customer c(custID, custName, phone);
        Rental r(c, rentedVehicle, days, discount);
        r.display();
        saveRental(r);
    } else {
        cout << "Vehicle ID not found.\n";
    }
}

// ===================== MAIN =====================
int main() {
    int choice;
    while (true) {
        cout << "\n========== VEHICLE RENTAL SYSTEM ==========\n";
        cout << "1. Add Vehicle\n";
        cout << "2. Display All Vehicles\n";
        cout << "3. Rent a Vehicle\n";
        cout << "4. Delete Vehicle\n";
        cout << "5. View Offers\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            string id, name, type;
            double rate;
            cout << "Enter Vehicle ID: ";
            cin.ignore();
            getline(cin, id);
            cout << "Enter Vehicle Type (Car/Bike/Truck): ";
            getline(cin, type);
            cout << "Enter Vehicle Name: ";
            getline(cin, name);
            cout << "Enter Rate per Day: ";
            cin >> rate;

            Vehicle v(id, name, type, rate, false);
            saveVehicle(v);
            cout << "Vehicle added successfully!\n";
        }
        else if (choice == 2) {
            displayAllVehicles();
        }
        else if (choice == 3) {
            rentVehicle();
        }
        else if (choice == 4) {
            deleteVehicleByID();
        }
        else if (choice == 5) {
            viewOffers();
        }
        else if (choice == 6) {
            cout << "Exiting program. Thank you!\n";
            break;
        }
        else {
            cout << "Invalid choice, try again.\n";
        }
    }

    return 0;
}
