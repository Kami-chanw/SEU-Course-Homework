#ifndef HUGEINT_H
#define HUGEINT_H
#include <cstring>
#include <iostream>

using std::ostream;
class HugeInt {
  friend ostream &operator<<(ostream &, const HugeInt &);

public:
  HugeInt(long = 0);     // conversion/default constructor
  HugeInt(const char *); // conversion constructor
  // addition operator; HugeInt + HugeInt
  HugeInt operator+(const HugeInt &) const;
  // addition operator; HugeInt + int
  HugeInt operator+(int) const;
  // addition operator;
  // HugeInt + string that represents large integer value
  HugeInt operator+(const char *) const;
  bool operator==(const HugeInt &) const; // equality operator
  bool operator!=(const HugeInt &) const; // inequality operator
  bool operator<(const HugeInt &) const;  // less than operator
  // less than or equal to operator
  bool operator<=(const HugeInt &) const;
  bool operator>(const HugeInt &) const; // greater than operator
  // greater than or equal to operator
  bool operator>=(const HugeInt &) const;
  HugeInt operator-(const HugeInt &) const; // subtraction operator
  HugeInt operator*(const HugeInt &) const; // multiply two HugeInts
  HugeInt operator/(const HugeInt &) const; // divide two HugeInts
  int getLength() const;

private:
  int integer[40];
}; // end class HugeInt
#endif
