#include <regex>
#include <vector>
#include "Main.h"

using namespace std;

const regex PHONE_NUMBER_PATTERN(R"(^01\d-\d{7,8}$)"); // regex with pattern like 012-3456789

// validation for username
bool validateUsername(User user, vector<Customer> &customers, vector<Staff> &staffs, string *message) {
    // Validate: no redundant username
    for (int i = 0; i < customers.size(); i++) {
        if (user.name == (customers.at(i)).user.name) {
            clearScreen();
            *message = "Username has been used! ";
            return false;
        }
    }

    for (int i = 0; i < staffs.size(); i++) {
        if (user.name == (staffs.at(i)).user.name) {
            clearScreen();
            *message = "Username has been used! ";
            return false;
        }
    }

    // Validate: username cannot be empty
    if ((user.name).empty()) {
        clearScreen();
        *message = "Username cannot be empty! ";
        return false;
    }

    // Validate: username's length must more than 3 char
    if ((user.name).length() <= 3) {
        clearScreen();
        *message = "Username must more than 3 characters! ";
        return false;
    }

    return true;
}

// Validation for phone number
bool validatePhoneNo(User user, vector<Customer>& customers, vector<Staff>& staffs, string* message) {
    // Validate: no redundant phone no.
    for (int i = 0; i < customers.size(); i++) {
        if (user.phoneNo == (customers.at(i)).user.phoneNo) {
            clearScreen();
            *message = "Phone number has been registered! ";
            return false;
        }
    }

    for (int i = 0; i < staffs.size(); i++) {
        if (user.phoneNo == (staffs.at(i)).user.phoneNo) {
            clearScreen();
            *message = "Phone number has been registered! ";
            return false;
        }
    }

    // Validate: phone number cannot be empty
    if ((user.phoneNo).empty()) {
        clearScreen();
        *message = "Phone number cannot be empty! ";
        return false;
    }

    // Validate: phone number is in a certain format
    if (!(regex_search((user.phoneNo), PHONE_NUMBER_PATTERN))) {
        clearScreen();
        *message = "Invalid phone number format! ";
        return false;
    }

    // Validate: password cannot be empty
    if ((user.password).empty()) {
        clearScreen();
        *message = "Password cannot be empty! ";
        return false;
    }

    return true;
}

// Validation for password
bool validatePassword(User user, string confirmPassword, string* message) {
    // Validate: both password is same
    if ((user.password) != confirmPassword) {
        clearScreen();
        *message = "Password not the same! ";
        return false;
    }

    return true;
}

// Validation for salary
bool validateSalary(Staff staff, string* message) {
	// Validate: salary cannot be empty
	if (staff.salary <= 0) {
		clearScreen();
		*message = "Salary must be greater than 0! ";
		return false;
	}
	else if (staff.salary < 3000.00 || staff.salary > 20000.00) {
		clearScreen();
		*message = "Salary must be within RM 3000.00 to RM 20000.00! ";
		return false;
	}
	return true;
}