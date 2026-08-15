#include <iostream>
#include <ctime>
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

void getCurrentDateTime(Date &currentDate, Time &currentTime) {
	time_t now = time(nullptr);
	tm local_tm{};

	if (localtime_s(&local_tm, &now) != 0) {
		return;
	}

	currentDate = { local_tm.tm_mday, local_tm.tm_mon + 1, local_tm.tm_year + 1900 }; // tm_mon start from (0 -11), tm_year start from year 1900
	currentTime = { local_tm.tm_hour, local_tm.tm_min };
}

bool isEarlierDate(Date& a, Date& b) {
	if (a.year != b.year) {
		return a.year < b.year;
	}

	if (a.month != b.month) {
		return a.month < b.month;
	}

	return a.day < b.day;
}

bool isEarlierTime(Time& a, Time& b) {
	if (a.hour != b.hour) {
		return a.hour < b.hour;
	}

	return a.minute < b.minute;
}

bool isEarlierAppointment(Appointment& a, Appointment& b) {
	if (isEarlierDate(a.date, b.date)) {
		return true;
	}
	if (isEarlierDate(b.date, a.date)) {
		return false;
	}

	return isEarlierTime(a.time, b.time);
}

void compareDateTime(vector<Appointment>& customerAppointment, vector<Appointment>& upcomingAppointment) {
	Date currentDate;
	Time currentTime;
	getCurrentDateTime(currentDate, currentTime);

	//DEBUG USAGE//
	/*
	cout << "Current: "
		<< currentDate.day << "/" << currentDate.month << "/" << currentDate.year << " "
		<< currentTime.hour << ":" << currentTime.minute << "\n";
	*/

	Appointment nowAppt;
	nowAppt.date = currentDate;
	nowAppt.time = currentTime;

	upcomingAppointment.clear();
	for (Appointment appt : customerAppointment) {
		if (!isEarlierAppointment(appt, nowAppt)) {
			upcomingAppointment.push_back(appt);
		}
	}
}

Appointment findNearestAppointment(const vector<Appointment>& upcomingAppointment) {
	if (upcomingAppointment.empty()) {
		throw runtime_error("No upcoming appointments");
	}

	Appointment nearestAppointment = upcomingAppointment[0];

	for (size_t i = 1; i < upcomingAppointment.size(); ++i) {
		const Appointment& appt = upcomingAppointment[i];

		if (appt.date.year < nearestAppointment.date.year) {
			nearestAppointment = appt;
		}
		else if (appt.date.year == nearestAppointment.date.year) {
			if (appt.date.month < nearestAppointment.date.month) {
				nearestAppointment = appt;
			}
			else if (appt.date.month == nearestAppointment.date.month) {
				if (appt.date.day < nearestAppointment.date.day) {
					nearestAppointment = appt;
				}
				else if (appt.date.day == nearestAppointment.date.day) {
					if (appt.time.hour < nearestAppointment.time.hour) {
						nearestAppointment = appt;
					}
					else if (appt.time.hour == nearestAppointment.time.hour &&
						appt.time.minute < nearestAppointment.time.minute) {
						nearestAppointment = appt;
					}
				}
			}
		}
	}

	return nearestAppointment;
}

void appointmentReminder(Customer customer, vector<Appointment>& appointments) {
	vector<Appointment> upcomingAppointment, customerAppointment;
	string appointmentDate;
	string appointmentTime;

	loadCustomerPendingAppointments(customer, appointments, customerAppointment);
	compareDateTime(customerAppointment, upcomingAppointment);

	if (upcomingAppointment.size() > 0) {
		Appointment nearestAppointment = findNearestAppointment(upcomingAppointment);
		appointmentDate = format("{:02d}/{:02d}/{:d}", nearestAppointment.date.day, nearestAppointment.date.month, nearestAppointment.date.year);
		appointmentTime = format("{:02d}:{:02d}", nearestAppointment.time.hour, nearestAppointment.time.minute);

		cout << "You have an upcoming appointment in " << appointmentDate << " (" << appointmentTime << ")!" << endl;
	}
	else {
		cout << "You have no upcoming appointments!" << endl;
	}
}