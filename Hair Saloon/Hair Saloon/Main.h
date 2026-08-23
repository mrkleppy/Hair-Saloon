#pragma once

#ifndef Main
#define Main

#include <string>
#include <vector>
using namespace std;

const int MAX_PERSONS_PER_SLOT = 7; // Maximum number of persons allowed per time slot
const int MAX_SERVICES_PER_APPOINTMENT = 7; // Maximum number of services allowed per appointment
const int MAX_STAFF_PER_PAGE = 10; // Maximum number of staff shown in one page
const int MAX_APPOINTMENTS_PER_PAGE = 10; // Maximum number of appointment shown in one page
const int MAX_RECEIPT_PER_PAGE = 10; // Maximum number of receipt shown in one page

// Define the service structure
struct Service {
	string serviceId;
	string name;
	double malePrice;
	double femalePrice;
	int maleCounter;
	int femaleCounter;
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
	int soldCounter;
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

// Define the date structure
struct Date {
	int day;
	int month;
	int year;
};

// Define the time structure
struct Time {
	int hour;
	int minute;
};

// Define the services in appointment structure
struct AppointmentService {
	string serviceId;
	char gender;
	int persons;
	double subtotal;
};

// Define the appointment structure
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

// Define the item in invoice structure
struct InvoiceItem {
	string itemId;
	int quantity;
};

// Define the invoice structure
struct Invoice {
	string invoiceId;
	Date date;
	string customerName;
	vector<InvoiceItem> invoiceItem;
};

// Define the receipt structure
struct Receipt {
	string receiptId;
	string referenceId;
	Date date;
	string customerName;
	double totalPrice;
	string status;
	string paymentType;
};

// shared function
void clearScreen();

#endif