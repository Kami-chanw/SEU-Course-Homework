#include "1.h"

Complex::Complex(double r, double i) {
  real = r;
  imaginary = i;
}

Complex Complex::operator+(const Complex &operand2) const {
  return Complex(real + operand2.real, imaginary + operand2.imaginary);
}

Complex Complex::operator-(const Complex &operand2) const {
  return Complex(real - operand2.real, imaginary - operand2.imaginary);
}

Complex Complex::operator*(const Complex &operand2) const {
  return Complex(real * operand2.real - imaginary * operand2.imaginary,
                 real * operand2.imaginary + imaginary * operand2.real);
}

Complex &Complex::operator=(const Complex &right) {
  real = right.real;
  imaginary = right.imaginary;
  return *this;
}

bool Complex::operator==(const Complex &operand2) const {
  return (real == operand2.real) && (imaginary == operand2.imaginary);
}

bool Complex::operator!=(const Complex &operand2) const {
  return !(*this == operand2);
}

ostream &operator<<(ostream &output, const Complex &c) {
  output << "(" << c.real << ", " << c.imaginary << ")";
  return output;
}

istream &operator>>(istream &input, Complex &c) {
  input >> c.real >> c.imaginary;
  return input;
}