#pragma once

#ifndef Main
#define Main

#include <string>
#include <vector>
using namespace std;

// Define the service structure
struct Service {
	string type;
	double malePrice;
	double femalePrice;
};
 
// Define the item structure
struct Item {
    string name;
	double price;
    int pointCost;
    int stock;
};

// Define the user structure
struct User {
	string name;
	string password;
	string phoneNo;
};

// Define the customer structure
struct Customer {
	User user;
	int points;
};

// Define the staff structure
struct Staff {
	User user;
	string staffCode;
	double salary;
	int appointmentDone;
};

// Define the appointment structure
struct Appointment {
	string appointmentNo;
	string service;
	string date;
	string time;
	string status;
	Customer customer;
	Staff assignedStaff[3]; // Assuming a maximum of 3 staff can be assigned to an appointment	
};

// Define the invoice structure
struct Invoice {
	string invoiceId;
	vector<Item> items;
	vector<int> quantity;
};
// These two struct based on condition, can be merge together if want so
// Define the receipt structure
struct Receipt {
	Invoice receiptId;
	string date;
	double totalPrice;
	string status;
};

vector<Appointment> readAppointmentFile();
vector<Item> readItemFile();
vector<Staff> readStaffFile();
vector<Customer> readCustomerFile();
void overwriteItemFile(vector<Item> items);
void overwriteStaffFile(vector<Staff> staffs);
void overwriteCustomerFile(vector<Customer> customers);
void appendStaffToFile(Staff staff);
void appendCustomerToFile(Customer customer);
void clearScreen();

#endif