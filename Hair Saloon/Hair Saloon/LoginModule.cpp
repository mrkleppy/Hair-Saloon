#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

<<<<<<< Updated upstream
void clearScreen() {
    cout << "\033[2J\033[H";
}

int exit() {
    return 0;
}

int validateUser(string username, string password) {
    return 0;
=======
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
>>>>>>> Stashed changes
}

void loginScreen() {
    string username, password;

    do {
        cout << "Login" << endl;
        cout << "Username: ";
        getline(cin, username);
        cout << "Password: ";
        getline(cin, password);

        int user = stoi(username); // placeholder
        //int user = validateUser(username, password);

        switch (user) {
<<<<<<< Updated upstream
        case 1:
            // member
            break;
        case 2:
            // staff
            break;
        case 3:
            // admin
=======
        case 1: // navigate to member home page (user module)
			clearScreen(); 
            memberHomePage(username);
            break;
		case 2: // navigate to staff home page (user module)
            clearScreen();
            staffHomePage(username);
            break;
		case 3: // navigate to admin home page (user module)
            clearScreen();
            adminHomePage(username);
>>>>>>> Stashed changes
            break;
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "User not found! Please try again!" << endl;
<<<<<<< Updated upstream
            validate = false;
            break;
        }
    } while (!validate);
=======
        }

        break;
    } while (true);
>>>>>>> Stashed changes
}

void registerScreen() {
    string username, phoneNo, password, confirmPassword;

    do {
        cout << "Login" << endl;
        cout << "Username: ";
        getline(cin, username); //cannot duplicate, empty, more than 3 char
        cout << "Phone number: ";
        getline(cin, phoneNo); // regex of "012-3456789", 3+(7 or 8)
        cout << "Password: ";
        getline(cin, password);
        cout << "Confirm password: ";
        getline(cin, confirmPassword); // match with password
<<<<<<< Updated upstream
    } while (!validate);
}

void mainScreen() {
    int selection;
    
    do {
        cout << "Welcome to XYZ Saloon!" << endl;
        cout << "1. Login\n2. Register as member\n0. Exit" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

        switch(selection) {
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
                break;
        }
    } while (selection != 1 || selection != 2 || selection != 0);
=======
    } while (true);
>>>>>>> Stashed changes
}