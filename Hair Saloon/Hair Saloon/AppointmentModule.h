#pragma once

#ifndef AppointmentModule
#define AppointmentModule

#include <string>
using namespace std;

// Member side Appointment Management Module
void appointmentManager(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Service>& services);

// Staff side Appointment Management Module
void assignedAppointmentsView(Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services);
void allAppointmentsView(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments);
void completedAppointmentsView(Staff& staff, vector<Appointment>& appointments);

// Admin side Appointment Management Module
void assignAppointmentsPage(vector<Appointment>& appointments, vector<Staff>& staffs, vector<Service>& services);

// Helpers
string getServiceNameById(const string& serviceId, const vector<Service>& services);
string generateNextAppointmentNo(const vector<Appointment>& appointments);
void loadCustomerPendingAppointments(Customer& customer, vector<Appointment>& appointments, vector<Appointment>& customerPendingAppointments);

template <typename T, typename Predicate>
T* findBy(vector<T>& items, Predicate condition) {
    for (T& item : items) {
        if (condition(item)) {
            return &item;
        }
    }
    return nullptr;
}

#endif
