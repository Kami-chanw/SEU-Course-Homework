// Fig. 12.12: HourlyEmployee.cpp
// HourlyEmployee class member-function definitions.
#include "HourlyEmployee.h"  // HourlyEmployee class definition
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>


using namespace std;

// constructor
HourlyEmployee::HourlyEmployee(const string& first, const string& last, const string& ssn, const Date& bd, double salary,
                               double hours)
    : Employee(first, last, ssn, bd) {
    setHourlySalary(salary);
    setHoursWorked(hours);
}

HourlyEmployee::~HourlyEmployee() { cout << "Deleting object of class HourlyEmployee\n"; }

// set salary
void HourlyEmployee::setHourlySalary(double salary) {
    if (salary < 0.0) {
        throw invalid_argument("Hourly salary must be >= 0.0");
    }

    hourlySalary = salary;
}

// return salary
double HourlyEmployee::getHourlySalary() const { return hourlySalary; }

void HourlyEmployee::setHoursWorked(const double& hw) { hoursWorked = hw; }

double HourlyEmployee::getHoursWorked() const { return hoursWorked; }

// to calculate earnings;
// to override pure virtual function earnings in Employee
double HourlyEmployee::earnings() const { return getHourlySalary() * getHoursWorked(); }

// return a string representation of HourlyEmployee's information
string HourlyEmployee::toString() const {
    ostringstream output;
    output << fixed << setprecision(2);
    output << "hourly employee: " << Employee::toString()  // reuse abstract base-class function
           << "\nHourly wage: " << getHourlySalary() << ";hours worked: " << getHoursWorked();
    return output.str();
}