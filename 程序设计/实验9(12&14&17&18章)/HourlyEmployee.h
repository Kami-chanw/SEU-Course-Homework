#ifndef HOURLY_H
#define HOURLY_H

#include "Employee.h"  // Employee class definition
#include <string>      // C++ standard string class
using std::string;
class HourlyEmployee : public Employee {
public:
    HourlyEmployee(const std::string&, const std::string&, const std::string&, const Date&, double = 0.0, double = 0.0);
    virtual ~HourlyEmployee();  // virtual destructor

    void   setHourlySalary(double);  // set hourly salary
    double getHourlySalary() const;  // return hourly salary

    void   setHoursWorked(const double&);  // set hours worked
    double getHoursWorked() const;         // return hours worked

    // keyword virtual signals intent to override
    virtual double earnings() const override;  // calculate earnings
    virtual string toString() const override;  //
private:
    double hourlySalary;  // salary per hour
    double hoursWorked;   // workinghours
};

#endif  // HOURLY_H
