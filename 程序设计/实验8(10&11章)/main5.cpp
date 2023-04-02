#include "5.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
  Account account1(50.0);           // create Account object
  SavingsAccount account2(25.0, .03);  // create SavingsAccount object
  CheckingAccount account3(80.0, 1.0); // create CheckingAccount object
  cout << fixed << setprecision(2);
  // display initial balance of each object
  cout << "account1 balance: $" << account1.getBalance() << endl;
  cout << "account2 balance: $" << account2.getBalance() << endl;
  cout << "account3 balance: $" << account3.getBalance() << endl;
  cout << "\nAttempting to debit $25.00 from account1." << endl;
  account1.debit(25.0); // try to debit $25.00 from account1
  cout << "\nAttempting to debit $30.00 from account2." << endl;
  account2.debit(30.0); // try to debit $30.00 from account2
  cout << "\nAttempting to debit $40.00 from account3." << endl;
  account3.debit(40.0); // try to debit $40.00 from account3
  // display balances
  cout << "\naccount1 balance: $" << account1.getBalance() << endl;
  cout << "account2 balance: $" << account2.getBalance() << endl;
  cout << "account3 balance: $" << account3.getBalance() << endl;
  cout << "\nCrediting $40.00 to account1." << endl;
  account1.credit(40.0); // credit $40.00 to account1
  cout << "\nCrediting $65.00 to account2." << endl;
  account2.credit(65.0); // credit $65.00 to account2
  cout << "\nCrediting $20.00 to account3." << endl;
  account3.credit(20.0); // credit $20.00 to account3
  // display balances
  cout << "\naccount1 balance: $" << account1.getBalance() << endl;
  cout << "account2 balance: $" << account2.getBalance() << endl;
  cout << "account3 balance: $" << account3.getBalance() << endl;
  // add interest to SavingsAccount object account2
  double interestEarned = account2.calculateInterest();
  cout << "\nAdding $" << interestEarned << " interest to account2." << endl;
  account2.credit(interestEarned);
  cout << "\nNew account2 balance: $" << account2.getBalance() << endl;
  return 0;
}