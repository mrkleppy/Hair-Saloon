#include <iostream>
#include <string>
#include "Main.h"
#include "LoginModule.h"
#include "FileProcessing.h"

using namespace std;

void clearScreen() { // Clear the console screen
    cout << "\033[2J\033[H";
}

void loadData(vector<Item>& items, vector<Service>& services, vector<Staff>& staffs, vector<Customer>& customers, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices) {
	readItemFile(items); // Load item data from file
	readServiceFile(services); // Load service data from file
	readStaffFile(staffs); // Load staff data from file
	readCustomerFile(customers); // Load customer data from file
	readAppointmentFile(appointments); // Load appointment data from file
    readReceiptFile(receipts); // Load receipt data from file
    readInvoiceFile(invoices); // Load invoice data from file
}
int main() {
    // Varicable declaration
    int selection = 0;
    string input;

    // All data storing vector
	vector<Item> items;
	vector<Service> services;
	vector<Staff> staffs;
	vector<Customer> customers;
	vector<Appointment> appointments;
    vector<Receipt> receipts;
    vector<Invoice> invoices;

	loadData(items, services, staffs, customers, appointments, receipts, invoices); // Load data from files

	do { // Display the main menu UI
        cout << "Welcome to XYZ Saloon!" << endl;
        cout << "=======================\n";
        cout << "1. Login\n2. Register as member\n0. Exit" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);
        
        // Input validation
        if (input.empty()) { // Cannot be empty
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
            continue;
        }
        
        try {
            size_t pos;
            selection = stoi(input, &pos); // Convert string to integer

            if (pos != input.size()) { 
                clearScreen();
                cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
                continue;
            }
        }
        catch (...) { // Mismtach happens
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
            continue;
        }

        switch (selection) {
		case 1: // navigate to login screen (login module)
            clearScreen();
            loginScreen(items, customers, staffs, appointments, receipts, invoices, services);
            break;
		case 2: // navigate to register screen (login module)
            clearScreen();
            registerScreen(staffs, customers);
            break;
		case 0: // exit the program (Byebye!)
            exit(0);
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
        }
    } while (true);

    return 0;
}