#pragma once

#ifndef FileProcessing
#define FileProcessing

#include <string>
#include <vector>
#include "Main.h"

// shared function
void readItemFile(vector<Item>& items);
void readServiceFile(vector<Service>& services);
void readStaffFile(vector<Staff>& staffs);
void readCustomerFile(vector<Customer>& customers);
void readAppointmentFile(vector<Appointment>& appointments);
void readReceiptFile(vector<Receipt>& receipts);
void readInvoiceFile(vector<Invoice>& invoices);
void overwriteItemFile(vector<Item>& items);
void overwriteServiceFile(vector<Service>& services);
void overwriteStaffFile(vector<Staff>& staffs);
void overwriteCustomerFile(vector<Customer>& customers);
void overwriteAppointmentFile(vector<Appointment>& appointments);
void overwriteReceiptFile(vector<Receipt>& receipts);
void appendStaffToFile(Staff staff);
void appendCustomerToFile(Customer customer);
void appendAppointmentToFile(Appointment appointment);
void appendCancelledAppointmentToFile(Appointment appointment, string& reason);
void appendInvoiceToFile(Invoice invoice);
void appendReceiptToFile(Receipt receipt);

#endif