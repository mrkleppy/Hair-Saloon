#include <iostream>
#include <iomanip>
#include <string>
#include <format>
#include <sstream>
#include <algorithm>
#include "Main.h"
#include "AppointmentModule.h"
#include "AppointmentReminder.h"
#include "FileProcessing.h"
#include "InventoryModule.h"

using namespace std;

void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Service>& services, vector<Receipt>& receipts, Appointment& appointment);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts, vector<CartItem>& cart);
void viewInvoiceDetailScreen(Invoice& invoice, vector<Item>& items);
void processInvoiceItem(vector<InvoiceItem> invoiceItem, vector<Item> items, double& grandTotal);
void viewPOSScreen(vector<Item>& items, vector<Customer>& customers, vector<Service>& services, vector<Receipt>& receipts);

// Helpers
string generateNextInvoiceId(vector<Invoice>& invoices);
string generateNextReceiptId(vector<Receipt>& receipts);
void loadCustomerReceipts(Customer customer, vector<Receipt>& receipts, vector<Receipt>& customerReceipts);
Receipt* findReceipt(vector<Receipt>& receipts, string receiptId);

// Member
// Displays full details of a single receipt
void printReceiptDetails(Receipt receipt) {
	clearScreen();
	cout << "Receipt Details\n";
	cout << "================\n\n";
	cout << "Receipt ID   : " << receipt.receiptId << "\n";
	cout << "Reference ID : " << receipt.referenceId << "\n";
	cout << "Date         : " << right << setw(2) << setfill('0') << receipt.date.day << "/" << setw(2) << receipt.date.month << "/" << setw(4) << receipt.date.year << left << setfill(' ') << "\n";
	cout << "Total (RM)   : " << fixed << setprecision(2) << receipt.totalPrice << "\n";
	cout << "Status		  : " << statusToString(receipt.status) << "\n";
	cout << "Payment Type : " << paymentTypeToString(receipt.paymentType) << "\n\n";
	cout << "Press enter to continue...";
	cin.get();
	clearScreen();
}

void viewReceiptScreen(Customer customer, vector<Receipt>& receipts) {
	int currentPage = 1;
	char selection;
	string input;

	vector<Receipt> customerReceipts{};

	do {
		loadCustomerReceipts(customer, receipts, customerReceipts);

		int totalReceipt = int(customerReceipts.size());
		int totalPages = totalReceipt > 0
			? int(ceil(static_cast<double>(totalReceipt) / MAX_RECEIPT_PER_PAGE))
			: 1;

		if (currentPage > totalPages) currentPage = totalPages;

		cout << "View Receipts\n";
		cout << "==============\n\n";
		cout << "Kindly show the e-receipt to the cashier to pick up your item(s)! "
			<< "(Invoice number to see receipt details)\n\n";

		if (customerReceipts.empty()) {
			cout << "No receipts found." << endl;
			cout << "Press enter to continue...";
			cin.get();
			clearScreen();
			return;
		}

		cout << left << setw(15) << setfill(' ') << "Receipt(s)" << setw(15) << "Date" << setw(15) << "Total (RM)" << setw(15) << "Status" << "\n";
		cout << left << setw(15) << "===========" << setw(15) << "==========" << setw(15) << "===========" << setw(15) << "===========" << "\n";

		int start = (currentPage - 1) * MAX_RECEIPT_PER_PAGE;
		Receipt* receiptPtr = &customerReceipts[start];

		for (int i = 0; i < MAX_RECEIPT_PER_PAGE && (start + i) < totalReceipt; i++) {
			cout << left << setw(15) << receiptPtr->receiptId
				<< setw(15) << right << setw(2) << setfill('0') << receiptPtr->date.day << "/" << setw(2) << receiptPtr->date.month << "/" << setw(4) << receiptPtr->date.year << left << setfill(' ') << setw(5) << " "
				<< "RM " << setw(12) << fixed << setprecision(2) << receiptPtr->totalPrice
				<< setw(15) << statusToString(receiptPtr->status) << endl;
			receiptPtr++;
		}

		cout << "\nPage " << currentPage << "/" << totalPages << endl;
		cout << "(n = next page, p = previous page, q = quit)\n";
		cout << "Selection: ";

		getline(cin, input);

		if (input.empty()) {
			clearScreen();
			cout << "Invalid input! Please enter valid receipt ID, n, p, or q!" << endl;
			continue;
		}
		else if (input.size() == 1) {
			selection = tolower(input[0]);
		}
		else {
			selection = '\0'; // multi-char input, fall through to receipt ID lookup
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
		default: {
			Receipt* found = findReceipt(customerReceipts, input);
			if (found != nullptr) {
				printReceiptDetails(*found);
			}
			else {
				clearScreen();
				cout << "Invalid input! Please enter valid receipt ID, n, p, or q!" << endl;
			}
			break;
		}
		}
	} while (true);
}

void processInvoiceItem(vector<InvoiceItem> invoiceItem, vector<Item> items, double &grandTotal) {
	int totalItem = invoiceItem.size();
	double totalItemPrice;
	Item itemChosen;

	for (int i = 0; i < totalItem; i++) {
		for (Item item : items) {
			if (item.itemId == invoiceItem[i].itemId) {
				itemChosen = item;
				break;
			}
			else {
				cout << "Item not found!" << endl;
				return;
			}
		}

		totalItemPrice = itemChosen.price * invoiceItem[i].quantity;
		cout << left << setw(6) << i << setw(22) << itemChosen.name << setw(14) << itemChosen.price << setw(12) << invoiceItem[i].quantity << setw(14) << totalItemPrice << endl;
		grandTotal += totalItemPrice;
	}
}

void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Service>& services, vector<Receipt>& receipts, Appointment& appointment) {
	string input;
	char confirm;
	int selection;
	int memberPointsAdded = 0;
	int updatedMemberPoints = 0;
	Receipt newReceipt;
	string receiptId = generateNextReceiptId(receipts);

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
				<< setw(20) << getServiceNameById(appointment.bookedServices[i].serviceId, services)
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
			do {
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

				break;
			} while (true);
			switch (selection) {
			case 1:
			case 2:
				// Save the appointment file first
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

				// Add member points to the customer record and save it to the file
				overwriteCustomerFile(customers);

				// Save the receipt file
				// Build the new receipt
				Time placeholder; // Only get the date so time is not needed, thats why a placeholder is needed

				newReceipt.receiptId = receiptId;
				newReceipt.referenceId = appointment.appointmentNo;
				getCurrentDateTime(newReceipt.date, placeholder);
				newReceipt.customerName = customer.user.name;
				newReceipt.totalPrice = appointment.total;
				newReceipt.status = appointment.status;
				newReceipt.paymentType = (selection == 1 ? CASH : BANK);

				receipts.push_back(newReceipt);
				appendReceiptToFile(newReceipt);

				// Save the service file (Update the male and female counters for each)
				for (int i = 0; i < appointment.serviceCount; i++) {
					for (Service& service : services) {
						if (service.serviceId == appointment.bookedServices[i].serviceId) {
							if (appointment.bookedServices[i].gender == 'M') {
								service.maleCounter += appointment.bookedServices[i].persons;
							}
							else if (appointment.bookedServices[i].gender == 'F') {
								service.femaleCounter += appointment.bookedServices[i].persons;
							}
							break;
						}
					}
				}

				overwriteServiceFile(services);

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
	vector<Invoice>& invoices,
	vector<Receipt>& receipts,
	vector<CartItem>& cart) {
	string input;
	char confirm;
	int selection;
	int memberPointsAdded = 0;
	int updatedMemberPoints = 0;
	double grandTotal = 0;
	Invoice newInvoice;
	Receipt newReceipt;
	string invoiceId = generateNextInvoiceId(invoices);
	string receiptId = generateNextReceiptId(receipts);

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

			cout << left << setw(5) << (i + 1)
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
			do {
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

				break;
			} while (true);

			switch (selection) {
			case 1:
			case 2:
				// Update the stock and sold counter for each item in the cart, Save to item file
				for (const CartItem& cartItem : cart) {
					for (Item& item : items) {
						if (item.itemId == cartItem.itemId) {
							item.soldCounter += cartItem.quantity;
							break;
						}
					}
				}
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

				// Save invoice file
				// Build the new invoice
				Time placeholder; // Only get the date so time is not needed, thats why a placeholder is needed

				newInvoice.invoiceId = invoiceId;
				getCurrentDateTime(newInvoice.date, placeholder);
				newInvoice.customerName = customer.user.name;
				for (CartItem& c : cart) {
					newInvoice.invoiceItem.push_back({ c.itemId, c.quantity });
				}
				invoices.push_back(newInvoice); // Update memory
				appendInvoiceToFile(newInvoice); // Update File

				// Save receipt file
				// Build the new receipt
				newReceipt.receiptId = receiptId;
				newReceipt.referenceId = invoiceId;
				getCurrentDateTime(newReceipt.date, placeholder);
				newReceipt.customerName = customer.user.name;
				newReceipt.totalPrice = grandTotal;
				newReceipt.status = NOT_PICKED_UP;
				newReceipt.paymentType = (selection == 1 ? CASH : BANK);

				receipts.push_back(newReceipt);
				appendReceiptToFile(newReceipt);

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

void viewInvoiceDetailScreen(Invoice& invoice, vector<Item>& items) {
	double grandTotal;
	vector<InvoiceItem> invoiceItem;
	cout << left << setw(35) << ("Invoice : " + invoice.invoiceId) << "Date : " << right << setw(2) << setfill('0') << invoice.date.day << "/" << setw(2) << invoice.date.month << "/" << setw(4) << invoice.date.year << setfill(' ');
	cout << left << setw(6) << "No." << setw(22) << "Item" << setw(14) << "Price (RM)" << setw(12) << "Quantity" << setw(14) << "Total (RM)" << endl;
	cout << left << setw(6) << "===" << setw(22) << "=====" << setw(14) << "===========" << setw(12) << "=========" << setw(14) << "===========" << endl;

	processInvoiceItem(invoiceItem, items, grandTotal);

	cout << right << setw(58) << "Grand Total (RM) " << setw(10) << fixed << setprecision(2) << grandTotal;

	cout << "\nPress enter to go back...\n";
	cin.ignore();
	cin.get();
	return;
}

// Staff
void viewInvoiceScreen(Customer customer,
	vector<Customer>& customers,
	vector<Item>& items,
	vector<Service>& services,
	vector<Receipt>& receipts,
	vector<AppointmentService>& orderedServices,
	vector<CartItem>& orderedItems) {
	
	string input;
	char confirm;
	int selection;
	int memberPointsAdded = 0;
	int updatedMemberPoints = customer.points;
	double grandTotal = 0;
	Receipt newReceipt;
	string receiptId = generateNextReceiptId(receipts);

	do {
		clearScreen();
		grandTotal = 0;

		cout << "Invoice Detail\n";
		cout << "===============\n";
		cout << "Customer Name : " << customer.user.name << endl;
		cout << "Receipt No.   : " << receiptId << endl;

		if (!orderedServices.empty()) {
			cout << "\nBooked Services\n";
			cout << left << setw(6) << "No."
				<< setw(18) << "Service"
				<< setw(14) << "Gender"
				<< setw(12) << "Persons"
				<< setw(14) << "Price(RM)" << endl;

			for (int i = 0; i < orderedServices.size(); i++) {
				grandTotal += orderedServices[i].subtotal;

				cout << left << setw(6) << (i + 1)
					<< setw(18) << getServiceNameById(orderedServices[i].serviceId, services)
					<< setw(14) << (orderedServices[i].gender == 'M' ? "Male" : "Female")
					<< setw(12) << orderedServices[i].persons
					<< setw(14) << fixed << setprecision(2) << orderedServices[i].subtotal
					<< endl;
			}
		}

		if (!orderedItems.empty()) {
			cout << "\nOrdered Items\n";
			cout << left << setw(6) << "No."
				<< setw(18) << "Item"
				<< setw(14) << "Price (RM)"
				<< setw(12) << "Quantity"
				<< setw(14) << "Total (RM)" << endl;

			for (int i = 0; i < orderedItems.size(); i++) {
				double lineTotal = orderedItems[i].price * orderedItems[i].quantity;
				grandTotal += lineTotal;

				cout << left << setw(6) << (i + 1)
					<< setw(18) << orderedItems[i].name
					<< setw(14) << fixed << setprecision(2) << orderedItems[i].price
					<< setw(12) << orderedItems[i].quantity
					<< setw(14) << fixed << setprecision(2) << lineTotal
					<< endl;
			}
		}

		cout << "\nGrand Total: RM " << fixed << setprecision(2) << grandTotal << endl;
		cout << "\nProceed with transaction? (Y/N): ";
		getline(cin, input);

		if (input.empty() || input.size() != 1) {
			clearScreen();
			cout << "Invalid input! Please enter Y or N!" << endl;
			continue;
		}

		confirm = toupper(input[0]);

		if (confirm == 'N') {
			clearScreen();
			cout << "Transaction cancelled!" << endl;
			return;
		}
		else if (confirm == 'Y') {
			do {
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

				break;
			} while (true);

			switch (selection) {
			case 1:
			case 2:
				// Update both service and item counters and save to file
				for (int i = 0; i < orderedServices.size(); i++) {
					for (Service& service : services) {
						if (service.serviceId == orderedServices[i].serviceId) {
							if (orderedServices[i].gender == 'M') {
								service.maleCounter += orderedServices[i].persons;
							}
							else if (orderedServices[i].gender == 'F') {
								service.femaleCounter += orderedServices[i].persons;
							}
							break;
						}
					}
				}

				for (int i = 0; i < orderedItems.size(); i++) {
					for (Item& item : items) {
						if (item.itemId == orderedItems[i].itemId) {
							item.soldCounter += orderedItems[i].quantity;
							break;
						}
					}
				}

				overwriteServiceFile(services);
				overwriteItemFile(items);

				// Save the customer points to file and update the customer record
				if (customer.user.phoneNo != "Cash") {
					memberPointsAdded = static_cast<int>(grandTotal);

					for (Customer& customerRecord : customers) {
						if (customerRecord.user.phoneNo == customer.user.phoneNo) {
							customerRecord.points += memberPointsAdded;
							updatedMemberPoints = customerRecord.points;
							break;
						}
					}

					overwriteCustomerFile(customers);
				}

				// Save the receipt file
				Time placeholder; // Only get the date so time is not needed, thats why a placeholder is needed

				newReceipt.receiptId = receiptId;
				newReceipt.referenceId = "POS";
				getCurrentDateTime(newReceipt.date, placeholder);
				newReceipt.customerName = customer.user.name;
				newReceipt.totalPrice = grandTotal;
				newReceipt.status = COMPLETED;
				newReceipt.paymentType = (selection == 1 ? CASH : BANK);

				receipts.push_back(newReceipt);
				appendReceiptToFile(newReceipt);

				orderedItems.clear();
				orderedServices.clear();

				clearScreen();
				cout << "Payment Done!" << endl;
				cout << "Receipt " << receiptId << " generated!" << endl;

				if (customer.user.phoneNo != "Cash") {
					cout << memberPointsAdded << " points has been credited to your account!" << endl;
					cout << "Current Member Points: " << updatedMemberPoints << endl;
				}

				cout << "Press enter to continue...";
				cin.get();

				clearScreen();
				return;

			case 0:
				restoreCartStock(items, orderedItems);
				orderedItems.clear();
				orderedServices.clear();

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

void viewPOSScreen(vector<Item> &items, vector<Customer> &customers, vector<Service>& services, vector<Receipt>& receipts) {
	Customer customerChosen;
	string input, memberPhone, selection;
	char gender;
	int quantity, totalPerson;
	int remaining = MAX_PERSONS_PER_SLOT;
	vector<AppointmentService> orderedServices;
	vector<CartItem> orderedItems;

	do {
		cout << "Enter member phone (\"cash\" for non-member): ";
		getline(cin, memberPhone);

		if (memberPhone == "q" || memberPhone == "Q") {
			clearScreen();
			return;
		}

		string normalisedInput = memberPhone;
		transform(normalisedInput.begin(), normalisedInput.end(), normalisedInput.begin(), ::tolower);

		bool isCash = (normalisedInput == "cash");
		bool customerFound = false;

		if (!isCash) {
			for (const Customer& customer : customers) {
				if (memberPhone == customer.user.phoneNo) {
					customerChosen = customer;
					customerFound = true;
					break;
				}
			}

			if (!customerFound) {
				clearScreen();
				cout << "Customer not found!" << endl;
				continue;
			}
		}
		else {
			customerChosen.user.name = "Walk in Customer";
			customerChosen.user.phoneNo = "Cash";
			customerChosen.user.password = " ";
			customerChosen.points = 0;
		}

		clearScreen();
		do {
			cout << "C to complete transaction, Q to quit\n";
			cout << "Add an item:\n";

			cout << "Services\n";
			cout << "=========\n";
			cout << left << setw(18) << ("1. " + services[0].name) << ("\t2. " + services[1].name) << endl;
			cout << left << setw(18) << ("3. " + services[2].name) << ("\t4. " + services[3].name) << endl;

			cout << "\nItems\n";
			cout << "======\n";
			cout << left << "5.  " << setw(18) << items[0].name << setw(10) << ("RM " + format("{:.2f}", items[0].price)) << "\t6.  " << setw(18) << items[1].name << setw(10) << ("RM " + format("{:.2f}", items[1].price)) << endl;
			cout << left << "7.  " << setw(18) << items[2].name << setw(10) << ("RM " + format("{:.2f}", items[2].price)) << "\t8.  " << setw(18) << items[3].name << setw(10) << ("RM " + format("{:.2f}", items[3].price)) << endl;
			cout << left << "9.  " << setw(18) << items[4].name << setw(10) << ("RM " + format("{:.2f}", items[4].price)) << "\t10. " << setw(18) << items[5].name << setw(10) << ("RM " + format("{:.2f}", items[5].price)) << endl;
			cout << left << "11. " << setw(18) << items[6].name << setw(10) << ("RM " + format("{:.2f}", items[6].price)) << "\t12. " << setw(18) << items[7].name << setw(10) << ("RM " + format("{:.2f}", items[7].price)) << endl;

			cout << "\nSelection : ";
			getline(cin, selection);
			
			if (selection.empty()) {
				clearScreen();
				cout << "Invalid input! Please enter 1 - 12, c or q!" << endl;
				continue;
			}

			if (remaining == 0 && (selection == "1" || selection == "2" || selection == "3" || selection == "4")) {
				clearScreen();
				cout << "Maximum of 7 service persons already reached for this receipt!" << endl;
				continue;
			}

			if (selection == "1" || selection == "2" || selection == "3" || selection == "4") {
				
				int serviceIndex = stoi(selection) - 1;
				bool cancelService = false;

				do {
					cout << "\nGender (M/F): ";
					getline(cin, input);

					if (input == "q" || input == "Q") {
						cancelService = true;
						break;
					}

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

					if (gender != 'M' && gender != 'F') {
						clearScreen();
						cout << "Invalid input! Please enter M or F!" << endl;
						continue;
					}

					break;
				} while (true);

				if (cancelService) {
					continue;
				}

				do {
					cout << "Person(s) : ";
					getline(cin, input);

					if (input == "q" || input == "Q") {
						cancelService = true;
						break;
					}

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

						if (totalPerson <= 0) {
							clearScreen();
							cout << "Please enter a number more than 0!" << endl;
							continue;
						}

						if (totalPerson > remaining) {
							clearScreen();
							cout << "Please enter a maximum person of " << remaining << " persons." << endl;
							continue;
						}

						break;
					}
					catch (...) {
						clearScreen();
						cout << "Invalid input! Please enter an integer!" << endl;
						continue;
					}
				} while (true);

				if (cancelService) {
					clearScreen();
					continue;
				}

				remaining -= totalPerson;

				// Build the new service to be put in the list of ordered services
				AppointmentService newService;
				newService.serviceId = services[serviceIndex].serviceId;
				newService.gender = gender;
				newService.persons = totalPerson;
				newService.subtotal = gender == 'M' ? services[serviceIndex].malePrice * totalPerson : services[serviceIndex].femalePrice * totalPerson;

				orderedServices.push_back(newService);

				cout << "\nService added successfully.\n";
				cout << "\nPress enter to go back...\n";
				cin.get();
				clearScreen();
			}
			else if (selection == "5" || selection == "6" || selection == "7" || selection == "8" ||
				selection == "9" || selection == "10" || selection == "11" || selection == "12") {

				int itemIndex = stoi(selection) - 5;

				if (items[itemIndex].stock <= 0) {
					clearScreen();
					cout << "Item is out of stock!" << endl;
					continue;
				}

				do {
					cout << "\nQuantity (Stock available : " << items[itemIndex].stock << ") : ";
					getline(cin, input);

					if (input == "q") {
						clearScreen();
						break;
					}

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

					if (quantity <= 0) {
						clearScreen();
						cout << "Quantity must be more than 0!" << endl;
						continue;
					}

					if (quantity > items[itemIndex].stock) {
						clearScreen();
						cout << "Not enough stock available!" << endl;
						continue;
					}

					items[itemIndex].stock -= quantity;  // Deduct the stock immediately

					// Build the new item to be put in the list of ordered items
					CartItem newItem;

					bool foundInCart = false;
					for (CartItem& cartItem : orderedItems) {
						if (cartItem.itemId == items[itemIndex].itemId) {
							cartItem.quantity += quantity;
							foundInCart = true;
							break;
						}
					}

					if (!foundInCart) {
						newItem.itemId = items[itemIndex].itemId;
						newItem.name = items[itemIndex].name;
						newItem.price = items[itemIndex].price;
						newItem.quantity = quantity;
						orderedItems.push_back(newItem);
					}

					cout << "\nItem added successfully.\n";
					cout << "\nPress enter to go back...\n";
					cin.get();
					clearScreen();
					break;

				} while (true);
			}
			else if (tolower(selection[0]) == 'c') {
				if (orderedServices.empty() && orderedItems.empty()) {
					clearScreen();
					cout << "No items or services have been added yet!" << endl;
					continue;
				}

				viewInvoiceScreen(customerChosen, customers, items, services, receipts, orderedServices, orderedItems);
			}
			else if (tolower(selection[0]) == 'q') {
				restoreCartStock(items, orderedItems);
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
string generateNextInvoiceId(vector<Invoice>& invoices) {
	int nextInvoiceNumber = 0;

	if (nextInvoiceNumber == 0) {
		for (const Invoice& invoice : invoices) {
			const string& id = invoice.invoiceId;

			if (id.length() == 8 && id.substr(0, 3) == "INV") {
				try {
					nextInvoiceNumber = max(nextInvoiceNumber, stoi(id.substr(3)));
				}
				catch (...) {
				}
			}
		}

		nextInvoiceNumber++;
	}

	stringstream ss;
	ss << "INV" << setw(5) << setfill('0') << nextInvoiceNumber++;
	return ss.str();
}

string generateNextReceiptId(vector<Receipt>& receipts) {
	int nextReceiptNumber = 0;

	if (nextReceiptNumber == 0) {
		for (const Receipt& receipt : receipts) {
			const string& id = receipt.receiptId;

			if (id.length() == 8 && id.substr(0, 3) == "REC") {
				try {
					nextReceiptNumber = max(nextReceiptNumber, stoi(id.substr(3)));
				}
				catch (...) {
				}
			}
		}

		nextReceiptNumber++;
	}

	stringstream ss;
	ss << "REC" << setw(5) << setfill('0') << nextReceiptNumber++;
	return ss.str();
}

// Filters the master receipts list down to this customer's receipts
void loadCustomerReceipts(Customer customer, vector<Receipt>& receipts, vector<Receipt>& customerReceipts) {
	customerReceipts.clear();
	for (Receipt& r : receipts) {
		if (r.customerName == customer.user.name) {  // adjust field name to match your Customer/Receipt linkage
			customerReceipts.push_back(r);
		}
	}
}

// Finds a receipt by ID within a given vector, returns nullptr if not found
Receipt* findReceipt(vector<Receipt>& receipts, string receiptId) {
	for (Receipt& r : receipts) {
		if (r.receiptId == receiptId) {
			return &r;
		}
	}
	return nullptr;
}

