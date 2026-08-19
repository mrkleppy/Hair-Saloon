#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Main.h"

using namespace std;

void readItemFile(vector<Item>& items) {
    ifstream inFile("Item.txt");
    string line;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string element;
        vector<string> elements;

        if (!inFile.is_open()) {
            cerr << "Error: Could not open the file!" << endl;
            return;
        }

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Item item = { elements.at(0), 
            elements.at(1), stod(elements.at(2)), stod(elements.at(3)),stoi(elements.at(4)), stoi(elements.at(5)),stoi(elements.at(6)) };
        items.push_back(item);
    }

    inFile.close();
}

void readStaffFile(vector<Staff>& staffs) {
    ifstream inFile("Staff.txt");
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Staff staff = { elements.at(0), elements.at(1), elements.at(2), elements.at(3), stod(elements.at(4)), stoi(elements.at(5)) };
        staffs.push_back(staff);
    }

    inFile.close();
}

void readCustomerFile(vector<Customer>& customers) {
    ifstream inFile("Customer.txt");
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        Customer customer = { elements.at(0), elements.at(1), elements.at(2), stoi(elements.at(3)) };
        customers.push_back(customer);
    }

    inFile.close();
}

void readAppointmentFile(vector<Appointment>& appointments) {
	ifstream inFile("Appointment.txt");
	string line;

	if (!inFile.is_open()) {
		cerr << "Error: Could not open the file!" << endl;
		return;
	}

	while (getline(inFile, line)) {
		stringstream ss(line);
		vector<string> elements;
		string element;

		while (getline(ss, element, ',')) {
			elements.push_back(element);
		}

        if (!line.empty() && line.back() == ',') {
            elements.push_back("");
        }

        try {
            Appointment appointment{};
            int index = 0;

            appointment.appointmentNo = elements.at(index++);
            appointment.serviceCount = stoi(elements.at(index++));
            appointment.totalPersons = stoi(elements.at(index++));

            appointment.date.day = stoi(elements.at(index++));
            appointment.date.month = stoi(elements.at(index++));
            appointment.date.year = stoi(elements.at(index++));

            appointment.time.hour = stoi(elements.at(index++));
            appointment.time.minute = stoi(elements.at(index++));

            appointment.status = elements.at(index++);
            appointment.customerName = elements.at(index++);
            appointment.staffCount = stoi(elements.at(index++));
            appointment.total = stod(elements.at(index++));

            for (int i = 0; i < appointment.serviceCount; i++) {
                appointment.bookedServices[i].serviceId = elements.at(index++);
                appointment.bookedServices[i].gender = elements.at(index++)[0];
                appointment.bookedServices[i].persons = stoi(elements.at(index++));
                appointment.bookedServices[i].subtotal = stod(elements.at(index++));
            }

            for (int i = 0; i < appointment.totalPersons && index < elements.size(); i++) {
                if (!elements[index].empty()) {
                    appointment.assignedStaffIds[i] = elements.at(index++);
                }
                else {
                    index++;
                }
            }

            appointments.push_back(appointment);
        }
        catch (const exception& e) {
			cerr << "Error parsing appointment data: " << e.what() << endl;
			continue; // Skip this line and continue with the next
        }
	}
	inFile.close();
}

void readReceiptFile(vector<Receipt>& receipts) {
    ifstream inFile("Receipt.txt");
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        // Preserve the final empty field if the line ends with a comma
        if (!line.empty() && line.back() == ',') {
            elements.push_back("");
        }

        try {
            Receipt receipt{};
            int index = 0;

            receipt.receiptId = elements.at(index++);
            receipt.invoiceId = elements.at(index++);

            receipt.date.day = stoi(elements.at(index++));
            receipt.date.month = stoi(elements.at(index++));
            receipt.date.year = stoi(elements.at(index++));

            receipt.customerName = elements.at(index++);
            receipt.totalPrice = stod(elements.at(index++));
            receipt.status = elements.at(index++);
            receipt.paymentType = elements.at(index++);

            receipts.push_back(receipt);
        }
        catch (const exception& e) {
            cerr << "Error parsing receipt data: "
                << e.what() << endl;
            continue;
        }
    }

    inFile.close();
}

void readInvoiceFile(vector<Invoice>& invoices) {
    ifstream inFile("Invoice.txt");
    string line;

    if (!inFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) {
            elements.push_back(element);
        }

        // Preserve the final empty field if the line ends with a comma
        if (!line.empty() && line.back() == ',') {
            elements.push_back("");
        }

        try {
            Invoice invoice{};
            int index = 0;

            invoice.invoiceId = elements.at(index++);

            invoice.date.day = stoi(elements.at(index++));
            invoice.date.month = stoi(elements.at(index++));
            invoice.date.year = stoi(elements.at(index++));

            invoice.customerName = elements.at(index++);
            
            for (int i = 0; i < invoice.invoiceItem.size(); i++) {
                invoice.invoiceItem[i].itemId = elements.at(index++);
                invoice.invoiceItem[i].quantity = stod(elements.at(index++));
            }

            invoices.push_back(invoice);
        }
        catch (const exception& e) {
            cerr << "Error parsing invoice data: "
                << e.what() << endl;
            continue;
        }
    }

    inFile.close();
}

void overwriteItemFile(vector<Item>& items) {
    ofstream outFile("Item.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Item item : items) {
        outFile << item.itemId << "," 
            << item.name << "," 
            << item.price << "," 
            << item.reorderPrice << "," 
            << item.pointCost << "," 
            << item.stock << ","
            << item.restockCounter << "\n";
    }

    outFile.close();
}

void overwriteStaffFile(vector<Staff>& staffs) {
    ofstream outFile("Staff.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Staff staff : staffs) {
        outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode
            << "," << staff.salary << "," << staff.appointmentDone << "\n";
    }

    outFile.close();
}

void overwriteCustomerFile(vector<Customer>& customers) {
    ofstream outFile("Customer.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Customer customer : customers) {
        outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    }

    outFile.close();
}

void overwriteAppointmentFile(vector<Appointment>& appointments) {
    ofstream outFile("Appointment.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Appointment appointment : appointments) {
        outFile
            << appointment.appointmentNo << ","
            << appointment.serviceCount << ","
            << appointment.totalPersons << ","
            << appointment.date.day << ","
            << appointment.date.month << ","
            << appointment.date.year << ","
            << appointment.time.hour << ","
            << appointment.time.minute << ","
            << appointment.status << ","
            << appointment.customerName << ","
            << appointment.staffCount << ","
            << fixed << setprecision(2) << appointment.total;

        for (int i = 0; i < appointment.serviceCount; i++) {
            outFile
                << "," << appointment.bookedServices[i].serviceId
                << "," << appointment.bookedServices[i].gender
                << "," << appointment.bookedServices[i].persons
                << "," << fixed << setprecision(2) << appointment.bookedServices[i].subtotal;
        }

		for (int i = 0; i < appointment.totalPersons; i++) {
			outFile << "," << appointment.assignedStaffIds[i];
		}

		outFile << "\n";
    }

    outFile.close();
}

void overwriteReceiptFile(vector<Receipt>& receipts) {
    ofstream outFile("Receipt.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Receipt receipt : receipts) {
        outFile
            << receipt.receiptId << ","
            << receipt.invoiceId << ","
            << receipt.date.day << ","
            << receipt.date.month << ","
            << receipt.date.year << ","
            << receipt.customerName << ","
            << receipt.totalPrice << ","
            << receipt.status << ","
            << receipt.paymentType;

        outFile << "\n";
    }

    outFile.close();
}

void appendStaffToFile(Staff staff) {
    ofstream outFile("Staff.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode
        << "," << staff.salary << "," << staff.appointmentDone << "\n";
    outFile.close();
}

void appendCustomerToFile(Customer customer) {
    ofstream outFile("Customer.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    outFile.close();
}

void appendAppointmentToFile(Appointment appointment) {
    ofstream outFile("Appointment.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << fixed << setprecision(2);

    outFile
        << appointment.appointmentNo << ","
        << appointment.serviceCount << ","
        << appointment.totalPersons << ","
        << appointment.date.day << ","
        << appointment.date.month << ","
        << appointment.date.year << ","
        << appointment.time.hour << ","
        << appointment.time.minute << ","
        << appointment.status << ","
        << appointment.customerName << ","
        << appointment.staffCount << ","
        << fixed << setprecision(2) << appointment.total;

    for (int i = 0; i < appointment.serviceCount; i++) {
        outFile
            << "," << appointment.bookedServices[i].serviceId
            << "," << appointment.bookedServices[i].gender
            << "," << appointment.bookedServices[i].persons
            << "," << fixed << setprecision(2) << appointment.bookedServices[i].subtotal;
    }

    for (int i = 0; i < appointment.totalPersons; i++) {
        outFile << "," << appointment.assignedStaffIds[i];
    }

    outFile << endl;

    outFile.close();
}

void appendCancelledAppointmentToFile(Appointment appointment, string& reason) {
    ofstream outFile("CancelledAppointments.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << fixed << setprecision(2);

    outFile
        << appointment.appointmentNo << ", "
        << appointment.customerName << ", "
        << appointment.date.day << "/"
        << appointment.date.month << "/"
        << appointment.date.year << ", "
        << appointment.time.hour << ":"
        << appointment.time.minute << ", "
        << appointment.status << ", "
        << reason;

    outFile << endl;

    outFile.close();
}

void appendInvoiceToFile(Invoice invoice) {
    ofstream outFile("Invoice.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile
        << invoice.invoiceId << ","
        << invoice.date.day << ","
        << invoice.date.month << ","
        << invoice.date.year << ","
        << invoice.customerName;

    for (int i = 0; i < invoice.invoiceItem.size(); i++) {
        outFile
            << "," << invoice.invoiceItem[i].itemId
            << "," << invoice.invoiceItem[i].quantity;
    }

    outFile << endl;

    outFile.close();
}

void appendReceiptToFile(Receipt receipt) {
    ofstream outFile("Receipt.txt", ios::app);

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile
        << receipt.receiptId << ","
        << receipt.invoiceId << ","
        << receipt.date.day << ","
        << receipt.date.month << ","
        << receipt.date.year << ","
        << receipt.customerName << ","
        << receipt.totalPrice << ","
        << receipt.status << ","
        << receipt.paymentType;

    outFile << endl;

    outFile.close();
}