#pragma once

#ifndef Main
#define Main

#include <string>
#include <vector>
using namespace std;

const int MAX_PERSONS_PER_SLOT = 7; // Maximum number of persons allowed per time slot
const int MAX_SERVICES_PER_APPOINTMENT = 7; // Maximum number of services allowed per appointment
const int MAX_STAFF_PER_PAGE = 10;
const int MAX_APPOINTMENTS_PER_PAGE = 10;
const int MAX_RECEIPT_PER_PAGE = 10;

// Define the service structure
struct Service {
	string serviceId;
	string name;
	double malePrice;
	double femalePrice;
};
 
// Define the item structure
struct CartItem {
	string itemId;
	string name;
	double price;
	int quantity;
};

struct Item {
	string itemId;
	string name;
	double price;
	double reorderPrice;
	int pointCost;
	int stock;
	int restockCounter;
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
struct AppointmentService {
	string serviceId;
	char gender;
	int persons;
	double subtotal;
};

struct Appointment {
	string appointmentNo;
	AppointmentService bookedServices[7]; // Assuming a maximum of 7 services can be booked per appointment
	int serviceCount;
	int totalPersons;
	Date date;
	Time time;
	string status;
	string customerName;
	string assignedStaffIds[7]; // Assuming a maximum of 7 staff can be assigned to an appointment
	int staffCount;
	double total;
};

// Define the invoice structure
struct Invoice {
	string invoiceId;
	vector<Item> items;
	vector<int> quantity;
	string date;
};
// These two struct based on condition, can be merge together if want so
// Define the receipt structure
struct Receipt {
	string receiptId; // temporary
	string date;
	double totalPrice;
	string status;
};

const Service services[4] = {
    {"SV001", "Hair Cut", 25, 45},
    {"SV002", "Hair Coloring", 80, 170},
    {"SV003", "Hair Treatment", 70, 90},
    {"SV004", "Styling", 30, 35}
};

void clearScreen();

#endif