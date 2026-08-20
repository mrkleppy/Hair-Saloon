#pragma once

#ifndef InventoryModule
#define InventoryModule

#include <string>
using namespace std;

// Member side
void purchaseItemPage(vector<Item>& items);

// Staff and Admin side
void inventoryMaintenancePage(vector<Item>& items);

// Helpers
void restoreCartStock(vector<Item>& items, const vector<CartItem>& cart);

#endif
