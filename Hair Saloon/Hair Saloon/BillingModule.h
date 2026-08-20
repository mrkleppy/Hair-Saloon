#pragma once

#ifndef BillingModule
#define BillingModule

#include <string>
using namespace std;

void viewInvoiceDetailScreen(Invoice& invoice, vector<Item>& items);
void viewReceiptScreen(Customer customer, vector<Receipt>& receipts);
void viewPOSScreen(vector<Item>& items, vector<Customer>& customers, vector<Service>& services, vector<Receipt>& receipts);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Service>& services, vector<Receipt>& receipts, Appointment& appointment);
void viewInvoiceScreen(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts, vector<CartItem>& cart);

#endif
 