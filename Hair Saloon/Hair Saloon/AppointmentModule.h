#pragma once

#ifndef AppointmentModule
#define AppointmentModule

#include <string>
using namespace std;

void appointmentManager(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments);
void assignedAppointmentsView(Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments);
void completedAppointmentsView(Staff& staff, vector<Appointment>& appointments);
void assignAppointmentsPage(vector<Appointment>& appointments);

#endif