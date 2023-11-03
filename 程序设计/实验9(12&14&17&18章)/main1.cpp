#include "BasePlusCommissionEmployee.h"
#include "CommissionEmployee.h"
#include "HourlyEmployee.h"
#include "SalariedEmployee.h"

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

int main() {
    vector<unique_ptr<Employee>> employees;
    cout << "Employees processed polymorphically via dynamic binding:\n\n";
    employees.emplace_back(make_unique<SalariedEmployee>("John", "Smith", "111-11-1111", Date("June", 15, 1944), 800));
    employees.emplace_back(make_unique<HourlyEmployee>("Karen", "Price", "222-22-2222", Date("May", 29, 1960), 16.75, 40.00));
    employees.emplace_back(make_unique<CommissionEmployee>("Sue", "Jones", "333-33-3333", Date("September", 8, 1954), 10000, 0.06));
    employees.emplace_back(
        make_unique<BasePlusCommissionEmployee>("Bob", "Lewis", "444-44-4444", Date("March", 2, 1965), 5000, 0.04, 300));

    for (const auto& i : employees) {
        cout << i->toBirthString();
    }
    return 0;
}