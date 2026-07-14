#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "LoginModule.h"
#include "AppointmentModule.h"
#include "InventoryModule.h"
#include "BillingModule.h"
#include "ReportingModule.h"

void customerMaintenancePage() {

}

void staffMaintenancePage() {

}

void memberHomePage(string username) {
    int selection = 0;
    bool validate = true;

    do {
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Manage an appointment\n2. Buy an item\n3. View reciepts\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;

        switch (selection) {
        case 1:
            clearScreen();
            //navigate to manage appointment (appointment module)
            break;
        case 2:
            clearScreen();
            //navigate to buy item (inventory module)
            break;
        case 3:
            clearScreen();
            //navigate to view reciept (billing module)
            break;
        case 0:
            clearScreen();
            mainScreen();
            break;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2 or 3!";
            validate = false;
            break;
        }
    } while (!validate);
}

void staffHomePage(string username) {
    int selection = 0;
    bool validate = true;

    do {
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. POS system\n2. View assigned appointment\n3. Inventory maintenance \
        \n4. Customer maintenance\n5. View appointment done\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;

        switch (selection) {
        case 1:
            clearScreen();
            //navigate to POS system (inventory module??)
            break;
        case 2:
            clearScreen();
            //navigate to view assigned appointment (appointment module)
            break;
        case 3:
            clearScreen();
            //navigate to inventory maintenance (inventory module)
            break;
        case 4:
            clearScreen();
            customerMaintenancePage();
            break;
        case 5:
            clearScreen();
            //navigate to view appointment done (appointment module)
            break;
        case 0:
            clearScreen();
            mainScreen();
            break;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2 or 3!"; \
                validate = false;
            break;
        }
    } while (!validate);
}

void adminHomePage(string username) {
    int selection = 0;
    bool validate = true;

    do {
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Inventory maintenance\n2. Staff maintenance\n3. Assign appointments \
        \n4. View reportings\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;

        switch (selection) {
        case 1:
            clearScreen();
            //navigate to inventory maintenance (inventory module)
            break;
        case 2:
            clearScreen();
            staffMaintenancePage();
            break;
        case 3:
            clearScreen();
            //navigate to assign appointments (appointment module)
            break;
        case 4:
            clearScreen();
            //navigate to view reportings (reporting module)
            break;
        case 0:
            clearScreen();
            mainScreen();
            break;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2 or 3!"; \
                validate = false;
            break;
        }
    } while (!validate);
}