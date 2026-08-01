#pragma once

#ifndef FileProcessing
#define FileProcessing

#include <string>
#include <vector>
#include "Main.h"

void readItemFile(vector<Item>& items);
void readStaffFile(vector<Staff>& staffs);
void readCustomerFile(vector<Customer>& customers);
void overwriteItemFile(vector<Item>& items);
void overwriteStaffFile(vector<Staff>& staffs);
void overwriteCustomerFile(vector<Customer>& customers);
void appendStaffToFile(Staff staff);
void appendCustomerToFile(Customer customer);

#endif