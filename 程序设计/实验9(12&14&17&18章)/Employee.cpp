// Fig. 12.10: Employee.cpp
// Abstract-base-class Employee member-function definitions.
// Note: No definitions are given for pure virtual functions.
#include "Employee.h"  // Employee class definition
#include <iomanip>
#include <sstream>

using namespace std;

// constructor
Employee::Employee(const string& first, const string& last, const string& ssn, const Date& bd)
    : firstName(first), lastName(last), socialSecurityNumber(ssn), birthDay(bd) {}

// set first name
void Employee::setFirstName(const string& first) { firstName = first; }  // end function gerFirstName

// return first name
string Employee::getFirstName() const { return firstName; }

// set last name
void Employee::setLastName(const string& last) { lastName = last; }

// return last name

// set social security number
void Employee::setSocialSecurityNumber(const string& ssn) {
    socialSecurityNumber = ssn;  // should validate
}

// return social security number
string Employee::getSocialSecurityNumber() const { return socialSecurityNumber; }
string Employee::getLastName() const { return lastName; }

// set birth day
void Employee::setBirthDay(const Date& bd) { birthDay = bd; }

// return birth day
Date Employee::getBirthDay() const { return birthDay; }

double Employee::bonusOrNot() const {
    if (Date().isSameMonth(getBirthDay())) {
        return earnings() + 100;
    }
    return earnings();
}

// toString Employee's information (virtual, but not pure virtual)
string Employee::toString() const {
    return getFirstName() + " "s + getLastName() + "\nbirthday: " + getBirthDay().toString() + "\nsocial security number: "s
           + getSocialSecurityNumber();
}

string Employee::toBirthString() const {
    ostringstream output;
    if (Date().isSameMonth(birthDay)) {
        output << toString() << "\nHAPPY BIRTHDAY!\nearned $" << fixed << setprecision(2) << bonusOrNot() << endl << endl;
    }
    else {
        output << toString() << "\nearned $" << fixed << setprecision(2) << earnings() << endl << endl;
    }
    return output.str();
}
