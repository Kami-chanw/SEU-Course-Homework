#include "2.h"
#include <algorithm>
#include <iterator>

HugeInt::HugeInt(long num) {
    int i;
    for (i = 0; num; ++i, num /= 10)
        integer[i] = num % 10;
    std::fill(std::begin(integer) + i, std::end(integer), 0);
}

HugeInt::HugeInt(const char* str) {
    int len = strlen(str);
    std::fill(integer + len, std::end(integer), 0);
    std::transform(std::make_reverse_iterator(str + len), std::make_reverse_iterator(str), integer,
                   [](const char ch) { return ch - '0'; });
}

ostream& operator<<(ostream& os, const HugeInt& op2) {
    for (int i = op2.getLength() - 1; i >= 0; --i)
        os << op2.integer[i];
    return os;
}

HugeInt HugeInt::operator+(const HugeInt& op2) const {
    HugeInt res(long(0));
    int     carry = 0;

    for (int i = 0; i < 40; i++) {
        int sum        = integer[i] + op2.integer[i] + carry;
        res.integer[i] = sum % 10;
        carry          = sum / 10;
    }
    return res;
}

HugeInt HugeInt::operator+(int op2) const { return *this + HugeInt(op2); }

HugeInt HugeInt::operator+(const char* op2) const { return *this + HugeInt(op2); }

bool HugeInt::operator==(const HugeInt& op2) const {
    for (int i = 0; i < 40; ++i)
        if (integer[i] != op2.integer[i])
            return false;

    return true;
}

bool HugeInt::operator!=(const HugeInt& op2) const { return !(*this == op2); }

bool HugeInt::operator<(const HugeInt& op2) const {
    for (int i = 39; i >= 0; --i)
        if (integer[i] != op2.integer[i])
            return integer[i] < op2.integer[i];
    return false;
}

bool HugeInt::operator<=(const HugeInt& op2) const { return !(*this > op2); }

bool HugeInt::operator>(const HugeInt& op2) const { return op2 < *this; }

bool HugeInt::operator>=(const HugeInt& op2) const { return !(*this < op2); }

HugeInt HugeInt::operator-(const HugeInt& op2) const {
    HugeInt res;
    int     borrow = 0;
    for (int i = 0; i < 40; i++) {
        int diff = integer[i] - op2.integer[i] - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        }
        else
            borrow = 0;
        res.integer[i] = diff;
    }
    return res;
}

HugeInt HugeInt::operator*(const HugeInt& op2) const {
    HugeInt product;
    int     carry = 0;
    int     digit;
    for (int i = 0; i < 40; i++) {
        carry = 0;
        for (int j = 0; j < 40; j++) {
            if (i + j >= 40) {
                break;
            }
            digit                  = product.integer[i + j] + integer[i] * op2.integer[j] + carry;
            carry                  = digit / 10;
            product.integer[i + j] = digit % 10;
        }
    }
    return product;
}

HugeInt HugeInt::operator/(const HugeInt& divisor) const {
    HugeInt quotient;
    HugeInt remainder  = *this;
    int     num_digits = remainder.getLength();
    int     div_digits = divisor.getLength();
    if (divisor == HugeInt(long(0))) {
        throw std::invalid_argument("division by zero");
    }
    if (*this <= divisor) {
        return quotient;
    }
    HugeInt dividend;
    for (int i = num_digits - 1; i >= 0; i--) {
        dividend = dividend * HugeInt(10) + HugeInt(remainder.integer[i]);
        if (dividend < divisor) {
            quotient.integer[i] = 0;
        }
        else {
            int     digit    = 0;
            HugeInt multiple = divisor;
            while (dividend >= multiple) {
                multiple = multiple + divisor;
                digit++;
            }
            quotient.integer[i] = digit;
            dividend            = dividend - (multiple - divisor);
        }
    }
    return quotient;
}

int HugeInt::getLength() const {
    for (int i = 39; i >= 0; --i)
        if (integer[i])
            return i + 1;
    return 0;
}
