#include <iostream>
#include <iomanip>
#include <string>
#include "LoginModule.h"

using namespace std;

<<<<<<< Updated upstream
int main() {
    mainScreen();
=======
void clearScreen() { // Clear the console screen
    cout << "\033[2J\033[H";
}

int main() {
    int selection = 0;

	do { // Display the main menu UI
        cout << "Welcome to XYZ Saloon!" << endl;
        cout << "1. Login\n2. Register as member\n0. Exit" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

        switch (selection) {
		case 1: // navigate to login screen (login module)
            clearScreen();
            loginScreen();
            break;
		case 2: // navigate to register screen (login module)
            clearScreen();
            registerScreen();
            break;
		case 0: // exit the program (Byebye!)
            exit(0);
		default: // any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1 or 2!";
        }
    } while (true);
>>>>>>> Stashed changes

    return 0;
}

void clearScreen() {
    cout << "\033[2J\033[H";
}