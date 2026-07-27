#include <iostream>
#include <iomanip>
#include <string>
#include <format>
#include <vector>
#include <regex>
#include "Main.h"
#include "LoginModule.h"
#include "AppointmentModule.h"
#include "AppointmentReminder.h"
#include "InventoryModule.h"
#include "BillingModule.h"
#include "ReportingModule.h"

using namespace std;

void RedeemPointsPage(Customer customer) { // Redeem points page for customer maintenance	
	// Variable declarations
    int selection = 0, quantity = 0, totalPointCost = 0, indexFound = 0;
    char confirm = 'N';
    string input;
    bool found = false;

	// Pointer to the chosen item
    Item * itemChosen;

	do { 
        vector<Customer> customers = readCustomerFile();
        vector<Item> items = readItemFile();

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
        cout << left << "1. " << setw(25) << items[0].name << setw(4) << items[0].pointCost << "pts\t2. " << setw(25) << items[1].name << setw(4) << items[1].pointCost << "pts" << endl;
        cout << left << "3. " << setw(25) << items[2].name << setw(4) << items[2].pointCost << "pts\t4. " << setw(25) << items[3].name << setw(4) << items[3].pointCost << "pts" << endl;
        cout << left << "5. " << setw(25) << items[4].name << setw(4) << items[4].pointCost << "pts\t6. " << setw(25) << items[5].name << setw(4) << items[5].pointCost << "pts" << endl;
        cout << left << "7. " << setw(25) << items[6].name << setw(4) << items[6].pointCost << "pts\t8. " << setw(25) << items[7].name << setw(4) << items[7].pointCost << "pts" << endl;
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
                                cout << "Press any key to continue..." << endl;
                                cin.get();
                                clearScreen();
                                break;
                            }
                            else if (confirm == 'N') { // If the user cancels, display cancel message
                                cout << "Redeem cancelled." << endl;
                                cout << "Press any key to continue..." << endl;
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

void customerMaintenancePage() { // Customer maintenance page for staff
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
            RedeemPointsPage(customer);
            break;
        case 2:
            clearScreen();
            // navigate to view all appointment (appointment module)
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

bool editValidationSalary(Staff editStaff, vector<Staff>* satffList) {
    if (editStaff.salary < 3000.00) {
        clearScreen();
        cout << "Salary must be more than 3000! ";
        return false;
    }

    return true;
}

bool editValidationPhoneNo(Staff editStaff, vector<Staff> *staffList) {
    regex phonePattern(R"(^01\d-\d{7,8}$)"); // regex with pattern like 012-3456789

    vector<Customer> customers = readCustomerFile();

    if ((editStaff.user.phoneNo).empty()) {
        clearScreen();
        cout << "Phone number cannot be empty! ";
        return false;
    }

    // Validate: phone number is in a certain format
    if (!(regex_search((editStaff.user.phoneNo), phonePattern))) {
        clearScreen();
        cout << "Invalid phone number format! ";
        return false;
    }

    // Validate: no redundant phone no.
    for (int i = 0; i < customers.size(); i++) {
        if (editStaff.user.phoneNo == (customers.at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    for (int i = 0; i < (*staffList).size(); i++) {
        if (editStaff.user.phoneNo == ((*staffList).at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    return true;
}

void editStaff(int indexFound, Staff *inputStaff, vector<Staff> *staffList, bool *success) {
    int selection = 0;
    string input;

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

                if (editValidationPhoneNo(*inputStaff, &(*staffList))) {
                    (*staffList)[indexFound] = *inputStaff;
                    *success = true;
                    overwriteStaffFile(*staffList);

                    clearScreen();
                    return;
                    break;
                }
                else {
                    cout << "Please try again!" << endl;
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

                if (editValidationSalary(*inputStaff, &(*staffList))) {
                    (*staffList)[indexFound] = *inputStaff;
                    *success = true;
                    overwriteStaffFile(*staffList);

                    clearScreen();
                    return;
                    break;
                }
                else {
                    cout << "Please try again!" << endl;
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

bool staffValidation(Staff newStaff, string confirmPassword, vector<Staff> *staffList) {
    regex phonePattern(R"(^01\d-\d{7,8}$)"); // regex with pattern like 012-3456789

    vector<Customer> customers = readCustomerFile();

    // Validate: no redundant username
    for (int i = 0; i < (*staffList).size(); i++) {
        if (newStaff.user.name == ((*staffList).at(i)).user.name) {
            clearScreen();
            cout << "Username has been used! ";
            return false;
        }
    }

    // Validate: username cannot be empty
    if ((newStaff.user.name).empty()) {
        clearScreen();
        cout << "Username cannot be empty! ";
        return false;
    }

    // Validate: username's length must more than 3 char
    if ((newStaff.user.name).length() <= 3) {
        clearScreen();
        cout << "Username must more than 3 characters! ";
        return false;
    }

    // Validate: no redundant phone no.
    for (int i = 0; i < customers.size(); i++) {
        if (newStaff.user.phoneNo == (customers.at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    for (int i = 0; i < (*staffList).size(); i++) {
        if (newStaff.user.phoneNo == ((*staffList).at(i)).user.phoneNo) {
            clearScreen();
            cout << "Phone number has been registered! ";
            return false;
        }
    }

    // Validate: phone number cannot be empty
    if ((newStaff.user.phoneNo).empty()) {
        clearScreen();
        cout << "Phone number cannot be empty! ";
        return false;
    }

    // Validate: phone number is in a certain format
    else if (!(regex_search((newStaff.user.phoneNo), phonePattern))) {
        clearScreen();
        cout << "Invalid phone number format! ";
        return false;
    }

    if (newStaff.salary < 3000.00) {
        clearScreen();
        cout << "Salary must be more than 3000! ";
        return false;
    }

    // Validate: password cannot be empty
    if ((newStaff.user.password).empty()) {
        clearScreen();
        cout << "Password cannot be empty! ";
        return false;
    }

    // Validate: both password is same
    if ((newStaff.user.password) != confirmPassword) {
        clearScreen();
        cout << "Password not the same! ";
        return false;
    }

    (*staffList).push_back(newStaff);
    return true;    
}

void addStaff(vector<Staff> *staffList) {
    Staff newStaff;
    newStaff.staffCode = format("S{:03d}", ((*staffList).size() + 1)); // Generate staff code based on total staff count
	newStaff.appointmentDone = 0; // Initialize appointment done to 0

	string confirmPassword = "", input;
	char confirm = 'N';

    do {
        cout << "Add Staff" << endl;
        cout << "==========\n";
        cout << "Staff code: " << newStaff.staffCode << endl;
		cout << "Staff name: ";
		getline(cin, newStaff.user.name);
		cout << "Staff phone No.: ";
		getline(cin, newStaff.user.phoneNo);
        cout << "Staff salary: ";
        getline(cin, input);

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

        if (staffValidation(newStaff, confirmPassword, &(*staffList))) {
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
            cout << "\nStaff added successfully!" << endl;
            cout << "Press any key to continue..." << endl;
            cin.get();
            clearScreen();
            return;
        }
        else if (confirm == 'N') { // If the user cancels, display cancel message
            cout << "\nStaff addition cancelled." << endl;
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

void staffMaintenancePage() {
    char selection = 0;
    int currentpage = 1, indexFound = 0;
    bool found = false, success = false;
    string input;
    Staff inputStaff;
    const int MAX_STAFF_PER_PAGE = 10;

    do {
        vector<Staff> staffList = readStaffFile();

        int totalStaff = staffList.size();
		int totalPages = ceil(static_cast<double>(totalStaff) / MAX_STAFF_PER_PAGE);

		cout << "Staff Maintenance" << endl;
        cout << "==================\n\n";
		cout << left << setw(20) << "Staff code" << setw(25) << "Name" << setw(15) << "Phone No." << setw(15) << "Salary" << setw(16) << "Appointment done" << endl;
        cout << left << setw(20) << "===========" << setw(25) << "=====" << setw(15) << "==========" << setw(15) << "=======" << setw(16) << "=================" << endl;

        int start = (currentpage - 1) * MAX_STAFF_PER_PAGE;
        Staff *staffPtr = &staffList[start]; // Pointer to the start of the staff list

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
			addStaff(&staffList);
			break;
		case 'e':
			cout << "Staff code: ";
			getline(cin, inputStaff.staffCode);

            staffPtr = &staffList[start]; // Pointer to the start of the staff list
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
                cout << "Staff code not found!" << endl;
            }
            else {
                clearScreen();
                editStaff(indexFound, &inputStaff, &staffList, &success);
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

void memberHomePage(Customer customer) {
	// Variable declarations
    int selection = 0;
    string input;

	do { // Display the member home page UI
        cout << "Welcome " << customer.user.name << "!" << endl;
		appointmentReminder(customer);
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
		case 1: // navigate to manage appointment (appointment module)
            clearScreen();
            appointmentManager();
            break;
        case 2:
            clearScreen();
            //navigate to buy item (inventory module)
            break;
        case 3:
            clearScreen();
            //navigate to view reciept (billing module)
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

void staffHomePage(Staff staff) {
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
            break;
		case 2: // navigate to view assigned appointment (appointment module)
            clearScreen();
            assignedAppointmentsView(staff.user.name);
            break;
        case 3:
            //navigate to inventory maintenance (inventory module)
            break;
		case 4: // navigate to customer maintenance (user module)
            clearScreen();
            customerMaintenancePage();
            break;
		case 5: // navigate to view completed appointment (appointment module)
            clearScreen();
            completedAppointmentsView(staff.user.name);
            break;
		case 0: // exit the staff home page
            clearScreen();
            return;
		default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
        }
    } while(true);
}

void adminHomePage() {
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
            //navigate to inventory maintenance (inventory module)
            break;
		case 2: //navigate to staff maintenance (user module)
            clearScreen();
            staffMaintenancePage();
            break;
		case 3: // navigate to assign appointments (appointment module)
            clearScreen();
            assignAppointmentsPage();
            break;
        case 4:
            clearScreen();
            //navigate to view reportings (reporting module)
            break;
		case 0: // exit the admin home page
            clearScreen();
            return;
		default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!" << endl;
        }
    } while(true);
}