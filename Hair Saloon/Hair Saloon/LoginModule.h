#pragma once

#ifndef LoginModule
#define LoginModule

#include <string>
using namespace std;


int loginValidation(User user, Customer* customer, Staff* staff, vector<Customer>& customers, vector<Staff>& staffs);
void loginScreen(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs);
void registerScreen(vector<Staff>& staffs, vector<Customer>& customers);

#endif