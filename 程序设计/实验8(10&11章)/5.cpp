#include <iostream>
using std::cout;
using std::endl;
#include "5.h" // include definition of class Account
// Account constructor initializes data member balance
Account::Account(double initialBalance) {
  // if initialBalance is greater than or equal to 0.0, set this value
  // as the balance of the Account
  if (initialBalance >= 0.0)
    balance = initialBalance;
  else // otherwise, output message and set balance to 0.0
  {
    cout << "Error: Initial balance cannot be negative." << endl;
    balance = 0.0;
  } // end if...else
} // end Account constructor
// credit (add) an amount to the account balance
void Account::credit(double amount) {
  balance = balance + amount; // add amount to balance
} // end function credit
// debit (subtract) an amount from the account balance
// return bool indicating whether money was debited
bool Account::debit(double amount) {
  if (amount > balance) // debit amount exceeds balance
  {
    cout << "Debit amount exceeded account balance." << endl;
    return false;
  }    // end if
  else // debit amount does not exceed balance
  {
    balance = balance - amount;
    return true;
  } // end else
} // end function debit
// set the account balance
void Account::setBalance(double newBalance) {
  balance = newBalance;
} // end function setBalance
// return the account balance
double Account::getBalance() const {
  return balance;
} // end function getBalance

SavingsAccount::SavingsAccount(double initialBalance, double rate)
    : Account(initialBalance) {
  interestRate = rate;
}

double SavingsAccount::calculateInterest() const {
  return getBalance() * interestRate;
}

CheckingAccount::CheckingAccount(double initialBalance, double fee)
    : SavingsAccount(initialBalance, 0.0) {
  transactionFee = fee;
}

void CheckingAccount::credit(double amount) {
  setBalance(getBalance() + amount - transactionFee);
}

bool CheckingAccount::debit(double amount) {
  bool success = SavingsAccount::debit(amount);
  if (success)
    setBalance(getBalance() - transactionFee);
  return success;
}