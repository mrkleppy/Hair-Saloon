#pragma once

#ifndef UserModule
#define UserModule

#include <vector>
using namespace std;

// shared functions
void memberHomePage(Customer customer, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices, vector<Service>& services);
void staffHomePage(Staff staff, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services, vector<Receipt>& receipts);
void adminHomePage(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services, vector<Receipt>& receipts, vector<Invoice>& invoices);

#endif
