#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>
#include "Main.h"
#include "UserModule.h"

using namespace std;

int validateUser(User user, Customer* customer, Staff* staff) {
    // validatate user from database
    // username must exist in database and password must match with corresponding user
    User currentUser;
    vector<Customer> customers = readCustomerFile();
    vector<Staff> staffs = readStaffFile();
    regex staffCodePattern(R"(S\d{3})");
    bool found = false;

    if (user.name == "admin" && user.password == "ADMIN") {
        return 3;
    }

    if (regex_search(user.name, staffCodePattern)) {
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

void loginScreen() {
    User user;
    Customer customer;
    Staff staff;
    vector<Customer> customers;

    do {
        cout << "Login" << endl;
        cout << "======\n";
        cout << "Username: ";
        getline(cin, user.name);
        cout << "Password: ";
        getline(cin, user.password);

        int userType = validateUser(user, &customer, &staff);

        switch (userType) {
        case 1: // navigate to member home page (user module)
            clearScreen();
            memberHomePage(customer);
            break;
		case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(staff);
            break;
		case 3: // navigate to admin home page (user module)
            clearScreen();
            adminHomePage();
            break;
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "User not found! Please try again!" << endl;
        }

        break;
    } while (true);
}

bool registerValidation(User newUser, string confirmPassword, Customer *newCustomer) {
    regex phonePattern(R"(^01\d-\d{7,8}$)"); // regex with pattern like 012-3456789

    vector<Customer> customers = readCustomerFile(); // read from file
    vector<Staff> staffs = readStaffFile();

    // Validate: no redundant username
    for (int i = 0; i < customers.size(); i++) {
        if (newUser.name == (customers.at(i)).user.name) {
            clearScreen();
            cout << "Username has been used! ";
            return false;
        }
    }
    
    // Validate: username cannot be empty
    if ((newUser.name).empty()) {
        clearScreen();
        cout << "Username cannot be empty! ";
        return false;
    }

    // Validate: username's length must more than 3 char
    if ((newUser.name).length() <= 3) {
        clearScreen();
        cout << "Username must more than 3 characters! ";
        return false;
    }

    // Validate: no redundant phone no.
    for (int i = 0; i < customers.size(); i++) {
        if (newUser.phoneNo == (customers.at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    for (int i = 0; i < staffs.size(); i++) {
        if (newUser.phoneNo == (staffs.at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    // Validate: phone number cannot be empty
    if ((newUser.phoneNo).empty()) {
        clearScreen();
        cout << "Phone number cannot be empty! ";
        return false;
    }

    // Validate: phone number is in a certain format
    if (!(regex_search((newUser.phoneNo), phonePattern))) {
        clearScreen();
        cout << "Invalid phone number format! ";
        return false;
    }

    // Validate: password cannot be empty
    if ((newUser.password).empty()) {
        clearScreen();
        cout << "Password cannot be empty! ";
        return false;
    }

    // Validate: both password is same
    if ((newUser.password) != confirmPassword) {
        clearScreen();
        cout << "Password not the same! ";
        return false;
    }

    (*newCustomer).user = newUser;
    return true;     
}

void registerScreen() {
    string confirmPassword, input;
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

        if (registerValidation(newUser, confirmPassword, &newCustomer)) {
            newCustomer.points = 0;
            clearScreen();
            break;
        }
        else {
            cout << "Please try again!" << endl;
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