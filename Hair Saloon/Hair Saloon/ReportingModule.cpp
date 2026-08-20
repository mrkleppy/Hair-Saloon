#include <iostream>
#include <iomanip>
#include <string>
#include "Main.h"

using namespace std;

// Aiden
// Function prototypes
void companyFinancialReport();
void staffPerformanceReport();
void inventoryReport();
void servicesReport();
void appointmentReport();

// Admin side Reporting Module
void reportingHomePage() {
    int selection = 0;
    clearScreen();

    do {
        cout << "What report are you gonna view?" << endl;
        cout << "1. Company Financial Report\n2. Staff Performance Report\n3. Inventory Report\
            \n4. Services Report\n5. Appointment Report\n0. Exit" << endl;
        cout << "Selection: ";
        cin >> selection;
        cin.ignore();

        switch (selection) {
        case 1: // financial report
            clearScreen();
            companyFinancialReport();
            break;
        case 2: // performance report
            clearScreen();
            staffPerformanceReport();
            break;
        case 3: // inventory report
            clearScreen();
            inventoryReport();
            break;
        case 4: // services report
            clearScreen();
            servicesReport();
            break;
        case 5: // appointment report
            clearScreen();
            appointmentReport();
            break;
        case 0: // exit
            clearScreen();
            return;
        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, 2, 3, 4, or 5!" << endl;
        }
    } while (true);
}

void companyFinancialReport() {
    // Read sales, cost of sales and appointment refund costs
    double sales = 0.00;
    double costOfSales = 0.00;
    double appointmentRefundCosts = 0.00;

    double grossProfit = sales - costOfSales;
    double netProfit = grossProfit - appointmentRefundCosts;

    clearScreen();
    cout << "Company Financial Report" << endl << endl;

    cout << fixed << setprecision(2);
    cout << left << setw(28) << "Sales" << "RM " << sales << endl;
    cout << left << setw(28) << "(-) Cost of Sales" << "RM " << costOfSales << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(28) << "Gross Profit" << "RM " << grossProfit << endl << endl;
    cout << left << setw(28) << "(-) Appointment Refund Costs" << "RM " << appointmentRefundCosts << endl;
    cout << "----------------------------------------" << endl;
    cout << left << setw(28) << "Net Profit" << "RM " << netProfit << endl;

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

void staffPerformanceReport() {
    // Read staff list, appointments done and POS sales made
    Staff staffList[5] = {
        { {"John Doe", "password1", "0123456789"}, "S001", 3000.00, 10 },
        { {"Jane Smith", "password2", "0987654321"}, "S002", 3200.00, 15 },
        { {"Alice Johnson", "password3", "0112233445"}, "S003", 2800.00, 8 },
        { {"Bob Brown", "password4", "0109876543"}, "S004", 3500.00, 20 },
        { {"Charlie Davis", "password5", "0134567890"}, "S005", 3100.00, 12 }
    };
    double posSalesMade[5] = { 340.50, 210.00, 180.90, 500.00, 275.30 };
    int totalStaff = sizeof(staffList) / sizeof(Staff);

    // Judge best employee based on POS sales made
    int bestIndex = 0;
    for (int i = 1; i < totalStaff; i++) {
        if (posSalesMade[i] > posSalesMade[bestIndex]) {
            bestIndex = i;
        }
    }

    clearScreen();
    cout << "Staff Performance Report" << endl << endl;
    cout << left << setw(10) << "Staff" << setw(20) << "Name" << setw(18) << "Appointments done" \
        << setw(16) << "POS Sales (RM)" << "Remarks" << endl;

    cout << fixed << setprecision(2);
    for (int i = 0; i < totalStaff; i++) {
        cout << left << setw(10) << staffList[i].staffCode
            << setw(20) << staffList[i].user.name
            << setw(18) << staffList[i].appointmentDone
            << setw(16) << posSalesMade[i]
            << (i == bestIndex ? "Best employee" : "") << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

void inventoryReport() {
    // Read item list, reorder times, sold times and profit
    string itemName[8] = { "Shampoo", "Dry shampoo", "Conditioner", "Hair dye", "Hair gel", "Hair spray", "Styling cream", "Hair scalp" };
    int reorderTimes[8] = { 2, 1, 0, 3, 1, 0, 1, 0 };
    int soldTimes[8] = { 15, 8, 10, 5, 12, 7, 6, 3 };
    double profit[8] = { 320.50, 180.00, 210.90, 90.00, 250.30, 130.00, 145.60, 60.00 };
    int totalItems = 8;

    // Judge best selling item based on profit
    int bestIndex = 0;
    for (int i = 1; i < totalItems; i++) {
        if (profit[i] > profit[bestIndex]) {
            bestIndex = i;
        }
    }

    clearScreen();
    cout << "Inventory Report" << endl << endl;
    cout << left << setw(16) << "Item" << setw(14) << "Reorder times" << setw(12) << "Sold times" \
        << setw(14) << "Profit (RM)" << "Remarks" << endl;

    cout << fixed << setprecision(2);
    for (int i = 0; i < totalItems; i++) {
        // Need sales - reorder logic for low stock flag
        cout << left << setw(16) << itemName[i]
            << setw(14) << reorderTimes[i]
            << setw(12) << soldTimes[i]
            << setw(14) << profit[i]
            << (i == bestIndex ? "Best selling item" : "") << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

void servicesReport() {
    // Read services done, male count and female count
    string serviceName[4] = { "Hair Cut", "Hair Coloring", "Hair Treatment", "Styling" };
    int servicesDone[4] = { 20, 12, 9, 15 };
    int maleCount[4] = { 14, 5, 4, 8 };
    int femaleCount[4] = { 6, 7, 5, 7 };
    int totalServices = 4;

    // Judge most done service for male and female
    int mostMaleIndex = 0, mostFemaleIndex = 0;
    for (int i = 1; i < totalServices; i++) {
        if (maleCount[i] > maleCount[mostMaleIndex]) mostMaleIndex = i;
        if (femaleCount[i] > femaleCount[mostFemaleIndex]) mostFemaleIndex = i;
    }

    clearScreen();
    cout << "Services Report" << endl << endl;
    cout << left << setw(16) << "Service" << setw(14) << "Services done" << setw(12) << "Male count" \
        << setw(14) << "Female count" << "Remarks" << endl;

    for (int i = 0; i < totalServices; i++) {
        string remark = "";
        if (i == mostMaleIndex) remark = "Most done as male";
        else if (i == mostFemaleIndex) remark = "Most done as female";

        cout << left << setw(16) << serviceName[i]
            << setw(14) << servicesDone[i]
            << setw(12) << maleCount[i]
            << setw(14) << femaleCount[i]
            << remark << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}

void appointmentReport() {
    // Read appointments made and refunded, grouped by month, from file (TODO)
    // Placeholder for now
    string monthYear[2] = { "Jan '26", "Feb '26" };
    int appointmentsMade[2] = { 30, 42 };
    int refunded[2] = { 2, 5 };
    int totalMonths = 2;
    int totalAppointments = 0;

    for (int i = 0; i < totalMonths; i++) {
        totalAppointments += appointmentsMade[i];
    }

    // Judge best performing month based on (made - refunded), highest value
    int bestIndex = 0;
    int bestValue = appointmentsMade[0] - refunded[0];
    for (int i = 1; i < totalMonths; i++) {
        int value = appointmentsMade[i] - refunded[i];
        if (value > bestValue) {
            bestValue = value;
            bestIndex = i;
        }
    }

    clearScreen();
    cout << "Appointment Report" << endl << endl;
    cout << "Total Appointments count: " << totalAppointments << endl << endl;
    cout << left << setw(14) << "Month/Year" << setw(18) << "Appointments made" << setw(12) << "Refunded" \
        << "Remarks" << endl;

    for (int i = 0; i < totalMonths; i++) {
        cout << left << setw(14) << monthYear[i]
            << setw(18) << appointmentsMade[i]
            << setw(12) << refunded[i]
            << (i == bestIndex ? "Best performing month" : "") << endl;
    }

    cout << "\nPress enter to go back...";
    cin.get();
    clearScreen();
}