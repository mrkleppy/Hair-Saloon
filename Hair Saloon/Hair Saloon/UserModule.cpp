#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <format>
#include <regex>
#include "Main.h"
#include "LoginModule.h"
#include "AppointmentModule.h"
#include "AppointmentReminder.h"
#include "InventoryModule.h"
#include "BillingModule.h"
#include "ReportingModule.h"
#include "FileProcessing.h"
#include "Validation.h"

using namespace std;

void displayItem(vector<Item>& items) {
    cout << left << "1. " << setw(25) << items[0].name << setw(4) << items[0].pointCost << "pts\t2. " << setw(25) << items[1].name << setw(4) << items[1].pointCost << "pts" << endl;
    cout << left << "3. " << setw(25) << items[2].name << setw(4) << items[2].pointCost << "pts\t4. " << setw(25) << items[3].name << setw(4) << items[3].pointCost << "pts" << endl;
    cout << left << "5. " << setw(25) << items[4].name << setw(4) << items[4].pointCost << "pts\t6. " << setw(25) << items[5].name << setw(4) << items[5].pointCost << "pts" << endl;
    cout << left << "7. " << setw(25) << items[6].name << setw(4) << items[6].pointCost << "pts\t8. " << setw(25) << items[7].name << setw(4) << items[7].pointCost << "pts" << endl;
}

void RedeemPointsPage(Customer customer, vector<Customer>& customers, vector<Item>& items) { // Redeem points page for customer maintenance	
    // Variable declarations
    int selection = 0, quantity = 0, totalPointCost = 0, indexFound = 0;
    char confirm = 'N';
    string input;
    bool found = false;

    // Pointer to the chosen item
    Item* itemChosen;

    do {
        for (int i = 0; i < customers.size(); i++) {
            if (customer.user.phoneNo == customers[i].user.phoneNo) {
                customer.user.name = customers[i].user.name;
                customer.user.password = customers[i].user.password;
                customer.points = customers[i].points;
                indexFound = i;
                found = true;
                break;
            }
        }

        if (!found) {
            clearScreen();
            cout << "Phone number not found!" << endl;
            return;
        }

        // Display the redeem points page UI
        cout << "Customer Name: " << customer.user.name << endl;
        cout << "Membership Points: " << customer.points << "pts" << endl;
        cout << "Select an item to redeem" << endl;
        displayItem(items);
        cout << endl << "Selection (0 to exit): ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        switch (selection) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8: // If the selection is 1-8, proceed to redeem points
            itemChosen = &items[selection - 1]; // Get the pointer to the chosen item
            cout << "Quantity (stock available: " << itemChosen->stock << " ): ";
            getline(cin, input);

            if (input.empty()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                continue;
            }

            try {
                size_t pos;
                quantity = stoi(input, &pos);

                if (pos != input.size()) {
                    clearScreen();
                    cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                    continue;
                }
            }
            catch (...) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                continue;
            }

            if (quantity > itemChosen->stock) { // Only proceed if the quantity is less than or equal to the stock available
                clearScreen();
                cout << "Not enough stock available!" << endl;
            }
            else {
                totalPointCost = quantity * itemChosen->pointCost; // Calculate the total point cost

                if (totalPointCost > customer.points) { // Only proceed if the total point cost is less than or equal to the points available
                    clearScreen();
                    cout << "Not enough points to redeem!" << endl;
                }
                else {
                    do {// Display confirmation message
                        cout << "Confirm redeem " << quantity << " " << itemChosen->name << " for " << totalPointCost << " pts? (Y/N): ";
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
                            for (int i = 0; i < items.size(); i++) {
                                if ((*itemChosen).name == items[i].name) {
                                    items[i].stock -= quantity;
                                    break;
                                }
                            }

                            customers[indexFound].points -= totalPointCost;

                            overwriteItemFile(items);
                            overwriteCustomerFile(customers);

                            cout << "Redeem successful! Deducted " << totalPointCost << " points" << endl;
                            cout << "Press enter to continue..." << endl;
                            cin.get();
                            clearScreen();
                            break;
                        }
                        else if (confirm == 'N') { // If the user cancels, display cancel message
                            cout << "Redeem cancelled." << endl;
                            cout << "Press enter to continue..." << endl;
                            cin.get();
                            clearScreen();
                            break;
                        }
                        else {
                            clearScreen();
                            cout << "Invalid input! Please enter N or Y!" << endl;
                        }
                    } while (true);
                }
            }
            break;
        case 0: // Exit the redeem points page
            clearScreen();
            return;
        default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0 to exit, 1 - 8 to select an item!" << endl;
        }
    } while (true);
}

void customerMaintenancePage(vector<Customer>& customers, vector<Item>& items, vector<Appointment>& appointments) { // Customer maintenance page for staff
    // Variable declarations
    int selection = 0;
    string input;
    Customer customer;

    do { // Display the customer maintenance page UI
        cout << "Customer Maintenance" << endl;
        cout << "=====================\n";
        cout << "1. Redeem points\n2. View all appointment\n0. Exit" << endl << endl;
        cout << "selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        if (selection == 1 || selection == 2) { // If the selection is 1 or 2, prompt for customer's phone number
            cout << "Customer's phone number: ";
            getline(cin, customer.user.phoneNo);
        }

        switch (selection) {
        case 1: // navigate to redeem points page (user module)
            clearScreen();
            RedeemPointsPage(customer, customers, items);
            break;
        case 2:
            clearScreen(); // navigate to view all appointment (appointment module)
            allAppointmentsView(customer, customers, appointments);
            break;
        case 0: // exit the customer maintenance page
            clearScreen();
            return;
        default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
        }
    } while (true);
}

void editStaff(int indexFound, Staff* inputStaff, vector<Staff>& staffs, vector<Customer>& customers, bool* success) {
    int selection = 0;
    string input, message;

    do {
        cout << "Edit Staff" << endl;
        cout << "===========\n";
        cout << (*inputStaff).user.name << "(" << (*inputStaff).staffCode << ")" << endl;
        cout << "1. Phone No.\n2. Salary\n0. Back" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        switch (selection) {
        case 1:
            cout << "Change phone no. to: ";
            getline(cin, (*inputStaff).user.phoneNo);

            if (validatePhoneNo((*inputStaff).user, customers, staffs, &message)) {
                staffs[indexFound] = *inputStaff;
                *success = true;
                overwriteStaffFile(staffs);

                clearScreen();
                return;
                break;
            }
            else {
                cout << message << "Please try again!" << endl;
                continue;
            }
        case 2:
            cout << "Change salary to: RM ";
            getline(cin, input);

            if (input.empty()) {
                clearScreen();
                cout << "Invalid salary input! Please try again!" << endl;
                continue;
            }

            try {
                size_t pos;
                (*inputStaff).salary = stod(input, &pos);

                if (pos != input.size()) {
                    clearScreen();
                    cout << "Invalid salary input! Please try again!" << endl;
                    continue;
                }
            }
            catch (...) {
                clearScreen();
                cout << "Invalid salary input! Please try again!" << endl;
                continue;
            }

            if (validateSalary(*inputStaff, &message)) {
                staffs[indexFound] = *inputStaff;
                *success = true;
                overwriteStaffFile(staffs);

                clearScreen();
                return;
                break;
            }
            else {
                cout << message << "Please try again!" << endl;
                continue;
            }
        case 0:
            clearScreen();
            return;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
        }
    } while (true);
}

bool staffValidation(Staff newStaff, string confirmPassword, vector<Staff>& staffs, vector<Customer>& customers, string& message) {
    if (validateUsername(newStaff.user, customers, staffs, &message) && validatePhoneNo(newStaff.user, customers, staffs, &message)
        && validateSalary(newStaff, &message) && validatePassword(newStaff.user, confirmPassword, &message)) {
        return true;
    }
    else {
        return false;
    }
}

void addStaff(vector<Staff>& staffs, vector<Customer>& customers) {
    Staff newStaff;
    newStaff.staffCode = format("S{:03d}", (staffs.size() + 1)); // Generate staff code based on total staff count
    newStaff.appointmentDone = 0; // Initialize appointment done to 0

    string confirmPassword = "", input, message;
    char confirm = 'N';

    do {
        cout << "Add Staff" << endl;
        cout << "==========\n";
        cout << "Enter \'q\' to exit\n";
        cout << "Staff code: " << newStaff.staffCode << endl;
        cout << "Staff name (no less than 3 characters): ";
        getline(cin, newStaff.user.name);

        if (newStaff.user.name == "q" || newStaff.user.name == "Q") {
            clearScreen();
            return;
        }

        cout << "Staff phone No. (e.g.: 012-3456789): ";
        getline(cin, newStaff.user.phoneNo);

        if (newStaff.user.phoneNo == "q" || newStaff.user.phoneNo == "Q") {
            clearScreen();
            return;
        }

        cout << "Staff salary (no less than RM 3000): RM ";
        getline(cin, input);

        if (input == "q" || input == "Q") {
            clearScreen();
            return;
        }

        if (input.empty()) {
            clearScreen();
            cout << "Invalid salary input! Please try again!" << endl;
            continue;
        }

        try {
            size_t pos;
            newStaff.salary = stod(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid salary input! Please try again!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid salary input! Please try again!" << endl;
            continue;
        }

        cout << "Staff password: ";
        getline(cin, newStaff.user.password);
        cout << "Confirm password: ";
        getline(cin, confirmPassword);

        if (staffValidation(newStaff, confirmPassword, staffs, customers, message)) {
            clearScreen();
            break;
        }
        else {
            cout << "Please try again!" << endl;
        }

    } while (true);

    do {
        cout << "Staff name: " << newStaff.user.name << endl;
        cout << "Staff phone No.: " << newStaff.user.phoneNo << endl;
        cout << "Staff salary: RM " << newStaff.salary << endl;
        cout << "Staff password: " << newStaff.user.password << endl;
        cout << "\nConfirm adding staff (Y/N): ";
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
            appendStaffToFile(newStaff);
            staffs.push_back(newStaff);
            cout << "\nStaff added successfully!" << endl;
            cout << "Press enter to continue..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        else if (confirm == 'N') { // If the user cancels, display cancel message
            cout << "\nStaff addition cancelled." << endl;
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

void staffMaintenancePage(vector<Staff>& staffs, vector<Customer>& customers) {
    char selection = 0;
    int currentpage = 1, indexFound = 0;
    bool found = false, success = false;
    string input;
    Staff inputStaff;

    do {
        int totalStaff = int(staffs.size());
        int totalPages = int(ceil(static_cast<double>(totalStaff) / MAX_STAFF_PER_PAGE));

        cout << "Staff Maintenance" << endl;
        cout << "==================\n\n";
        cout << left << setw(20) << "Staff code" << setw(25) << "Name" << setw(15) << "Phone No." << setw(15) << "Salary" << setw(16) << "Appointment done" << endl;
        cout << left << setw(20) << "===========" << setw(25) << "=====" << setw(15) << "==========" << setw(15) << "=======" << setw(16) << "=================" << endl;

        int start = (currentpage - 1) * MAX_STAFF_PER_PAGE;
        Staff* staffPtr = &staffs[start]; // Pointer to the start of the staff list

        for (int i = 0; i < MAX_STAFF_PER_PAGE && (start + i) < totalStaff; i++) {
            cout << left << setw(20) << staffPtr->staffCode
                << setw(25) << staffPtr->user.name
                << setw(15) << staffPtr->user.phoneNo
                << "RM " << setw(12) << fixed << setprecision(2) << staffPtr->salary
                << setw(16) << staffPtr->appointmentDone << endl;
            staffPtr++;
        }

        cout << "\nPage " << currentpage << "/" << totalPages << endl;
        cout << "(n = next page, p = previous page, a = add staff, e = edit staff, q = quit)" << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter n, p, a, e, or q!" << endl;
            continue;
        }
        else if (input.size() == 1) {
            selection = input[0];
            selection = tolower(selection);
        }
        else {
            clearScreen();
            cout << "Invalid input! Please enter n, p, a, e, or q!" << endl;
            continue;
        }

        switch (selection) {
        case 'n':
            if (currentpage < totalPages) {
                currentpage++;
            }
            else {
                clearScreen();
                cout << "You are on the last page!" << endl;
            }
            break;
        case 'p':
            if (currentpage > 1) {
                currentpage--;
            }
            else {
                clearScreen();
                cout << "You are on the first page!" << endl;
            }
            break;
        case 'a':
            clearScreen();
            addStaff(staffs, customers);
            break;
        case 'e':
            cout << "Staff code: ";
            getline(cin, inputStaff.staffCode);

            staffPtr = &staffs[start]; // Pointer to the start of the staff list
            for (int i = 0; i < totalStaff; i++) {
                if (inputStaff.staffCode == staffPtr->staffCode) {
                    inputStaff.user = staffPtr->user;
                    inputStaff.salary = staffPtr->salary;
                    inputStaff.appointmentDone = staffPtr->appointmentDone;
                    indexFound = i;
                    found = true;

                    break;
                }
                else {
                    staffPtr++;
                }
            }

            if (!found) {
                clearScreen();
                cout << "Staff code not found!" << endl;
            }
            else {
                clearScreen();
                editStaff(indexFound, &inputStaff, staffs, customers, &success);
                if (success) {
                    cout << "Staff succesfully updated!" << endl;
                }
            }

            break;
        case 'q':
            clearScreen();
            return;
        default:
            clearScreen();
            cout << "Invalid input! Please enter n, p, a, e, or q!" << endl;
        }

    } while (true);
}

void memberHomePage(Customer customer, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments) {
    // Variable declarations
    int selection = 0;
    string input;

    do { // Display the member home page UI
        cout << "Welcome " << customer.user.name << "!" << endl;
        appointmentReminder(customer, appointments);
        cout << "What would you like to do?" << endl;
        cout << "===========================\n";
        cout << "1. Manage an appointment\n2. Buy an item\n3. View receipts\n0. Logout" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, or 3!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, 2, or 3!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, or 3!" << endl;
            continue;
        }

        switch (selection) {
        case 1: // navigate to manage appointment (Appointment module)
            clearScreen();
            appointmentManager(customer, customers, appointments);
            break;
        case 2: // navigate to purchasing items (Inventory module)
            clearScreen();
            purchaseItemPage(items);
            break;
        case 3:
            clearScreen();
            //navigate to view reciept (Billing module)
            viewReceiptScreen();
            break;
        case 0: // exit the member home page
            clearScreen();
            return;
        default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, or 3!" << endl;
        }
    } while (true);
}

void staffHomePage(Staff staff, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments) {
    // Variable declarations
    int selection = 0;
    string input;

    do { // Display the staff home page UI
        cout << "Welcome " << staff.user.name << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "===========================\n";
        cout << "1. POS system\n2. View assigned appointment\n3. Inventory maintenance \
            \n4. Customer maintenance\n5. View appointment done\n0. Logout" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
            continue;
        }

        switch (selection) {
        case 1:
            clearScreen();
            //navigate to POS system (billing and payment module)
            viewPOSScreen(items);
            break;
        case 2: // navigate to view assigned appointment (appointment module)
            clearScreen();
            assignedAppointmentsView(staff, staffs, appointments);
            break;
        case 3:
            clearScreen();
            inventoryMaintenancePage(items);
            break;
        case 4: // navigate to customer maintenance (user module)
            clearScreen();
            customerMaintenancePage(customers, items, appointments);
            break;
        case 5: // navigate to view completed appointment (appointment module)
            clearScreen();
            completedAppointmentsView(staff, appointments);
            break;
        case 0: // exit the staff home page
            clearScreen();
            return;
        default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
        }
    } while (true);
}

void adminHomePage(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments) {
    // Variable declarations
    int selection = 0;
    string input;

    do { // Display the admin home page UI
        cout << "Welcome admin!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "===========================\n";
        cout << "1. Inventory maintenance\n2. Staff maintenance\n3. Assign appointments \
        \n4. View reportings\n0. Logout" << endl << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!" << endl;
            continue;
        }

        try {
            size_t pos;
            selection = stoi(input, &pos);

            if (pos != input.size()) {
                clearScreen();
                cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!" << endl;
                continue;
            }
        }
        catch (...) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!" << endl;
            continue;
        }

        switch (selection) {
        case 1:
            clearScreen();
            inventoryMaintenancePage(items);
            break;
        case 2: //navigate to staff maintenance (user module)
            clearScreen();
            staffMaintenancePage(staffs, customers);
            break;
        case 3: // navigate to assign appointments (appointment module)
            clearScreen();
            assignAppointmentsPage(appointments, staffs);
            break;
        case 4:
            clearScreen(); //navigate to view reportings (reporting module)
            reportingHomePage();
            break;
        case 0: // exit the admin home page
            clearScreen();
            return;
        default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!" << endl;
        }
    } while (true);
}