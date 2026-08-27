#pragma once

#ifndef ReportingModule
#define ReportingModule

#include <string>
#include <vector>
#include "Main.h"

using namespace std;

// Aiden Lengzai
// Reporting module uses the application's central data vectors so reports reflect real data
void reportingHomePage(vector<Item>& items, vector<Service>& services, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices);

#endif
