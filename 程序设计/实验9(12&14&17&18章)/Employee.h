// Fig. 12.9: Employee.h
// Employee abstract base class.
#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "Date.h"
#include <string>  // C++ standard string class

class Employee {
public:
    Employee(const std::string&, const std::string&, const std::string&, const Date&);
    virtual ~Employee() = default;  // compiler generates virtual destructor

    void        setFirstName(const std::string&);  // set first name
    std::string getFirstName() const;              // return first name

    void        setLastName(const std::string&);  // set last name
    std::string getLastName() const;              // return last name

    void setBirthDay(const Date&);  // set birth day
    Date getBirthDay() const;       // return birth day

    void        setSocialSecurityNumber(const std::string&);  // set SSN
    std::string getSocialSecurityNumber() const;              // return SSN

    // pure virtual function makes Employee an abstract base class
    virtual double      earnings() const = 0;  // pure virtual
    double              bonusOrNot() const;
    virtual std::string toString() const;  // virtual
    std::string         toBirthString() const;

private:
    std::string firstName;
    std::string lastName;
    std::string socialSecurityNumber;
    Date        birthDay;
};

#endif  // EMPLOYEE_H
