#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Main.h"

using namespace std;

// Read item file and load data into vector
void readItemFile(vector<Item>& items) {
    ifstream inFile("Item.txt");
    string line;

    while (getline(inFile, line)) { // get every line in the file
        stringstream ss(line);
        string element;
        vector<string> elements;

        if (!inFile.is_open()) { // check file is exist
            cerr << "Error: Could not open the file!" << endl;
            return;
        }

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
        }

        Item item = { elements.at(0), 
            elements.at(1), 
            stod(elements.at(2)), 
            stod(elements.at(3)),
            stoi(elements.at(4)), 
            stoi(elements.at(5)),
            stoi(elements.at(6)),
            stoi(elements.at(7)) }; // reconstruct into structure
        items.push_back(item); // store into the vector
    }

    inFile.close();
}

// Read service file and load data into vector
void readServiceFile(vector<Service>& services) {
    ifstream inFile("Service.txt");
    string line;

    while (getline(inFile, line)) { // get every line in the file
        stringstream ss(line);
        string element;
        vector<string> elements;

        if (!inFile.is_open()) { // check file is exist
            cerr << "Error: Could not open the file!" << endl;
            return;
        }

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
        }

        Service service = { elements.at(0),
            elements.at(1),
            stod(elements.at(2)),
            stod(elements.at(3)),
            stoi(elements.at(4)), 
            stoi(elements.at(5)) }; // reconstruct into structure
        services.push_back(service); // store into the vector
    }

    inFile.close();
}

// Read staff file and load data into vector
void readStaffFile(vector<Staff>& staffs) {
    ifstream inFile("Staff.txt");
    string line;

    if (!inFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) { // get every line in the file
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
        }

        Staff staff = { elements.at(0), elements.at(1), elements.at(2), elements.at(3), stod(elements.at(4)), stoi(elements.at(5)) }; // reconstruct into structure
        staffs.push_back(staff); // store into the vector
    }

    inFile.close();
}

// Read customer file and load data into vector
void readCustomerFile(vector<Customer>& customers) {
    ifstream inFile("Customer.txt");
    string line;

    if (!inFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) { // get every line in the file
        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
        }

        Customer customer = { elements.at(0), elements.at(1), elements.at(2), stoi(elements.at(3)) }; // reconstruct into structure
        customers.push_back(customer); // store into the vector
    }

    inFile.close();
}

// Read appointmnet file and load data into vector
void readAppointmentFile(vector<Appointment>& appointments) {
	ifstream inFile("Appointment.txt");
	string line;

	if (!inFile.is_open()) { // check file is exist
		cerr << "Error: Could not open the file!" << endl;
		return;
	}

	while (getline(inFile, line)) { // get every line in the file
		stringstream ss(line);
		vector<string> elements;
		string element;

		while (getline(ss, element, ',')) { // seperate the line by ','
			elements.push_back(element); // store every seperated element
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

            appointment.status = stringToStatus(elements.at(index++));
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
            } // reconstruct into structure

            appointments.push_back(appointment); // store into the vector
        }
        catch (const exception& e) {
			cerr << "Error parsing appointment data: " << e.what() << endl;
			continue; // Skip this line and continue with the next
        }
	}
	inFile.close();
}

// Read receipt file and load data into vector
void readReceiptFile(vector<Receipt>& receipts) {
    ifstream inFile("Receipt.txt");
    string line;

    if (!inFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) { // get every line in the file
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
        }

        // Preserve the final empty field if the line ends with a comma
        if (!line.empty() && line.back() == ',') {
            elements.push_back("");
        }

        try {
            Receipt receipt{};
            int index = 0;

            receipt.receiptId = elements.at(index++);
            receipt.referenceId = elements.at(index++);

            receipt.date.day = stoi(elements.at(index++));
            receipt.date.month = stoi(elements.at(index++));
            receipt.date.year = stoi(elements.at(index++));

            receipt.customerName = elements.at(index++);
            receipt.totalPrice = stod(elements.at(index++));
            receipt.status = stringToStatus(elements.at(index++));
            receipt.paymentType = stringToPaymentType(elements.at(index++)); // reconstruct into structure

            receipts.push_back(receipt); // store into the vector
        }
        catch (const exception& e) {
            cerr << "Error parsing receipt data: "
                << e.what() << endl;
            continue;
        }
    }

    inFile.close();
}

// Read invoice file and load data into vector
void readInvoiceFile(vector<Invoice>& invoices) {
    ifstream inFile("Invoice.txt");
    string line;

    if (!inFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    while (getline(inFile, line)) { // get every line in the file
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        vector<string> elements;
        string element;

        while (getline(ss, element, ',')) { // seperate the line by ','
            elements.push_back(element); // store every seperated element
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
            
            while (index + 1 < elements.size()) {
                InvoiceItem item{};
                item.itemId = elements.at(index++);
                item.quantity = stoi(elements.at(index++));
                invoice.invoiceItem.push_back(item);
            } // reconstruct into structure

            invoices.push_back(invoice); // store into the vector
        }
        catch (const exception& e) {
            cerr << "Error parsing invoice data: "
                << e.what() << endl;
            continue;
        }
    }

    inFile.close();
}

// Write every data from the vector into item file
void overwriteItemFile(vector<Item>& items) {
    ofstream outFile("Item.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Item item : items) { // loop through every elements in vector
        outFile << item.itemId << "," 
            << item.name << "," 
            << item.price << "," 
            << item.reorderPrice << "," 
            << item.pointCost << "," 
            << item.stock << ","
            << item.soldCounter << ","
            << item.restockCounter << "\n";
    }

    outFile.close();
}

// Write every data from the vector into service file
void overwriteServiceFile(vector<Service>& services) {
    ofstream outFile("Service.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Service service : services) { // loop through every elements in vector
        outFile << service.serviceId << ","
            << service.name << ","
            << service.malePrice << ","
            << service.femalePrice << ","
            << service.maleCounter << ","
            << service.femaleCounter << "\n";
    }

    outFile.close();
}

// Write every data from the vector into staff file
void overwriteStaffFile(vector<Staff>& staffs) {
    ofstream outFile("Staff.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Staff staff : staffs) { // loop through every elements in vector
        outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode
            << "," << staff.salary << "," << staff.appointmentDone << "\n";
    }

    outFile.close();
}

// Write every data from the vector into customer file
void overwriteCustomerFile(vector<Customer>& customers) {
    ofstream outFile("Customer.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Customer customer : customers) { // loop through every elements in vector
        outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    }

    outFile.close();
}

// Write every data from the vector into appointment file
void overwriteAppointmentFile(vector<Appointment>& appointments) {
    ofstream outFile("Appointment.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Appointment appointment : appointments) { // loop through every elements in vector
        outFile
            << appointment.appointmentNo << ","
            << appointment.serviceCount << ","
            << appointment.totalPersons << ","
            << appointment.date.day << ","
            << appointment.date.month << ","
            << appointment.date.year << ","
            << appointment.time.hour << ","
            << appointment.time.minute << ","
            << statusToString(appointment.status) << ","
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

// Write every data from the vector into receipt file
void overwriteReceiptFile(vector<Receipt>& receipts) {
    ofstream outFile("Receipt.txt");

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Receipt receipt : receipts) { // loop through every elements in vector
        outFile
            << receipt.receiptId << ","
            << receipt.referenceId << ","
            << receipt.date.day << ","
            << receipt.date.month << ","
            << receipt.date.year << ","
            << receipt.customerName << ","
            << receipt.totalPrice << ","
            << statusToString(receipt.status) << ","
            << paymentTypeToString(receipt.paymentType);

        outFile << "\n";
    }

    outFile.close();
}

// append one new data into staff file
void appendStaffToFile(Staff staff) {
    ofstream outFile("Staff.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << staff.user.name << "," << staff.user.password << "," << staff.user.phoneNo << "," << staff.staffCode
        << "," << staff.salary << "," << staff.appointmentDone << "\n";
    outFile.close();
}

// append one new data into customer file
void appendCustomerToFile(Customer customer) {
    ofstream outFile("Customer.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << customer.user.name << "," << customer.user.password << "," << customer.user.phoneNo << "," << customer.points << "\n";
    outFile.close();
}

// append one new data into appointment file
void appendAppointmentToFile(Appointment appointment) {
    ofstream outFile("Appointment.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
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
        << statusToString(appointment.status) << ","
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

// append one new data into cancelled appointment file
void appendCancelledAppointmentToFile(Appointment appointment, string& reason) {
    ofstream outFile("CancelledAppointments.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile << fixed << setprecision(2);

    outFile
        << appointment.appointmentNo << ","
        << appointment.customerName << ","
        << right << setw(2) << setfill('0') << appointment.date.day << "/"
        << right << setw(2) << setfill('0') << appointment.date.month << "/"
        << right << setw(4) << setfill('0') << appointment.date.year << ","
        << right << setw(2) << setfill('0') << appointment.time.hour << ":"
        << right << setw(2) << setfill('0') << appointment.time.minute << ","
        << left << setfill(' ') << statusToString(appointment.status) << ","
        << reason;

    outFile << endl;

    outFile.close();
}

// append one new data into invoice file
void appendInvoiceToFile(Invoice invoice) {
    ofstream outFile("Invoice.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
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

// append one new data into receipt file
void appendReceiptToFile(Receipt receipt) {
    ofstream outFile("Receipt.txt", ios::app); // call as appending

    if (!outFile.is_open()) { // check file is exist
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    outFile
        << receipt.receiptId << ","
        << receipt.referenceId << ","
        << receipt.date.day << ","
        << receipt.date.month << ","
        << receipt.date.year << ","
        << receipt.customerName << ","
        << receipt.totalPrice << ","
        << statusToString(receipt.status) << ","
        << paymentTypeToString(receipt.paymentType);

    outFile << endl;

    outFile.close();
}