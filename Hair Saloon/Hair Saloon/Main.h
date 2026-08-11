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
	string itemId;
	string name;
	double price;
	double reorderPrice;
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

struct Date {
	int day;
	int month;
	int year;
};

struct Time {
	int hour;
	int minute;
};

// Define the appointment structure
struct Appointment {
	string appointmentNo;
	string service;
	Date date;
	Time time;
	string status;
	Customer customer;
	Staff assignedStaff[5]; // Assuming a maximum of 5 staff can be assigned to an appointment	
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
	string receiptId;
	string date;
	double totalPrice;
	string status;
};

const Service services[4] = {
    {"Hair Cut", 25, 45},
    {"Hair Coloring", 80, 170},
    {"Hair Treatment", 70, 90},
    {"Styling", 30, 35}
};

vector<Appointment> readAppointmentFile();
void clearScreen();

#endif