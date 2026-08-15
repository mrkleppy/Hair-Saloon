#include <iostream>
#include <iomanip>
#include <string>
#include <format>
#include <sstream>
#include "Main.h"
#include "AppointmentModule.h"
#include "FileProcessing.h"
#include "InventoryModule.h"

using namespace std;

void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, Appointment& appointment);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<CartItem>& cart);
void viewInvoiceDetailScreen(Invoice& invoice);
void processInvoiceItem(vector<Item> items, vector<int> quantity, double& grandTotal);
void viewReceiptScreen();
void viewPOSScreen(vector<Item>& items);

// Mwmber
void viewReceiptScreen() {
	int currentPage = 1;
	char selection;
	string input;

	// temporary data
	vector<Receipt> receipts = {
		{"INV00001", "xx/xx/xxxx", 104.70, "No"},
		{"AP00001",  "xx/xx/xxxx",  22.30, "-"}
	};

	do {
		int totalReceipt = int(receipts.size());
		int totalPages = int(ceil(static_cast<double>(totalReceipt) / MAX_RECEIPT_PER_PAGE));

		cout << "View Receipts\n";
		cout << "==============\n\n";
		cout << "Kindly show the e-receipt to the cashier to pick up your item(s)! "
			<< "(Invoice number to see receipt details)\n\n";

		cout << left << setw(15) << "Receipt(s)" << setw(15) << "Date" << setw(15) << "Total (RM)" << setw(15) << "Picked up?" << "\n";
		cout << left << setw(15) << "===========" << setw(15) << "=====" << setw(15) << "===========" << setw(15) << "===========" << "\n";

		int start = (currentPage - 1) * MAX_STAFF_PER_PAGE;
		Receipt* receiptPtr = &receipts[start];

		for (int i = 0; i < MAX_RECEIPT_PER_PAGE && (start + i) < totalReceipt; i++) {
			cout << left << setw(15) << receiptPtr->receiptId
				<< setw(15) << receiptPtr->date
				<< "RM " << setw(15) << fixed << setprecision(2) << receiptPtr->totalPrice
				<< setw(15) << receiptPtr->status << endl;
			receiptPtr++;
		}

		cout << "\nPage " << currentPage << "/" << totalPages << endl;
		cout << "(n = next page, p = previous page, q = quit)\n";
		cout << "Selection: ";

		getline(cin, input);

		if (input.empty()) {
			clearScreen();
			cout << "Invalid input! Please enter valid recipet ID, n, p, or q!" << endl;
			continue;
		}
		else if (input.size() == 1) {
			selection = input[0];
			selection = tolower(selection);
		}
		else {
			clearScreen();
			cout << "Invalid input! Please enter valid recipet ID, n, p, or q!" << endl;
			continue;
		}

		switch (selection) {
		case 'n':
			if (currentPage < totalPages) {
				currentPage++;
			}
			else {
				clearScreen();
				cout << "You are on the last page!" << endl;
			}
			break;
		case 'p':
			if (currentPage > 1) {
				currentPage--;
			}
			else {
				clearScreen();
				cout << "You are on the first page!" << endl;
			}
			break;
		case 'q':
			clearScreen();
			return;
		default:
			// TODO: do the reciept ID checks
			clearScreen();
			cout << "Invalid input! Please enter valid recipet ID, n, p, or q!" << endl;
		}
	} while (true);
}

void processInvoiceItem(vector<Item> items, vector<int> quantity, double &grandTotal) {
	int totalItem = items.size();
	double totalItemPrice;

	for (int i = 0; i < totalItem; i++) {
		totalItemPrice = items[i].price + quantity[i];
		cout << left << setw(6) << i << setw(22) << items[i].name << setw(14) << items[i].price << setw(12) << quantity[i] << setw(14) << totalItemPrice << endl;
		grandTotal += totalItemPrice;
	}
}

void viewInvoiceScreen(Customer customer,
	vector<Customer>& customers,
	vector<Appointment>& appointments,
	Appointment& appointment) {
	string input;
	char confirm;
	int selection;
	int memberPointsAdded = 0;
	int updatedMemberPoints = 0;

	do {
		clearScreen();

		cout << "Invoice Detail\n";
		cout << "===============\n";
		cout << "Customer Name : " << customer.user.name << endl;
		cout << "Reference No. : " << appointment.appointmentNo << endl;
		cout << "Date          : "
			<< left << setw(2) << setfill('0') << appointment.date.day << "/"
			<< right << setw(2) << appointment.date.month << "/"
			<< left << setw(4) << appointment.date.year << setfill(' ') << endl;
		cout << "Time          : "
			<< setw(2) << setfill('0') << appointment.time.hour << ":"
			<< setw(2) << appointment.time.minute << setfill(' ') << endl;

		cout << "\nBooked Services\n";
		cout << left << setw(6) << "No."
			<< setw(20) << "Service"
			<< setw(10) << "Gender"
			<< setw(10) << "Persons"
			<< setw(12) << "Price(RM)" << endl;

		for (int i = 0; i < appointment.serviceCount; i++) {
			cout << left << setw(6) << (i + 1)
				<< setw(20) << getServiceNameById(appointment.bookedServices[i].serviceId)
				<< setw(10) << (appointment.bookedServices[i].gender == 'M' ? "Male" : "Female")
				<< setw(10) << appointment.bookedServices[i].persons
				<< setw(12) << fixed << setprecision(2) << appointment.bookedServices[i].subtotal
				<< endl;
		}

		cout << "\nGrand Total: RM " << fixed << setprecision(2) << appointment.total << endl;
		cout << "\nProceed with transaction? (Y/N): ";
		getline(cin, input);

		if (input.empty()) {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
			continue;
		}
		else if (input.size() == 1) {
			confirm = toupper(input[0]);
		}
		else {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
			continue;
		}

		if (confirm == 'N') {
			clearScreen();
			cout << "Transaction cancelled!" << endl;
			return;
		}
		else if (confirm == 'Y') {
			cout << "\nPayment type :" << endl;
			cout << "1. Cash" << endl;
			cout << "2. Bank" << endl;
			cout << "0. Cancel Payment" << endl;
			cout << "Selection: ";
			getline(cin, input);

			if (input.empty()) {
				clearScreen();
				cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
				continue;
			}

			try {
				size_t pos;
				selection = stoi(input, &pos);

				if (pos != input.size()) {
					clearScreen();
					cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
					continue;
				}
			}
			catch (...) {
				clearScreen();
				cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
				continue;
			}

			switch (selection) {
			case 1:
			case 2:
				appointments.push_back(appointment);
				appendAppointmentToFile(appointment);

				memberPointsAdded = static_cast<int>(appointment.total);

				for (Customer& customerRecord : customers) {
					if (customerRecord.user.name == customer.user.name) {
						customerRecord.points += memberPointsAdded;
						updatedMemberPoints = customerRecord.points;
						break;
					}
				}

				overwriteCustomerFile(customers);

				clearScreen();
				cout << "Payment Done!" <<
					"\nAppointment Request Added!" <<
					"\n" << memberPointsAdded << " points has been credited to your account!" <<
					"\nCurrent Member Points: " << updatedMemberPoints <<
					"\nPress enter to continue...";
				cin.get();

				clearScreen();
				return;

			case 0:
				clearScreen();
				cout << "Payment cancelled!" << endl;
				cout << "Press enter to continue...";
				cin.get();
				clearScreen();
				return;

			default:
				clearScreen();
				cout << "Invalid payment selection!" << endl;
			}
		}
		else {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
		}

	} while (true);
}

void viewInvoiceScreen(Customer customer,
	vector<Customer>& customers,
	vector<Item>& items,
	vector<CartItem>& cart) {
	string input;
	char confirm;
	int selection;
	int memberPointsAdded = 0;
	int updatedMemberPoints = 0;
	double grandTotal = 0;

	string invoiceId = generateNextInvoiceId(); // e.g. INV00001

	do {
		clearScreen();
		grandTotal = 0;

		cout << "Invoice Detail\n";
		cout << "===============\n";
		cout << "Customer Name : " << customer.user.name << endl;
		cout << "Invoice No.   : " << invoiceId << endl;

		cout << left << setw(6) << "\nNo."
			<< setw(22) << "Item"
			<< setw(14) << "Price (RM)"
			<< setw(12) << "Quantity"
			<< setw(14) << "Total (RM)" << endl;

		for (int i = 0; i < cart.size(); i++) {
			double lineTotal = cart[i].price * cart[i].quantity;
			grandTotal += lineTotal;

			cout << left << setw(6) << (i + 1)
				<< setw(22) << cart[i].name
				<< setw(14) << fixed << setprecision(2) << cart[i].price
				<< setw(12) << cart[i].quantity
				<< setw(14) << fixed << setprecision(2) << lineTotal
				<< endl;
		}

		cout << "\nGrand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
		cout << "\nProceed with transaction? (Y/N): ";
		getline(cin, input);

		if (input.empty()) {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
			continue;
		}
		else if (input.size() == 1) {
			confirm = toupper(input[0]);
		}
		else {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
			continue;
		}

		if (confirm == 'N') {
			restoreCartStock(items, cart);
			cart.clear();

			clearScreen();
			cout << "Transaction cancelled. Items have been returned to stock." << endl;
			return;
		}
		else if (confirm == 'Y') {
			cout << "\nPayment type :" << endl;
			cout << "1. Cash" << endl;
			cout << "2. Bank" << endl;
			cout << "0. Cancel Payment" << endl;
			cout << "Selection: ";
			getline(cin, input);

			if (input.empty()) {
				clearScreen();
				cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
				continue;
			}

			try {
				size_t pos;
				selection = stoi(input, &pos);

				if (pos != input.size()) {
					clearScreen();
					cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
					continue;
				}
			}
			catch (...) {
				clearScreen();
				cout << "Invalid input! Please enter 0, 1 or 2!" << endl;
				continue;
			}

			switch (selection) {
			case 1:
			case 2:
				overwriteItemFile(items);

				memberPointsAdded = static_cast<int>(grandTotal);

				for (Customer& customerRecord : customers) {
					if (customerRecord.user.name == customer.user.name) {
						customerRecord.points += memberPointsAdded;
						updatedMemberPoints = customerRecord.points;
						break;
					}
				}

				overwriteCustomerFile(customers);

				// Optional: append invoice to file here
				// appendInvoiceToFile(invoiceId, customer.user.name, cart, grandTotal);

				cart.clear();

				clearScreen();
				cout << "Payment Done!" <<
					"\nInvoice " << invoiceId << " generated!" <<
					"\n" << memberPointsAdded << " points has been credited to your account!" <<
					"\nCurrent Member Points: " << updatedMemberPoints <<
					"\nPress enter to continue...";
				cin.get();

				clearScreen();
				return;

			case 0:
				restoreCartStock(items, cart);
				cart.clear();

				clearScreen();
				cout << "Payment cancelled! Items have been returned to stock." << endl;
				cout << "Press enter to continue...";
				cin.get();
				clearScreen();
				return;

			default:
				clearScreen();
				cout << "Invalid payment selection!" << endl;
			}
		}
		else {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
		}

	} while (true);
}

void viewInvoiceDetailScreen(Invoice& invoice) {
	double grandTotal;

	cout << left << setw(35) << ("Invoice : " + invoice.invoiceId) << "Date : " << invoice.date;
	cout << left << setw(6) << "No." << setw(22) << "Item" << setw(14) << "Price (RM)" << setw(12) << "Quantity" << setw(14) << "Total (RM)" << endl;
	cout << left << setw(6) << "===" << setw(22) << "=====" << setw(14) << "===========" << setw(12) << "=========" << setw(14) << "===========" << endl;

	processInvoiceItem(invoice.items, invoice.quantity, grandTotal);

	cout << right << setw(58) << "Grand Total (RM) " << setw(10) << fixed << setprecision(2) << grandTotal;

	cout << "\nPress enter to go back...\n";
	cin.ignore();
	cin.get();
	return;
}

// Staff;
void viewPOSScreen(vector<Item> &items) {
	string input, memberPhone, selection;
	char gender;
	int quantity, totalPerson;

	do {
		cout << "\nEnter member phone (\"cash\" for non-member): ";
		cin >> memberPhone;
		cin.ignore();
		// TODO: Verify the phone No is in the database

		clearScreen();
		do {
			cout << "C to complete transaction, Q to quit\n";
			cout << "Add an item:\n";

			cout << "Services\n";
			cout << "=========\n";
			cout << left << setw(30) << ("1." + services[0].name) << ("2." + services[1].name) << endl;
			cout << left << setw(30) << ("3." + services[2].name) << ("4." + services[3].name) << endl;

			cout << "\nItems\n";
			cout << "======\n";
			cout << left << "5.  " << setw(25) << items[0].name << setw(10) << ("RM " + format("{:.2f}", items[0].price)) << "\t6.  " << setw(25) << items[1].name << setw(10) << ("RM " + format("{:.2f}", items[1].price)) << endl;
			cout << left << "7.  " << setw(25) << items[2].name << setw(10) << ("RM " + format("{:.2f}", items[2].price)) << "\t8.  " << setw(25) << items[3].name << setw(10) << ("RM " + format("{:.2f}", items[3].price)) << endl;
			cout << left << "9.  " << setw(25) << items[4].name << setw(10) << ("RM " + format("{:.2f}", items[4].price)) << "\t10. " << setw(25) << items[5].name << setw(10) << ("RM " + format("{:.2f}", items[5].price)) << endl;
			cout << left << "11. " << setw(25) << items[6].name << setw(10) << ("RM " + format("{:.2f}", items[6].price)) << "\t12. " << setw(25) << items[7].name << setw(10) << ("RM " + format("{:.2f}", items[7].price)) << endl;

			cout << "Selection : ";
			getline(cin, selection);
			

			if (selection == "1" || selection == "2" || selection == "3" || selection == "4") {
				do {
					cout << "\nGender (M/F): ";
					getline(cin, input);

					if (input.empty()) {
						clearScreen();
						cout << "Invalid input! Please enter M or F!" << endl;
						continue;
					}
					else if (input.size() == 1) {
						gender = input[0];
						gender = toupper(gender);
					}
					else {
						clearScreen();
						cout << "Invalid input! Please enter M or F!" << endl;
						continue;
					}

					cout << "Person(s) : ";
					getline(cin, input);

					if (input.empty()) {
						clearScreen();
						cout << "Invalid input! Please enter an integer!" << endl;
						continue;
					}

					try {
						size_t pos;
						totalPerson = stoi(input, &pos);

						if (pos != input.size()) {
							clearScreen();
							cout << "Invalid input! Please enter an integer!" << endl;
							continue;
						}
					}
					catch (...) {
						clearScreen();
						cout << "Invalid input! Please enter an integer!" << endl;
						continue;
					}

					cout << "\nService added successfully.\n";
					cout << "\nPress enter to go back...\n";
					cin.get();
					clearScreen();

				} while (true);
			}
			else if (selection == "5" || selection == "6" || selection == "7" || selection == "8" ||
				selection == "9" || selection == "10" || selection == "11" || selection == "12") {

				do {
					cout << "\nQuantity (Stock available : " << items[stoi(selection) - 5].stock << ") : ";
					getline(cin, input);

					if (input.empty()) {
						clearScreen();
						cout << "Invalid input! Please enter an integer!" << endl;
						continue;
					}

					try {
						size_t pos;
						quantity = stoi(input, &pos);

						if (pos != input.size()) {
							clearScreen();
							cout << "Invalid input! Please enter an integer!" << endl;
							continue;
						}
					}
					catch (...) {
						clearScreen();
						cout << "Invalid input! Please enter an integer!" << endl;
						continue;
					}

					cout << "\nItem added successfully.\n";
					cout << "\nPress enter to go back...\n";
					cin.get();
					clearScreen();
					break;
				} while (true);
			}
			else if (selection == "c") {
				// TODO: navigate to Invoice Screen
			}
			else if (selection == "q") {
				clearScreen();
				return;
			}
			else {
				clearScreen();
				cout << "Invalid input! Please enter 1 - 12, c or q!" << endl;
			}
		} while (true);
	} while (true);
}

// Helpers
string generateNextInvoiceId() {
	static int nextNumber = 1;

	stringstream ss;
	ss << "INV" << setw(5) << setfill('0') << nextNumber++;
	return ss.str();
}