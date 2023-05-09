// Fig. 12.12: SalariedEmployee.cpp
// SalariedEmployee class member-function definitions.
#include "SalariedEmployee.h"  // SalariedEmployee class definition
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

// constructor
SalariedEmployee::SalariedEmployee(const string& first, const string& last, const string& ssn, const Date& bd, double salary)
    : Employee(first, last, ssn, bd) {
    setWeeklySalary(salary);
}

SalariedEmployee::~SalariedEmployee() { cout << "Deleting object of class SalariedEmployee\n"; }

// set salary
void SalariedEmployee::setWeeklySalary(double salary) {
    if (salary < 0.0) {
        throw invalid_argument("Weekly salary must be >= 0.0");
    }

    weeklySalary = salary;
}

// return salary
double SalariedEmployee::getWeeklySalary() const { return weeklySalary; }

// calculate earnings;
// override pure virtual function earnings in Employee
double SalariedEmployee::earnings() const { return getWeeklySalary(); }

// return a string representation of SalariedEmployee's information
string SalariedEmployee::toString() const {
    ostringstream output;
    output << fixed << setprecision(2);
    output << "salaried employee: " << Employee::toString()  // reuse abstract base-class function
           << "\nweekly salary: " << getWeeklySalary();
    return output.str();
}
