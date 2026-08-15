#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"
#include "FileProcessing.h"
#include "BillingModule.h"

using namespace std;

// Functions to be defined early
void viewCartPage(vector<Item>& items, vector<CartItem>& cart);
void inventoryMaintenancePage(vector<Item>& items);
void removeItemFromCart(vector<Item>& items, vector<CartItem>& cart);

// Helpers
Item* findItemById(vector<Item>& items, const string& itemId);
void addToCart(vector<Item>& items, vector<CartItem>& cart, int itemIndex, int quantity);
void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart);
double calculateCartTotal(const vector<CartItem>& cart);

// Member side Inventory Module
void purchaseItemPage(vector<Item>& items) {
    vector<CartItem> cart;
    int quantity;
    char selection, confirmation;
    Item* itemChosen;
    clearScreen();

    do {
        cout << "Select an item to order and pick up at our store!\nEnter 'c' to complete transaction\nEnter 'v' to view cart\n" << endl;
        
        for (int i = 0; i < items.size(); i++) {
            cout << left << (i + 1) << ". " << setw(15) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price << setw(5) << " ";
            i++;
            cout << left << (i + 1) << ". " << setw(15) << items[i].name << setw(3) << "RM " << fixed << setprecision(2) << items[i].price << endl;
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

            if (tolower(confirmation) == 'y') {
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
            restoreCartStock(items, cart);
            cart.clear();
            clearScreen();
            return;
        case 'c':
            clearScreen();

            if (cart.empty()) {
                cout << "Your cart is empty! Please purchase any items!";
                continue;
            }
            else {
                // invoice screen
                break;
            }

            break;

        case 'v':
            clearScreen();
            viewCartPage(items, cart);
            continue;

        default:
            clearScreen();
            cout << "Invalid input! Please enter 0-8, c or v!" << endl;
            break;
        }
    } while (true);
}

void viewCartPage(vector<Item>& items, vector<CartItem>& cart) {
    char selection;
    clearScreen();

    if (cart.empty()) {
        clearScreen();
        cout << "Cart is empty." << endl;
        cout << "Press enter to go back...";
        cin.get();
        return;
    }

    do {
        double total = 0;

        cout << "Cart page" << endl;
        cout << "=========" << endl;
        cout << left << setw(15) << "Item Name" << setw(13) << "Quantity" << setw(10) << "Price" << endl;

        for (const CartItem& cartItem : cart) {
            double subtotal = cartItem.price * cartItem.quantity;
            total += subtotal;

            cout << left << setw(20) << cartItem.name
                << setw(10) << cartItem.quantity
                << "RM " << fixed << setprecision(2) << subtotal << endl;
        }

        cout << "\nTotal: RM " << fixed << setprecision(2) << total << endl; // Placeholder for total price
        cout << "\n(c = complete transaction, r = remove an item, q = exit cart)" << endl;
        
        cin >> selection;
        cin.ignore();
        selection = tolower(selection);

        switch (selection) {
        case 'c':
            clearScreen();
            // invoice screen
            return;

        case 'r':
            clearScreen();
            removeItemFromCart(items, cart);
            break;

        case 'q':
            clearScreen();
            return;

        default:
            clearScreen();
            cout << "Invalid input! Please enter c, r, or q!" << endl;
            continue;
        }
    } while (true);
}

void removeItemFromCart(vector<Item>& items, vector<CartItem>& cart) {
    int itemIndexToRemove;
    Item* itemChosen;
    clearScreen();

    do {
        cout << "Remove item from cart" << endl;
        cout << "=====================" << endl;

        cout << left << setw(5) << "No."
            << setw(20) << "Item Name"
            << setw(10) << "Quantity"
            << setw(10) << "Price" << endl;

        for (int i = 0; i < cart.size(); i++) {
            cout << left << setw(5) << (i + 1)
                << setw(20) << cart[i].name
                << setw(10) << cart[i].quantity
                << "RM " << fixed << setprecision(2) << cart[i].price * cart[i].quantity
                << endl;
        }

        cout << "\nEnter item number to remove (0 to cancel): ";
        cin >> itemIndexToRemove;
        cin.ignore();

        if (itemIndexToRemove == 0) {
            clearScreen();
            return;
        }

		if (itemIndexToRemove < 1 || itemIndexToRemove > cart.size()) {
			clearScreen();
			cout << "Invalid item index! Please enter a valid index." << endl;
			continue;
		}

        itemChosen = findItemById(items, cart[itemIndexToRemove - 1].itemId);

        if (itemChosen != nullptr) {
            itemChosen->stock += cart[itemIndexToRemove - 1].quantity;
        }

        string removedItemName = cart[itemIndexToRemove - 1].name;
        int removedQuantity = cart[itemIndexToRemove - 1].quantity;

        cart.erase(cart.begin() + (itemIndexToRemove - 1));

        clearScreen();
        cout << "Item " << removedItemName << " x " << removedQuantity << " has been removed from the cart." << endl;
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
        cout << "Inventory Maintenance" << endl;
        cout << "=====================" << endl;
        cout << left << setw(8) << "ID" 
            << setw(16) << "Item Name" 
            << setw(20) << "Stock(s) available" 
            << setw(14) << "Reorder Price" << endl;

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
                cout << "Invalid quantity! Please enter a quantity less than or equal to 50." << endl;
                continue;
            }

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
                    itemChosen->stock += quantity; // Update stock
                    itemChosen->restockCounter += quantity;
                    overwriteItemFile(items); // Save changes to file

                    clearScreen();
                    cout << quantity << " " << itemChosen->name << "(s) have been restocked!" << endl;
                    return;
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
    for (Item& item : items) {
        if (item.itemId == itemId) {
            return &item;
        }
    }

    return nullptr;
}

void addToCart(vector<Item>& items, vector<CartItem>& cart, int itemIndex, int quantity) {
    Item& item = items[itemIndex];

    for (CartItem& cartItem : cart) {
        if (cartItem.itemId == item.itemId) {
            cartItem.quantity += quantity;
            item.stock -= quantity;
            return;
        }
    }

    CartItem newCartItem{};
    newCartItem.itemId = item.itemId;
    newCartItem.name = item.name;
    newCartItem.price = item.price;
    newCartItem.quantity = quantity;

    cart.push_back(newCartItem);
    item.stock -= quantity;
}

void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart) {
    for (const CartItem& cartItem : cart) {
        Item* itemPtr = findItemById(items, cartItem.itemId);
        if (itemPtr != nullptr) {
            itemPtr->stock += cartItem.quantity;
        }
    }
}

double calculateCartTotal(const vector<CartItem>& cart) {
    double total = 0;

    for (const CartItem& cartItem : cart) {
        total += cartItem.price * cartItem.quantity;
    }

    return total;
}