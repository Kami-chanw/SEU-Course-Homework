#ifndef COMPLEX_H
#define COMPLEX_H
#include <iostream>
using std::istream;
using std::ostream;
class Complex {
  friend ostream &operator<<(ostream &, const Complex &);
  friend istream &operator>>(istream &, Complex &);

public:
  Complex(double = 0.0, double = 0.0);      // constructor
  Complex operator+(const Complex &) const; // addition
  Complex operator-(const Complex &) const; // subtraction
  Complex operator*(const Complex &) const; // multiplication
  Complex &operator=(const Complex &);      // assignment
  bool operator==(const Complex &) const;
  bool operator!=(const Complex &) const;

private:
  double real;      // real part
  double imaginary; // imaginary part
};                  // end class Complex
#endif