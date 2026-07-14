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
    bool validate = true;
    string username, password;

    do {
        cout << "Login" << endl;
        cout << "Username: ";
        getline(cin, username); // exist in database
        cout << "Password: ";
        getline(cin, password); // match with correspoding user

        int user = validateUser(username, password);

        switch (user) {
        case 1:
            clearScreen();
            memberHomePage(username);
            break;
        case 2:
            clearScreen();
            staffHomePage(username);
            break;
        case 3:
            clearScreen();
            adminHomePage(username);
            break;
        default:
            clearScreen();
            cout << "User not found! Please try again!" << endl;
            validate = false;
            break;
        }
    } while (!validate);
}

void registerScreen() {
    bool validate = true;
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
    } while (!validate);
}

void mainScreen() {
    int selection = 0;
    bool validate = true;

    do {
        cout << "Welcome to XYZ Saloon!" << endl;
        cout << "1. Login\n2. Register as member\n0. Exit" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

        switch (selection) {
        case 1:
            clearScreen();
            loginScreen();
            break;
        case 2:
            clearScreen();
            registerScreen();
            break;
        case 0:
            exit();
            break;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!";
            validate = false;
            break;
        }
    } while (!validate);
}