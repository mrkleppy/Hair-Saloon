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

// Enum for appointment status
enum Status {
	PENDING,
	COMPLETED,
	CANCELLED,
	NOT_PICKED_UP,
	PICKED_UP
};

// Enum for payment type
enum PaymentType {
	CASH,
	BANK
};

// Helpers for enum classes
static string statusToString(Status status) {
	switch (status) {
	case PENDING:		return "Pending";
	case COMPLETED:		return "Completed";
	case CANCELLED:		return "Cancelled";
	case NOT_PICKED_UP: return "Not Picked Up";
	case PICKED_UP:		return "Picked Up";
	default:					return "Unknown";
	}
}

static Status stringToStatus(const string& statusStr) {
	if (statusStr == "Pending")				return PENDING;
	else if (statusStr == "Completed")		return COMPLETED;
	else if (statusStr == "Cancelled")		return CANCELLED;
	else if (statusStr == "Not Picked Up")	return NOT_PICKED_UP;
	else if (statusStr == "Picked Up")		return PICKED_UP;
	return PENDING; // Default case
}

static string paymentTypeToString(PaymentType paymentType) {
	switch (paymentType) {
	case CASH:	return "Cash";
	case BANK:	return "Bank";
	default:	return "Unknown";
	}
}

static PaymentType stringToPaymentType(const string& paymentTypeStr) {
	if (paymentTypeStr == "Cash")		return CASH;
	else if (paymentTypeStr == "Bank")	return BANK;
	return CASH; // Default case
}

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
	Status status;
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
	Status status;
	PaymentType paymentType;
};

// shared function
void clearScreen();

#endif