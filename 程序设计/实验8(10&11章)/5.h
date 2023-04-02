#ifndef ACCOUNT_H
#define ACCOUNT_H
class Account {
public:
  Account(double);         // constructor initializes balance
  void credit(double);     // add an amount to the account balance
  bool debit(double);      // subtract an amount from the account balance
  void setBalance(double); // sets the account balance
  double getBalance() const;     // return the account balance
private:
  double balance; // data member that stores the balance
};                // end class Account

class SavingsAccount : public Account
{
public:
    SavingsAccount(double initialBalance, double rate);
    ~SavingsAccount() {};
    double calculateInterest() const;
private:
    double interestRate;             // data member that stores the interest rate
};


class CheckingAccount : public SavingsAccount
{
public:
    CheckingAccount(double initialBalance, double fee);
     ~CheckingAccount() {};
    void credit(double amount);     // redefine credit() to include transaction fee
    bool debit(double amount);      // redefine debit() to include transaction fee
private:
    double transactionFee;           // data member that stores the transaction fee
};

#endif