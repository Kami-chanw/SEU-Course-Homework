// Fig. 12.15: BasePlusCommissionEmployee.h
// BasePlusCommissionEmployee class derived from CommissionEmployee.
#ifndef BASEPLUSCOMMISSIONEMPLOYEE_H
#define BASEPLUSCOMMISSIONEMPLOYEE_H

#include "CommissionEmployee.h"  // CommissionEmployee class definition
#include <string>                // C++ standard string class

class BasePlusCommissionEmployee : public CommissionEmployee {
public:
    BasePlusCommissionEmployee(const std::string&, const std::string&, const std::string&, const Date&, double = 0.0,
                               double = 0.0, double = 0.0);
    virtual ~BasePlusCommissionEmployee();  // virtual destructor

    void   setBaseSalary(double);  // set base salary
    double getBaseSalary() const;  // return base salary

    // keyword virtual signals intent to override
    virtual double      earnings() const override;  // calculate earnings
    virtual std::string toString() const override;  // toString object
private:
    double baseSalary;  // base salary per week
};

#endif  // BASEPLUS_H
