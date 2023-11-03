#pragma once
#include <iostream>
using std::ostream;

class Term {
  friend class Polynomial;
  double coef;
  int exp;
};

class Polynomial {
  friend ostream& operator<<(ostream&, const Polynomial&);
public:
  Polynomial();
  Polynomial(const Polynomial&);
  ~Polynomial();
  void enterTerms();
  void printPolynomial() const;
  Polynomial operator+(const Polynomial&) const;
  Polynomial operator-(const Polynomial&) const;
  Polynomial operator*(const Polynomial&) const;
  Polynomial& operator=(const Polynomial&);
  Polynomial& operator+=(const Polynomial&);
  Polynomial& operator-=(const Polynomial&);
  Polynomial& operator*=(const Polynomial&);
private:
  Term *termArray;
  int capacity;
  int terms;
};