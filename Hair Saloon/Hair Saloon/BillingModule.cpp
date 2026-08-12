#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"

using namespace std;

void viewInvoiceScreen(const string& invoiceNo);
void viewReceiptScreen();
void viewPOSScreen();

// Mwmber
void viewReceiptScreen() {
	vector<Receipt> receipts = {
		{"INV00001", "xx/xx/xxxx", 104.70, "No"},
		{"AP00001",  "xx/xx/xxxx",  22.30, "-"}
	};

	do {
		cout << "\n";
		cout << "View Receipts\n";
		cout << "------------------------------------------------------------\n";
		cout << "Kindly show the e-receipt to the cashier to pick up your item(s)! "
			<< "(Invoice number to see receipt details)\n\n";

		cout << left << setw(15) << "Receipt(s)"
			<< setw(15) << "Date"
			<< setw(15) << "Total (RM)"
			<< setw(15) << "Picked up?" << "\n";

		cout << "------------------------------------------------------------\n";

		for (const Receipt& r : receipts) {
			cout << left << setw(15) << r.receiptId
				<< setw(15) << r.date
				<< setw(15) << fixed << setprecision(2) << r.totalPrice
				<< setw(15) << r.status << "\n";
		}

		cout << "\nPage 1/1\n";
		cout << "(n = next page, p = previous page, q = quit)\n";
		cout << "Selection : ";

		string choice;
		cin >> choice;

		if (choice == "INV00001") {
			cout << "\033[34m" << choice << "\033[0m\n";
			viewInvoiceScreen(choice);
		}
	} while (true);
}

void viewInvoiceScreen(const string& invoiceNo) {
	cout << "\n";
	cout << "                     Invoice Details\n";
	cout << "============================================================\n";
	cout << "Invoice : " << invoiceNo << "\n";
	cout << "Date    : xx/xx/xxxx\n\n";

	cout << left << setw(6) << "No."
		<< setw(20) << "Item"
		<< setw(15) << "Price (RM)"
		<< setw(10) << "Quantity"
		<< setw(15) << "Total (RM)" << "\n";

	cout << "------------------------------------------------------------\n";

	vector<Item> items = {
		{"1", "Shampoo", 39.90, 79.80, 5, 2},
		{"2", "Hair gel", 24.90, 24.90, 5, 2}
	};

	for (const Item& item : items) {
		cout << left << setw(6) << item.itemId
			<< setw(20) << item.name
			<< setw(15) << fixed << setprecision(2) << item.price
			<< setw(10) << item.stock
			<< setw(15) << item.price << "\n";
	}

	cout << "\n";
	cout << right << setw(55) << "Grand Total (RM) 104.70\n";
	cout << "\nPress any key to go back...\n";
	cin.ignore();
	cin.get();
}

// Staff;
void viewPOSScreen() {
	string memberPhone;
	string selection;
	string gender;
	int quantity;

	cout << "\nEnter member phone (\"cash\" for non-member): ";
	cin >> memberPhone;

	cout << "\nC to complete transaction, Q to quit\n";
	cout << "Add an item:\n\n";

	cout << "Services\n";
	cout << "---------------------------------------------\n";
	cout << left << setw(30) << "1. Hair Cut"
		<< "2. Hair Coloring\n";
	cout << left << setw(30) << "3. Hair Treatment"
		<< "4. Styling\n\n";

	cout << "Items\n";
	cout << "---------------------------------------------\n";
	cout << left << setw(30) << "5. Shampoo        RM39.90"
		<< "6. Dry Shampoo    RM45.00\n";
	cout << left << setw(30) << "7. Conditioner    RM36.00"
		<< "8. Hair Dye       RM27.90\n";
	cout << left << setw(30) << "9. Hair Gel       RM24.90"
		<< "10. Hair Spray    RM32.00\n";
	cout << left << setw(30) << "11. Styling Cream RM34.00"
		<< "12. Hair Scalp    RM35.00\n\n";

	cout << "Selection : ";
	cin >> selection;

	if (selection == "1" || selection == "2" || selection == "3" || selection == "4") {
		cout << "\nGender : ";
		cin >> gender;

		cout << "Quantity : ";
		cin >> quantity;

		cout << "\nService added successfully.\n";
	}
	else if (selection == "5" || selection == "6" || selection == "7" || selection == "8" ||
		selection == "9" || selection == "10" || selection == "11" || selection == "12") {
		cout << "\nQuantity (Stock available : x) : ";
		cin >> quantity;

		cout << "\nItem added successfully.\n";
	}
	else if (selection == "C" || selection == "c") {
		cout << "\nTransaction completed.\n";
	}
	else if (selection == "Q" || selection == "q") {
		cout << "\nExit POS screen.\n";
	}
	else {
		cout << "\nInvalid selection.\n";
	}
}