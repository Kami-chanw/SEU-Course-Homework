// Fig. 12.16: BasePlusCommissionEmployee.cpp
// BasePlusCommissionEmployee member-function definitions.
#include "BasePlusCommissionEmployee.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

// constructor
BasePlusCommissionEmployee::BasePlusCommissionEmployee(const string& first, const string& last, const string& ssn, const Date& bd,
                                                       double sales, double rate, double salary)
    : CommissionEmployee(first, last, ssn, bd, sales, rate) {
    setBaseSalary(salary);  // validate and store base salary
}

BasePlusCommissionEmployee::~BasePlusCommissionEmployee() { cout << "deleting object of class BasePlusCommissionEmployee\n"; }

// set base salary
void BasePlusCommissionEmployee::setBaseSalary(double salary) {
    if (salary < 0.0) {
        throw invalid_argument("Salary must be >= 0.0");
    }

    baseSalary = salary;
}

// return base salary
double BasePlusCommissionEmployee::getBaseSalary() const { return baseSalary; }

// calculate earnings;
// override virtual function earnings in CommissionEmployee
double BasePlusCommissionEmployee::earnings() const { return getBaseSalary() + CommissionEmployee::earnings(); }

// return a string representation of a BasePlusCommissionEmployee
string BasePlusCommissionEmployee::toString() const {
    ostringstream output;
    output << fixed << setprecision(2);
    output << "base-salaried " << CommissionEmployee::toString() << "; base salary: " << getBaseSalary();
    return output.str();
}
