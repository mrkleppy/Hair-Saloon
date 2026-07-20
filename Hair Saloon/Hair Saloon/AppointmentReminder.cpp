#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"

using namespace std;

void appointmentReminder(Customer customer) {
	// Read file to get appointment details based on customer (TODO)
	// Compare the date and time with system's date and time (TODO)
	// placeholder for now
	bool appointmentReminder = true;
	string date = "20/12/2026", time = "12:00";

	if (appointmentReminder) {
		cout << "You have an upcoming appointment in " << date << " (" << time << ")!" << endl;
	}
	else {
		cout << "You have no upcoming appointments!" << endl;
	}
}