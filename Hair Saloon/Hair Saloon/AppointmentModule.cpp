#include <iostream>
#include <iomanip>
#include <string>
#include <regex>
#include <sstream>
#include <ctime>
#include <vector>
#include <cctype>
#include "Main.h"
#include "FileProcessing.h"
#include "BillingModule.h"
#include "AppointmentModule.h"
#include "AppointmentReminder.h"

using namespace std;

// ==============================================================================================
// Member side Appointment Management Module
void bookAppointment(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Service>& services);
void cancelAppointment(Customer customer, vector<Appointment>& appointments);

// ==============================================================================================
// Staff side Appointment Management Module
void assignedAppointmentsView(Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services);
void appointmentStatusManager(Appointment* appointmentPtr, Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services);
void allAppointmentsView(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments);

// ==============================================================================================
// Admin side Appointment Management Module
void assignAppointmentsPage(vector<Appointment>& appointments, vector<Staff>& staffs, vector<Service>& services);
void assignAppointmentToStaff(Appointment* appointmentPtr, vector<Appointment>& appointments, vector<Staff>& staffs, vector<Service>& services);

// ==============================================================================================
// Helpers
string generateNextAppointmentNo(const vector<Appointment>& appointments);
Appointment* findAppointment(vector<Appointment>& appointments, const string& appointmentNo);
string getServiceNameById(const string& serviceId, const vector<Service>& services);
bool parseDate(const string& input, Date& date);
bool parseTime(const string& input, Time& time);
bool isValidDate(const Date& date);
bool isValidTime(const Time& time);
bool isFutureAppointment(const Date& appointmentDate);
bool isStaffAvailable(const string& staffId, const Appointment& targetAppointment, vector<Appointment> appointments);
int getRemainingSlotCapacity(const Date& date, const Time& time, const vector<Appointment>& appointments);
int getAssignedPersons(const Appointment& appointment);
void loadCustomerPendingAppointments(Customer& customer, vector<Appointment>& appointments, vector<Appointment>& customerPendingAppointments);
void loadStaffAssignedAppointments(Staff& staff, vector<Appointment>& appointments, vector<Appointment>& staffAssignedAppointments);
void loadStaffCompletedAppointments(Staff& staff, vector<Appointment>& appointments, vector<Appointment>& staffCompletedAppointments);
void loadUnassignedAppointments(vector<Appointment>& appointments, vector<Appointment>& unassignedAppointments);
void printPendingAppointments(const vector<Appointment> loadedAppointments);

// ==============================================================================================
// Member side Appointment Management Module
void appointmentManager(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Service>& services) {
    char selection;
    string input;
    clearScreen();

    do {
        cout << "Reminder: 50% would be charged in case of cancelled appointments." << endl;
        cout << "1. Book an Appointment" << endl;
        cout << "2. Remove an Appointment" << endl;
        cout << "0. Exit" << endl;
        cout << "Selection: ";

        getline(cin, input);

        if (input.size() > 1 || input.empty()) {
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
            continue;
        }

        selection = input[0];

        switch(selection) {
        case '1':
            clearScreen();
            bookAppointment(customer, customers, appointments, receipts, services);
            break;

        case '2':
            clearScreen();
            cancelAppointment(customer, appointments);
            break;

        case '0':
            clearScreen();
            return;

        default:
            clearScreen();
            cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
        }
    } while(true);
}

// Book appointment
void bookAppointment(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments, vector<Receipt>& receipts, vector<Service>& services) {
    Appointment appointment{};
    appointment.appointmentNo = generateNextAppointmentNo(appointments);
    appointment.customerName = customer.user.name;
    appointment.serviceCount = 0;
    appointment.staffCount = 0;
    appointment.totalPersons = 0;
    appointment.total = 0;
    appointment.status = "Pending";

    string dateStr, timeSlotStr, input;
    clearScreen();

    do {
        cout << "Reminder: A maximum of " << MAX_PERSONS_PER_SLOT << " persons can be booked per appointment slot." << endl;
        cout << "Enter 'q' to quit." << endl;
        cout << "Enter date to book appointment (DD/MM/YYYY): ";
        getline(cin, dateStr);

        // Validations for date
        if (dateStr == "q" || dateStr == "Q") {
            clearScreen();
            return;
        }
        
		if (!parseDate(dateStr, appointment.date) || !isValidDate(appointment.date) || dateStr.empty()) {
			clearScreen();
			cout << "Invalid date format or invalid date! Please enter a valid date in DD/MM/YYYY format." << endl;
			continue;
		}

		if (!isFutureAppointment(appointment.date)) {
			clearScreen();
			cout << "Invalid date! Please enter a date that is within tomorrow and a week from now." << endl;
			continue;
		}

        do {
            cout << "\nPlease book within a slot of 9am to 5pm." << endl <<
                "Please enter 00 or 30 for minutes and in 24-Hours format (13:30)" << endl;
            cout << "Enter time slot (HH:MM): ";
            getline(cin, timeSlotStr);

            // Validations for time slot
            if (timeSlotStr == "q" || timeSlotStr == "Q") {
                clearScreen();
                return;

            }

            if (!parseTime(timeSlotStr, appointment.time) || !isValidTime(appointment.time) || timeSlotStr.empty()) {
                clearScreen();
                cout << "Invalid time slot! Please enter a valid time in HH:MM format within 09:00 and 17:00, using 00 or 30 minutes." << endl;
                continue;
            }

            if (timeSlotStr.size() > 5) {
                clearScreen();
                cout << "Please enter only Hours and Minutes" << endl;
                continue;
            }

            // Checks if the appointment slot is fully booked or not
            if (getRemainingSlotCapacity(appointment.date, appointment.time, appointments) <= 0) {
                clearScreen();
                cout << "The selected time slot is fully booked! Please choose another time slot.\n" << endl;
                continue;
            }

            break;
        } while (true);

        break;
    } while(true);

    // After choosing date and time slot, user now choose person count.
    do {
        int remaining = getRemainingSlotCapacity(appointment.date, appointment.time, appointments);

        cout << "\nPerson(s) (1 to " << remaining << "): ";
        getline(cin, input);

        try {
            size_t pos;
            appointment.totalPersons = stoi(input, &pos);

            if (appointment.totalPersons < 1 || appointment.totalPersons > remaining) {
                clearScreen();
                cout << "Invalid number of persons! Please enter a number between 1 and " << remaining << "!" << endl;
                continue;
            }

            break;
        }
        catch (const invalid_argument&) {
            clearScreen();
            cout << "Invalid input! Please enter a whole number only!" << endl
                << "Within 1 to " << remaining;
        }
        catch (const out_of_range&) {
            clearScreen();
            cout << "Input is too large! Please enter a valid number between 1 to " << remaining << endl;
        }
	} while (true);

    clearScreen();

	// After choosing date, time slot, and person count, user now choose service type.
    do {
        char selection, gender, confirmation;
        int persons;
        const Service* serviceChosen = nullptr;

        int assignedPersons = getAssignedPersons(appointment);
        int remainingPersons = appointment.totalPersons - assignedPersons;

        cout << "Appointment Date: "
            << setw(2) << setfill('0') << appointment.date.day << "/"
            << setw(2) << setfill('0') << appointment.date.month << "/"
            << appointment.date.year;

        cout << "\tAppointment Time: "
            << setw(2) << setfill('0') << appointment.time.hour << ":"
            << setw(2) << setfill('0') << appointment.time.minute << endl;

        cout << "Total Persons: " << appointment.totalPersons;
        cout << "\t\tAssigned Persons: " << assignedPersons << endl;
        cout << "Remaining Persons: " << remainingPersons;
        cout << "\t\tCurrent Services Added: " << appointment.serviceCount << "/" << MAX_SERVICES_PER_APPOINTMENT << endl;

        if (appointment.serviceCount > 0) {
            cout << "\nCurrent Services: " << endl;
            for (int i = 0; i < appointment.serviceCount; i++) {
                cout << i + 1 << ". "
                    << getServiceNameById(appointment.bookedServices[i].serviceId, services)
                    << " (" << appointment.bookedServices[i].gender << ") x "
                    << appointment.bookedServices[i].persons
                    << " - RM " << fixed << setprecision(2) << appointment.bookedServices[i].subtotal << endl;
            }

            cout << "\nCurrent Total: RM " << fixed << setprecision(2) << appointment.total << endl;
        }

        cout << setfill(' ') << "\nQ to exit" << endl;
        cout << "\nChoose Service Type:" << endl;
        cout << left << setw(5) << "No."
            << setw(22) << "Service"
            << setw(15) << "Male (RM)"
            << setw(15) << "Female (RM)" << endl;

        for (int i = 0; i < services.size(); i++) {
            cout << left << setw(5) << (i + 1)
                << setw(22) << services[i].name
                << setw(15) << fixed << setprecision(2) << services[i].malePrice
                << setw(15) << fixed << setprecision(2) << services[i].femalePrice
                << endl;
        }

        cout << "\nEnter 'c' to complete your appointments!" << endl;

        cout << "Selection: ";
        cin >> selection; 
        cin.ignore();

        if (selection == 'q' || selection == 'Q') {
            clearScreen();
            return;
        }

		if (selection == 'c' || selection == 'C') {
			if (appointment.serviceCount == 0) {
				clearScreen();
				cout << "You have not added any services yet!" << endl;
				continue;
			}

            if (getAssignedPersons(appointment) != appointment.totalPersons) {
                clearScreen();
                cout << "You must assign all " << appointment.totalPersons
                    << " person(s) to services before completing the appointment!" << endl << endl;
                continue;
            }

			clearScreen();
            viewInvoiceScreen(customer, customers, appointments, services, receipts, appointment);
            break;
		}

        if ((getAssignedPersons(appointment) == appointment.totalPersons)) {
            clearScreen();
            cout << "You have assigned all " << appointment.totalPersons << " person(s) to services!" << endl;
            cout << "Please complete your appointment by entering 'c'!" << endl << endl;
            continue;
        }

        if (selection < '1' || selection > '4') {
            clearScreen();
            cout << "Invalid input. Please enter 1, 2, 3, 4, or c to complete transaction!\n";
            continue;
        }

        if (appointment.serviceCount >= MAX_SERVICES_PER_APPOINTMENT) {
            clearScreen();
            cout << "You have reached the maximum number of services (" << MAX_SERVICES_PER_APPOINTMENT << ") for this appointment!" << endl;
            continue;
        }

        // Points to the service that is selected
		serviceChosen = &services[selection - '1'];

        do {
            cout << "Gender (M/F): ";
            cin >> gender;
            cin.ignore();

            gender = toupper(gender);
            
            if (gender != 'M' && gender != 'F') {
                cout << "Invalid gender! Please enter M or F!" << endl << endl;
                continue;
            }

            break;
        } while (true);

        do {
            if (remainingPersons == 1) {
                persons = 1;
                cout << "Only 1 person remaining, automatically assigning 1 person to this service." << endl;
            }
            else {
                cout << "Person(s) (Not more than " << remainingPersons << "): ";
                cin >> persons;
                cin.ignore();

                if (persons < 1 || persons > remainingPersons) {
                    cout << "Invalid number of persons! Please enter a number between 1 and " << remainingPersons << "!" << endl << endl;
                    continue;
                }
            }

            break;
        } while (true);

        double servicePrice = (gender == 'M')? serviceChosen->malePrice : serviceChosen->femalePrice;
        double subtotal = servicePrice * persons;

		do {
			cout << "Confirm adding " << serviceChosen->name
				<< " (" << gender << ") for " << persons << " person(s)?"
				<< "\nSubtotal: RM " << fixed << setprecision(2) << subtotal
				<< "\n(Y/N): ";

			cin >> confirmation;
			cin.ignore();

			if (toupper(confirmation) == 'N') {
				clearScreen();
				cout << "Service not added.\n" << endl;
				break;
			}
			else if (toupper(confirmation) == 'Y') {
				appointment.bookedServices[appointment.serviceCount].serviceId = serviceChosen->serviceId;
				appointment.bookedServices[appointment.serviceCount].gender = gender;
				appointment.bookedServices[appointment.serviceCount].persons = persons;
				appointment.bookedServices[appointment.serviceCount].subtotal = subtotal;
				appointment.serviceCount++;
				appointment.total += subtotal;

				clearScreen();
				cout << serviceChosen->name << " added successfully!" << endl << endl;
				break;
			}
			else {
				clearScreen();
				cout << "Invalid input. Please enter Y or N only!" << endl << endl;
				continue;
			}
		} while (true);
    } while(true);
}

// Remove Appointment
void cancelAppointment(Customer customer, vector<Appointment>& appointments) {
    clearScreen();

    char confirmation;
    string reason = "Customer cancelled appointment.";
    vector<Appointment> customerPendingAppointments{};

    do {
        string appointmentNo;
        loadCustomerPendingAppointments(customer, appointments, customerPendingAppointments);

        if (customerPendingAppointments.empty()) {
            cout << "No pending appointments found." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        cout << "Appointments: \n";
        printPendingAppointments(customerPendingAppointments);
  
        cout << "\nEnter 'q' to exit." << endl;
        cout << "Select Appointment No. to remove appointment: ";
        getline(cin, appointmentNo);

        if (appointmentNo == "q" || appointmentNo == "Q") {
            clearScreen();
            return;
        }
        
        Appointment* appointmentPtr = findAppointment(customerPendingAppointments, appointmentNo);

        if (appointmentPtr == nullptr) {
            cout << "Invalid appointment number." << endl;
            continue;
        }
        else {
            do {
                cout << "Are you sure you want to cancel this appointment? " << endl;
                cout << "(Y/N): ";
                cin >> confirmation;
                cin.ignore();

                if (toupper(confirmation) == 'Y') {
					Appointment* actualPtr = findAppointment(appointments, appointmentNo); // Find the actual appointment in the main appointments vector
                    if (actualPtr != nullptr) {
                        actualPtr->status = "Cancelled";
                        overwriteAppointmentFile(appointments);
                        appendCancelledAppointmentToFile(*actualPtr, reason);
                    }
                    
                    cout << "Appointment cancelled successfully." << endl;
                    cout << "Press enter to continue...";
                    cin.get();

                    clearScreen();
                    return;
                }
                else if (toupper(confirmation) == 'N') {
                    cout << "Cancellation stopped." << endl;
                    break;
                }
                else {
                    cout << "Invalid input! Please enter Y or N!" << endl;
                }
            } while (true);
        }
    } while (true);
}

// ==============================================================================================
// Staff side Appointment Management Module
// View assigned Appointments
void assignedAppointmentsView(Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services) {
    clearScreen();
    vector<Appointment> staffAssignedAppointments{};

    do {
        string appointmentNo;
        loadStaffAssignedAppointments(staff, appointments, staffAssignedAppointments);

        if (staffAssignedAppointments.empty()) {
            cout << "No assigned appointments found." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        cout << "Upcoming Appointments for " << staff.user.name << endl;
        printPendingAppointments(staffAssignedAppointments);

        cout << "\n(n = next page, p = previous page, q = quit" << endl <<
            "Enter Appointment No. to manage status: ";

        getline(cin, appointmentNo);

		if (appointmentNo == "q" || appointmentNo == "Q") {
			clearScreen();
            return;
		}

        Appointment* appointmentPtr = findAppointment(staffAssignedAppointments, appointmentNo);

        if (appointmentPtr == nullptr) {
            clearScreen();
            cout << "Invalid appointment number" << endl;
            continue;
        }
        else {
            appointmentStatusManager(appointmentPtr, staff, staffs, appointments, services);
        }
    } while(true);
}   

// Manage Appointment Status
void appointmentStatusManager(Appointment* appointmentPtr, Staff& staff, vector<Staff>& staffs, vector<Appointment>& appointments, vector<Service>& services) {
    string reason;
    char selection, confirmation;
    clearScreen();

	do {
        cout << "Appointment No. " << appointmentPtr->appointmentNo
            << " is being managed by ";

        for (int i = 0; i < appointmentPtr->staffCount; i++) {
            for (const Staff& assignedStaff : staffs) {
                if (appointmentPtr->assignedStaffIds[i] == assignedStaff.staffCode) {
                    if (i == (appointmentPtr->staffCount - 1)) {
                        cout << "and " << assignedStaff.user.name;
                    }
                    else {
                        cout << assignedStaff.user.name << ", ";
                    }
                }
            }
        }

        cout << left << setw(7) << "\n\nNo."
            << setw(18) << "Services"
            << setw(10) << "Gender"
            << setw(13) << "Persons(s)"
            << endl;

        for (int i = 0; i < appointmentPtr->serviceCount; i++) {
            cout << left << setw(5) << (i + 1)
                << setw(18) << getServiceNameById(appointmentPtr->bookedServices[i].serviceId, services)
                << setw(10) << (appointmentPtr->bookedServices[i].gender == 'M' ? "Male" : "Female")
                << setw(13) << appointmentPtr->bookedServices[i].persons
                << endl;
        }
        
        cout << "\nUpdate Appointment Status:" << endl;
        cout << "1. Mark as Completed" << endl;
		cout << "2. Cancel Appointment" << endl;
		cout << "0. Exit" << endl;
		cout << "Selection: ";
		
        cin >> selection;
		cin.ignore();
		
        switch (selection) {
		case '1':
			clearScreen();

            do {
                cout << "Appointment No. " << appointmentPtr->appointmentNo << " is done?" << endl <<
                    "Confirm (Y/N): ";

				cin >> confirmation;
				cin.ignore();

				if (toupper(confirmation) == 'Y') {
					appointmentPtr->status = "Completed";

                    for (int i = 0; i < appointmentPtr->staffCount; i++) {
                        for (Staff& assignedStaff : staffs) {
                            if (appointmentPtr->assignedStaffIds[i] == assignedStaff.staffCode) {
                                assignedStaff.appointmentDone++;
                                break;
                            }
                        }
                    }

					overwriteAppointmentFile(appointments);
                    overwriteStaffFile(staffs);

					clearScreen();
					cout << "Appointment No. " << appointmentPtr->appointmentNo << " has been marked as completed!" << endl;
					return;
				}
				else if (toupper(confirmation) == 'N') {
					clearScreen();
					return;

				}
				else {
					clearScreen();
					cout << "Invalid input! Please enter Y or N!" << endl;
				}

            } while(true);

			break;

		case '2':
			clearScreen();
			
            do {
                cout << "Q to exit";
                cout << "Why is " << appointmentPtr->appointmentNo << " being cancelled?" << endl <<
                    "Reason: ";

                getline(cin, reason);

                if (reason == "Q" || reason == "q") {
                    clearScreen();
                    return;
                }

                cout << "\nAppointment No. " << appointmentPtr->appointmentNo << " is being cancelled for the following reason: " << endl <<
                    reason << endl <<
                    "Confirm (Y/N): ";

				cin >> confirmation;
				cin.ignore();

				if (toupper(confirmation) == 'Y') {
					appointmentPtr->status = "Cancelled";
					overwriteAppointmentFile(appointments);

					clearScreen();
					cout << "Appointment No. " << appointmentPtr->appointmentNo << " has been cancelled!" << endl;
					return;
				}
				else if (toupper(confirmation) == 'N') {
					clearScreen();
					return;
				}
				else {
					clearScreen();
					cout << "Invalid input! Please enter Y or N!" << endl;
				}
            } while (true);
            break;

		case '0':
			clearScreen();
			return;

		default:
			clearScreen();
			cout << "Invalid input! Please enter 0, 1, or 2!" << endl;
		}
        break;
	} while (true);
}

// View all appointments (under Customer Maintenance)
void allAppointmentsView(Customer customer, vector<Customer>& customers, vector<Appointment>& appointments) {
    clearScreen();
    int indexFound = 0;
    bool found = false;
    vector<Appointment> customerPendingAppointments{};

    do {
        for (int i = 0; i < customers.size(); i++) {
            if (customer.user.phoneNo == customers[i].user.phoneNo) {
                customer.user.name = customers[i].user.name;
                customer.user.password = customers[i].user.password;
                customer.points = customers[i].points;
                indexFound = i;
                found = true;
                break;
            }
        }

        if (!found) {
            clearScreen();
            cout << "Phone number not found!" << endl;
            return;
        }

        loadCustomerPendingAppointments(customer, appointments, customerPendingAppointments);

        if (customerPendingAppointments.empty()) {
            cout << "No pending appointments found." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        cout << "Customer Name: " << customer.user.name << endl;
        cout << "Pending Appointments" << endl;
        printPendingAppointments(customerPendingAppointments);

        cout << "\n\nPress any key to continue...";
        cin.ignore();
        break;

    } while (true);
}

// View all completed past Appointments
void completedAppointmentsView(Staff& staff, vector<Appointment>& appointments) {
	clearScreen();
    int currentPage = 1;
    vector<Appointment> staffCompletedAppointments{};

	do {
        loadStaffCompletedAppointments(staff, appointments, staffCompletedAppointments);

        if (staffCompletedAppointments.empty()) {
            cout << "No completed appointments found." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        int totalAppointments = int(staffCompletedAppointments.size());
        int totalPages = int(ceil(static_cast<double>(totalAppointments) / MAX_APPOINTMENTS_PER_PAGE));

		cout << "Completed Appointments for " << staff.user.name << endl << endl;
		cout << left << setw(20) << "Appointment No."
            << setw(24) << "Customer Name"
            << setw(14) << "Date"
            << setw(9) << "Time" 
            << setw(12) << "Person(s)" 
            << setw(14) << "Total (RM)" << endl;

        int start = (currentPage - 1) * MAX_APPOINTMENTS_PER_PAGE;
        const Appointment* appointmentPtr = &staffCompletedAppointments[start];
		
        for (int i = 0; i < MAX_APPOINTMENTS_PER_PAGE && (start + i) < totalAppointments; i++) {
            cout << left << setw(20) << appointmentPtr->appointmentNo
                << setw(24) << appointmentPtr->customerName
                << setw(2) << setfill('0') << appointmentPtr->date.day << "/"
                << setw(2) << appointmentPtr->date.month << "/"
                << setw(8) << appointmentPtr->date.year << setfill(' ')
                << setw(2) << setfill('0') << appointmentPtr->time.hour << ":"
                << setw(6) << appointmentPtr->time.minute
                << setw(12) << appointmentPtr->totalPersons
                << setw(3) << "RM " << setw(11) << fixed << setprecision(2) << appointmentPtr->total << endl;

            appointmentPtr++;
        }

        cout << "\nPage " << currentPage << "/" << totalPages << endl;
		cout << "\n(n = next page, p = previous page, q = quit)" << endl <<
			"Enter 'q' to exit." << endl;
        cout << "Selection: ";

        char selection;
        cin >> selection;
        cin.ignore();

        selection = tolower(selection);

        switch (selection) {
        case 'n':
            if (currentPage < totalPages) {
                currentPage++;
                clearScreen();
            }
            else {
                clearScreen();
                cout << "You are already on the last page!";
            }
            break;
        case 'p':
            if (currentPage > 1) {
                currentPage--;
                clearScreen();
            }
            else {
                clearScreen();
                cout << "You are already on the first page!";
            }
            break;
        case 'q':
            clearScreen();
            return;
        default:
            clearScreen();
            cout << "Invalid input! Please enter n, p, or q!" << endl;
        }
	} while (true);
}

// ===============================================================================================
// Admin side Appointment Management Module
void assignAppointmentsPage(vector<Appointment>& appointments, vector<Staff>& staffs, vector<Service>& services) {
    clearScreen();
    int currentPage = 1;
    vector<Appointment> unassignedAppointments{};

    do {
        string appointmentNo;
        loadUnassignedAppointments(appointments, unassignedAppointments);

        if (unassignedAppointments.empty()) {
            cout << "No unassigned appointments found." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        int totalAppointments = int(unassignedAppointments.size());
        int totalPages = int(ceil(static_cast<double>(totalAppointments) / MAX_APPOINTMENTS_PER_PAGE));

        cout << "Appointment Assignment: " << endl 
            << left << setw(20) << "Appointment No."
            << setw(24) << "Customer Name"
            << setw(14) << "Date"
            << setw(9) << "Time"
            << setw(12) << "Person(s)"
            << setw(17) << "Staff Needed" << endl;

        int start = (currentPage - 1) * MAX_APPOINTMENTS_PER_PAGE;
        const Appointment* appointmentDisplayPtr = &unassignedAppointments[start];

        for (int i = 0; i < MAX_APPOINTMENTS_PER_PAGE && (start + i) < totalAppointments; i++) {
            cout << left << setw(20) << appointmentDisplayPtr->appointmentNo
                << setw(24) << appointmentDisplayPtr->customerName
                << setw(2) << setfill('0') << appointmentDisplayPtr->date.day << "/"
                << right << setw(2) << appointmentDisplayPtr->date.month << "/"
                << left << setw(8) << setfill(' ') << appointmentDisplayPtr->date.year
                << setw(2) << setfill('0') << appointmentDisplayPtr->time.hour << ":"
                << setw(2) << appointmentDisplayPtr->time.minute << setw(4) << setfill(' ') << " "
                << setw(12) << appointmentDisplayPtr->totalPersons
                << setw(17) << (appointmentDisplayPtr->totalPersons - appointmentDisplayPtr->staffCount) << endl;

            appointmentDisplayPtr++;
        }

        cout << "\nPage " << currentPage << "/" << totalPages << endl;
        cout << "\n(n = next page, p = previous page, q = quit)" << endl <<
            "Enter 'q' to exit." << endl;
        cout << "Select Appointment No. to assign: ";
		getline(cin, appointmentNo);

        if (appointmentNo.empty()) {
            clearScreen();
            cout << "Invalid input!" << endl;
            continue;
        }

        if (tolower(appointmentNo[0]) == 'n') {
            if (currentPage < totalPages) {
                currentPage++;
                clearScreen();
            }
            else {
                clearScreen();
                cout << "You are already on the last page!";
            }
        }
        else if (tolower(appointmentNo[0]) == 'p') {
            if (currentPage > 1) {
                currentPage--;
                clearScreen();
            }
            else {
                clearScreen();
                cout << "You are already on the first page!";
            }
        }
        else if (tolower(appointmentNo[0]) == 'q') {
            clearScreen();
            return;
        }

        Appointment* appointmentPtr = findAppointment(appointments, appointmentNo);

        if (appointmentPtr == nullptr) {
            clearScreen();
            cout << "Invalid appointment number" << endl;
            continue;
        }
        else {
            assignAppointmentToStaff(appointmentPtr, appointments, staffs, services);
        }
    } while(true);
}

// Assign Appointment to Staff
void assignAppointmentToStaff(Appointment* appointmentPtr, vector<Appointment>& appointments, vector<Staff>& staffs, vector<Service>& services) {
	clearScreen();
    int staffCount = 0; // Initialise

	do {
        cout << "Assigning Appointment No. " << appointmentPtr->appointmentNo
            << "\t(" << appointmentPtr->totalPersons << " staff needed!)" << endl
            << "Date: " << setw(2) << setfill('0') << appointmentPtr->date.day << "/" << right << setw(2) << appointmentPtr->date.month << "/" << left << setw(4) << appointmentPtr->date.year << setfill(' ') << "\t\t\t"
            << "Time: " << setw(2) << setfill('0') << appointmentPtr->time.hour << ":" << setw(2) << appointmentPtr->time.minute << setfill(' ') << endl;

        cout << left << setw(7) << "\nNo."
            << setw(18) << "Services"
            << setw(10) << "Gender"
            << setw(13) << "Persons(s)"
            << endl;

        for (int i = 0; i < appointmentPtr->serviceCount; i++) {
            cout << left << setw(6) << (i + 1)
                << setw(18) << getServiceNameById(appointmentPtr->bookedServices[i].serviceId, services)
                << setw(10) << (appointmentPtr->bookedServices[i].gender == 'M' ? "Male" : "Female")
                << setw(13) << appointmentPtr->bookedServices[i].persons
                << endl;
        }
        
        cout << "\nCurrently Assigned Staff: ";
        if (appointmentPtr->staffCount == 0) {
            cout << "None";
        }
        else {
            for (int i = 0; i < appointmentPtr->staffCount; i++) {
                string staffName = appointmentPtr->assignedStaffIds[i];

                for (const Staff& staff : staffs) {
                    if (staff.staffCode == appointmentPtr->assignedStaffIds[i]) {
                        staffName = staff.user.name;
                        break;
                    }
                }

                if (i > 0) {
                    cout << ", ";
                }

                cout << staffName;
            }
        }

        cout << "\nAvailable Staff List: " << endl;
        cout << left << setw(13) << "Staff ID" << setw(25) << "Name" << endl;

        bool anyAvailable = false;

        for (Staff& staff : staffs) {
            bool alreadyAssigned = false;

            for (int i = 0; i < appointmentPtr->staffCount; i++) {
                if (appointmentPtr->assignedStaffIds[i] == staff.staffCode) {
                    alreadyAssigned = true;
                    break;
                }
            }

            if (!alreadyAssigned && isStaffAvailable(staff.staffCode, *appointmentPtr, appointments)) {
                anyAvailable = true;
                cout << left << setw(13) << staff.staffCode << setw(25) << staff.user.name << endl;
            }
        }
        
        if (!anyAvailable) {
            cout << "No available staff found for this time slot." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        if (appointmentPtr->staffCount >= appointmentPtr->totalPersons) {
            cout << "\nAll required staff have already been assigned." << endl;
            cout << "Press enter to continue...";
            cin.get();
            clearScreen();
            return;
        }

        cout << "\nAssign staff " << (appointmentPtr->staffCount + 1) << " of " << appointmentPtr->totalPersons << "\n(enter Staff ID or q to quit): ";
        
        string staffId;
        getline(cin, staffId);

		if (staffId == "q" || staffId == "Q") {
			clearScreen();
			return;

		}

        Staff* selectedStaff = nullptr;
        bool alreadyAssigned = false;

        for (int i = 0; i < appointmentPtr->staffCount; i++) {
            if (appointmentPtr->assignedStaffIds[i] == staffId) {
                alreadyAssigned = true;
                break;
            }
        }

        if (alreadyAssigned) {
            clearScreen();
            cout << "This staff has already been assigned to the appointment!\nPlease pick another staff." << endl;
            continue;
        }

        for (Staff& staff : staffs) {
            if (staff.staffCode == staffId) {
                selectedStaff = &staff;
                break;
            }
        }

        if (selectedStaff == nullptr) {
            clearScreen();
            cout << "Invalid staff ID!" << endl;
            continue;
        }

        if (!isStaffAvailable(staffId, *appointmentPtr, appointments)) {
            clearScreen();
            cout << "This staff is not available for the selected date and time!" << endl;
            continue;
        }

        while(true) {
            cout << "Confirm assigning staff " << selectedStaff->user.name << " to Appointment No. " << appointmentPtr->appointmentNo << "?" << endl;
            cout << "Choice (Y/N): ";

            char confirmation;
            cin >> confirmation;
            cin.ignore();

            if (toupper(confirmation) == 'Y') {
                appointmentPtr->assignedStaffIds[appointmentPtr->staffCount] = staffId;
                appointmentPtr->staffCount++;
                overwriteAppointmentFile(appointments);

                clearScreen();
                cout << "Staff " << selectedStaff->user.name << " has been assigned to Appointment No. " << appointmentPtr->appointmentNo << "!" << endl;

                if (appointmentPtr->staffCount == appointmentPtr->totalPersons) {
                    cout << "All required staff have been assigned." << endl;
                    return;
                }
                
                break;

            }
            else if (toupper(confirmation) == 'N') {
                clearScreen();
                break;

            }
            else {
                clearScreen();
                cout << "Invalid input! Please enter Y or N!" << endl;
            }
        }
	} while (true);
}

// ===============================================================================================
// Helpers
string generateNextAppointmentNo(const vector<Appointment>& appointments) {
    static int nextNumber = 0;

    if (nextNumber == 0) {
        for (const Appointment& appointment : appointments) {
            const string& id = appointment.appointmentNo;

            if (id.length() == 7 && id.substr(0, 2) == "AP") {
                try {
                    nextNumber = max(nextNumber, stoi(id.substr(2)));
                }
                catch (...) {
                }
            }
        }

        nextNumber++;
    }

    stringstream ss;
    ss << "AP" << setw(5) << setfill('0') << nextNumber++;
    return ss.str();
}

string getServiceNameById(const string& serviceId, const vector<Service>& services) {
    for (const Service& service : services) {
        if (service.serviceId == serviceId) {
            return service.name;
        }
    }

    return "Unknown Service";
}

bool parseDate(const string& input, Date& date) {
    // Converts the input string into a Date struct and checks if the format is correct.
    char slash1, slash2;
    stringstream ss(input);

    if (ss >> date.day >> slash1 >> date.month >> slash2 >> date.year) {
        return slash1 == '/' && slash2 == '/';
    }
    
    return false;
}

bool parseTime(const string& input, Time& time) {
	// Converts the input string into a Time struct and checks if the format is correct.
    char colon;
    stringstream ss(input);

    if (ss >> time.hour >> colon >> time.minute) {
        return colon == ':';
    }

    return false;
}

bool isLeapYear(int year) {
    return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

bool isValidDate(const Date& date) {
    stringstream ss;
    ss << setw(2) << setfill('0') << date.day << "/"
        << setw(2) << setfill('0') << date.month << "/"
        << setw(4) << setfill('0') << date.year;

    string dateStr = ss.str();

    regex datePattern(R"(^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/(2026|20[3-9][0-9]|2[1-9][0-9]{2})$)");

    if (!regex_match(dateStr, datePattern)) {
        return false;
    }

	// Check the number of days in the given month (Checks for leap year for february)
    int daysInMonth[] = { 31, isLeapYear(date.year) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    return date.day <= daysInMonth[date.month - 1];
}

bool isValidTime(const Time& time) {
    stringstream ss;
    ss << setw(2) << setfill('0') << time.hour << ":"
        << setw(2) << setfill('0') << time.minute;

    regex timePattern(R"(^(09|1[0-6]):(00|30)$|^17:00$)");
    return regex_match(ss.str(), timePattern);
}

bool isFutureAppointment(const Date& appointmentDate) {
	// Build the appointment date (user's input)
	tm appointment_tm = {};
    appointment_tm.tm_mday = appointmentDate.day; // Get the day of the appointment
	appointment_tm.tm_mon = appointmentDate.month - 1; // Get the month of the appointment (0-11)
	appointment_tm.tm_year = appointmentDate.year - 1900; // Get the year of the appointment (years since 1900)

	// Since we are only comparing dates, we can set the time to 00:00:00 for the appointment date
    appointment_tm.tm_hour = 0;
	appointment_tm.tm_min = 0;
	appointment_tm.tm_sec = 0;

	time_t appointmentTime = mktime(&appointment_tm); // Convert to time_t for comparison
    if (appointmentTime == -1) {
		return false; // Error in converting appointment date to time_t
    }

	// Get today's date (system's current date)
    Date currentDate;
    Time currentTime;
    getCurrentDateTime(currentDate, currentTime);

    // Set the time to 00:00:00 for today's date
    tm todayTm = {};
    todayTm.tm_mday = currentDate.day;
	todayTm.tm_mon = currentDate.month - 1;
	todayTm.tm_year = currentDate.year - 1900;
    todayTm.tm_hour = 0;
	todayTm.tm_min = 0;
	todayTm.tm_sec = 0;

    time_t todayTime = mktime(&todayTm); // Convert to time_t for comparison
	if (todayTime == -1) {
		return false; // Error in getting current time
	}

	const time_t ONE_DAY = 24 * 60 * 60; // Number of seconds in a day

	// Make sure the appointment booked is within tomorrow and a week from now (7 days)
	time_t tomorrowTime = todayTime + ONE_DAY; // Get the time for tomorrow
	time_t limitTime = todayTime + (7 * ONE_DAY); // Get the time for 7 days from now

    return appointmentTime >= tomorrowTime && appointmentTime <= limitTime;
}

bool sameDate(const Date& date1, const Date& date2) {
    return date1.day == date2.day &&
        date1.month == date2.month &&
        date1.year == date2.year;
}

bool sameTime(const Time& time1, const Time& time2) {
    return time1.hour == time2.hour &&
        time1.minute == time2.minute;
}

int getBookedPersonsForSlot(const Date& date, const Time& time, const vector<Appointment>& appointments) {
    int totalBooked = 0;

    for (const Appointment& appt : appointments) {
		// Check if the selected appointment date and time slot matches the existing appointments and is not cancelled
        if (sameDate(appt.date, date) &&
            sameTime(appt.time, time) &&
            appt.status != "Cancelled") {
            totalBooked += appt.totalPersons;
        }
    }

    return totalBooked;
}

bool canBookSlot(const Date& date, const Time& time, int newPersons, const vector<Appointment>& appointments) {
    int bookedPersons = getBookedPersonsForSlot(date, time, appointments);
	return bookedPersons + newPersons <= MAX_PERSONS_PER_SLOT; // Maximum of 7 persons per slot
}

int getRemainingSlotCapacity(const Date& date, const Time& time, const vector<Appointment>& appointments) {
    int remaining = MAX_PERSONS_PER_SLOT - getBookedPersonsForSlot(date, time, appointments);
    return (remaining < 0) ? 0 : remaining; // Ensure it doesn't go below 0
}

int getAssignedPersons(const Appointment& appointment) {
    int totalAssigned = 0;
    for (int i = 0; i < appointment.serviceCount; i++) {
        totalAssigned += appointment.bookedServices[i].persons;
    }

    return totalAssigned;
}

void loadCustomerPendingAppointments(Customer& customer, vector<Appointment>& appointments, vector<Appointment>& customerPendingAppointments) {
    customerPendingAppointments.clear();

    for (Appointment& appointment : appointments) {
        if (customer.user.name == appointment.customerName && appointment.status == "Pending") {
            customerPendingAppointments.push_back(appointment);
        }
    }
}

void loadStaffAssignedAppointments(Staff& staff, vector<Appointment>& appointments, vector<Appointment>& staffAssignedAppointments) {
    staffAssignedAppointments.clear();

    for (Appointment& appointment : appointments) {
        for (int i = 0; i < appointment.staffCount; i++) {
            if (staff.staffCode == appointment.assignedStaffIds[i] && appointment.status == "Pending") {
                staffAssignedAppointments.push_back(appointment);
            }
        }
    }
}

void loadStaffCompletedAppointments(Staff& staff, vector<Appointment>& appointments, vector<Appointment>& staffCompletedAppointments) {
    staffCompletedAppointments.clear();

    for (Appointment& appointment : appointments) {
        for (int i = 0; i < appointment.staffCount; i++) {
            if (staff.staffCode == appointment.assignedStaffIds[i] && appointment.status == "Completed") {
                staffCompletedAppointments.push_back(appointment);
            }
        }
    }
}

void loadUnassignedAppointments(vector<Appointment>& appointments, vector<Appointment>& unassignedAppointments) {
    unassignedAppointments.clear();

    for (Appointment& appointment : appointments) {
        int assignedCount = 0;

        for (int i = 0; i < appointment.totalPersons; i++) {
            if (!appointment.assignedStaffIds[i].empty()) {
                assignedCount++;
            }
        }

        if (appointment.status == "Pending" && assignedCount < appointment.totalPersons) {
            unassignedAppointments.push_back(appointment);
        }
    }
}

void printPendingAppointments(const vector<Appointment> loadedAppointments) {
    cout << left << setw(20) << setfill(' ') << "Appointment No."
        << setw(14) << "Date"
        << setw(9) << "Time"
        << setw(14) << "Person(s)"
        << setw(11) << "Status"
        << setw(15) << "Total (RM)" << endl;

    for (const Appointment& appointment : loadedAppointments) {
        cout << left << setw(20) << setfill(' ') << appointment.appointmentNo
            << setw(2) << setfill('0') << appointment.date.day << "/" << right << setw(2) << setfill('0') << appointment.date.month << "/" << left << setw(4) << setfill('0') << appointment.date.year << setw(4) << setfill(' ') << " "
            << setw(2) << setfill('0') << appointment.time.hour << ":" << setw(2) << appointment.time.minute << setw(4) << setfill(' ') << " "
            << setw(14) << appointment.totalPersons
            << setw(11) << appointment.status
            << setw(3) << "RM " << right << setw(7) << fixed << setprecision(2) << appointment.total << endl;
    }
}

Appointment* findAppointment(vector<Appointment>& appointments, const string& appointmentNo) {
    return findBy(appointments, [&](Appointment& appointment) {
        return appointment.appointmentNo == appointmentNo;
        });
}

bool isStaffAvailable(const string& staffId, const Appointment& targetAppointment, vector<Appointment> appointments) {
    for (const Appointment& appointment : appointments) {
        if (appointment.appointmentNo == targetAppointment.appointmentNo) {
            continue;
        }

        if (appointment.date.day == targetAppointment.date.day &&
            appointment.date.month == targetAppointment.date.month &&
            appointment.date.year == targetAppointment.date.year &&
            appointment.time.hour == targetAppointment.time.hour &&
            appointment.time.minute == targetAppointment.time.minute) {

            for (int i = 0; i < appointment.totalPersons; i++) {
                if (appointment.assignedStaffIds[i] == staffId) {
                    return false;
                }
            }
        }
    }

    return true;
}