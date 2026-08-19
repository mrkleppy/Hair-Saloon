#pragma once

#ifndef BillingModule
#define BillingModule

#include <string>
using namespace std;

void viewInvoiceDetailScreen(Invoice& invoice);
void viewReceiptScreen(Customer customer, vector<Receipt>& receipts);
void viewPOSScreen(vector<Item> &items);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, Appointment& appointment);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts, vector<CartItem>& cart);

#endif
 