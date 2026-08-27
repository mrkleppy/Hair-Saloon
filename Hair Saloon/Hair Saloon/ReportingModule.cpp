#include "ReportingModule.h"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <sstream>

using namespace std;

const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string BLUE = "\033[34m";
const string RESET = "\033[0m";

// Internal Function Prototypes
void companyFinancialReport(const vector<Receipt>& receipts, const vector<Item>& items);
void staffPerformanceReport(const vector<Staff>& staffs);
void inventoryReport(const vector<Item>& items);
void servicesReport(const vector<Service>& services);
void appointmentReport(const vector<Appointment>& appointments);

// Admin side Reporting Module (Matches ReportingModule.h)
void reportingHomePage(vector<Item>& items, vector<Service>& services, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Invoice>& invoices) {
    int selection = 0;

    do {
        clearScreen();
        cout << "What report are you gonna view?" << endl;
        cout << "1. Company Financial Report\n"
             << "2. Staff Performance Report\n"
             << "3. Inventory Report\n"
             << "4. Services Report\n"
             << "5. Appointment Report\n"
             << "0. Exit" << endl;
        cout << "Selection: ";

        if (!(cin >> selection)) {
            cin.clear();
            cin.ignore(10000, '\n');
            clearScreen();
            cout << "Invalid input! Please enter a valid number." << endl;
            continue;
        }
        cin.ignore();

        switch (selection) {
        case 1:
            clearScreen();
            companyFinancialReport(receipts, items);
            break;
        case 2:
            clearScreen();
            staffPerformanceReport(staffs);
            break;
        case 3:
            clearScreen();
            inventoryReport(items);
            break;
        case 4:
            clearScreen();
            servicesReport(services);
            break;
        case 5:
            clearScreen();
            appointmentReport(appointments);
            break;
        case 0:
            clearScreen();
            return;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
        }
    } while (true);
}

// 1. Financial Report (Calculates from live Receipts)
void companyFinancialReport(const vector<Receipt>& receipts, const vector<Item>& items) {
    double sales = 0.00;
    double cashSales = 0.00;
    double bankSales = 0.00;
    double appointmentRefundCosts = 0.00;

    for (const Receipt& r : receipts) {
        if (r.status != CANCELLED) {
            sales += r.totalPrice;
            if (r.paymentType == CASH) {
                cashSales += r.totalPrice;
            } else if (r.paymentType == BANK) {
                bankSales += r.totalPrice;
            }
        } else {
            appointmentRefundCosts += r.totalPrice / 2.0;
        }
    }

	// Cost of Sales is Items Restock cost
    double costOfSales = 0.00;

	for (const Item& item : items) {
		costOfSales += item.reorderPrice * item.soldCounter;
	}

    double grossProfit = sales - costOfSales;
    double netProfit = grossProfit - appointmentRefundCosts;

    clearScreen();
    cout << "Company Financial Report" << endl << endl;

    cout << fixed << setprecision(2);
    cout << left << setw(28) << "  - Cash Payments" << BLUE << "RM " << setw(10) << right << fixed << setprecision(2) << cashSales << RESET << endl;
    cout << left << setw(28) << "  - Bank Payments" << BLUE << "RM " << setw(10) << right << fixed << setprecision(2) << bankSales << RESET << endl;
    cout << left << setw(28) << "Gross Sales " << GREEN << "RM " << setw(10) << right << fixed << setprecision(2) << sales << RESET << endl;
    cout << left << setw(28) << "(-) Cost of Sales" << RED << "RM " << setw(10) << right << fixed << setprecision(2) << costOfSales << RESET << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(28) << "Gross Profit" << BLUE << "RM " << setw(10) << right << fixed << setprecision(2) << grossProfit << RESET << endl << endl;
    cout << left << setw(28) << "(-) Refund / Cancel Costs" << RED << "RM " << setw(10) << right<< fixed << setprecision(2)<< appointmentRefundCosts<< RESET<< endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(28) << "Net Profit" << BLUE << "RM " << setw(10) << right << fixed << setprecision(2) << netProfit << RESET << endl;

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

// 2. Staff Performance Report (Uses live Staff list)
void staffPerformanceReport(const vector<Staff>& staffs) {
    if (staffs.empty()) {
        cout << "No staff records available." << endl;
        cout << "\nPress enter to go back...";
        cin.get();
        clearScreen();
        return;
    }

    int highestDone = staffs[0].appointmentDone;
    for (size_t i = 1; i < staffs.size(); ++i) {
        if (staffs[i].appointmentDone > highestDone) {
            highestDone = staffs[i].appointmentDone;
        }
    }

    clearScreen();
    cout << "Staff Performance Report" << endl << endl;
    cout << left
        << setw(10) << "Staff"
        << setw(25) << "Name"
        << setw(20) << "Appointments Done"
        << "Remarks" << endl;
    cout << "--------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < staffs.size(); ++i) {
        cout << left
            << setw(10) << staffs[i].staffCode
            << setw(25) << staffs[i].user.name
            << setw(20) << staffs[i].appointmentDone;

        if (highestDone > 0 && staffs[i].appointmentDone == highestDone) {
            cout << "Best employee";
        }

        cout << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

// 3. Inventory Report (Uses live Item list & soldCounter)
void inventoryReport(const vector<Item>& items) {
    if (items.empty()) {
        cout << "No items found in inventory." << endl;
        cout << "\nPress enter to go back...";
        cin.get();
        return;
    }

    double maxProfit = items[0].soldCounter * items[0].price;

    for (size_t i = 1; i < items.size(); i++) {
        double currentProfit = items[i].soldCounter * items[i].price;
        if (currentProfit > maxProfit) {
            maxProfit = currentProfit;
        }
    }

    clearScreen();
    cout << "Inventory Report" << endl << endl;
    cout << left << setw(18) << "Item"
        << setw(10) << "Stock"
        << setw(12) << "Sold times"
        << setw(14) << "Profit (RM)"
        << "Remarks" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    cout << fixed << setprecision(2);

    for (size_t i = 0; i < items.size(); i++) {
        double profit = items[i].soldCounter * items[i].price;
        string remark = "";

        if (items[i].soldCounter > 0 && profit == maxProfit) {
            remark += "Best selling item";
        }

        if (items[i].stock <= 5) {
            if (!remark.empty()) {
                remark += " | ";
            }
            remark += "Low stock warning";
        }

        cout << left << setw(18) << items[i].name
            << setw(10) << items[i].stock
            << setw(12) << items[i].soldCounter
            << setw(14) << profit
            << remark << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

// 4. Services Report (Uses live maleCounter & femaleCounter)
void servicesReport(const vector<Service>& services) {
    if (services.empty()) {
        cout << "No services available." << endl;
        cout << "\nPress enter to go back...";
        cin.get();
        return;
    }

    int highestMaleCount = services[0].maleCounter;
    int highestFemaleCount = services[0].femaleCounter;

    for (size_t i = 1; i < services.size(); i++) {
        if (services[i].maleCounter > highestMaleCount) {
            highestMaleCount = services[i].maleCounter;
        }

        if (services[i].femaleCounter > highestFemaleCount) {
            highestFemaleCount = services[i].femaleCounter;
        }
    }

    clearScreen();
    cout << "Services Report" << endl << endl;
    cout << left << setw(18) << "Service"
        << setw(14) << "Services done"
        << setw(12) << "Male count"
        << setw(14) << "Female count"
        << "Remarks" << endl;
    cout << "-------------------------------------------------------------------" << endl;

    for (size_t i = 0; i < services.size(); i++) {
        int totalDone = services[i].maleCounter + services[i].femaleCounter;
        string remark = "";

        if (services[i].maleCounter > 0 && services[i].maleCounter == highestMaleCount) {
            remark += "Most done as male";
        }

        if (services[i].femaleCounter > 0 && services[i].femaleCounter == highestFemaleCount) {
            if (!remark.empty()) {
                remark += " | ";
            }
            remark += "Most done as female";
        }

        cout << left << setw(18) << services[i].name
            << setw(14) << totalDone
            << setw(12) << services[i].maleCounter
            << setw(14) << services[i].femaleCounter
            << remark << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

// 5. Appointment Report (Uses live Appointment list)
void appointmentReport(const vector<Appointment>& appointments) {
    int totalAppointments = static_cast<int>(appointments.size());
    int completedCount = 0;
    int cancelledCount = 0;

    struct MonthlyReport {
        int made = 0;
        int refunded = 0;
    };

    map<pair<int, int>, MonthlyReport> monthlyData;

    for (const Appointment& app : appointments) {
        if (app.status == COMPLETED) {
            completedCount++;
        }
        else if (app.status == CANCELLED) {
            cancelledCount++;
        }

        pair<int, int> monthKey = { app.date.year, app.date.month };
        monthlyData[monthKey].made++;

        if (app.status == CANCELLED) {
            monthlyData[monthKey].refunded++;
        }
    }

    int bestNet = -1;
    for (const auto& entry : monthlyData) {
        int net = entry.second.made - entry.second.refunded;
        if (net > bestNet) {
            bestNet = net;
        }
    }

    clearScreen();
    cout << "Appointment Report" << endl << endl;
    cout << "Total Appointments count: " << totalAppointments << endl;
    cout << "Completed Appointments  : " << completedCount << endl;
    cout << "Cancelled Appointments  : " << cancelledCount << endl << endl;

    cout << left
        << setw(12) << "Month/Year"
        << setw(20) << "Appointments made"
        << setw(12) << "Refunded"
        << "Remarks" << endl;
    cout << "--------------------------------------------------------------" << endl;

    for (const auto& entry : monthlyData) {
        int year = entry.first.first;
        int month = entry.first.second;
        int made = entry.second.made;
        int refunded = entry.second.refunded;
        int net = made - refunded;

        ostringstream monthYear;
        monthYear << setw(2) << setfill('0') << month
            << "/" << year;

        string remark = "";
        if (net == bestNet && bestNet >= 0) {
            remark = "Best performing month";
        }

        cout << left
            << setw(12) << monthYear.str()
            << setw(20) << made
            << setw(12) << refunded
            << remark << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}