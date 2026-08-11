#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "FileProcessing.h"

using namespace std;

// Functions to be defined early

void viewCartPage();
void inventoryMaintenancePage(vector<Item>& items);
void removeItemFromCart();
void purchaseItemPage(vector<Item>& items);

// Member side Inventory Module

void purchaseItemPage(vector<Item>& items) {
    int quantity;
    char selection, confirmation;
    Item* itemChosen;

    do {
        cout << "Select an item to order and pick up at our store!\nEnter 'c' to complete transaction\nEnter 'v' to view cart\n" << endl;
        
        for (int i = 0; i < items.size(); i++) {
            cout << left << (i + 1) << ". " << setw(20) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price << setw(10) << " ";
            i++;
            cout << left << (i + 1) << ". " << setw(20) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price << endl;
        }

        cout << endl << "Selection (0 to exit): ";
        cin >> selection;

        selection = tolower(selection);

        switch (selection) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            itemChosen = &items[selection - '1']; // Get the pointer to the chosen item

            cout << "Quantity (Stock available: " << itemChosen->stock << "): ";
            cin >> quantity;
            cin.ignore();

            if (quantity <= 0 || quantity > itemChosen->stock) {
                clearScreen();
                cout << "Invalid quantity! Please enter a valid quantity." << endl;
                continue;
            }

            cout << "Confirm adding item " << itemChosen->name << " x " << quantity << "? (Total: RM " << fixed << setprecision(2) << itemChosen->price * quantity << "): ";
            cin >> confirmation;
            cin.ignore();

            if (confirmation == 'Y' || confirmation == 'y') {
                // Add item to order (cart)
                itemChosen->stock -= quantity; // Update stock
                clearScreen();
                cout << "Item " << itemChosen->name << " x " << quantity << " has been added to your order!" << endl;
            }
            else if (confirmation == 'N' || confirmation == 'n') {
                clearScreen();
                cout << "Order cancelled." << endl;
                continue;
            }
            break;
        case '0':
            clearScreen();
            return;
        case 'c':
            clearScreen();
            // invoiceScreen(); // placeholder for invoice screen
            continue;
        case 'v':
            clearScreen();
            viewCartPage();
            continue;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0-8, c or v!" << endl;
            break;
        }
    } while (true);
}

void viewCartPage() {
    char selection;

    do {
        cout << "Cart page" << endl;
        cout << "=========" << endl;
        cout << left << setw(20) << "Item Name" << setw(10) << "Quantity" << setw(10) << "Price" << endl;

        // Display Items here


        cout << "\nTotal: RM " << fixed << setprecision(2) << 0.00 << endl; // Placeholder for total price
        cout << "\n(c = complete transaction, r = remove an item, q = exit cart)" << endl;
        cin >> selection;
        selection = tolower(selection);

        if (selection == 'c') {
            clearScreen();
            // invoiceScreen(); // placeholder for invoice screen
            return;
        }
        else if (selection == 'r') {
            clearScreen();
            removeItemFromCart();
            return;
        }
        else if (selection == 'q') {
            clearScreen();
            return;
        }
        else {
            clearScreen();
            cout << "Invalid input! Please enter c, r, or q!" << endl;
            continue;
        }
    } while (true);
}

void removeItemFromCart() {
    int itemIndexToRemove;
    Item* itemChosen;

    do {
        cout << "Remove item from cart" << endl;
        cout << "=====================" << endl;

        // Display items in cart here

        cin >> itemIndexToRemove;
        cin.ignore();

		// TODO: Implement the logic to remove the item from the cart based on the index provided by the user.
		if (itemIndexToRemove < 1 || itemIndexToRemove > 8) { // Assuming a maximum of 8 items in the cart
			clearScreen();
			cout << "Invalid item index! Please enter a valid index." << endl;
			continue;
		}


    } while (true);
}

// Staff and Admin side Inventory Module

void inventoryMaintenancePage(vector<Item>& items) {
    Item* itemChosen;
    char itemId[5], confirmation;
    int quantity;

    do {
        cout << "Inventory Maintenance" << endl;
        cout << "=====================" << endl;
        cout << left << setw(8) << "ID" << setw(16) << "Item Name" << setw(20) << "Stock(s) available" << setw(14) << "Reorder Price" << endl;

        for (const Item& item : items) {
            cout << left << setw(8) << item.itemId << setw(24) << item.name << setw(15) << item.stock << setw(3) << "RM " << fixed << setprecision(2) << item.price << endl;
        }

        cout << "\nWhat item to reorder? (q to quit): ";
        cin >> itemId;

        if (strcmp(itemId, "q") == 0 || strcmp(itemId, "Q") == 0) {
            clearScreen();
            return;
        }

        itemChosen = nullptr; // Reset itemChosen for each iteration
        for (Item& item : items) {
            if (item.itemId == itemId) {
                itemChosen = &item; // Get the pointer to the chosen item
                break;
            }
        }

        if (itemChosen == nullptr) {
            clearScreen();
            cout << "Invalid item ID! Please enter a valid Item ID!" << endl;
            continue;
        }

        do {
            cout << "Quantity: ";
            cin >> quantity;
            cin.ignore();

            if (quantity <= 0) {
                clearScreen();
                cout << "Invalid quantity! Please enter a positive quantity." << endl;
                continue;
            }
            else if (quantity > 50) {
                clearScreen();
                cout << "Invalid quantity! Please enter a quantity less than or equal to 30." << endl;
                continue;
            }

            do {
                cout << "Confirm restock " << itemChosen->name << " x " << quantity << "? (Cost: RM " << fixed << setprecision(2) << itemChosen->reorderPrice * quantity << "): ";
                cin >> confirmation;
                cin.ignore();

                confirmation = tolower(confirmation);
                if (confirmation == 'n') {
                    cout << "Restock cancelled." << endl;
                    return;
                }
                else if (confirmation == 'y') {
                    itemChosen->stock += quantity; // Update stock
                    overwriteItemFile(items); // Save changes to file
                    clearScreen();
                    cout << quantity << " " << itemChosen->name << "(s) have been restocked!" << endl;
                    return;
                }
                else {
                    cout << "Invalid choice! Please enter Y or N!" << endl;
                }
            } while (true);
        } while (true);
    } while (true);
}