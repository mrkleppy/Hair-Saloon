#include <iostream>
#include <string>
#include <regex>
#include "Main.h"
#include "UserModule.h"
#include "FileProcessing.h"
#include "Validation.h"

using namespace std;

const regex STAFF_CODE_PATTERN(R"(S\d{3})"); // regex with pattern like S001

int login(User user, Customer* customer, Staff* staff, vector<Customer>& customers, vector<Staff>& staffs) {
    // username must exist in database and password must match with corresponding user
    User currentUser;
    bool found = false;

    if (user.name == "admin" && user.password == "ADMIN") {
        return 3;
    }

    if (regex_search(user.name, STAFF_CODE_PATTERN)) {
        for (Staff s : staffs) {
            if (user.name == s.staffCode && user.password == s.user.password) {
                (*staff).user.name = s.user.name;
                (*staff).user.password = s.user.password;
                (*staff).user.phoneNo = s.user.phoneNo;
                (*staff).staffCode = s.staffCode;
                (*staff).salary = s.salary;
                (*staff).appointmentDone = s.appointmentDone;

                found = true;
                break;
            }
        }

        if (found) {
            return 2;
        }
    }

    for (Customer c : customers) {
        if (user.name == c.user.name && user.password == c.user.password) {
            (*customer).user.name = c.user.name;
            (*customer).user.password = c.user.password;
            (*customer).points = c.points;

            found = true;
            break;
        }
    }

    if (found) {
        return 1;
    }
    else {
        return 0;
    }
}

void loginScreen(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments) {
    User user;
    Customer customer;
    Staff staff;

    do {
        cout << "Login" << endl;
        cout << "======\n";
        cout << "Username: ";
        getline(cin, user.name);
        cout << "Password: ";
        getline(cin, user.password);

        int userType = login(user, &customer, &staff, customers, staffs);

        switch (userType) {
        case 1: // navigate to member home page (user module)
            clearScreen();
            memberHomePage(customer, items, customers, staffs, appointments);
            break;
		case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(staff, items, customers, staffs, appointments);
            break;
		case 3: // navigate to admin home page (user module)
            clearScreen();
            adminHomePage(items, customers, staffs, appointments);
            break;
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Incorrect username or password! Please try again!" << endl;
        }

        break;
    } while (true);
}

bool registerValidation(User newUser, string confirmPassword, Customer *newCustomer, vector<Customer>& customers, vector<Staff>& staffs, string* message) {
    if (validateUsername(newUser, customers, staffs, message) && validatePhoneNo(newUser, customers, staffs, message) && validatePassword(newUser, confirmPassword, message)) {
        (*newCustomer).user = newUser;
        return true;
    }

    return false;
}

void registerScreen(vector<Staff>& staffs, vector<Customer>& customers) {
    string confirmPassword, input, message;
    User newUser;
    Customer newCustomer;
    char confirm;

    do {
        cout << "Register as Member" << endl;
        cout << "===================\n";
        cout << "Username: ";
        getline(cin, newUser.name);
        cout << "Phone number: ";
        getline(cin, newUser.phoneNo);
        cout << "Password: ";
        getline(cin, newUser.password);
        cout << "Confirm password: ";
        getline(cin, confirmPassword);

        if (registerValidation(newUser, confirmPassword, &newCustomer, customers, staffs, &message)) {
            newCustomer.points = 0;
            clearScreen();
            break;
        }
        else {
            cout << message << "Please try again!" << endl;
        }
    } while (true);

    do {
        cout << "Username: " << newCustomer.user.name << endl;
        cout << "Phone Number: " << newCustomer.user.phoneNo << endl << endl;
        cout << "Confirm register this member (Y/N): ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter N or Y!" << endl;
            continue;
        }
        else if (input.size() == 1) {
            confirm = input[0];
            confirm = toupper(confirm);
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
            cout << "Press any key to continue..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        else if (confirm == 'N') { // If the user cancels, display cancel message
            cout << "\nMember registration cancelled." << endl;
            cout << "Press any key to continue..." << endl;
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