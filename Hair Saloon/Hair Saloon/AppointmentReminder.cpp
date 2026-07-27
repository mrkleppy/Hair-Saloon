#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include "Main.h"

using namespace std;

void appointmentReminder(Customer customer) {
	// Read file to get appointment details based on customer (TODO)
	bool hasAppointment = true;
	string date = "12/12/2026", time = "15:00";

	/*vector<Appointment> appointments = readAppointmentFile();
	for (Appointment appointment : appointments) {
		if (customer.user.name == appointment.customer.user.name) {
			date = appointment.date;
			time = appointment.time;
			hasAppointment = true;
			break;
		}
	}*/	

	if (hasAppointment) {
		cout << "You have an upcoming appointment in " << date << " (" << time << ")!" << endl;
	}
	else {
		cout << "You have no upcoming appointments!" << endl;
	}
}