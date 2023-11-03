#include "3.h"
#include <iostream>
#include <stdlib.h>

using std::cerr;
using std::cout;
using std::endl;
RationalNumber::RationalNumber(int n, int d) {
  if (d == 0) {
    cerr << "Denominator cannot be zero." << endl;
    exit(EXIT_FAILURE);
  }
  if (d < 0) {
    n = -n;
    d = -d;
  }
  numerator = n;
  denominator = d;
  reduction();
}

RationalNumber RationalNumber::operator+(const RationalNumber &op2) const {
  RationalNumber result;
  result.numerator = numerator * op2.denominator + op2.numerator * denominator;
  result.denominator = denominator * op2.denominator;
  result.reduction();
  return result;
}

RationalNumber RationalNumber::operator-(const RationalNumber &op2) const {
  RationalNumber result;
  result.numerator = numerator * op2.denominator - op2.numerator * denominator;
  result.denominator = denominator * op2.denominator;
  result.reduction();
  return result;
}

RationalNumber RationalNumber::operator*(const RationalNumber &op2) const {
  RationalNumber result;
  result.numerator = numerator * op2.numerator;
  result.denominator = denominator * op2.denominator;
  result.reduction();
  return result;
}

RationalNumber RationalNumber::operator/(RationalNumber &op2) const {
  if (op2.numerator == 0) {
    cerr << "Cannot divide by zero." << endl;
    exit(EXIT_FAILURE);
  }
  RationalNumber result;
  result.numerator = numerator * op2.denominator;
  result.denominator = denominator * op2.numerator;
  result.reduction();
  return result;
}

bool RationalNumber::operator>(const RationalNumber &op2) const {
  RationalNumber temp = *this - op2;
  return temp.numerator > 0;
}

bool RationalNumber::operator<(const RationalNumber &op2) const {
  RationalNumber temp = *this - op2;
  return temp.numerator < 0;
}

bool RationalNumber::operator>=(const RationalNumber &op2) const {
  return !(*this < op2);
}

bool RationalNumber::operator<=(const RationalNumber &op2) const {
  return !(*this > op2);
}

bool RationalNumber::operator==(const RationalNumber &op2) const {
  return numerator == op2.numerator && denominator == op2.denominator;
}

bool RationalNumber::operator!=(const RationalNumber &op2) const {
  return !(*this == op2);
}

void RationalNumber::printRational() const {
  if (denominator == 1) {
    cout << numerator;
  } else {
    cout << numerator << '/' << denominator;
  }
}

int gcd(int a, int b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}

void RationalNumber::reduction() {
  if (numerator == 0) {
    denominator = 1;
    return;
  }
  int common = gcd(abs(numerator), denominator);
  numerator /= common;
  denominator /= common;
}