#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "UserModule.h"

using namespace std;

int exit() {
    return 0;
}

int validateUser(string username, string password) {
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
    if (username == "1") {// login as member // temp value only
        return 1;
    }
    else if (username == "2") {// login as staff // temp value only
        return 2;
    }
    else if (username == "3") {// login as admin // temp value only
        return 3;
    }
    else {
        return 0;
    }
}

void loginScreen() {
    string username, password;
    clearScreen();

    do {
        cout << "Login" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        int user = validateUser(username, password);

        switch (user) {
        case 1: // navigate to member home page (user module)
			      clearScreen(); 
            memberHomePage(username);
            break;
		    case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(username);
            adminHomePage(username);
            break;
		    case 3: // navigate to admin home page (user module)
            clearScreen();
            adminHomePage(username);
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
    clearScreen();

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