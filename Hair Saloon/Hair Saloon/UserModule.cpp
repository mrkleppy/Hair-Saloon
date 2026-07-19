#include <iostream>
#include <iomanip>
<<<<<<< Updated upstream
#include <string>
=======
#include <string>
#include "Main.h"
#include "LoginModule.h"
#include "AppointmentModule.h"
#include "InventoryModule.h"
#include "BillingModule.h"
#include "ReportingModule.h"

using namespace std;

void RedeemPointsPage(string phoneNo) { // Redeem points page for customer maintenance
    // placeholder for now
    string customerName = "Lim Ah Kau";
    int points = 1000;
	int stock[8] = { 10, 5, 8, 12, 15, 20, 7, 3 };

    // Read file to get customer name and points based on phoneNo (TODO)
    
    // Read file to get the stock (TODO)

	string itemName[8] = { "shampoo", "dry shampoo", "conditioner", "hair dye", "hair gel", "hair spray", "styling cream", "hair scalp" }; // item name array
	int itemPointCost[8] = { 600, 675, 570, 450, 375, 480, 510, 825 }; // item point cost array

	// Variable declarations
    int selection = 0, quantity = 0, totalPointCost = 0;
    char confirm = 'N';

	// Define a struct to hold item information
    struct item {
        string name;
		int pointCost;
		int stock;
    };   

    // Create an array of items
    item items[8];
    for (int i = 0; i < 8; i++) {
        items[i].name = itemName[i];
        items[i].pointCost = itemPointCost[i];
        items[i].stock = stock[i];
    }

	// Pointer to the chosen item
    item * itemChosen;

	do { // Display the redeem points page UI
        cout << "Customer Name: " << customerName << endl;
        cout << "Membership Points: " << points << "pts" << endl;
        cout << "Select an item to redeem" << endl;
        cout << left << setw(25) << "1. Shampoo" << setw(15) << "600 pts" << setw(25) << "2. Dry shampoo" << setw(15) << "675 pts" << endl;
        cout << left << setw(25) << "3. Conditioner" << setw(15) << "570 pts" << setw(25) << "4. Hair dye" << setw(15) << "450 pts" << endl;
        cout << left << setw(25) << "5. Hair gel" << setw(15) << "375 pts" << setw(25) << "6. Hair spray" << setw(15) << "480 pts" << endl;
        cout << left << setw(25) << "7. Styling cream" << setw(15) << "510 pts" << setw(25) << "8. Hair scalp" << setw(15) << "825 pts" << endl;
        cout << endl << "Selection (0 to exit): ";
		cin >> selection;
        cin.ignore();

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
				cin >> quantity;
                cin.ignore();

				if (quantity > itemChosen->stock) { // Only proceed if the quantity is less than or equal to the stock available
                    clearScreen();
                    cout << "Not enough stock available!" << endl;
                } else {
					totalPointCost = quantity * itemChosen->pointCost; // Calculate the total point cost

					if (totalPointCost > points) { // Only proceed if the total point cost is less than or equal to the points available
                        clearScreen();
                        cout << "Not enough points to redeem!" << endl;
                    } else {
                        // Deduct points and stock (TODO)
						// placeholder for now
						points -= totalPointCost;
                        itemChosen->stock -= quantity; 

                        // Update the stock and points in the file (TODO)


						// Display confirmation message
						cout << "Confirm redeem " << quantity << " " << itemChosen->name << " for " << totalPointCost << " pts? (Y/N): ";
						cin >> confirm;
                        cin.ignore();
						confirm = toupper(confirm);
						
						if (confirm == 'Y') { // If the user confirms, display success message
							cout << "Redeem successful! Deducted " << totalPointCost << " points" << endl;
							cout << "Press any key to continue..." << endl;
                            cin.get();
                            clearScreen();
						}
						else if (confirm == 'N') { // If the user cancels, display cancel message
							cout << "Redeem cancelled." << endl;
							cout << "Press any key to continue..." << endl;
							cin.get();
                            clearScreen();
						}
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
    string phoneNo = "";

	do { // Display the customer maintenance page UI
        cout << "Customer Maintenance" << endl;
        cout << "1. Redeem points\n2. View all appointment\n0. Exit" << endl;
        cout << "selection: ";
        cin >> selection;
        cin.ignore();

		if (selection == 1 || selection == 2) { // If the selection is 1 or 2, prompt for customer's phone number
            cout << "Customer's phone number: ";
			getline(cin, phoneNo);
        }

        switch (selection) {
		case 1: // navigate to redeem points page (user module)
            clearScreen();
            RedeemPointsPage(phoneNo);
            break;
        case 2:
            clearScreen();
            // navigate to view all appointment (appointment module)
            break;
		case 3: // exit the customer maintenance page
            clearScreen();
            return;
		default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!";
        } 
    } while (true);
}

void staffMaintenancePage() {

}

void memberHomePage(string username) {
	// Variable declarations
    int selection = 0;

	do { // Display the member home page UI
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Manage an appointment\n2. Buy an item\n3. View receipts\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

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
            cout << "Invalid input! Please enter 0, 1, 2, or 3!";
        }
    } while (true);
}

void staffHomePage(string username) {
	// Variable declarations
    int selection = 0;

	do { // Display the staff home page UI
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. POS system\n2. View assigned appointment\n3. Inventory maintenance \n4. Customer maintenance\n5. View appointment done\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

        switch (selection) {
        case 1:
            clearScreen();
            //navigate to POS system (billing and payment module)
            break;
		case 2: // navigate to view assigned appointment (appointment module)
            clearScreen();
            assignedAppointmentsView(username);
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
            completedAppointmentsView(username);
            break;
		case 0: // exit the staff home page
            clearScreen();
            return;
		default: // Any invalid situation, display error message and prompt user to try again
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!";
        }
    } while(true);
}

void adminHomePage(string username) {
	// Variable declarations
    int selection = 0;

	do { // Display the admin home page UI
        cout << "Welcome " << username << "!" << endl;
        cout << "What would you like to do?" << endl;
        cout << "1. Inventory maintenance\n2. Staff maintenance\n3. Assign appointments \
        \n4. View reportings\n0. Logout" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

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
            cout << "Invalid input! Please enter 0, 1, 2, 3, or 4!";
        }
    } while(true);
}
>>>>>>> Stashed changes
