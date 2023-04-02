#include "4.h"
using std::cin;
using std::cout;
using std::endl;

Polynomial::Polynomial() : termArray(nullptr), capacity(0), terms(0) {}

Polynomial::Polynomial(const Polynomial &p) {
  terms = p.terms;
  capacity = p.capacity;
  termArray = new Term[capacity];
  for (int i = 0; i < terms; i++) {
    termArray[i] = p.termArray[i];
  }
}

Polynomial::~Polynomial() { delete[] termArray; }

void Polynomial::enterTerms() {
  if (termArray != nullptr) {
    delete[] termArray;
  }
  cout << "Enter number of polynomial terms: ";
  cin >> terms;
  capacity = terms;
  termArray = new Term[capacity];
  for (int i = 0; i < terms; i++) {
    cout << "Enter coefficient: ";
    cin >> termArray[i].coef;
    cout << "Enter exponent: ";
    cin >> termArray[i].exp;
    cout << "\n";
  }
}

void Polynomial::printPolynomial() const {
  for (int i = 0; i < terms; i++) {
    if (abs(termArray[i].coef) < 1e-6)
      continue;
    else if (termArray[i].exp == 0)
      cout << termArray[i].coef;
    else if (termArray[i].exp == 1)
      cout << termArray[i].coef << "x";
    else
      cout << termArray[i].coef << "x^" << termArray[i].exp;
    if (i < terms - 1 && termArray[i + 1].coef > 0)
      cout << " + ";
    else
      cout << " ";
  }
  cout << endl;
}

Polynomial Polynomial::operator+(const Polynomial &p) const {
  Polynomial result;
  result.termArray = new Term[terms + p.terms];
  int i = 0, j = 0, k = 0;
  while (i < terms && j < p.terms) {
    if (termArray[i].exp == p.termArray[j].exp) {
      if (termArray[i].coef + p.termArray[j].coef != 0) {
        result.termArray[k].coef = termArray[i].coef + p.termArray[j].coef;
        result.termArray[k].exp = termArray[i].exp;
        k++;
      }
      i++;
      j++;
    } else if (termArray[i].exp > p.termArray[j].exp) {
      result.termArray[k].coef = termArray[i].coef;
      result.termArray[k].exp = termArray[i].exp;
      k++;
      i++;
    } else {
      result.termArray[k].coef = p.termArray[j].coef;
      result.termArray[k].exp = p.termArray[j].exp;
      k++;
      j++;
    }
  }
  while (i < terms) {
    result.termArray[k].coef = termArray[i].coef;
    result.termArray[k].exp = termArray[i].exp;
    k++;
    i++;
  }
  while (j < p.terms) {
    result.termArray[k].coef = p.termArray[j].coef;
    result.termArray[k].exp = p.termArray[j].exp;
    k++;
    j++;
  }
  result.terms = k;
  result.capacity = k;
  return result;
}

Polynomial Polynomial::operator-(const Polynomial &p) const {
  Polynomial result;
  result.termArray = new Term[terms + p.terms];
  int i = 0, j = 0, k = 0;
  while (i < terms && j < p.terms) {
    if (termArray[i].exp == p.termArray[j].exp) {
      if (termArray[i].coef - p.termArray[j].coef != 0) {
        result.termArray[k].coef = termArray[i].coef - p.termArray[j].coef;
        result.termArray[k].exp = termArray[i].exp;
        k++;
      }
      i++;
      j++;
    } else if (termArray[i].exp > p.termArray[j].exp) {
      result.termArray[k].coef = termArray[i].coef;
      result.termArray[k].exp = termArray[i].exp;
      k++;
      i++;
    } else {
      result.termArray[k].coef = -p.termArray[j].coef;
      result.termArray[k].exp = p.termArray[j].exp;
      k++;
      j++;
    }
  }
  while (i < terms) {
    result.termArray[k].coef = termArray[i].coef;
    result.termArray[k].exp = termArray[i].exp;
    k++;
    i++;
  }
  while (j < p.terms) {
    result.termArray[k].coef = -p.termArray[j].coef;
    result.termArray[k].exp = p.termArray[j].exp;
    k++;
    j++;
  }
  result.terms = k;
  result.capacity = k;
  return result;
}

Polynomial Polynomial::operator*(const Polynomial &p) const {
  Polynomial result;
  result.capacity = terms * p.terms;
  result.termArray = new Term[result.capacity];
  for (int i = 0; i < result.capacity; i++) {
    result.termArray[i].coef = 0;
    result.termArray[i].exp = 0;
  }
  for (int i = 0; i < terms; i++) {
    for (int j = 0; j < p.terms; j++) {
      int k = 0;
      while (k < result.capacity &&
             result.termArray[k].exp != termArray[i].exp + p.termArray[j].exp) {
        k++;
      }
      if (k == result.capacity) {
        result.capacity++;
        Term *temp = new Term[result.capacity];
        for (int l = 0; l < result.capacity - 1; l++) {
          temp[l] = result.termArray[l];
        }
        temp[k].coef = 0;
        temp[k].exp = termArray[i].exp + p.termArray[j].exp;
        delete[] result.termArray;
        result.termArray = temp;
      }
      result.termArray[k].coef += termArray[i].coef * p.termArray[j].coef;
    }
  }
  int k = result.capacity - 1;
  while (k >= 0 && result.termArray[k].coef == 0) {
    k--;
  }
  result.terms = k + 1;
  return result;
}

Polynomial &Polynomial::operator=(const Polynomial &p) {
  if (&p == this) {
    return *this;
  }
  delete[] termArray;
  terms = capacity = p.capacity;
  terms = p.terms;
  termArray = new Term[capacity];
  for (int i = 0; i < terms; i++) {
    termArray[i].coef = p.termArray[i].coef;
    termArray[i].exp = p.termArray[i].exp;
  }
  return *this;
}

Polynomial &Polynomial::operator+=(const Polynomial &p) {
  *this = *this + p;
  return *this;
}

Polynomial &Polynomial::operator-=(const Polynomial &p) {
  *this = *this - p;
  return *this;
}

Polynomial &Polynomial::operator*=(const Polynomial &p) {
  *this = *this * p;
  return *this;
}