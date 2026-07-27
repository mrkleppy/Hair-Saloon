#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Main.h"
#include "LoginModule.h"

using namespace std;

const Service services[4] = {
    {"Hair Cut", 25, 45},
    {"Hair Coloring", 80, 170},
    {"Hair Treatment", 70, 90},
    {"Styling", 30, 35}
};

void clearScreen() { // Clear the console screen
    cout << "\033[2J\033[H";
}

vector<Item> readItemFile() {
    ifstream inFile("Item.txt");
    string line;
    vector<Item> items;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string element;
        vector<string> elements;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Item item = { elements.at(0), stod(elements.at(1)), stoi(elements.at(2)), stoi(elements.at(3)) };
        items.push_back(item);
    }

    inFile.close();
    return items;
}

vector<Staff> readStaffFile() {
    ifstream inFile("Staff.txt");
    string line;
    vector<Staff> staffs;

    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Staff staff = { elements.at(0), elements.at(1), elements.at(2), elements.at(3), stod(elements.at(4)), stoi(elements.at(5)) };
        staffs.push_back(staff);
    }

    inFile.close();
    return staffs;
}

vector<Customer> readCustomerFile() {
    ifstream inFile("Customer.txt");
    string line;
    vector<Customer> customers;

    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Customer customer = { elements.at(0), elements.at(1), elements.at(2), stoi(elements.at(3)) };
        customers.push_back(customer);
    }

    inFile.close();
    return customers;
}

void overwriteItemFile(vector<Item> items) {
    ofstream outFile("Item.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Item item : items) {
        outFile << item.name << "," << item.price << "," << item.pointCost << "," << item.stock << "\n";
    }

    outFile.close();
}

void overwriteStaffFile(vector<Staff> staffs) {
    ofstream outFile("Staff.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Staff staff : staffs) {
        outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode
            << "," << staff.salary << "," << staff.appointmentDone << "\n";
    }

    outFile.close();
}

void overwriteCustomerFile(vector<Customer> customers) {
    ofstream outFile("Customer.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Customer customer : customers) {
        outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    }

    outFile.close();
}

void appendStaffToFile(Staff staff) {
    ofstream outFile("Staff.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode 
        << "," << staff.salary << "," << staff.appointmentDone << "\n";
    outFile.close();
}

void appendCustomerToFile(Customer customer) {
    ofstream outFile("Customer.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    outFile.close();
}

int main() {
    int selection = 0;
    string input;

	do { // Display the main menu UI
        cout << "Welcome to XYZ Saloon!" << endl;
        cout << "=======================\n";
        cout << "1. Login\n2. Register as member\n0. Exit" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
            continue;
        }
        
        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
            continue;
        }

        switch (selection) {
		case 1: // navigate to login screen (login module)
            clearScreen();
            loginScreen();
            break;
		case 2: // navigate to register screen (login module)
            clearScreen();
            registerScreen();
            break;
		case 0: // exit the program (Byebye!)
            exit(0);
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!";
        }
    } while (true);

    return 0;
}