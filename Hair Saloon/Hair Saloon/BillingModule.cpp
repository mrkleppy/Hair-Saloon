#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"

using namespace std;

void viewInvoiceScreen(const string& invoiceNo);
void viewReceiptsScreen();


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

// Staff


