#pragma once

#ifndef InventoryModule
#define InventoryModule

#include <string>
using namespace std;

// Member side
void purchaseItemPage(Customer customer, vector<Customer>& customers, vector<Item>& items, vector<Invoice>& invoices, vector<Receipt>& receipts);

// Staff and Admin side
void inventoryMaintenancePage(vector<Item>& items);

// Helpers
void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart);

#endif
