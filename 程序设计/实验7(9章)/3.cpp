#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

class HugeInteger {
private:
    int digits[40];

public:
    HugeInteger(int num = 0) {
        int i;
        for (i = 0; num; ++i, num /= 10)
            digits[i] = num % 10;
        fill(digits + i, end(digits), 0);
    }
    HugeInteger(const char* str) {
        int len = strlen(str);
        fill(digits + len, end(digits), 0);
        transform(make_reverse_iterator(str + len), make_reverse_iterator(str), digits, [](const char ch) { return ch - '0'; });
    }
    ~HugeInteger() {}
    void input() {
        char str[41];
        cin >> str;
        *this = HugeInteger(str);
    }
    void output() const {
        int i;
        for (i = 39; i >= 0; --i)
            if (digits[i]) {
                for (; i >= 0; --i)
                    cout << digits[i];
                return;
            }
        cout << 0;
    }
    HugeInteger add(const HugeInteger& other) const {
        HugeInteger res;
        int         carry = 0;

        for (int i = 0; i < 40; i++) {
            int sum       = digits[i] + other.digits[i] + carry;
            res.digits[i] = sum % 10;
            carry         = sum / 10;
        }
        return res;
    }
    HugeInteger subtract(const HugeInteger& other) const {
        HugeInteger res;
        int         borrow = 0;
        for (int i = 0; i < 40; i++) {
            int diff = digits[i] - other.digits[i] - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            }
            else
                borrow = 0;
            res.digits[i] = diff;
        }
        return res;
    }
    bool isEqualTo(const HugeInteger& other) const {
        for (int i = 0; i < 40; ++i)
            if (digits[i] != other.digits[i])
                return false;
        return true;
    }
    bool isNotEqualTo(const HugeInteger& other) const { return !isEqualTo(other); }
    bool isGreaterThan(const HugeInteger& other) const {
        for (int i = 39; i >= 0; --i)
            if (digits[i] != other.digits[i])
                return digits[i] > other.digits[i];
        return false;
    }
    bool isLessThan(const HugeInteger& other) const { return !isGreaterThan(other) && !isEqualTo(other); }
    bool isGreaterThanOrEqualTo(const HugeInteger& other) const { return isGreaterThan(other) || isEqualTo(other); }
    bool isLessThanOrEqualTo(const HugeInteger& other) const { return isLessThan(other) || isEqualTo(other); }
    bool isZero() const {
        return all_of(begin(digits), end(digits), [](const int a) { return !a; });
    }
    HugeInteger multiply(const HugeInteger& other) const {
        HugeInteger product;
        int         carry = 0;
        int         digit;
        for (int i = 0; i < 40; i++) {
            carry = 0;
            for (int j = 0; j < 40; j++) {
                if (i + j >= 40) {
                    break;
                }
                digit                 = product.digits[i + j] + digits[i] * other.digits[j] + carry;
                carry                 = digit / 10;
                product.digits[i + j] = digit % 10;
            }
        }
        return product;
    }
    HugeInteger divide(const HugeInteger& other) const {
        HugeInteger quotient;
        HugeInteger remainder = *this;
        static auto getLength = [](const HugeInteger& integer) {
            for (int i = 39; i >= 0; --i)
                if (integer.digits[i])
                    return i + 1;
            return 0;
        };
        int num_digits = getLength(remainder);
        int div_digits = getLength(other);
        if (other.isEqualTo(0)) {
            throw std::invalid_argument("division by zero");
        }
        if (isLessThan(other)) {
            return quotient;
        }
        HugeInteger dividend;
        for (int i = num_digits - 1; i >= 0; i--) {
            dividend = dividend.multiply(HugeInteger(10)).add(HugeInteger(remainder.digits[i]));
            if (dividend.isLessThan(other)) {
                quotient.digits[i] = 0;
            }
            else {
                int         digit    = 0;
                HugeInteger multiple = other;
                while (dividend.isGreaterThanOrEqualTo(multiple)) {
                    multiple = multiple.add(other);
                    digit++;
                }
                quotient.digits[i] = digit;
                dividend           = dividend.subtract(multiple.subtract(other));
            }
        }
        return quotient;
    }
    HugeInteger modulus(const HugeInteger& other) const {
        if (other.isZero())
            throw invalid_argument("mod zero");
        HugeInteger remainder = *this;
        for (int i = 39; i >= 0; i--) {
            int quotient = 0;
            while (remainder.isGreaterThanOrEqualTo(other)) {
                remainder = remainder.subtract(other);
                quotient++;
            }
            remainder           = remainder.multiply(HugeInteger("10"));
            remainder.digits[0] = digits[i];
        }
        return remainder;
    }
};

int main() {
    HugeInteger a("7654321"), b("7891234"), c(5), d(0);
    a.output();
    cout << " + ";
    b.output();
    cout << " = ";
    a.add(b).output();
    cout << endl;
    b.output();
    cout << " - ";
    c.output();
    cout << " = ";
    b.subtract(c).output();
    cout << endl;
    assert(a.isEqualTo(a));
    a.output();
    cout << " is equal to ";
    a.output();
    cout << endl;
    assert(a.isNotEqualTo(b));
    a.output();
    cout << " is not equal to ";
    b.output();
    cout << endl;
    assert(b.isGreaterThan(a));
    b.output();
    cout << " is greater than ";
    a.output();
    cout << endl;
    assert(c.isLessThan(a));
    c.output();
    cout << " is less than ";
    a.output();
    cout << endl;
    assert(c.isLessThanOrEqualTo(c));
    c.output();
    cout << " is less than or equal to ";
    c.output();
    cout << endl;
    assert(d.isGreaterThanOrEqualTo(d));
    d.output();
    cout << " is greater than or equal to ";
    d.output();
    cout << endl;
    return 0;
}