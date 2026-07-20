#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "UserModule.h"

using namespace std;

int validateUser(string username, string password) { 
    // validatate user from database
	// username must exist in database and password must match with corresponding user


	// comparison is placeholder for now
    if (username == "1") {// login as member
        return 1;
    }
    else if (username == "2") {// login as staff 
        return 2;
    }
    else if (username == "3") {// login as admin
        return 3;
    }
    else {
        return 0;
    }
}

void loginScreen() {
    string username, password;
<<<<<<< Updated upstream
=======
    Customer customer;
    Staff staff;
>>>>>>> Stashed changes

    do {
        cout << "Login" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        int user = validateUser(username, password);

        

        switch (user) {
        case 1: // navigate to member home page (user module)
<<<<<<< Updated upstream
			clearScreen(); 
            memberHomePage(username);
            break;
		case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(username);
            break;
		case 3: // navigate to admin home page (user module)
=======
			// Read file to get customer details based on username (TODO)
			// Placeholder for now
			customer.name = "Lim Ah Kau";
			customer.password = "password1";
			customer.phoneNo = "0123456789";
			customer.points = 1000;

            clearScreen();
            memberHomePage(customer);
            break;
		case 2: // navigate to staff home page (user module)
			// Read file to get staff details based on username (TODO)
			// Placeholder for now
			staff.staffCode = "S001";
			staff.name = "John Doe";
			staff.phoneNo = "0123456789";
			staff.salary = 3000.00;
			staff.password = "password1";
			staff.appointmentDone = 10;

>>>>>>> Stashed changes
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

void registerScreen() {
    string username, phoneNo, password, confirmPassword;

    do {
        cout << "Register" << endl;
        cout << "Username: ";
        getline(cin, username); //cannot duplicate, empty, more than 3 char
        cout << "Phone number: ";
        getline(cin, phoneNo); // regex of "012-3456789", 3+(7 or 8)
        cout << "Password: ";
        getline(cin, password);
        cout << "Confirm password: ";
        getline(cin, confirmPassword); // match with password
    } while (true);
}