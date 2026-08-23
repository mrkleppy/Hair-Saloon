#include <iostream>
#include <string>
#include <regex>
#include "Main.h"
#include "UserModule.h"
#include "FileProcessing.h"
#include "Validation.h"

using namespace std;

const regex STAFF_CODE_PATTERN(R"(S\d{3})"); // regex with pattern like S001

// checking which user logging in
int login(User user, Customer* customer, Staff* staff, vector<Customer>& customers, vector<Staff>& staffs) {
    // username must exist in database and password must match with corresponding user
    User currentUser;
    bool found = false;

    if (user.name == "admin" && user.password == "ADMIN") { // check for admin login
        return 3; // case 3: login as admin
    }

    if (regex_search(user.name, STAFF_CODE_PATTERN)) { // check for staff login
        for (Staff s : staffs) { 
            // verifying the code and password for corresponding staff
            if (user.name == s.staffCode && user.password == s.user.password) {
                // parsing data current user into found staff
                (*staff).user.name = s.user.name;
                (*staff).user.password = s.user.password;
                (*staff).user.phoneNo = s.user.phoneNo;
                (*staff).staffCode = s.staffCode;
                (*staff).salary = s.salary;
                (*staff).appointmentDone = s.appointmentDone;

                found = true; // flag become true
                break;
            }
        }

        if (found) { 
            return 2; // case 2: login as staff
        }
    }

    for (Customer c : customers) { // check for customer login
        // verifying the username and password for corresponding customer
        if (user.name == c.user.name && user.password == c.user.password) {
            // parsing data current user into found customer
            (*customer).user.name = c.user.name;
            (*customer).user.password = c.user.password;
            (*customer).points = c.points;

            found = true; // flag become true
            break;
        }
    }

    if (found) {
        return 1; // case 1: login as customer
    }
    else {
        return 0; // case 0: none of the user is found
    }
}

// UI for login screen
void loginScreen(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices, vector<Service>& services) {
    User user;
    Customer customer;
    Staff staff;

    do {
        // Prompt message for input
        cout << "Login" << endl;
        cout << "======\n";
        cout << "Username: ";
        getline(cin, user.name);
        cout << "Password: ";
        getline(cin, user.password);

        // get which user is logging in
        int userType = login(user, &customer, &staff, customers, staffs);

        switch (userType) {
        case 1: // navigate to member home page (user module)
            clearScreen();
            memberHomePage(customer, items, customers, staffs, appointments, receipts, invoices, services);
            break;
		case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(staff, items, customers, staffs, appointments, services, receipts);
            break;
		case 3: // navigate to admin home page (user module)
            clearScreen();
            adminHomePage(items, customers, staffs, appointments, services);
            break;
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Incorrect username or password! Please try again!" << endl;
        }

        break;
    } while (true);
}

// registration validation
bool registerValidation(User newUser, string confirmPassword, Customer *newCustomer, vector<Customer>& customers, vector<Staff>& staffs, string* message) {
    // valid the username and phone number
    if (validateUsername(newUser, customers, staffs, message) && validatePhoneNo(newUser, customers, staffs, message) && validatePassword(newUser, confirmPassword, message)) {
        (*newCustomer).user = newUser; // parse input user data into customer
        return true; // valid input
    }

    return false; // invalid input
}

// UI for registration screen
void registerScreen(vector<Staff>& staffs, vector<Customer>& customers) {
    string confirmPassword, input, message;
    User newUser;
    Customer newCustomer;
    char confirm;

    do {
        // Prompt message for input
        cout << "Register as Member" << endl;
        cout << "===================\n";
        cout << "Enter \'q\' to exit\n";
        cout << "Username (no less then 3 characters): ";
        getline(cin, newUser.name);

        if (newUser.name == "q" || newUser.name == "Q") { // return sequence
            clearScreen();
            return;
        }

        cout << "Phone number (e.g: 012-3456789): ";
        getline(cin, newUser.phoneNo);

        if (newUser.phoneNo == "q" || newUser.phoneNo == "Q") { // return sequence
            clearScreen();
            return;
        }

        cout << "Password: ";
        getline(cin, newUser.password);

        if (newUser.password == "q" || newUser.password == "Q") { // return sequence
            clearScreen();
            return;
        }

        cout << "Confirm password: ";
        getline(cin, confirmPassword);

        if (confirmPassword == "q" || confirmPassword == "Q") { // return sequence
            clearScreen();
            return;
        }

        // validating input
        if (registerValidation(newUser, confirmPassword, &newCustomer, customers, staffs, &message)) {
            newCustomer.points = 0; // default points for new customer
            clearScreen();
            break;
        }
        else {
            cout << message << "Please try again!" << endl; // invalid sequence
        }
    } while (true);

    do {
        // Prompt data to comfirm
        cout << "Username: " << newCustomer.user.name << endl;
        cout << "Phone Number: " << newCustomer.user.phoneNo << endl << endl;
        cout << "Confirm register this member (Y/N): ";
        getline(cin, input);

        if (input.empty()) { // cannot be empty
            clearScreen();
            cout << "Invalid input! Please enter N or Y!" << endl;
            continue;
        }
        else if (input.size() == 1) { // must be one character
            confirm = input[0];
            confirm = toupper(confirm); // normalise the character
        }
        else {
            clearScreen();
            cout << "Invalid input! Please enter N or Y!" << endl;
            continue;
        }

        if (confirm == 'Y') { // If the user confirms, display success message
            appendCustomerToFile(newCustomer);
			customers.push_back(newCustomer);
            cout << "\nMember registered successfully!" << endl;
            cout << "Press enter to continue..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        else if (confirm == 'N') { // If the user cancels, display cancel message
            cout << "\nMember registration cancelled." << endl;
            cout << "Press enter to continue..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        else {
            clearScreen();
            cout << "Invalid input! Please enter N or Y!" << endl;
        }
    } while (true);
}