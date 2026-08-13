#include <iostream>
#include <format>
#include "Main.h"
#include "AppointmentModule.h"

using namespace std;

Appointment findNearestAppointment(Customer customer, vector<Appointment>& customerAppointment) {
	Appointment nearestAppointment;

	nearestAppointment = customerAppointment[0];

	for (int i = 1; i < customerAppointment.size(); i++) {
		if (customerAppointment[i].date.year > nearestAppointment.date.year) {
			continue;
		}
		else if (customerAppointment[i].date.month > nearestAppointment.date.month) {
			continue;
		}
		else if (customerAppointment[i].date.day > nearestAppointment.date.day) {
			continue;
		}
		else if (customerAppointment[i].time.hour > nearestAppointment.time.hour) {
			continue;
		}
		else if (customerAppointment[i].time.minute > nearestAppointment.time.minute) {
			continue;
		}
		else {
			nearestAppointment = customerAppointment[i];
		}
	}

	return nearestAppointment;
}

void appointmentReminder(Customer customer, vector<Appointment> &appointments) {
	vector<Appointment> customerAppointment;
	string appointmentDate;
	string appointmentTime;
	
	loadCustomerPendingAppointments(customer, appointments, customerAppointment);

	if (customerAppointment.size() > 0) {
		Appointment nearestAppointment = findNearestAppointment(customer, customerAppointment);
		appointmentDate = format("{:02d}/{:02d}/{:d}",nearestAppointment.date.day, nearestAppointment.date.month, nearestAppointment.date.year);
		appointmentTime = format("{:02d}:{:02d}", nearestAppointment.time.hour, nearestAppointment.time.minute);

		cout << "You have an upcoming appointment in " << appointmentDate << " (" << appointmentTime << ")!" << endl;
	}
	else {
		cout << "You have no upcoming appointments!" << endl;
	}
}