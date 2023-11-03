#include <iostream>
#include <iomanip>
using namespace std;

class SavingsAccount {
private:
    double        savingsBalance;
    static double annualInterestRate;

public:
    SavingsAccount(double balance) { savingsBalance = balance; }
    void calculateMonthlyInterest() {
        double monthlyInterest = savingsBalance * annualInterestRate / 12;
        savingsBalance += monthlyInterest;
    }
    static void modifyInterestRate(double newRate) { annualInterestRate = newRate; }
    double      getBalance() { return savingsBalance; }
};

double SavingsAccount::annualInterestRate = 0;

int main() {
    SavingsAccount saver1(2000.00);
    SavingsAccount saver2(3000.00);
    cout << fixed << setprecision(2);
    cout << "Initial balances:\nSaver 1: $" << saver1.getBalance() << "\tSaver 2: $" << saver2.getBalance() << "\n\n";

    SavingsAccount::modifyInterestRate(0.03);
    saver1.calculateMonthlyInterest();
    saver2.calculateMonthlyInterest();
    cout << "Balances after 1 month's interest applied at .03:\nSaver 1: $" << saver1.getBalance() << "\tSaver 2: $"
         << saver2.getBalance() << "\n\n";

    SavingsAccount::modifyInterestRate(0.04);
    saver1.calculateMonthlyInterest();
    saver2.calculateMonthlyInterest();
    cout << "Balances after 1 month's interest applied at .04:\nSaver 1: $" << saver1.getBalance() << "\tSaver 2: $"
         << saver2.getBalance() << "\n\n";

    return 0;
}