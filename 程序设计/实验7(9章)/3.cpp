#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
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
        int i = std::extent<decltype(digits)>::value
                - count_if(rbegin(digits), rend(digits), [](const int a) { return !a; });  // compat before c++17
        if (i == 0)
            cout << 0;
        else
            copy_n(make_reverse_iterator(digits + i), i, ostream_iterator<int>(cout));
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
    bool isEqualTo(const HugeInteger& other) const { return equal(begin(digits), end(digits), other.digits); }
    bool isNotEqualTo(const HugeInteger& other) const { return !isEqualTo(other); }
    bool isGreaterThan(const HugeInteger& other) const {
        return lexicographical_compare(rbegin(digits), rend(digits), rbegin(other.digits), rend(other.digits), greater<int>());
    }
    bool isLessThan(const HugeInteger& other) const { return !isGreaterThan(other) && !isEqualTo(other); }
    bool isGreaterThanOrEqualTo(const HugeInteger& other) const { return isGreaterThan(other) || isEqualTo(other); }
    bool isLessThanOrEqualTo(const HugeInteger& other) const { return isLessThan(other) || isEqualTo(other); }
    bool isZero() const {
        return all_of(begin(digits), end(digits), [](const int a) { return !a; });
    }
    HugeInteger multiply(const HugeInteger& other) const {
        HugeInteger res;
        for (int i = 0; i < 40; i++) {
            for (int j = 0; j < 40; j++) {
                int k    = i + j;
                int prod = digits[i] * other.digits[j];
                while (prod > 0) {
                    prod += res.digits[k];
                    res.digits[k] = prod % 10;
                    prod /= 10;
                    k++;
                }
            }
        }
        return res;
    }
    HugeInteger divide(const HugeInteger& other) const {
        if (other.isZero())
            throw invalid_argument("divided zero");
        HugeInteger res;
        HugeInteger remainder = *this;
        for (int i = 39; i >= 0; i--) {
            int quotient = 0;
            while (remainder.isGreaterThanOrEqualTo(other)) {
                remainder = remainder.subtract(other);
                quotient++;
            }
            res.digits[i]       = quotient;
            remainder           = remainder.multiply(HugeInteger("10"));
            remainder.digits[0] = digits[i];
        }
        return res;
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