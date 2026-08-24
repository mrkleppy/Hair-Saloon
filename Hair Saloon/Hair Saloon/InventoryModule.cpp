#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "FileProcessing.h"
#include "BillingModule.h"
#include "AppointmentModule.h"

using namespace std;

// Functions to be defined early
void viewCartPage(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts, vector<CartItem>& cart);
void inventoryMaintenancePage(vector<Item>& items);
void removeItemFromCart(vector<Item>& items, vector<CartItem>& cart);

// Helpers
Item* findItemById(vector<Item>& items, const string& itemId);
void addToCart(vector<Item>& items, vector<CartItem>& cart, int itemIndex, int quantity);
void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart);
double calculateCartTotal(const vector<CartItem>& cart);

// Member side Inventory Module
void purchaseItemPage(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts) {
    vector<CartItem> cart;
    int quantity;
    char selection, confirmation;
    Item* itemChosen;
    clearScreen();

    do {
        cout << "Select an item to order and pick up at our store!\nEnter 'c' to complete transaction\nEnter 'v' to view cart\n" << endl;
        
		// Display items in two columns
        for (int i = 0; i < items.size(); i++) {
			// First column
            cout << left << (i + 1) << ". " << setw(15) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price << setw(5) << " ";
            
            i++;

			// Second column
            cout << left << (i + 1) << ". " << setw(15) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price;

			// Proceed to new row
            cout << endl;
        }

        cout << endl << "Selection (0 to exit): ";
        cin >> selection;
        cin.ignore();

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

			// Check if the item is out of stock
            if (itemChosen->stock <= 0) {
                clearScreen();
                cout << itemChosen->name << " is out of stock! Please select another item!" << endl;
                continue;
            }

            cout << "Quantity (Stock available: " << itemChosen->stock << "): ";
            cin >> quantity;
            cin.ignore();

			// Validate the quantity input (must be positive and not exceed available stock)
            if (quantity <= 0 || quantity > itemChosen->stock) {
                clearScreen();
                cout << "Invalid quantity! Please enter a valid quantity." << endl;
                continue;
            }

            cout << "Confirm adding item " << itemChosen->name << " x " << quantity << "? (Total: RM " << fixed << setprecision(2) << itemChosen->price * quantity << "): ";
            cin >> confirmation;
            cin.ignore();

			// Handle confirmation input
            if (tolower(confirmation) == 'y') {
				// Add the item to the cart
                addToCart(items, cart, selection - '1', quantity);
                clearScreen();
                cout << "Item " << itemChosen->name << " x " << quantity << " has been added to your order!" << endl;
            }
            else if (tolower(confirmation) == 'n') {
                clearScreen();
                cout << "Order cancelled." << endl;
                continue;
            }
            else {
                clearScreen();
                cout << "Invalid input! Please enter Y or N!" << endl;
                continue;
            }
            break;

        case '0':
			// Exit the purchase item page and restore stock for items that is in the cart
            restoreCartStock(items, cart);

			// Clear the cart and exit
            cart.clear();
            clearScreen();
            return;
        case 'c':
            clearScreen();

			// Check if the cart is empty before proceeding to the invoice screen
            if (cart.empty()) {
                cout << "Your cart is empty! Please purchase any items!";
                continue;
            }

			// If the cart is not empty, proceed to the invoice screen
            else {
                viewInvoiceScreen(customer, customers, items, invoices, receipts, cart);
                break;
            }

            break;

        case 'v':
			// View the cart page
            clearScreen();
            viewCartPage(customer, customers, items, invoices, receipts, cart);
            continue;

        default:
            clearScreen();
            cout << "Invalid input! Please enter 0-8, c or v!" << endl;
            break;
        }
    } while (true);
}

void viewCartPage(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts, vector<CartItem>& cart) {
    string input;
    char selection;

    clearScreen();

    do {
		// Check if the cart is empty
        if (cart.empty()) {
            cout << "Cart is empty." << endl;
            cout << "Press enter to go back...";
            cin.get();
            return;
        }

		// If it's not empty, display the cart items and total

        double total = 0;
        clearScreen();

        // Header
        cout << "Cart page" << endl;
        cout << "=========" << endl;
        cout << left << setw(20) << "Item Name"
            << setw(13) << "Quantity"
            << setw(10) << "Price" << endl;

		// Display each item in the cart with its subtotal
        for (const CartItem& cartItem : cart) {
            double subtotal = cartItem.price * cartItem.quantity;
            total += subtotal;

            cout << left << setw(24) << cartItem.name
                << setw(9) << cartItem.quantity
                << "RM " << fixed << setprecision(2) << subtotal << endl;
        }

        cout << "\nTotal: RM " << fixed << setprecision(2) << total << endl;
        cout << "\n(c = complete transaction, r = remove an item, q = exit cart)" << endl;
        cout << "Selection: ";
        getline(cin, input);

        if (input.empty()) {
            cout << "Invalid input! Please enter c, r, or q!" << endl;
            continue;
        }

        if (input.length() != 1) {
            cout << "Invalid input! Please enter only one character: c, r, or q!" << endl;
            continue;
        }

        selection = tolower(input[0]);

        switch (selection) {
        case 'c':
			// Proceed to the invoice screen if the customer chooses to complete the transaction
            clearScreen();
            viewInvoiceScreen(customer, customers, items, invoices, receipts, cart);
            return;

        case 'r':
			// Proceed to remove an item from the cart if the customer chooses to do so
            removeItemFromCart(items, cart);
            break;

        case 'q':
			// Exit the cart page and return to the previous menu
            clearScreen();
            return;

        default:
            cout << "Invalid input! Please enter c, r, or q!" << endl;
            break;
        }
    } while (true);
}

void removeItemFromCart(vector<Item>& items, vector<CartItem>& cart) {
    string input;
    int itemIndexToRemove;
    Item* itemChosen = nullptr;

    do {
        clearScreen();

        // Header
        cout << "Remove item from cart" << endl;
        cout << "=====================" << endl;

        cout << left << setw(5) << "No."
            << setw(20) << "Item Name"
            << setw(10) << "Quantity"
            << setw(10) << "Price" << endl;

		// Display each item in the cart with its subtotal
        for (int i = 0; i < cart.size(); i++) {
            cout << left << setw(5) << (i + 1)
                << setw(20) << cart[i].name
                << setw(10) << cart[i].quantity
                << "RM " << fixed << setprecision(2)
                << cart[i].price * cart[i].quantity << endl;
        }

        cout << "\nEnter item number to remove (0 to cancel): ";
        getline(cin, input);

        if (input.empty()) {
            cout << "Invalid input! Please enter a number." << endl;
            continue;
        }

        try {
            size_t pos;
			itemIndexToRemove = stoi(input, &pos); // Convert string to integer

            if (pos != input.length()) {
                throw invalid_argument("Extra characters found");
            }
        }
        catch (...) {
            cout << "Invalid input! Please enter a valid number." << endl;
            continue;
        }

		// If the customer chooses to cancel the removal, exit the function
        if (itemIndexToRemove == 0) {
            clearScreen();
            return;
        }

		// Check if the item index is valid (within the range of the cart size)
        if (itemIndexToRemove < 1 || itemIndexToRemove > cart.size()) {
            cout << "Invalid item index! Please enter a valid index." << endl;
            continue;
        }

		itemIndexToRemove--; // Adjust for 0-based index

		// Find the corresponding item in the items vector to restore its stock
        itemChosen = findItemById(items, cart[itemIndexToRemove].itemId);

		// Restore the stock of the item in the items vector
        if (itemChosen != nullptr) {
            itemChosen->stock += cart[itemIndexToRemove].quantity;
        }

		// Store the name and quantity of the removed item for display
        string removedItemName = cart[itemIndexToRemove].name;
        int removedQuantity = cart[itemIndexToRemove].quantity;

		// Remove the item from the cart
        cart.erase(cart.begin() + itemIndexToRemove);

        clearScreen();
        cout << "Item " << removedItemName << " x " << removedQuantity
            << " has been removed from the cart." << endl;
        cout << "Press enter to continue...";
        cin.get();
        return;

    } while (true);
}

// Staff and Admin side Inventory Module
void inventoryMaintenancePage(vector<Item>& items) {
    Item* itemChosen;
    char confirmation;
    string itemId;
    int quantity;
    clearScreen();

    do {
        // Header
        cout << "Inventory Maintenance" << endl;
        cout << "=====================" << endl;
        cout << left << setw(8) << "ID" 
            << setw(16) << "Item Name" 
            << setw(20) << "Stock(s) available" 
            << setw(14) << "Reorder Price" << endl;

		// Display each item in the inventory with its details
        for (const Item& item : items) {
            cout << left << setw(8) << item.itemId 
                << setw(24) << item.name 
                << setw(15) << item.stock 
                << "RM " << fixed << setprecision(2) << item.reorderPrice << endl;
        }

        cout << "\nWhat item to reorder? (q to quit): ";
        getline(cin, itemId);

        if (itemId == "q" || itemId == "Q") {
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

		// If the item chosen is not found in the inventory, display an error message and continue the loop
        if (itemChosen == nullptr) {
            clearScreen();
            cout << "Invalid item ID! Please enter a valid Item ID!" << endl;
            continue;
        }

        // If the item chosen is found, prompt for restock
        do {
            cout << "Quantity: ";
            cin >> quantity;
            cin.ignore();

			// Validate the quantity input (must be positive and not exceed 50)
            if (quantity <= 0) {
                clearScreen();
                cout << "Invalid quantity! Please enter a positive quantity." << endl;
                continue;
            }
            else if (quantity > 50) {
                clearScreen();
                cout << "Invalid quantity! Please enter a quantity less than or equal to 50." << endl;
                continue;
            }

            // Confirmation
            do {
                cout << "Confirm restock " << itemChosen->name << " x " << quantity << "? (Cost: RM " << fixed << setprecision(2) << itemChosen->reorderPrice * quantity << "): ";
                cin >> confirmation;
                cin.ignore();

                if (tolower(confirmation) == 'n') {
                    clearScreen();
                    cout << "Restock cancelled." << endl;
                    break;
                }
                else if (tolower(confirmation) == 'y') {
                    itemChosen->stock += quantity; // Update stock (in memory)
					itemChosen->restockCounter += quantity; // Update restock counter (in memory)
                    overwriteItemFile(items); // Save changes to file

                    clearScreen();
                    cout << quantity << " " << itemChosen->name << "(s) have been restocked!" << endl;
                    break;
                }
                else {
                    cout << "Invalid choice! Please enter Y or N!" << endl;
                }
            } while (true);

            break;
        } while (true);
    } while (true);
}

// Helpers
Item* findItemById(vector<Item>& items, const string& itemId) {
    // Find the item in the items vector based on the item ID
    // Return a pointer to it. If not found, return nullptr.
    return findBy(items, [&](Item& item) {
        return item.itemId == itemId;
        });
}

void addToCart(vector<Item>& items, vector<CartItem>& cart, int itemIndex, int quantity) {
	Item& item = items[itemIndex]; // Get a reference to the item in the items vector

	// Check if the item is already in the cart
    for (CartItem& cartItem : cart) {
        if (cartItem.itemId == item.itemId) {
			// If its found, update the quantity in the cart and reduce the stock in the items vector
            cartItem.quantity += quantity;
            item.stock -= quantity;
            return;
        }
    }

    // Build the new item that was added in the cart
    CartItem newCartItem{};
    newCartItem.itemId = item.itemId;
    newCartItem.name = item.name;
    newCartItem.price = item.price;
    newCartItem.quantity = quantity;

	// Add them to the cart and reduce the stock in the items vector accordingly
    cart.push_back(newCartItem);
    item.stock -= quantity;
}

void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart) {
	// Restore the stock of items in the items vector based on the quantities in the cart
    for (const CartItem& cartItem : cart) {
		// Find the corresponding item in the items vector and restore its stock
        Item* itemPtr = findItemById(items, cartItem.itemId);
        if (itemPtr != nullptr) {
			// Once the item is found, we restore its quantity in the stock by adding the quantity from the cart back to the stock
            itemPtr->stock += cartItem.quantity;
        }
    }
}

double calculateCartTotal(const vector<CartItem>& cart) {
	double total = 0; // Initialize total to 0

    for (const CartItem& cartItem : cart) {
		// Add up the total cost of each item in the cart by multiplying its price by its quantity
        total += cartItem.price * cartItem.quantity;
    }

    return total;
}