#pragma once

#ifndef UserModule
#define UserModule

#include <string>
#include <vector>
using namespace std;

// Liew Teng Lok
void memberHomePage(Customer customer, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs);
void staffHomePage(Staff staff, vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs);
void adminHomePage(vector<Item>& items, vector<Customer>& customers, vector<Staff>& staffs);

#endif
