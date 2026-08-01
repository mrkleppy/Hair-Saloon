#include <iostream>
#include <string>
#include "Main.h"
#include "LoginModule.h"
#include "FileProcessing.h"

using namespace std;

void clearScreen() { // Clear the console screen
    cout << "\033[2J\033[H";
}

void loadData(vector<Item>& items, vector<Staff>& staffs, vector<Customer>& customers) {
	readItemFile(items); // Load item data from file
	readStaffFile(staffs); // Load staff data from file
	readCustomerFile(customers); // Load customer data from file
}

int main() {
    int selection = 0;
    string input;
	vector<Item> items;
	vector<Staff> staffs;
	vector<Customer> customers;

	loadData(items, staffs, customers); // Load data from files // Add more later

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
            loginScreen(items, customers, staffs);
            break;
		case 2: // navigate to register screen (login module)
            clearScreen();
            registerScreen(staffs, customers);
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