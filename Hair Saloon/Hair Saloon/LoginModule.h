#pragma once

#ifndef LoginModule
#define LoginModule

#include <string>
using namespace std;

void loginScreen(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices, vector<Service>& services);
void registerScreen(vector<Staff>& staffs, vector<Customer>& customers);

#endif