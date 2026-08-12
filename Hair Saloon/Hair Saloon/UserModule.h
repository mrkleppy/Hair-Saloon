#pragma once

#ifndef UserModule
#define UserModule

#include <string>
#include <vector>
using namespace std;

// Liew Teng Lok
void memberHomePage(Customer customer, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments);
void staffHomePage(Staff staf, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments);
void adminHomePage(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs, vector<Appointment>& appointments);

#endif
