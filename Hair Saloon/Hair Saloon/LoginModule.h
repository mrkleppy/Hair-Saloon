#pragma once

#ifndef LoginModule
#define LoginModule

using namespace std;

// shared functions
void loginScreen(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices, vector<Service>& services);
void registerScreen(vector<Staff>& staffs, vector<Customer>& customers);

#endif