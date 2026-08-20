#pragma once

#ifndef AppointmentReminderModule
#define AppointmentReminderModule

#include <string>
#include <vector>
using namespace std;

void appointmentReminder(Customer customer, vector<Appointment>& appoinments);
void getCurrentDateTime(Date& currentDate, Time& currentTime);
#endif