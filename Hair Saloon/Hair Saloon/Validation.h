#pragma once

#ifndef Validation
#define Validation

#include <string>
#include <vector>
#include "MAin.h"

bool validateUsername(User user, vector<Customer>& customers, vector<Staff>& staffs, string* message);
bool validatePhoneNo(User user, vector<Customer>& customers, vector<Staff>& staffs, string* message);
bool validatePassword(User user, string confirmPassword, string* message);
bool validateSalary(Staff staff, string* message);

#endif
