#pragma once

#ifndef Main
#define Main

#include <string>
using namespace std;

// Define the item structure
struct Item {
    string name;
    int pointCost;
    int stock;
};

// Define the customer structure
struct Customer {
	string name;
	string phoneNo;
	string password;
	int points;
};

// Define the staff structure
struct Staff {
	string staffCode;
	string name;
	string phoneNo;
	string password;
	double salary;
	int appointmentDone;
};

// Define the appointment structure
struct Appointment {
	string appointmentNo;
	string date;
	string time;
	string services;
	string status;
	Staff assignedStaff[3]; // Assuming a maximum of 3 staff can be assigned to an appointment
	Customer customers;
};

void clearScreen();

#endif