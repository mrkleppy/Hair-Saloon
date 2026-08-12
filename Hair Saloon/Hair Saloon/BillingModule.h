#pragma once

#ifndef BillingModule
#define BillingModule

#include <string>
using namespace std;

void viewInvoiceDetailScreen(Invoice& invoice);
void viewReceiptScreen();
void viewPOSScreen(vector<Item> &items);

#endif
 