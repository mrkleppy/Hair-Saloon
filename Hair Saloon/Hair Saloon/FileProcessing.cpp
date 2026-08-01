#include <iostream>
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

        Item item = { elements.at(0), elements.at(1), stod(elements.at(2)), stod(elements.at(3)), stoi(elements.at(4)), stoi(elements.at(5)) };
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

void overwriteItemFile(vector<Item>& items) {
    ofstream outFile("Item.txt");

    if (!outFile.is_open()) {
        cerr << "Error: Could not open the file!" << endl;
        return;
    }

    for (Item item : items) {
        outFile << item.itemId << "," << item.name << "," << item.price << "," << item.reorderPrice << "," << item.pointCost << "," << item.stock << "\n";
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